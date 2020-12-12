// DialogCalc05.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc05.h"
#include "afxdialogex.h"


// CDialogCalc05 对话框

IMPLEMENT_DYNAMIC(CDialogCalc05, CDialogEx)

CDialogCalc05::CDialogCalc05(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG5, pParent)
{
}

CDialogCalc05::~CDialogCalc05()
{
}

void CDialogCalc05::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_cbbPattern);
}


BEGIN_MESSAGE_MAP(CDialogCalc05, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogCalc05::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogCalc05::OnBnClickedButtonCalc)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CDialogCalc05::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDialogCalc05 消息处理程序


void CDialogCalc05::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogCalc05::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}
/*********************************************************************************/
// TODO
// 税率表:...


// 计算公式 : 工资所得
double __stdcall Fun1(double dTotal, double dOther, double dStart)
{
    double rate[] = {0.03, 0.1, 0.2, 0.25, 0.3, 0.35, 0.45};
    double mon[] = { 0, 105, 555, 1005, 2755, 5505, 13505 };
    auto GetIndex = [dStart](double mm)->int {
        if(mm - dStart >= 80000)
        {
            return 6;
        }
        if(mm - dStart >= 55000)
        {
            return 5;
        }
        if(mm - dStart >= 35000)
        {
            return 4;
        }
        if(mm - dStart >= 9000)
        {
            return 3;
        }
        if(mm - dStart >= 4500)
        {
            return 2;
        }
        if(mm - dStart >= 1500)
        {
            return 1;
        }
        return 0;
    };
    int index = GetIndex(dTotal);
    // 应纳税额＝（工资薪金所得 －“五险一金”－扣除数）×适用税率－速算扣除数
    return (dTotal - dOther - dStart) * rate[index] - mon[index];
}

// 计算公式 : 财产转让
double __stdcall Fun2(double dTotal, double dReal, double dOther)
{
    return std::max<double>((dTotal - dReal - dOther) * 0.20, 0);
}
double __stdcall Fun3(double money0, double money1, double money2)
{
    return std::max<double>(money0 * 0.2, 0);
}

typedef double (__stdcall *PFUN)(double money0, double money1, double money2);
struct {
    CString text;
    PFUN pFun;
} TEXT_FUN[] = {
    {_T("工资,薪金所得"), Fun1},
    {_T("财产转让所得"), Fun2 },
    {_T("财产租赁所得"), Fun3 },
};
// 根据名字获得计算公式
PFUN GetFun(const CString& str)
{
    for(auto km : TEXT_FUN) {
        if(km.text == str) {
            return km.pFun;
        }
    }
    return nullptr;
}

// 计算公式 End.
/*********************************************************************************/

BOOL CDialogCalc05::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 初始化控件
    {
        CComboBox * p = (CComboBox*)GetDlgItem(IDC_COMBO1);
        assert(p);
        for each(auto km in TEXT_FUN) {
            p->AddString(km.text);
        }
        p->SetCurSel(0);
    }
    SetDlgItemText(IDC_EDIT1, _T("0"));
    SetDlgItemText(IDC_EDIT2, _T("0"));
    SetDlgItemText(IDC_EDIT3, _T("3500"));
    SetDlgItemText(IDC_EDIT4, _T("0"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CDialogCalc05::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT1, _T("0"));
    SetDlgItemText(IDC_EDIT2, _T("0"));
    SetDlgItemText(IDC_EDIT3, _T("0"));
    SetDlgItemText(IDC_EDIT4, _T("0"));
}


void CDialogCalc05::OnBnClickedButtonCalc()
{
    // 获得 收入类型,并根据收入类型, 获得计算方式
    CString str;
    CComboBox * p = (CComboBox*)GetDlgItem(IDC_COMBO1);
    assert(p);
    if(p->GetCurSel() >= 0) {
        p->GetLBText(p->GetCurSel(), str);
    }
    PFUN pFun = GetFun(str);
    if(pFun) {
        CString s1, s2, s3;
        GetDlgItemText(IDC_EDIT1, s1);
        GetDlgItemText(IDC_EDIT2, s2);
        GetDlgItemText(IDC_EDIT3, s3);
        using StringHelper::StringTo;
        double d1 = StringTo<double>(s1.GetString());
        double d2 = StringTo<double>(s2.GetString());
        double d3 = StringTo<double>(s3.GetString());
        double fRes = pFun(d1, d2, d3);
        CString s4;
        s4.Format(_T("%lf"), fRes);
        SetDlgItemText(IDC_EDIT4, s4);
    }
    else {
        CTools::MessageBoxFormat(_T("没有计算公式:'%s'"), str.GetString());
    }
}



void CDialogCalc05::OnCbnSelchangeCombo1()
{
    UpdateData(TRUE);
    CWnd* pA = GetDlgItem(IDC_STATIC_A);
    CWnd* pB = GetDlgItem(IDC_STATIC_B);
    CWnd* pEdtA = GetDlgItem(IDC_EDIT2);
    CWnd* pEdtB = GetDlgItem(IDC_EDIT3);
    assert(pA);
    assert(pB);
    switch(m_cbbPattern.GetCurSel()) {
        case 0: {
                pA->SetWindowText(_T("各项保险:"));
                pB->SetWindowText(_T("起征额:"));
                pEdtA->EnableWindow(TRUE);
                pEdtB->EnableWindow(TRUE);
                break;
            }
        case 1: {
                pA->SetWindowText(_T("财产原值:"));
                pB->SetWindowText(_T("其它费用:"));
                pEdtA->EnableWindow(TRUE);
                pEdtB->EnableWindow(TRUE);
                break;
            }
        case 2: {
                pEdtA->EnableWindow(FALSE);
                pEdtB->EnableWindow(FALSE);
                break;
                break;
            }
    }
}


BOOL CDialogCalc05::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
