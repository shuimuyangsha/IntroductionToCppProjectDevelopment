// DialogCalc02.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc02.h"
#include "afxdialogex.h"


// CDialogCalc02 对话框

IMPLEMENT_DYNAMIC(CDialogCalc02, CDialogEx)

CDialogCalc02::CDialogCalc02(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG2, pParent)
    , m_strCode(_T(""))
    , m_strName(_T(""))
{
}

CDialogCalc02::~CDialogCalc02()
{
}

void CDialogCalc02::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_strCode);
    DDX_Text(pDX, IDC_EDIT2, m_strName);
}


BEGIN_MESSAGE_MAP(CDialogCalc02, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CDialogCalc02::OnEnKillfocusEdit1)
    ON_BN_CLICKED(IDC_BUTTON5, &CDialogCalc02::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogCalc02::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDialogCalc02 消息处理程序


void CDialogCalc02::OnEnKillfocusEdit1()
{
    UpdateData();
    std::wstring name;
    if(DB.QueryStockNameByCode(m_strCode, name)) {
        SetDlgItemText(IDC_EDIT2, name.c_str());
    }
}

void CDialogCalc02::OnBnClickedButton5()
{
    SetDlgItemText(IDC_EDIT1, _T(""));
    SetDlgItemText(IDC_EDIT2, _T(""));
    SetDlgItemText(IDC_EDIT9, _T(""));
    SetDlgItemText(IDC_EDIT10, _T(""));
    SetDlgItemText(IDC_EDIT11, _T(""));
}


void CDialogCalc02::OnBnClickedButton4()
{
    UpdateData();
    /* 计算式:
        涨停  = 价格 * 1.1
        跌停  = 价格 * 0.9
    */
    float fPrice;
    {
        CString sPrice;
        GetDlgItemText(IDC_EDIT9, sPrice);
        TCHAR * szEnd = nullptr;
        fPrice = _tcstof(sPrice, &szEnd);
    }
    float fDailyLimit = fPrice * 1.1f;
    float fDownLimit = fPrice * 0.9f;
    // 四舍五入 保留 两位小数
    using namespace std;
    SetDlgItemText(IDC_EDIT10, StringHelper::ToString(fDailyLimit, 2).c_str());
    SetDlgItemText(IDC_EDIT11, StringHelper::ToString(fDownLimit, 2).c_str());
    UpdateData(FALSE);
}


void CDialogCalc02::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogCalc02::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}
BOOL CDialogCalc02::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
