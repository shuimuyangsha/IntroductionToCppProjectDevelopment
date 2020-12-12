// ECSSrvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "ECSSrvDlg.h"
#include "DepartmentMng.h"
#include "AccountDlg.h"
#include "ClientInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About



class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSSrvDlg dialog

CECSSrvDlg::CECSSrvDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CECSSrvDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CECSSrvDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICON);
}

void CECSSrvDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CECSSrvDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CECSSrvDlg, CDialog)
    //{{AFX_MSG_MAP(CECSSrvDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_DEPARTMENT, OnDepartment)
    ON_BN_CLICKED(IDC_BTACCOUNT, OnBtaccount)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSSrvDlg message handlers

BOOL CECSSrvDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if(pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if(!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon


    // 初始化数据库
    m_DataMgn.InitDatabase();

    // 获得本地IP
    char chName[MAX_PATH] = {0};
    gethostname(chName, MAX_PATH);
    hostent *phost = gethostbyname(chName);
    char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);

    // 指定端口号
    BOOL bRet = m_ServerSock.Create(12346, SOCK_STREAM, chIP);

    // 设置 socket 属性
    char chDisable = 1;
    setsockopt(m_ServerSock.m_hSocket, IPPROTO_TCP, TCP_NODELAY, &chDisable, sizeof(chDisable));
    BOOL bEnable = TRUE;
    m_ServerSock.SetSockOpt(SO_OOBINLINE, &bEnable , sizeof(BOOL));

    // 网络通信socket开始进行监听
    bRet = m_ServerSock.Listen(MAXNUM);

    // 文件端口号
    bRet = m_FileSock.Create(601, SOCK_STREAM, chIP);
    setsockopt(m_FileSock.m_hSocket, IPPROTO_TCP, TCP_NODELAY, &chDisable, sizeof(chDisable));

    int nSize = PACKAGESIZE;
    m_FileSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
    m_FileSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));

    DWORD dwEnable = 1;
    m_FileSock.IOCtl(FIONBIO, &dwEnable);
    m_ServerSock.IOCtl(FIONBIO, &dwEnable);

    // 文件socket开始进行监听
    bRet = m_FileSock.Listen(MAXNUM);

    // 启动定时器 : 定时向客户端发送网络状态测试信息
    SetTimer(1, 2000, NULL);
    // 启动定时器 : 广播用户在线消息
    SetTimer(2, 2000, NULL);

    return TRUE;
}

void CECSSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CECSSrvDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CECSSrvDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CECSSrvDlg::OnOK()
{

}

BOOL CECSSrvDlg::PreTranslateMessage(MSG *pMsg)
{
    // 防止按ESC键关闭对话框
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

// 部门设置
void CECSSrvDlg::OnDepartment()
{
    CDepartmentMng DPMng;
    DPMng.DoModal();
}

void CECSSrvDlg::OnBtaccount()
{
    CAccountDlg AccountDlg;
    AccountDlg.DoModal();
}

BOOL CECSSrvDlg::VerifyUser(int nID, char *pchPassword)
{
    CString csSQL;
    csSQL.Format("select AccountID from tb_Account where AccountID = %d and Password = '%s'", nID, pchPassword);
    _RecordsetPtr pRecord = m_DataMgn.m_pConnect->Execute((_bstr_t)csSQL, 0, adCmdText);
    if(!pRecord->ADOEOF) {  // 判断用户是否存在
        return TRUE;
    }
    return FALSE;
}

void CECSSrvDlg::OnTimer(UINT nIDEvent)
{
    // 定时向客户端发送网络状态测试信息
    if(nIDEvent == 1) {
        CMessage Msg;
        Msg.m_nMsgType = MT_NETTEST;

        CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
        int nCount = pList->GetCount();
        POSITION pos = pList->GetHeadPosition();

        while(pos) {
            CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
            if(pClientInfo != NULL) {
                pClientInfo->m_pClientSock->Send(&Msg, sizeof(CMessage), MSG_OOB);
            }
        }
    }
    // 用户在线消息
    else if(nIDEvent == 2) {

        CPtrList *pList = &((CECSSrvApp *)AfxGetApp())->m_ClientList;
        int nCount = pList->GetCount();
        POSITION pos = pList->GetHeadPosition();
        while(pos) {
            POSITION tmpPos = pos;
            CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
            if(pClientInfo != NULL) {
                int &nNum = ((CClientSock *)pClientInfo->m_pClientSock)->m_nTestNum;
                BOOL &bTest = ((CClientSock *)pClientInfo->m_pClientSock)->m_bTesting;
                if(nNum == 10 && bTest == FALSE) {  // 向其他用户转发该用户在线消息
                    CMessage Msg;
                    Msg.m_nMsgType = MT_NETCONNECT;
                    int nID = atoi(((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID);
                    Msg.m_nAccountID[0] = nID;
                    POSITION nextPos = pList->GetHeadPosition();
                    while(nextPos) {
                        CClientInfo *pNextInfo = static_cast < CClientInfo *>(pList->GetNext(nextPos));
                        int nNextID = atoi(((CClientSock *)pNextInfo->m_pClientSock)->m_csAccountID);
                        if(pNextInfo != NULL && nNextID != nID) {   // 不向自己发送消息
                            pNextInfo->m_pClientSock->Send(&Msg, sizeof(CMessage), MSG_OOB);
                        }
                    }
                    ((CClientSock *)pClientInfo->m_pClientSock)->m_bTesting = FALSE;
                    ((CClientSock *)pClientInfo->m_pClientSock)->m_nTestNum = 0;
                }
                else if(bTest == TRUE && nNum < 4) { // 断线计时
                    ((CClientSock *)pClientInfo->m_pClientSock)->m_nTestNum++;
                }
                else if(bTest == TRUE && (nNum >= 4 && nNum < 10)) {    // 用户已掉线
                    POSITION nextPos = pList->GetHeadPosition();
                    CMessage Msg;
                    Msg.m_nMsgType = MT_DISCONNECT;
                    int nID = atoi(((CClientSock *)pClientInfo->m_pClientSock)->m_csAccountID);
                    Msg.m_nAccountID[0] = nID;
                    while(nextPos) {
                        CClientInfo *pNextInfo = static_cast < CClientInfo *>(pList->GetNext(nextPos));
                        int nNextID = atoi(((CClientSock *)pNextInfo->m_pClientSock)->m_csAccountID);
                        if(pNextInfo != NULL && nNextID != nID) {   // 不向自己发送消息
                            pNextInfo->m_pClientSock->Send(&Msg, sizeof(CMessage), MSG_OOB);
                        }
                    }
                    ((CClientSock *)pClientInfo->m_pClientSock)->m_bTesting = FALSE;
                    ((CClientSock *)pClientInfo->m_pClientSock)->m_nTestNum = 0;
                }
            }
        }
    }
    CDialog::OnTimer(nIDEvent);
}
