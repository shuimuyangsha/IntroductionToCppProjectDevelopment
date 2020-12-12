// DialogCalc04.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc04.h"
#include "afxdialogex.h"


// CDialogCalc04 �Ի���

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


// CDialogCalc04 ��Ϣ�������
namespace DialogCalc04_hh
{
    // ����������
    double CalcRateMonth(double rateYear)
    {
        return rateYear / 12.0 / 100.0;
    }
    // �����»����(�ȶϢ���: ÿ�»���� = P * ( (R * (1+R)^N) / ((1+R)^N - 1) ))
    double CalcPayMonth(double P, double R, int N)
    {
        return P * ((R * std::pow(1 + R, N)) / (std::pow(1 + R, N) - 1));
    }
    // �������������
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

    // �����n���µ���Ϣ�� ʣ���� * ��Ϣ
    extern double CalcInterest(double P, double R, int N, int n);

    // �����n����Ӧ���ı���
    double CalcPaymentMoney(double P, double R, int N, int n)
    {
        // ÿ�½�� - ���µ���Ϣ
        return CalcPayMonth(P, R, N) - CalcInterest(P, R, N, n);
    }

    // �����n���µ���Ϣ��
    double CalcInterest(double P, double R, int N, int n)
    {
        //ÿ�»���
        double dPerMon = CalcPayMonth(P, R, N);
        // ��1�� = �ܶ� * ����
        if(n == 1) {
            return P * R;
        }
        // (�ܶ� - ǰ��n-1���¼�������֮��) * ����
        double dTotalPay = dPerMon - P * R;
        for(int i = 2; i <= n - 1; ++i) {
            //            ÿ�»������� - ÿ�»�����Ϣ
            dTotalPay += (dPerMon - CalcInterest(P, R, N, i));
        }
        return (P - dTotalPay) * R;
    }

    // ����� n����ʱ(δ��֮ʱ),ʣ��δ���ı��� : �����ܶ� - (�����ܶ� - ������Ϣ)
    double CalcOtherPrincipal(double P, double R, int N, int n)
    {
        //ÿ�»���
        double dPerMon = CalcPayMonth(P, R, N);
        // (�ܶ� - ǰ��n-1���¼�������֮��)
        double dTotalPay = 0;
        for(int i = 1; i <= n - 1; ++i) {
            //            ÿ�»������� - ÿ�»�����Ϣ
            dTotalPay += (dPerMon - CalcInterest(P, R, N, i));
        }
        return P - dTotalPay;
    }

    // �����ܹ����˶�����Ϣ��
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

// ����
void CDialogCalc04::OnBnClickedButtonCalc()
{
    UpdateData(TRUE);
    // �״λ�����
    COleDateTime dtFirst(m_iYearFirst, m_iMonthFirst, 1, 0, 0, 0);
    // ��ǰ��������
    COleDateTime dtAdvance(m_iYearAdvance, m_iMonthAdvance, 1, 0, 0, 0);
    if(dtFirst > dtAdvance) {
        AfxMessageBox(_T("��������"));
        return;
    }
    //������
    double rateMonth = CalcRateMonth(m_dRateYear);
    //����������
    int iMonth = CalcMonth(m_iYearFirst, m_iMonthFirst, m_iYearAdvance, m_iMonthAdvance);
    //��ǰ�����
    //m_dMoneyAdvancePayment =
    // ԭ�»����
    m_dPayPerMonth = CalcPayMonth(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12));
    // �����ܶ�
    m_dPaymentTotal = m_dPayPerMonth * iMonth;
    // ������Ϣ��
    m_dPayInterest = CalcTotalInterest(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth);
    /***********************************************************/
    //���ֻ��ʽ
    switch(m_cbbPaymentPattern.GetCurSel()) {
        case 0: {
                // ����һ�λ���� : ���½��  + ����ʣ�౾��
                m_dPayThisMonth = CalcOtherPrincipal(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth)
                                  + CalcInterest(m_dTotal, rateMonth, static_cast<int>(m_totalYear * 12), iMonth);
                // �������»����
                m_dPayNew = 0;
                // ��ʡ��Ϣ֧��
                {
                    int nTotalMonth = static_cast<int>(m_totalYear * 12);
                    m_nInterestDecrease = m_dPayPerMonth * nTotalMonth - m_dTotal - m_dPayInterest;
                }
                // �µ���󻹿�����(��)
                m_iNewLastYear = m_iYearAdvance;
                // �µ���󻹿�����(��)
                m_dNewLastMonth = m_iMonthAdvance;
                break;
            }
        default:
            break;
    }
    UpdateData(FALSE);
}


// ���
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
    m_cbbPaymentPattern.AddString(_T("һ������ǰ����"));
    //m_cbbPaymentPattern.AddString(_T("������ǰ����"));
    m_cbbPaymentPattern.SetCurSel(0);
    m_edtMoneyAdvancePayment.EnableWindow(FALSE);
    m_cbbProcessPattern.AddString(_T("���޲���,�»������"));
    m_cbbProcessPattern.AddString(_T("��������,�»����"));
    m_cbbProcessPattern.SetCurSel(0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogCalc04::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


void CDialogCalc04::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
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
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
