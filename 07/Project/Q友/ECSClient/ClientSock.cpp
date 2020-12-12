// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "ClientSock.h"
#include "Message.h"
#include "ECSClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSock

CClientSock::CClientSock()
{

}

CClientSock::~CClientSock()
{

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

void CClientSock::OnReceive(int nErrorCode)
{
    CSocket::OnReceive(nErrorCode);
    DWORD dwMaxNum = 0;
    if(IOCtl(SIOCATMARK, &dwMaxNum) == FALSE) { // �����������Ҫ��ȡ
        char *pBuffer = new char[dwMaxNum]; // ����һ��5K�Ļ�����
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        int nLen = Receive(pBuffer, dwMaxNum, MSG_OOB);
        memcpy(&Msg, pBuffer, sizeof(Msg));
        char *pchFile = NULL;
        if(Msg.m_nMsgType == MT_NETTEST) {  // ��������״̬
            // ����Ӧ����Ϣ
            Msg.m_nMsgType = MT_NETCONNECT;
            Send(&Msg, sizeof(CMessage), MSG_OOB);
        }
        else if(Msg.m_nMsgType == MT_NETCONNECT) {  // ĳ���û���������״̬
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 1);
            return;
        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // ĳ���û����ڵ���״̬
            // ���е���״̬����
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 0);
            return;
        }
        delete  [] pBuffer;
    }
    else if(IOCtl(FIONREAD, &dwMaxNum)) {
        CECSClientDlg *pMainDlg = (CECSClientDlg *)AfxGetMainWnd();
        pMainDlg->m_nTestNum = 0;

        char *pBuffer = new char[dwMaxNum];
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        int nLen = Receive(pBuffer, dwMaxNum);
        memcpy(&Msg, pBuffer, sizeof(Msg));
        char *pchFile = NULL;
        if(nLen != sizeof(CMessage)) {      // ��Ϣ�а����ļ�����
            pchFile = pBuffer + sizeof(CMessage);
        }
        if(Msg.m_nMsgType == MT_NETTEST) {  // ��������״̬
            // ����Ӧ����Ϣ
            Msg.m_nMsgType = MT_NETCONNECT;
            Msg.m_nAccountID[0] = pMainDlg->m_nLoginUser;
            Send(&Msg, sizeof(CMessage), MSG_OOB);

        }
        else if(Msg.m_nMsgType == MT_NETCONNECT) {  // ĳ���û���������״̬
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 1);
            return;

        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // ĳ���û����ڵ���״̬
            // ���е���״̬����
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 0);
            return;
        }
        else if(Msg.m_nMsgType == MT_CONFIRM) { // ȷ����Ϣ
            if(Msg.m_nSubType == ST_LOGINCONFIRM)   // ��¼ȷ����Ϣ

            {
                if(strcmp(Msg.m_chContent, "success") == 0) {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_bLogined = TRUE;
                    // AfxMessageBox("��¼�ɹ�!",0,0);

                }
                else if(strcmp(Msg.m_chContent, "failed") == 0) {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_nLoginUser = 0;
                    pDlg->m_bLogined = FALSE;
                    AfxMessageBox("�û������������!", 0, 0);
                }
                else if(strcmp(Msg.m_chContent, "logined") == 0) // �˻����ڱ𴦵�¼

                {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_nLoginUser = 0;
                    pDlg->m_bLogined = FALSE;
                    AfxMessageBox("�˻����ڱ𴦵�¼!", 0, 0);
                }
            }
        }
        else if(Msg.m_nMsgType == MT_SENDDATA) { // ��������
            if(Msg.m_nSubType == ST_TEXT)       // �ı�����

            {
                CString csText = Msg.m_chContent;// �ж��ı��Ƿ�Ϊ��

                if(csText.IsEmpty()) {
                    return;
                }
                // ���֮ǰû������˻�����ͨ�ŶԻ�������ͨ�ŶԻ���

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                int nDesID = Msg.m_nSrcID;      // ��ȡ��Ϣ��Դ�˻�ID

                POSITION pos = NULL;
                CSendDlg *pSdDlg = NULL;
                CString csName = "";
                CString csID = "";
                if(pDlg->SearchSendDlg(nDesID, pos) == FALSE) // ���ҷ��͵ĶԻ����Ƿ����

                {
                    pSdDlg = new CSendDlg();
                    pSdDlg->m_UserID = nDesID;
                    pSdDlg->Create(IDD_SENDDLG_DIALOG);
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                    // ��ӶԷ��˻�ID

                    csID.Format("%d", Msg.m_nSrcID);
                    pSdDlg->m_DesID.ResetContent();
                    pSdDlg->m_DesID.AddString(csID);
                    pSdDlg->m_DesID.SetCurSel(0);

                    BOOL bRet = pDlg->FindUserName(Msg.m_nSrcID, csName);
                    // ��ӶԷ��˻���

                    if(bRet) {
                        pSdDlg->m_DesName.ResetContent();
                        pSdDlg->m_DesName.AddString(csName);
                        pSdDlg->m_DesName.SetCurSel(0);
                    }
                    // ��ӱ����˻�ID������

                    CString csLocalID;
                    csLocalID.Format("%d", pDlg->m_nLoginUser);
                    pSdDlg->m_LocalID.SetWindowText(csLocalID);

                    CString csLocalName = "";
                    bRet = pDlg->FindUserName(pDlg->m_nLoginUser, csLocalName);
                    if(bRet) {
                        pSdDlg->m_LocalName.SetWindowText(csLocalName);
                    }
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                }
                else if(pSdDlg == NULL && pos != NULL) {
                    pSdDlg = (CSendDlg *)pDlg->m_pSdDlgList.GetAt(pos);
                }
                // ��ʾ���ʹ���

                if(pSdDlg != NULL) {
                    // ��ӷ�������


                    pSdDlg->m_ShowEdit.SetSel(-1, -1);
                    csID.Format("%d", Msg.m_nSrcID);
                    CString csUsrInfo = csID;
                    if(csName.IsEmpty()) {
                        pDlg->FindUserName(Msg.m_nSrcID, csName);
                    }

                    csUsrInfo += "(";
                    csUsrInfo += csName;
                    csUsrInfo += ") ";
                    CString csTime;
                    CTime ctNow = CTime::GetCurrentTime();
                    csTime = ctNow.Format("%H:%M:%S");
                    csUsrInfo += csTime;

                    CHARFORMAT cf;
                    memset(&cf, 0, sizeof(CHARFORMAT));
                    BOOL m_bSelect = (pSdDlg->m_ShowEdit.GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
                    if(m_bSelect) {
                        pSdDlg->m_ShowEdit.GetSelectionCharFormat(cf);
                    }
                    else {
                        pSdDlg->m_ShowEdit.GetDefaultCharFormat(cf);
                    }
                    // �ж��Ƿ�ѡ��������


                    cf.dwMask = CFM_COLOR;
                    cf.dwEffects = CFE_BOLD;
                    cf.crTextColor = RGB(0, 0, 255);
                    if(m_bSelect) {
                        pSdDlg->m_ShowEdit.SetSelectionCharFormat(cf);
                    }
                    else {
                        pSdDlg->m_ShowEdit.SetWordCharFormat(cf);
                    }

                    pSdDlg->m_ShowEdit.ReplaceSel(csUsrInfo);

                    pSdDlg->m_ShowEdit.SetSel(-1, -1);
                    pSdDlg->m_ShowEdit.ReplaceSel("\n");

                    pSdDlg->m_ShowEdit.GetDefaultCharFormat(cf);
                    pSdDlg->m_ShowEdit.SetSelectionCharFormat(cf);
                    csText.TrimLeft();
                    csText.TrimRight();
                    if(!csText.IsEmpty()) {
                        pSdDlg->m_ShowEdit.ReplaceSel(Msg.m_chContent);
                        pSdDlg->m_ShowEdit.SetSel(-1, -1);  // ��ĩβ��ӻ���

                        pSdDlg->m_ShowEdit.ReplaceSel("\n");
                    }
                    // ��ʾ����

                    pSdDlg->ShowWindow(SW_SHOW);
                    // ���������ʹ���֮����ʾ��ǰ���ʹ���

                    pSdDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                }
            }
            else if(Msg.m_nSubType == ST_FILE)      // �ļ�����

            {
                CECSClientApp *pApp = (CECSClientApp *)AfxGetApp();
                char chName[MAX_PATH] = {0};
                strcpy(chName, pApp->m_chFullPath);
                strcat(chName, "\\File\\");
                // ����Ŀ¼

                CreateDirectory(chName, NULL);
                strcat(chName, Msg.m_chContent);
                // ��ȡ�ļ���

                CFile file(chName, CFile::modeCreate | CFile::modeReadWrite);

                file.Write(pchFile, Msg.dwDatalen);
                file.Close();
                // ��ʼ����Ա�б�

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                pDlg->LoadOrgFromFile((_bstr_t)(char *)chName);
            }
            else if(Msg.m_nSubType == ST_FILEREQUEST || Msg.m_nSubType == ST_IMAGEREQUEST)  // �Է����ļ�����

            {
                // �����ļ����ʹ���

                // ���֮ǰû������˻�����ͨ�ŶԻ�������ͨ�ŶԻ���

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                int nDesID = Msg.m_nSrcID;      // ��ȡ��Ϣ��Դ�˻�ID

                POSITION pos = NULL;
                CSendDlg *pSdDlg = NULL;
                CString csName = "";
                CString csID = "";
                if(pDlg->SearchSendDlg(nDesID, pos) == FALSE) // ���ҷ��͵ĶԻ����Ƿ����

                {
                    pSdDlg = new CSendDlg();
                    pSdDlg->m_UserID = nDesID;
                    pSdDlg->Create(IDD_SENDDLG_DIALOG);
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                    // ��ӶԷ��˻�ID

                    csID.Format("%d", Msg.m_nSrcID);
                    pSdDlg->m_DesID.ResetContent();
                    pSdDlg->m_DesID.AddString(csID);
                    pSdDlg->m_DesID.SetCurSel(0);

                    BOOL bRet = pDlg->FindUserName(Msg.m_nSrcID, csName);
                    // ��ӶԷ��˻���

                    if(bRet) {
                        pSdDlg->m_DesName.ResetContent();
                        pSdDlg->m_DesName.AddString(csName);
                        pSdDlg->m_DesName.SetCurSel(0);
                    }
                    // ��ӱ����˻�ID������

                    CString csLocalID;
                    csLocalID.Format("%d", pDlg->m_nLoginUser);
                    pSdDlg->m_LocalID.SetWindowText(csLocalID);

                    CString csLocalName = "";
                    bRet = pDlg->FindUserName(pDlg->m_nLoginUser, csLocalName);
                    if(bRet) {
                        pSdDlg->m_LocalName.SetWindowText(csLocalName);
                    }
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                }
                else if(pSdDlg == NULL && pos != NULL) {
                    pSdDlg = (CSendDlg *)pDlg->m_pSdDlgList.GetAt(pos);
                }
                // ��ʾ����

                pSdDlg->ShowWindow(SW_SHOW);
                // ���������ʹ���֮����ʾ��ǰ���ʹ���

                pSdDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


                if(Msg.m_nSubType == ST_IMAGEREQUEST)   // ��ʾֱ�ӽ���ͼ���ļ�

                {
                    CString csLocalID;
                    pSdDlg->m_LocalID.GetWindowText(csLocalID);
                    int nSrcID = Msg.m_nSrcID;

                    pSdDlg->m_csSock.ShutDown();
                    pSdDlg->m_csSock.Close();

                    Msg.m_nSubType = ST_IMAGEREQUEST;
                    Msg.m_nSrcID = atoi(csLocalID);
                    Msg.m_nAccountID[0] = nSrcID;
                    Msg.m_nSubType = ST_IMAGERESPONSE;  // ͼ������




                    int nPort = 0;
                    BOOL bRet = FALSE;
create:
                    nPort = 4000 + rand() % 3000;
                    bRet = pSdDlg->m_csSock.Create(nPort);
                    if(bRet == FALSE) {
                        goto create;
                    }

                    int nSize = PACKAGESIZE;
                    pSdDlg->m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
                    pSdDlg->m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));
connect:
                    bRet = pSdDlg->m_csSock.Connect(pDlg->m_ServerIP, 601);
                    if(bRet == FALSE) {
                        goto connect;
                    }
                    Send(&Msg, sizeof(CMessage));

                }
                else {
                    if(pSdDlg->MessageBox("�Ƿ�ͬ������ļ�?", "��ʾ", MB_YESNO) == IDYES) {
                        CString csLocalID;
                        pSdDlg->m_LocalID.GetWindowText(csLocalID);
                        int nSrcID = Msg.m_nSrcID;
                        Msg.m_nSrcID = atoi(csLocalID);
                        Msg.m_nAccountID[0] = nSrcID;
                        Msg.m_nSubType = ST_FILERESPONSE;


                        pSdDlg->m_csSock.ShutDown();
                        pSdDlg->m_csSock.Close();

                        int nPort = 0;
                        BOOL bRet = FALSE;
createimg:
                        nPort = 4000 + rand() % 3000;
                        bRet = pSdDlg->m_csSock.Create(nPort);
                        if(bRet == FALSE) {
                            goto createimg;
                        }

                        int nSize = PACKAGESIZE;
                        pSdDlg->m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
                        pSdDlg->m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));
connectimg:
                        bRet = pSdDlg->m_csSock.Connect(pDlg->m_ServerIP, 601);
                        if(bRet == FALSE) {
                            goto connectimg;
                        }
                        Send(&Msg, sizeof(CMessage));
                    }
                    else    // ���;ܾ���Ϣ

                    {
                        CString csLocalID;
                        pSdDlg->m_LocalID.GetWindowText(csLocalID);
                        int nSrcID = Msg.m_nSrcID;
                        Msg.m_nSrcID = atoi(csLocalID);
                        Msg.m_nAccountID[0] = nSrcID;
                        Msg.m_nSubType = ST_FILEDENY;
                        Send(&Msg, sizeof(CMessage));
                    }
                }
            }
        }
        delete [] pBuffer;
        CSocket::OnReceive(nErrorCode);
    }
}
