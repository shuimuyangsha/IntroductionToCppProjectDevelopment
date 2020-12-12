// DialogCalc01.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc01.h"
#include "afxdialogex.h"


// CDialogCalc01 对话框

IMPLEMENT_DYNAMIC(CDialogCalc01, CDialogEx)

CDialogCalc01::CDialogCalc01(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG1, pParent)
    , m_strCode(_T(""))
    , m_fSellPrice(0)
    , m_strName(_T(""))
    , m_fRate(0)
    , m_fPriceBuy(0)
    , m_fLossMoney(0)
    , m_iBuyCount(0)
    , m_fProfRate(0)
{
}

CDialogCalc01::~CDialogCalc01()
{
}

void CDialogCalc01::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_strCode);
    DDX_Text(pDX, IDC_EDIT2, m_fSellPrice);
    DDX_Text(pDX, IDC_EDIT3, m_strName);
    DDX_Text(pDX, IDC_EDIT4, m_fRate);
    DDX_Text(pDX, IDC_EDIT5, m_fPriceBuy);
    DDX_Text(pDX, IDC_EDIT6, m_fLossMoney);
    DDX_Text(pDX, IDC_EDIT7, m_iBuyCount);
    DDX_Text(pDX, IDC_EDIT8, m_fProfRate);
}


BEGIN_MESSAGE_MAP(CDialogCalc01, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogCalc01::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogCalc01::OnBnClickedButton1)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CDialogCalc01::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CDialogCalc01 消息处理程序


void CDialogCalc01::OnBnClickedButton2()
{
    m_strCode = _T("");
    m_fSellPrice = 0.0f;
    m_strName = _T("");
    m_fRate = 0.0f;
    m_fPriceBuy = 0;
    m_fLossMoney = 0;
    m_iBuyCount = 0;
    m_fProfRate = 0;
    UpdateData(FALSE);
}


void CDialogCalc01::OnBnClickedButton1()
{
    UpdateData(TRUE);
    /*
        计算方式:
        总额 = 买入数量 * 买入价格
        卖出金额 = 买入数量 * 卖出价格 *( 1 - 单边费率)
        盈亏金额 = 卖出金额 - 总额
        收益率 = 盈亏金额 / 总额 * 100
        // 注意  不要用整数算 '/'
    */
    try {
        float total = m_iBuyCount * m_fPriceBuy;
        float sell = m_iBuyCount * m_fSellPrice * (1 - m_fRate / 1000.0f);
        float loss = sell - total;
        if(total == 0) {
            throw("");
        }
        float fProfRate = loss / total * 100; //除0异常
        m_fLossMoney = loss;
        m_fProfRate = fProfRate;
    }
    catch(...) {
        TRACE("发生异常\r\n");
    }
    UpdateData(FALSE);
}


void CDialogCalc01::OnEnKillfocusEdit1()
{
    UpdateData(TRUE);
    std::wstring name;
    if(DB.QueryStockNameByCode(m_strCode, name)) {
        m_strName = name.c_str();
    }
    UpdateData(FALSE);
}


void CDialogCalc01::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogCalc01::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


BOOL CDialogCalc01::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
