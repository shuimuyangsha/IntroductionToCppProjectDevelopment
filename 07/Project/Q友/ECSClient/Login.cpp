// Login.cpp : implementation file
//

#include "stdafx.h"
#include "ecsclient.h"
#include "Login.h"
#include "ClientSock.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CECSClientDlg *MainDlg , CWnd *pParent /*=NULL*/)
    : CDialog(CLogin::IDD, pParent)
{
    //{{AFX_DATA_INIT(CLogin)
    //}}AFX_DATA_INIT
    m_pMainDlg = MainDlg;
    m_bConnect = FALSE;

}


void CLogin::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLogin)
    DDX_Control(pDX, IDC_PASSWORD, m_PassWord);
    DDX_Control(pDX, IDC_USERID, m_UserID);
    DDX_Control(pDX, IDC_SVRPORT, m_SvrPort);
    DDX_Control(pDX, IDC_SVRIP, m_SvrIP);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
    //{{AFX_MSG_MAP(CLogin)
    ON_BN_CLICKED(IDC_BT_CANCEL, OnBtCancel)
    ON_BN_CLICKED(IDC_BT_LOGIN, OnBtLogin)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

void CLogin::OnOK()
{

}

void CLogin::OnBtCancel()
{
    CDialog::OnCancel();
}

BOOL CLogin::OnInitDialog()
{
    CDialog::OnInitDialog();

    char chHostName[MAX_PATH] = {0};
    gethostname(chHostName, MAX_PATH);
    hostent *phost = gethostbyname(chHostName);
    char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);
    BOOL bRet = FALSE;
    m_pMainDlg->m_ClientSock.ShutDown();
    m_pMainDlg->m_ClientSock.Close();
create:
    bRet = m_pMainDlg->m_ClientSock.Create(3000 + rand() % 2000, SOCK_STREAM, chIP);
    if(bRet == FALSE) {
        goto create;
    }

    BOOL bEnable = TRUE;
    m_pMainDlg->m_ClientSock.SetSockOpt(SO_OOBINLINE, &bEnable , sizeof(BOOL));

    SetTimer(1, 500, NULL);

    // ��ȡIni�ļ����ж��Ƿ�Ϊ�Զ���¼
    CECSClientApp *pApp = (CECSClientApp *)AfxGetApp();
    char chName[MAX_PATH] = {0};
    strcpy(chName, pApp->m_chFullPath);
    strcat(chName, "\\Config\\");
    // ����Ŀ¼

    CreateDirectory(chName, NULL);
    strcat(chName, "Login.ini");
    CFileFind flFind;
    if(flFind.FindFile(chName)) {

        // ��ȡIni�ļ�
        char csIP[MAX_PATH] = {0};
        char csPort[MAX_PATH] = {0};
        char csUserID[MAX_PATH] = {0};
        char chPass[MAX_PATH] = {0};
        GetPrivateProfileString("��¼��Ϣ", "�Զ���¼", "", csIP, MAX_PATH, chName);
        // �Զ���¼
        if(strcmp(csIP, "1") == 0) {
            // ��ȡ�Զ���¼��Ϣ
            GetPrivateProfileString("��¼��Ϣ", "�˻�ID", "", csUserID, MAX_PATH, chName);
            GetPrivateProfileString("��¼��Ϣ", "��¼����", "", (char *)chPass, MAX_PATH, chName);
            GetPrivateProfileString("��¼��Ϣ", "������IP", "", csIP, MAX_PATH, chName);
            GetPrivateProfileString("��¼��Ϣ", "�˿ں�", "", csPort, MAX_PATH, chName);

            // ��ֵ�������еı���
            m_UserID.SetWindowText(csUserID);
            m_PassWord.SetWindowText(chPass);
            m_SvrIP.SetWindowText(csIP);
            m_SvrPort.SetWindowText(csPort);

            CButton *pCheckBox = (CButton *)GetDlgItem(IDC_CHK_AUTOLOGIN);
            pCheckBox->SetCheck(1);
            // ���õ�¼���ܣ����е�¼
            OnBtLogin();
        }
    }
    flFind.Close();

    // Ĭ�����ñ�����IPΪ������IP
    {
        char chName[MAX_PATH] = {0};
        gethostname(chName, MAX_PATH);
        hostent *phost = gethostbyname(chName);
        char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);

        m_SvrIP.SetWindowTextA(chIP);
        m_SvrPort.SetWindowText("12346");
    }

    return TRUE;
}


