// DialogAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogAbout.h"
#include "afxdialogex.h"


// CDialogAbout 对话框

IMPLEMENT_DYNAMIC(CDialogAbout, CDialogEx)

CDialogAbout::CDialogAbout(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_ABOUT, pParent)
    , m_strAboutText(_T(""))
    , m_strCopyright(_T(""))
    , m_strPhone(_T(""))
    , m_strUrl(_T(""))
{
}

CDialogAbout::~CDialogAbout()
{
}

void CDialogAbout::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_ABOUT_TXT, m_strAboutText);
    DDX_Text(pDX, IDC_STATIC_ABOUT_COPYRIGHT, m_strCopyright);
    DDX_Text(pDX, IDC_STATIC_ABOUT_PHONE, m_strPhone);
    DDX_Text(pDX, IDC_STATIC_ABOUT_URL, m_strUrl);
}


BEGIN_MESSAGE_MAP(CDialogAbout, CDialogEx)
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogAbout 消息处理程序

BOOL CDialogAbout::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 标题
    {
        CString str;
        str.LoadString(IDS_STRING_SYSTEM_NAME);
        str = L"关于" + str;
        SetWindowText(str);
    }
    // 图标
    {
        // 没有
    }
    //  载入字符串
    {
        m_strAboutText.LoadString(IDS_STRING_ABOUT_TEXT);
        m_strPhone.LoadString(IDS_STRING_ABOUT_PHONE);
        m_strUrl.LoadString(IDS_STRING_ABOUT_URL);
        m_strCopyright.LoadString(IDS_STRING_ABOUT_COPYRIGHT);
    }
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogAbout::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CDialogAbout::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogAbout::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogAbout::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


HBRUSH CDialogAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}
