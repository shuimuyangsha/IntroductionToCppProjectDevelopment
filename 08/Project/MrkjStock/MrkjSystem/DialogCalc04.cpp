// DialogCalc04.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc04.h"
#include "afxdialogex.h"


// CDialogCalc04 对话框

IMPLEMENT_DYNAMIC(CDialogCalc04, CDialogEx)

CDialogCalc04::CDialogCalc04(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG4, pParent)
    , m_dTotal(100 * 10000)
    , m_totalYear(10)
    , m_iYearFirst(1999)
    , m_iMonthFirst(1)
    , m_iYearAdvance(1999)
    , m_iMonthAdvance(11)
    , m_dRateYear(10)
    , m_strPaymentPattern(_T(""))
    , m_dMoneyAdvancePayment(0)
    , m_strProcessPattern(_T(""))
    , m_dPayPerMonth(0)
    , m_dPaymentTotal(0)
    , m_dPayInterest(0)
    , m_dPayThisMonth(0)
    , m_dPayNew(0)
    , m_nInterestDecrease(0)
    , m_iNewLastYear(0)
    , m_dNewLastMonth(0)
{
}

CDialogCalc04::~CDialogCalc04()
{
}

void CDialogCalc04::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_dTotal);
    DDX_Text(pDX, IDC_EDIT2, m_totalYear);
    DDX_Text(pDX, IDC_EDIT9, m_iYearFirst);
    DDX_Text(pDX, IDC_EDIT10, m_iMonthFirst);
    DDX_Text(pDX, IDC_EDIT11, m_iYearAdvance);
    DDX_Text(pDX, IDC_EDIT16, m_iMonthAdvance);
    DDX_Text(pDX, IDC_EDIT5, m_dRateYear);
    DDX_CBString(pDX, IDC_COMBO1, m_strPaymentPattern);
    DDX_Text(pDX, IDC_EDIT6, m_dMoneyAdvancePayment);
    DDX_CBString(pDX, IDC_COMBO2, m_strProcessPattern);
    DDX_Text(pDX, IDC_EDIT7, m_dPayPerMonth);
    DDX_Text(pDX, IDC_EDIT8, m_dPaymentTotal);
    DDX_Text(pDX, IDC_EDIT17, m_dPayInterest);
    DDX_Text(pDX, IDC_EDIT18, m_dPayThisMonth);
    DDX_Text(pDX, IDC_EDIT19, m_dPayNew);
    DDX_Text(pDX, IDC_EDIT20, m_nInterestDecrease);
    DDX_Text(pDX, IDC_EDIT21, m_iNewLastYear);
    DDX_Text(pDX, IDC_EDIT22, m_dNewLastMonth);
    DDX_Control(pDX, IDC_COMBO1, m_cbbPaymentPattern);
    DDX_Control(pDX, IDC_COMBO2, m_cbbProcessPattern);
    DDX_Control(pDX, IDC_EDIT6, m_edtMoneyAdvancePayment);
}


BEGIN_MESSAGE_MAP(CDialogCalc04, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON7, &CDialogCalc04::OnBnClickedButtonCalc)
    ON_BN_CLICKED(IDC_BUTTON6, &CDialogCalc04::OnBnClickedButtonClear)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CDialogCalc04::OnCbnSelchangeComboPaymentPattern)
END_MESSAGE_MAP()


// CDialogCalc04 消息处理程序
namespace DialogCalc04_hh
{
    // 计算月利率
    double CalcRateMonth(double rateYear)
    {
        return rateYear / 12.0 / 100.0;
    }
    // 计算月还款额(等额本息还款法: 每月还款额 = P * ( (R * (1+R)^N) / ((1+R)^N - 1) ))
    double CalcPayMonth(double P, double R, int N)
    {
        return P * ((R * std::pow(1 + R, N)) / (std::pow(1 + R, N) - 1));
    }
    // 计算相隔几个月
    int CalcMonth(int iYearFirst, int iMonthFirst, int iYearAdvance, int iMonthAdvance)
    {
        if(iYearFirst == iYearAdvance) {
            return iMonthAdvance - iMonthFirst;
        }
        else {
            return  12 * (iYearAdvance - 1 - iYearFirst)
                    + (iMonthAdvance - iMonthFirst);
        }
    }

    // 计算第n个月的利息额 剩余金额 * 利息
    extern double CalcInterest(double P, double R, int N, int n);

    // 计算第n个月应还的本金
    double CalcPaymentMoney(double P, double R, int N, int n)
    {
        // 每月金额 - 当月的利息
        return CalcPayMonth(P, R, N) - CalcInterest(P, R, N, n);
    }

    // 计算第n个月的利息额
    double CalcInterest(double P, double R, int N, int n)
    {
        //每月还款
        double dPerMon = CalcPayMonth(P, R, N);
        // 第1月 = 总额 * 利率
        if(n == 1) {
            return P * R;
        }
        // (总额 - 前面n-1个月己还本金之和) * 利率
        double dTotalPay = dPerMon - P * R;
        for(int i = 2; i <= n - 1; ++i) {
            //            每月还款数量 - 每月还款利息
            dTotalPay += (dPerMon - CalcInterest(P, R, N, i));
        }
        return (P - dTotalPay) * R;
    }

