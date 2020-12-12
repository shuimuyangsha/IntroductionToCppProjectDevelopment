// ContralDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "ContralDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContralDlg dialog


CContralDlg::CContralDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CContralDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CContralDlg)
    //}}AFX_DATA_INIT
}


void CContralDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CContralDlg)
    DDX_Control(pDX, IDC_BUTCOMPUTER, m_Computer);
    DDX_Control(pDX, IDC_BUTTIMEDATE, m_Timedate);
    DDX_Control(pDX, IDC_BUTSYSDM, m_Sysdm);
    DDX_Control(pDX, IDC_BUTMOUSE, m_Mouse);
    DDX_Control(pDX, IDC_BUTMMSYS, m_Mmsys);
    DDX_Control(pDX, IDC_BUTKEYBOARD, m_Keyboard);
    DDX_Control(pDX, IDC_BUTINTL, m_Intl);
    DDX_Control(pDX, IDC_BUTHDWWIZ, m_Hdwwiz);
    DDX_Control(pDX, IDC_BUTDESK, m_Desk);
    DDX_Control(pDX, IDC_BUTAPPWIZ, m_Appwiz);
    DDX_Control(pDX, IDC_BUTACCESS, m_Access);
    DDX_Control(pDX, IDC_BUTINTERNET, m_Internet);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContralDlg, CDialog)
    //{{AFX_MSG_MAP(CContralDlg)
    ON_BN_CLICKED(IDC_BUTINTERNET, OnButinternet)
    ON_BN_CLICKED(IDC_BUTMMSYS, OnButmmsys)
    ON_BN_CLICKED(IDC_BUTTIMEDATE, OnButtimedate)
    ON_BN_CLICKED(IDC_BUTDESK, OnButdesk)
    ON_BN_CLICKED(IDC_BUTACCESS, OnButaccess)
    ON_BN_CLICKED(IDC_BUTMOUSE, OnButmouse)
    ON_BN_CLICKED(IDC_BUTKEYBOARD, OnButkeyboard)
    ON_BN_CLICKED(IDC_BUTINTL, OnButintl)
    ON_BN_CLICKED(IDC_BUTAPPWIZ, OnButappwiz)
    ON_BN_CLICKED(IDC_BUTHDWWIZ, OnButhdwwiz)
    ON_BN_CLICKED(IDC_BUTSYSDM, OnButsysdm)
    ON_BN_CLICKED(IDC_BUTMODEM, OnButmodem)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContralDlg message handlers

BOOL CContralDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_Internet.SetImageIndex(0);            // Internet选项按钮显示图标
    m_Mmsys.SetImageIndex(1);               // 声音按钮显示图标
    m_Timedate.SetImageIndex(2);            // 时间和日期按钮显示图标
    m_Desk.SetImageIndex(3);                // 显示按钮显示图标
    m_Access.SetImageIndex(4);              // 辅助选项按钮显示图标
    m_Mouse.SetImageIndex(5);               // 鼠标按钮显示图标
    m_Keyboard.SetImageIndex(6);            // 键盘按钮显示图标
    m_Intl.SetImageIndex(7);                // 区域按钮显示图标
    m_Appwiz.SetImageIndex(8);              // 添加/删除程序按钮显示图标
    m_Hdwwiz.SetImageIndex(9);              // 添加硬件按钮显示图标
    m_Sysdm.SetImageIndex(10);              // 系统按钮显示图标
    m_Computer.SetImageIndex(11);           // 计算机管理按钮显示图标

    return TRUE;
}

// Internet选项按钮

void CContralDlg::OnButinternet()
{
    // 打开 IE 的设置窗口
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL inetcpl.cpl", NULL, SW_SHOW);
}
// 声音按钮
void CContralDlg::OnButmmsys()
{
    // 打开 声音的设置窗口
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL mmsys.cpl @1", NULL, SW_SHOW);
}
// 时间和日期按钮
void CContralDlg::OnButtimedate()
{
    // 启动日期和时间设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL timedate.cpl", NULL, SW_SHOW);
}
// 显示按钮
void CContralDlg::OnButdesk()
{
    // 启动显示设置面板
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL desk.cpl", NULL, SW_SHOW);
}
// 辅助选项按钮
void CContralDlg::OnButaccess()
{
    // 启动辅助选项
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL access.cpl", NULL, SW_SHOW);
}
// 鼠标按钮
void CContralDlg::OnButmouse()
{
    // 打开鼠标设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL main.cpl @0", NULL, SW_SHOW);
}
// 键盘按钮
void CContralDlg::OnButkeyboard()
{
    // 启动键盘设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL main.cpl @1", NULL, SW_SHOW);
}
// 区域按钮
void CContralDlg::OnButintl()
{
    // 打开区域设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL intl.cpl", NULL, SW_SHOW);
}
// 添加/删除程序按钮
void CContralDlg::OnButappwiz()
{
    // 启动添加软件设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL appwiz.cpl", NULL, SW_SHOW);
}
// 添加硬件按钮
void CContralDlg::OnButhdwwiz()
{
    // 启动添加硬件设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL hdwwiz.cpl", NULL, SW_SHOW);
}
// 系统按钮
void CContralDlg::OnButsysdm()
{
    // 打开系统设置
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL sysdm.cpl", NULL, SW_SHOW);
}
// 计算机管理按钮
void CContralDlg::OnButmodem()
{
    // 启动计算机管理设置
    ::ShellExecute(NULL, "OPEN", "compmgmt.msc",
                   "shell32.dll Control_RunDLL compmgmt.cpl", NULL, SW_SHOW);
}

HBRUSH CContralDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    CBitmap m_BKGround;
    m_BKGround.LoadBitmap(IDB_BITBLANK);
    if(nCtlColor == CTLCOLOR_DLG) {
        // 定义一个位图画刷
        CBrush m_Brush(&m_BKGround);
        CRect rect;
        GetClientRect(rect);
        // 选中画刷
        pDC->SelectObject(&m_Brush);
        // 填充客户区域
        pDC->FillRect(rect, &m_Brush);
        return m_Brush;
    }
    else {
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
    return hbr;
}
