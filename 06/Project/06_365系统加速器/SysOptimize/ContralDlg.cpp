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
    m_Internet.SetImageIndex(0);            // Internetѡ�ť��ʾͼ��
    m_Mmsys.SetImageIndex(1);               // ������ť��ʾͼ��
    m_Timedate.SetImageIndex(2);            // ʱ������ڰ�ť��ʾͼ��
    m_Desk.SetImageIndex(3);                // ��ʾ��ť��ʾͼ��
    m_Access.SetImageIndex(4);              // ����ѡ�ť��ʾͼ��
    m_Mouse.SetImageIndex(5);               // ��갴ť��ʾͼ��
    m_Keyboard.SetImageIndex(6);            // ���̰�ť��ʾͼ��
    m_Intl.SetImageIndex(7);                // ����ť��ʾͼ��
    m_Appwiz.SetImageIndex(8);              // ���/ɾ������ť��ʾͼ��
    m_Hdwwiz.SetImageIndex(9);              // ���Ӳ����ť��ʾͼ��
    m_Sysdm.SetImageIndex(10);              // ϵͳ��ť��ʾͼ��
    m_Computer.SetImageIndex(11);           // ���������ť��ʾͼ��

    return TRUE;
}

// Internetѡ�ť

void CContralDlg::OnButinternet()
{
    // �� IE �����ô���
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL inetcpl.cpl", NULL, SW_SHOW);
}
// ������ť
void CContralDlg::OnButmmsys()
{
    // �� ���������ô���
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL mmsys.cpl @1", NULL, SW_SHOW);
}
// ʱ������ڰ�ť
void CContralDlg::OnButtimedate()
{
    // �������ں�ʱ������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL timedate.cpl", NULL, SW_SHOW);
}
// ��ʾ��ť
void CContralDlg::OnButdesk()
{
    // ������ʾ�������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL desk.cpl", NULL, SW_SHOW);
}
// ����ѡ�ť
void CContralDlg::OnButaccess()
{
    // ��������ѡ��
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL access.cpl", NULL, SW_SHOW);
}
// ��갴ť
void CContralDlg::OnButmouse()
{
    // ���������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL main.cpl @0", NULL, SW_SHOW);
}
// ���̰�ť
void CContralDlg::OnButkeyboard()
{
    // ������������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL main.cpl @1", NULL, SW_SHOW);
}
// ����ť
void CContralDlg::OnButintl()
{
    // ����������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL intl.cpl", NULL, SW_SHOW);
}
// ���/ɾ������ť
void CContralDlg::OnButappwiz()
{
    // ��������������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL appwiz.cpl", NULL, SW_SHOW);
}
// ���Ӳ����ť
void CContralDlg::OnButhdwwiz()
{
    // �������Ӳ������
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL hdwwiz.cpl", NULL, SW_SHOW);
}
// ϵͳ��ť
void CContralDlg::OnButsysdm()
{
    // ��ϵͳ����
    ::ShellExecute(NULL, "OPEN", "rundll32.exe",
                   "shell32.dll Control_RunDLL sysdm.cpl", NULL, SW_SHOW);
}
// ���������ť
void CContralDlg::OnButmodem()
{
    // �����������������
    ::ShellExecute(NULL, "OPEN", "compmgmt.msc",
                   "shell32.dll Control_RunDLL compmgmt.cpl", NULL, SW_SHOW);
}

HBRUSH CContralDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    CBitmap m_BKGround;
    m_BKGround.LoadBitmap(IDB_BITBLANK);
    if(nCtlColor == CTLCOLOR_DLG) {
        // ����һ��λͼ��ˢ
        CBrush m_Brush(&m_BKGround);
        CRect rect;
        GetClientRect(rect);
        // ѡ�л�ˢ
        pDC->SelectObject(&m_Brush);
        // ���ͻ�����
        pDC->FillRect(rect, &m_Brush);
        return m_Brush;
    }
    else {
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
    return hbr;
}
