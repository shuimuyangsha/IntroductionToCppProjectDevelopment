// ClientFileSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "ClientFileSock.h"
#include "ECSSrvDlg.h"
#include "ClientInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientFileSock

CClientFileSock::CClientFileSock()
{
    m_csIP = "";
    m_csAccountID = "";
    m_bFirst    = TRUE;
    m_bRvFull = TRUE;
    m_pchBuffer = NULL;
    m_nRvNum = 0;
    m_nPackSize = 0;
}

CClientFileSock::~CClientFileSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientFileSock, CSocket)
    //{{AFX_MSG_MAP(CClientFileSock)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif  // 0

/////////////////////////////////////////////////////////////////////////////
// CClientFileSock member functions

void CClientFileSock::OnReceive(int nErrorCode)
{
    CSocket::OnReceive(nErrorCode);
    DWORD dwMaxNum = 0;
    if(!IOCtl(FIONREAD, &dwMaxNum)) {
        return;
    }

    char *pBuffer = new char[dwMaxNum]; // 定义一个缓冲区
    memset(pBuffer, 0, dwMaxNum);
    CMessage Msg;
    DWORD nLen = Receive(pBuffer, dwMaxNum);
    BOOL bMultiPack = FALSE;    // 判断是否有粘报现象
    // 判断数据报是否完整
    if(m_bRvFull == FALSE) {
        m_nRvNum += nLen;
        if(m_nRvNum >= m_nPackSize) {   // 完整的数据报接收完成
            m_bRvFull = TRUE;
            m_nRvNum = 0;
        }
    }
    else {
        m_bRvFull = TRUE;
        memcpy(&Msg, pBuffer, sizeof(CMessage));
        memcpy(&m_RvMsg, pBuffer, sizeof(CMessage));

        if((m_RvMsg.m_nSubType == ST_FILE || m_RvMsg.m_nSubType == ST_IMAGE)
           && dwMaxNum < Msg.dwDatalen) { // 包含CMessage结构，但是数据不完整
            m_bRvFull = FALSE;

            m_nRvNum = dwMaxNum;
            m_nPackSize = Msg.dwDatalen;
        }
        else if((m_RvMsg.m_nSubType == ST_FILE ||
                 m_RvMsg.m_nSubType == ST_IMAGE) &&
                dwMaxNum == Msg.dwDatalen) { // 接收完整的数据报
            m_nPackSize = Msg.dwDatalen;
            m_nRvNum = 0;
        }
        else if((m_RvMsg.m_nSubType == ST_FILE ||
                 m_RvMsg.m_nSubType == ST_IMAGE) &&
                dwMaxNum > Msg.dwDatalen) { // 接收数据大于完整的数据报
            m_nPackSize = Msg.dwDatalen;
            m_nRvNum = 0;
            bMultiPack = TRUE;      // 出现粘报现象
            char *pTmpBuffer = pBuffer;
            pTmpBuffer += Msg.dwDatalen;
            if(dwMaxNum - Msg.dwDatalen == sizeof(CMessage)) {
                memcpy(&Msg, pTmpBuffer, sizeof(CMessage));
            }
        }
        // 传输的不是文件数据，只是一个CMessage结构数据
        else if(m_RvMsg.m_nSubType != ST_FILE ||
                m_RvMsg.m_nSubType != ST_IMAGE) {
            m_nPackSize = nLen;
            m_nRvNum = 0;
        }
    }

    if(m_RvMsg.m_nMsgType == MT_SENDDATA) {
        // 如果为发送文件请求
        if(m_RvMsg.m_nSubType == ST_FILEREQUEST ||
           m_RvMsg.m_nSubType == ST_IMAGEREQUEST) {
            // 获取文件目的账户
            int nDesID = Msg.m_nAccountID[0];
            int nSrcID = Msg.m_nSrcID;
            // 获取文件名
            CString csFileName = m_RvMsg.m_chContent;
            // 利用之前与客户端建立连接的套接字向目标发送信息，
            // 使其得到消息是否同意或拒绝接收消息
            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
            int nCount = pList->GetCount();
            POSITION pos = pList->GetHeadPosition();
            if(pos) {
                // 获取目标对象的套接字
                for(int j = 0; j < nCount; j++) {
                    auto *pClientInfo = static_cast<CClientInfo *>(pList->GetNext(pos));
                    if(pClientInfo != NULL) {
                        auto pSock = (CClientSock *)pClientInfo->m_pClientSock;
                        auto id = (pSock)->m_csAccountID;
                        if(atoi(id) == nDesID) {
                            pClientInfo->m_pClientSock->Send(&Msg, sizeof(CMessage));
                        }
                    }
                }

            }
        }

        else if(m_RvMsg.m_nSubType == ST_FILE || m_RvMsg.m_nSubType == ST_IMAGE) {
            // 转发数据到目的
            int nDesID = m_RvMsg.m_nAccountID[0];
            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_FileList;
            int nCount = pList->GetCount();
            POSITION pos = pList->GetHeadPosition();
            if(pos) {
                // 获取目标对象的套接字
                for(int j = 0; j < nCount; j++) {
                    CClientInfo *pClientInfo = (CClientInfo *)pList->GetNext(pos);
                    if(pClientInfo != NULL) {
                        int nID = atoi(((CClientFileSock *)pClientInfo->m_pClientSock)->m_csAccountID);
                        if(nID == nDesID) {
                            if(!bMultiPack) {
                                pClientInfo->m_pClientSock->Send(pBuffer, nLen);
                            }
                            else {
                                pClientInfo->m_pClientSock->Send(pBuffer, m_RvMsg.dwDatalen);
                            }
                            break;
                        }
                    }
                }
            }


        }

    }
    if(Msg.m_nMsgType == MT_DISCONNECT) { // 客户端断开了连接
        CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_FileList;
        int nCount = pList->GetCount();
        POSITION pos = pList->GetHeadPosition();
        if(pos) {
            while(pos != NULL) {
                POSITION ptTmp = pos;
                CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                if(pClientInfo != NULL)
                    if(atoi(((CClientFileSock *)pClientInfo->m_pClientSock)->m_csAccountID) == atoi(m_csAccountID)) {
                        pClientInfo->m_pClientSock->ShutDown();
                        pClientInfo->m_pClientSock->Close();
                        delete pClientInfo;
                        pList->RemoveAt(ptTmp); // 从列表中移除
                        break;
                    }
            }
        }
    }

    if(m_bRvFull == TRUE && m_pchBuffer != NULL) {
        delete [] m_pchBuffer;
        m_pchBuffer = NULL;
    }
    delete []pBuffer;
    pBuffer = NULL;

}