    // 计算第 n个月时(未还之时),剩下未还的本金 : 贷款总额 - (己还总额 - 己还利息)
    double CalcOtherPrincipal(double P, double R, int N, int n)
    {
        //每月还款
        double dPerMon = CalcPayMonth(P, R, N);
        // (总额 - 前面n-1个月己还本金之和)
        double dTotalPay = 0;
        for(int i = 1; i <= n - 1; ++i) {
            //            每月还款数量 - 每月还款利息
            dTotalPay += (dPerMon - CalcInterest(P, R, N, i));
        }
        return P - dTotalPay;
    }

    // 计算总共还了多少利息额
    double CalcTotalInterest(double P, double R, int N, int n)
    {
        double d = 0;
        for(int i = 1; i <= n; ++i) {
            d += CalcInterest(P, R, N, i);
        }
        return d;
    }

}
using namespace DialogCalc04_hh;

// 计算
void CDialogCalc04::OnBnClickedButtonCalc()
{
    UpdateData(TRUE);
    // 首次还款日
    COleDateTime dtFirst(m_iYearFirst, m_iMonthFirst, 1, 0, 0, 0);
    // 提前还款日期
    COleDateTime dtAdvance(m_iYearAdvance, m_iMonthAdvance, 1, 0, 0, 0);
    if(dtFirst > dtAdvance) {
        AfxMessageBox(_T("请检查日期"));
        return;
    }
    //月利率
    double rateMonth = CalcRateMonth(m_dRateYear);
    //己还几个月
    int iMonth = CalcMonth(m_iYearFirst, m_iMonthFirst, m_iYearAdvance, m_iMonthAdvance);
    //提前还款额
    //m_dMoneyAdvancePayment =
    // 原月还款额
    m_dPayPerMonth = CalcPayMonth(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12));
    // 己还总额
    m_dPaymentTotal = m_dPayPerMonth * iMonth;
    // 己还利息额
    m_dPayInterest = CalcTotalInterest(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth);
    /***********************************************************/
    //两种还款方式
    switch(m_cbbPaymentPattern.GetCurSel()) {
        case 0: {
                // 该月一次还款额 : 本月金额  + 所有剩余本金
                m_dPayThisMonth = CalcOtherPrincipal(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth)
                                  + CalcInterest(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth);
                // 下月起月还款额
                m_dPayNew = 0;
                // 节省利息支出
                {
                    int nTotalMonth = static_cast<int>(m_totalYear * 12);
                    m_nInterestDecrease = m_dPayPerMonth * nTotalMonth - m_dTotal - m_dPayInterest;
                }
                // 新的最后还款日期(年)
                m_iNewLastYear = m_iYearAdvance;
                // 新的最后还款日期(月)
                m_dNewLastMonth = m_iMonthAdvance;
                break;
            }
        default:
            break;
    }
    UpdateData(FALSE);
}


// 清空
void CDialogCalc04::OnBnClickedButtonClear()
{
    UpdateData(TRUE);
    m_dTotal = (0);
    m_totalYear = (0);
    m_iYearFirst = (0);
    m_iMonthFirst = (0);
    m_iYearAdvance = (0);
    m_iMonthAdvance = (0);
    m_dRateYear = (0);
    m_dMoneyAdvancePayment = (0);
    m_dPayPerMonth = (0);
    m_dPaymentTotal = (0);
    m_dPayInterest = (0);
    m_dPayThisMonth = (0);
    m_dPayNew = (0);
    m_nInterestDecrease = (0);
    m_iNewLastYear = (0);
    m_dNewLastMonth = (0);
    UpdateData(FALSE);
}


BOOL CDialogCalc04::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    UpdateData(FALSE);
    m_cbbPaymentPattern.AddString(_T("一次性提前还清"));
    //m_cbbPaymentPattern.AddString(_T("部分提前还款"));
    m_cbbPaymentPattern.SetCurSel(0);
    m_edtMoneyAdvancePayment.EnableWindow(FALSE);
    m_cbbProcessPattern.AddString(_T("期限不变,月还款减少"));
    m_cbbProcessPattern.AddString(_T("期限缩短,月还款不变"));
    m_cbbProcessPattern.SetCurSel(0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CDialogCalc04::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


void CDialogCalc04::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogCalc04::OnCbnSelchangeComboPaymentPattern()
{
    UpdateData(TRUE);
    int cur = m_cbbPaymentPattern.GetCurSel();
    if(cur == 0) {
        m_edtMoneyAdvancePayment.EnableWindow(FALSE);
    }
    else {
        m_edtMoneyAdvancePayment.EnableWindow(TRUE);
    }
    UpdateData(FALSE);
}


BOOL CDialogCalc04::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
