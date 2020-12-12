// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "ClientSock.h"
// #include "Message.h"
#include "ECSSrvDlg.h"
#include "ClientInfo.h"
#include "ClientFileSock.h"
#include "AccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSock

CClientSock::CClientSock()
{
    m_csAccountID = "";
    m_nTestNum = 0;
    m_bTesting = FALSE;
}

CClientSock::~CClientSock()
{
    m_csAccountID = "";
    m_nTestNum = 0;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSock, CSocket)
    //{{AFX_MSG_MAP(CClientSock)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif  // 0

/////////////////////////////////////////////////////////////////////////////
// CClientSock member functions
// 接收客户端发来的数据
void CClientSock::OnReceive(int nErrorCode)
{
    DWORD dwMaxNum = 0;
    if(IOCtl(SIOCATMARK, &dwMaxNum) == FALSE) {     // 有外带数据需要读取
        if(dwMaxNum >= sizeof(CMessage)) {
            char *pBuffer = new char[dwMaxNum];     // 定义一个1024K的缓冲区
            memset(pBuffer, 0, dwMaxNum);
            CMessage Msg;
            DWORD nLen = Receive(pBuffer, dwMaxNum, MSG_OOB);
            if(nLen == sizeof(CMessage)) {          // 发送的不是文件
                memcpy(&Msg, pBuffer, nLen);
            }
            else {                                  // 消息中包含文件数据
                memcpy(&Msg, pBuffer, sizeof(CMessage));
            }
            if(Msg.m_nMsgType == MT_NETCONNECT) {
                m_nTestNum = 10;
            }
            delete []pBuffer;
        }
    }
    else if(IOCtl(FIONREAD, &dwMaxNum)) {
        char *pBuffer = new char[dwMaxNum];         // 定义一个1024K的缓冲区
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        DWORD nLen = Receive(pBuffer, dwMaxNum);
        if(nLen == sizeof(CMessage)) {              // 发送的不是文件
            memcpy(&Msg, pBuffer, nLen);
        }
        else {                                      // 消息中包含文件数据
            memcpy(&Msg, pBuffer, sizeof(CMessage));
        }
        if(Msg.m_nMsgType == MT_NETCONNECT) {
            m_nTestNum = 10;
            m_bTesting = FALSE;                     // 一次网络测试结束
        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // 用户掉线，转发掉线信息
            int nID = Msg.m_nAccountID[0];

            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
            POSITION pos = pList->GetHeadPosition();
            while(pos) {
                CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                if(pClientInfo != NULL) {
                    CString csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                    if(!csID.IsEmpty()) {           // 遍历已登录过的账户套接字
                        int nLoginID = atoi(csID);
                        if(nLoginID != nID) {

                            CMessage cfOrgInfo;
                            cfOrgInfo.m_nMsgType = MT_DISCONNECT;
                            cfOrgInfo.m_nAccountID[0] = nID;
                            pClientInfo->m_pClientSock->Send(&cfOrgInfo, sizeof(CMessage), MSG_OOB);
                        }
                    }
                }
            }

        }
        else if(Msg.m_nMsgType == MT_FIRSTLOG) {    // 登录信息
            int nID = atoi(Msg.m_chAccount);        // 读取用户ID和密码
            // 判断该用户目前是否已登录
            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
            POSITION pos = pList->GetHeadPosition();
            while(pos) {
                CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                if(pClientInfo != NULL) {
                    CString csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                    if(!csID.IsEmpty()) {           // 遍历已登录过的账户套接字
                        int nLoginID = atoi(csID);
                        if(nLoginID == nID) {       // 该账户已经登录

                            CMessage cfOrgInfo;

                            cfOrgInfo.m_nMsgType = MT_CONFIRM;
                            cfOrgInfo.m_nSubType = ST_LOGINCONFIRM;;
                            cfOrgInfo.m_nSrcID = nID;
                            strncpy(cfOrgInfo.m_chContent, "logined", 7);
                            Send(&cfOrgInfo, sizeof(CMessage));
                            delete []pBuffer;
                            return ;
                        }
                    }
                }
            }
            // 验证用户密码
            CECSSrvDlg *pDlg = (CECSSrvDlg *)AfxGetMainWnd();
            BOOL bValid = pDlg->VerifyUser(nID, Msg.m_chPassword);
            CMessage cfInfo;
            cfInfo.m_nMsgType = MT_CONFIRM;
            cfInfo.m_nSubType = ST_LOGINCONFIRM;
            cfInfo.m_nSrcID = atoi(m_csAccountID);
            if(bValid) {                            // 发送登录成功确认信息
                strncpy(cfInfo.m_chContent, "success", 7);
                Send(&cfInfo, sizeof(CMessage));
                m_csAccountID.Format("%d", nID);    // 在登录成功之后记录用户ID
                // 发送部门和人员组织结构org.xml
                CMessage cfOrgInfo;
                cfOrgInfo.m_nMsgType = MT_SENDDATA;
                cfOrgInfo.m_nSubType = ST_FILE;
                cfOrgInfo.m_nSrcID = atoi(m_csAccountID);

                // 通过智能指针创建一个解析器的实例
                CECSSrvApp *pApp = (CECSSrvApp *)AfxGetApp();
                char chFileName[MAX_PATH] = {0};
                strcpy(chFileName, pApp->m_chFullPath);
                strcat(chFileName, "\\File\\");
                strcat(chFileName, "org.xml");

                // 判断文件是否存在
                CFileFind flFind;
                if(flFind.FindFile(chFileName) == FALSE) { // 没有发现文件
                    // 调用窗户管理窗口创建文件
                    CAccountDlg ActDlg;
                    // 调用ActDlg.OnInitDialog()方法来构建org.xml文件
                    ActDlg.Create(IDD_ACCOUNTDLG_DIALOG);
                }
                CFile file;
                file.Open(chFileName, CFile::modeRead);
                DWORD dwLen = file.GetLength();
                char *pchBuffer = new char[dwLen + sizeof(CMessage)];
                memset(pchBuffer, 0, dwLen + sizeof(CMessage));
                cfOrgInfo.dwDatalen = dwLen;
                strncpy(cfOrgInfo.m_chContent, "org.xml", 20);
                memcpy(pchBuffer, &cfOrgInfo, sizeof(CMessage));
                char *pchTmp = pchBuffer;
                pchTmp += sizeof(CMessage);

                file.Read(pchTmp, dwLen);
                file.Close();
                Sleep(50);
                int nLen = Send(pchBuffer, sizeof(CMessage) + dwLen);
                Sleep(50);
                delete []pchBuffer;
                // 向其他用户转发用户上线信息
                // 读取用户ID
                int nID = atoi(Msg.m_chAccount);
                // 判断该用户目前是否已登录
                CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
                POSITION pos = pList->GetHeadPosition();
                while(pos) {
                    CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                    if(pClientInfo != NULL) {
                        auto csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                        if(!csID.IsEmpty()) {   // 遍历已登录过的账户套接字
                            int nLoginID = atoi(csID);
                            if(nLoginID != nID) { // 该账户已经登录
                                CMessage cfOrgInfo;
                                cfOrgInfo.m_nMsgType = MT_NETCONNECT;
                                cfOrgInfo.m_nAccountID[0] = nID;
                                pClientInfo->m_pClientSock->Send(&cfOrgInfo, sizeof(CMessage), MSG_OOB);
                            }
                        }
                    }
                }


            }
            else {                                          // 发送登录失败确认信息
                strncpy(cfInfo.m_chContent, "failed", 6);
                Send(&cfInfo, sizeof(CMessage));
            }
        }
        else if(Msg.m_nMsgType == MT_SENDDATA) {            // 发送信息
            if(Msg.m_nSubType == ST_TEXT) {                 // 发送文本
                CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
                int nCount = pList->GetCount();
                POSITION pos = pList->GetHeadPosition();

                if(pos) {
                    for(int i = 0; i < nCount; i++) {
                        if(Msg.m_nAccountID[i] == 0) {
                            continue;
                        }
                        int nID = Msg.m_nAccountID[i];
                        pos = pList->GetHeadPosition();
                        // 获取目标对象的套接字
                        for(int j = 0; j < nCount; j++) {
                            CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                            if(pClientInfo != NULL)
                                if(atoi(((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID) == nID) {
                                    pClientInfo->m_pClientSock->Send(&Msg, sizeof(CMessage));
                                }
                        }
                    }
                }
            }
            else if(Msg.m_nSubType == ST_FILERESPONSE ||
                    Msg.m_nSubType == ST_IMAGERESPONSE ||
                    Msg.m_nSubType == ST_FILEDENY) {
                int nDesID = Msg.m_nAccountID[0];
                CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_FileList;
                int nCount = pList->GetCount();
                POSITION pos = pList->GetHeadPosition();
                if(pos) {
                    for(int i = 0; i < nCount; i++) {
                        if(Msg.m_nAccountID[i] == 0) {
                            continue;
                        }
                        int nID = Msg.m_nAccountID[i];
                        pos = pList->GetHeadPosition();
                        // 获取目标对象的套接字
                        for(int j = 0; j < nCount; j++) {
                            CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                            if(pClientInfo != NULL)
                                if(atoi(((CClientFileSock *)pClientInfo->m_pClientSock)->m_csAccountID) == nID) {
                                    pClientInfo->m_pClientSock->Send(&Msg, sizeof(CMessage));
                                }
                        }
                    }
                }
            }
        }
        delete []pBuffer;
        CSocket::OnReceive(nErrorCode);
    }
}