// ��¼������
void CLogin::OnBtLogin()
{
    if(m_pMainDlg != NULL) {
        // ��ȡ�����������ֵ
        CString csIP, csPort, csUserID, csPassword;
        m_SvrIP.GetWindowText(csIP);
        m_SvrPort.GetWindowText(csPort);
        m_UserID.GetWindowText(csUserID);
        m_PassWord.GetWindowText(csPassword);

        if(!csIP.IsEmpty() && !csPort.IsEmpty() && !csUserID.IsEmpty()
           && !csPassword.IsEmpty()) {
            m_pMainDlg->m_bLogined = FALSE;

            BOOL bRet = FALSE;

            // ���ӷ�����
            bRet = m_pMainDlg->m_ClientSock.Connect(csIP, atoi(csPort));
            int nNum = 0;
            while(bRet == FALSE) {
                nNum++;
                bRet = m_pMainDlg->m_ClientSock.Connect(csIP, atoi(csPort));
                MSG msg;
                // �ô�Ϊ�˷�ֹ���濨�������зַ���Ϣ����
                if(GetMessage(&msg, m_hWnd, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                if(nNum > 20) {
                    MessageBox("���ӷ�������ʱ!", "��ʾ");
                    return;
                }
            }

            // ������Ϣ����
            CMessage cmLoginInfo;
            strncpy(cmLoginInfo.m_chAccount, csUserID, csUserID.GetLength());

            strncpy(cmLoginInfo.m_chPassword, csPassword, csPassword.GetLength());
            cmLoginInfo.m_nMsgType = MT_FIRSTLOG;

            // �����ֵ��¼����
            m_pMainDlg->m_nLoginUser = atoi(csUserID);
            m_pMainDlg->m_Password = csPassword;
            m_pMainDlg->m_nPort = atoi(csPort);
            m_pMainDlg->m_ServerIP = csIP;

            // ������Ϣ
            m_pMainDlg->m_ClientSock.Send(&cmLoginInfo, sizeof(CMessage));
        }
    }
}

void CLogin::OnTimer(UINT nIDEvent)
{
    if(m_pMainDlg != NULL && m_pMainDlg->m_bLogined == TRUE) {
        KillTimer(1);
        // ��¼��¼��Ϣ

        CECSClientApp *pApp = (CECSClientApp *)AfxGetApp();
        char chName[MAX_PATH] = {0};
        strcpy(chName, pApp->m_chFullPath);
        strcat(chName, "\\Config\\");

        // ����Ŀ¼
        CreateDirectory(chName, NULL);
        strcat(chName, "Login.ini");
        CString csIP, csPort, csUserID, csPassword;
        m_SvrIP.GetWindowText(csIP);
        m_SvrPort.GetWindowText(csPort);
        m_UserID.GetWindowText(csUserID);
        m_PassWord.GetWindowText(csPassword);
        int nLen = csPassword.GetLength();

        WritePrivateProfileString("��¼��Ϣ", "�˻�ID", csUserID, chName);
        WritePrivateProfileString("��¼��Ϣ", "��¼����", csPassword, chName);
        WritePrivateProfileString("��¼��Ϣ", "������IP", csIP, chName);
        WritePrivateProfileString("��¼��Ϣ", "�˿ں�", csPort, chName);

        CButton *pCheckBox = (CButton *)GetDlgItem(IDC_CHK_AUTOLOGIN);
        int nState = 0;
        if(pCheckBox != NULL) {
            nState = pCheckBox->GetCheck();
        }
        CString csAutoLogin;
        csAutoLogin.Format("%d", nState);
        WritePrivateProfileString("��¼��Ϣ", "�Զ���¼", csAutoLogin, chName);

        EndDialog(IDOK);
    }
    CDialog::OnTimer(nIDEvent);
}
