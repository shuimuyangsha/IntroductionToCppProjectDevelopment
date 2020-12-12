// DialogCalc03.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc03.h"
#include "afxdialogex.h"


// CDialogCalc03 对话框

IMPLEMENT_DYNAMIC(CDialogCalc03, CDialogEx)

CDialogCalc03::CDialogCalc03(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG3, pParent)
{
}

CDialogCalc03::~CDialogCalc03()
{
}

void CDialogCalc03::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogCalc03, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON5, &CDialogCalc03::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogCalc03::OnBnClickedButtonCalc)
END_MESSAGE_MAP()


// CDialogCalc03 消息处理程序


void CDialogCalc03::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT01, _T("0"));
    SetDlgItemText(IDC_EDIT02, _T("0"));
    SetDlgItemText(IDC_EDIT03, _T("0"));
    SetDlgItemText(IDC_EDIT04, _T("0"));
    SetDlgItemText(IDC_EDIT05, _T("0"));
    SetDlgItemText(IDC_EDIT06, _T("1"));
    SetDlgItemText(IDC_EDIT07, _T("2"));
    SetDlgItemText(IDC_EDIT08, _T("0"));
    SetDlgItemText(IDC_EDIT09, _T("0"));
    SetDlgItemText(IDC_EDIT10, _T("0"));
    SetDlgItemText(IDC_EDIT11, _T("0"));
    SetDlgItemText(IDC_EDIT12, _T("0"));
    SetDlgItemText(IDC_EDIT13, _T("0"));
    SetDlgItemText(IDC_EDIT14, _T("0"));
    SetDlgItemText(IDC_EDIT15, _T("0"));
}


// 获取控件的值,并传化为 double
double CDialogCalc03::GetDlgItemValueD(UINT ID)
{
    CString str;
    GetDlgItemText(ID, str);
    PTSTR szEndPtr = nullptr;
    return _tcstod(str, &szEndPtr);
}

// 设置控件的值
void CDialogCalc03::SetDlgItemValueD(UINT ID, double val)
{
    CString str;
    str.Format(_T("%.2lf"), val);
    SetDlgItemText(ID, str);
}



void CDialogCalc03::OnBnClickedButtonCalc()
{
    /*
        获取各个值
    */
    // 现有购房资金
    double d1 = GetDlgItemValueD(IDC_EDIT01);
    // 家庭月收入
    double d2 = GetDlgItemValueD(IDC_EDIT02);
    // 每月可用于购房支出
    double d3 = GetDlgItemValueD(IDC_EDIT03);
    // 计划还贷年限
    double d4 = GetDlgItemValueD(IDC_EDIT04);
    // 计划购房面积
    double d5 = GetDlgItemValueD(IDC_EDIT05);
    // 契税税率
    double d6 = GetDlgItemValueD(IDC_EDIT06);
    // 房屋公共维修基金收取比例
    double d7 = GetDlgItemValueD(IDC_EDIT07);
    // 检查个值的合法性
    {
        if(d4 < 1.0) {
            AfxMessageBox(_T("请输入合法年限"));
            return;
        }
    }
    // 开始计算,并赋值给控件
    {
        // 可购买房屋总价
        double d8 = (d1 * 10000 + d3) * 1.8;
        SetDlgItemValueD(IDC_EDIT8, d8);
        // 契税
        double d9 = d8 * d2 / 100.0;
        SetDlgItemValueD(IDC_EDIT9, d9);
        // 首 付款
        double d10 = d1 / d4;
        SetDlgItemValueD(IDC_EDIT10, d10);
        // 律师费
        double d11 = d10 * 0.02;
        SetDlgItemValueD(IDC_EDIT11, d11);
        // 可购买房屋单价
        double d12 = d10 / d5;
        SetDlgItemValueD(IDC_EDIT12, d12);
        // 公共维修基金
        double d13 = d10 * 0.003;
        SetDlgItemValueD(IDC_EDIT13, d13);
        // 保险费
        double d14 = d1 * 0.0035;
        SetDlgItemValueD(IDC_EDIT14, d14);
        // 抵押登记费 :此值固定
        //double d15 = 0;
        SetDlgItemText(IDC_EDIT15, _T("200 - 500"));
    }
}


BOOL CDialogCalc03::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    //清空数据
    OnBnClickedButtonClear();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CDialogCalc03::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogCalc03::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


BOOL CDialogCalc03::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
