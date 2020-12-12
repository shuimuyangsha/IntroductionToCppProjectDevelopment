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
// ���տͻ��˷���������
void CClientSock::OnReceive(int nErrorCode)
{
    DWORD dwMaxNum = 0;
    if(IOCtl(SIOCATMARK, &dwMaxNum) == FALSE) {     // �����������Ҫ��ȡ
        if(dwMaxNum >= sizeof(CMessage)) {
            char *pBuffer = new char[dwMaxNum];     // ����һ��1024K�Ļ�����
            memset(pBuffer, 0, dwMaxNum);
            CMessage Msg;
            DWORD nLen = Receive(pBuffer, dwMaxNum, MSG_OOB);
            if(nLen == sizeof(CMessage)) {          // ���͵Ĳ����ļ�
                memcpy(&Msg, pBuffer, nLen);
            }
            else {                                  // ��Ϣ�а����ļ�����
                memcpy(&Msg, pBuffer, sizeof(CMessage));
            }
            if(Msg.m_nMsgType == MT_NETCONNECT) {
                m_nTestNum = 10;
            }
            delete []pBuffer;
        }
    }
    else if(IOCtl(FIONREAD, &dwMaxNum)) {
        char *pBuffer = new char[dwMaxNum];         // ����һ��1024K�Ļ�����
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        DWORD nLen = Receive(pBuffer, dwMaxNum);
        if(nLen == sizeof(CMessage)) {              // ���͵Ĳ����ļ�
            memcpy(&Msg, pBuffer, nLen);
        }
        else {                                      // ��Ϣ�а����ļ�����
            memcpy(&Msg, pBuffer, sizeof(CMessage));
        }
        if(Msg.m_nMsgType == MT_NETCONNECT) {
            m_nTestNum = 10;
            m_bTesting = FALSE;                     // һ��������Խ���
        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // �û����ߣ�ת��������Ϣ
            int nID = Msg.m_nAccountID[0];

            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
            POSITION pos = pList->GetHeadPosition();
            while(pos) {
                CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                if(pClientInfo != NULL) {
                    CString csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                    if(!csID.IsEmpty()) {           // �����ѵ�¼�����˻��׽���
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
        else if(Msg.m_nMsgType == MT_FIRSTLOG) {    // ��¼��Ϣ
            int nID = atoi(Msg.m_chAccount);        // ��ȡ�û�ID������
            // �жϸ��û�Ŀǰ�Ƿ��ѵ�¼
            CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
            POSITION pos = pList->GetHeadPosition();
            while(pos) {
                CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                if(pClientInfo != NULL) {
                    CString csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                    if(!csID.IsEmpty()) {           // �����ѵ�¼�����˻��׽���
                        int nLoginID = atoi(csID);
                        if(nLoginID == nID) {       // ���˻��Ѿ���¼

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
            // ��֤�û�����
            CECSSrvDlg *pDlg = (CECSSrvDlg *)AfxGetMainWnd();
            BOOL bValid = pDlg->VerifyUser(nID, Msg.m_chPassword);
            CMessage cfInfo;
            cfInfo.m_nMsgType = MT_CONFIRM;
            cfInfo.m_nSubType = ST_LOGINCONFIRM;
            cfInfo.m_nSrcID = atoi(m_csAccountID);
            if(bValid) {                            // ���͵�¼�ɹ�ȷ����Ϣ
                strncpy(cfInfo.m_chContent, "success", 7);
                Send(&cfInfo, sizeof(CMessage));
                m_csAccountID.Format("%d", nID);    // �ڵ�¼�ɹ�֮���¼�û�ID
                // ���Ͳ��ź���Ա��֯�ṹorg.xml
                CMessage cfOrgInfo;
                cfOrgInfo.m_nMsgType = MT_SENDDATA;
                cfOrgInfo.m_nSubType = ST_FILE;
                cfOrgInfo.m_nSrcID = atoi(m_csAccountID);

                // ͨ������ָ�봴��һ����������ʵ��
                CECSSrvApp *pApp = (CECSSrvApp *)AfxGetApp();
                char chFileName[MAX_PATH] = {0};
                strcpy(chFileName, pApp->m_chFullPath);
                strcat(chFileName, "\\File\\");
                strcat(chFileName, "org.xml");

                // �ж��ļ��Ƿ����
                CFileFind flFind;
                if(flFind.FindFile(chFileName) == FALSE) { // û�з����ļ�
                    // ���ô��������ڴ����ļ�
                    CAccountDlg ActDlg;
                    // ����ActDlg.OnInitDialog()����������org.xml�ļ�
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
                // �������û�ת���û�������Ϣ
                // ��ȡ�û�ID
                int nID = atoi(Msg.m_chAccount);
                // �жϸ��û�Ŀǰ�Ƿ��ѵ�¼
                CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
                POSITION pos = pList->GetHeadPosition();
                while(pos) {
                    CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
                    if(pClientInfo != NULL) {
                        auto csID = ((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID;
                        if(!csID.IsEmpty()) {   // �����ѵ�¼�����˻��׽���
                            int nLoginID = atoi(csID);
                            if(nLoginID != nID) { // ���˻��Ѿ���¼
                                CMessage cfOrgInfo;
                                cfOrgInfo.m_nMsgType = MT_NETCONNECT;
                                cfOrgInfo.m_nAccountID[0] = nID;
                                pClientInfo->m_pClientSock->Send(&cfOrgInfo, sizeof(CMessage), MSG_OOB);
                            }
                        }
                    }
                }


            }
            else {                                          // ���͵�¼ʧ��ȷ����Ϣ
                strncpy(cfInfo.m_chContent, "failed", 6);
                Send(&cfInfo, sizeof(CMessage));
            }
        }
        else if(Msg.m_nMsgType == MT_SENDDATA) {            // ������Ϣ
            if(Msg.m_nSubType == ST_TEXT) {                 // �����ı�
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
                        // ��ȡĿ�������׽���
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
                        // ��ȡĿ�������׽���
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
