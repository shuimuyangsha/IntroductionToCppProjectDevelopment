#pragma once
#include "afxwin.h"


// CDialogCalc04 对话框

class CDialogCalc04 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc04)

public:
    CDialogCalc04(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc04();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG4 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonCalc();
    afx_msg void OnBnClickedButtonClear();
    // 贷款总额
    double m_dTotal;
    // 原贷款期限
    double m_totalYear;
    // 首次还款日期(年)
    int m_iYearFirst;
    // 首次还款日期(月)
    int m_iMonthFirst;
    // 提前还款日期(年)
    int m_iYearAdvance;
    // 提前还款日期(月)
    int m_iMonthAdvance;
    // 贷款年利率
    double m_dRateYear;
    // 还款方式
    CString m_strPaymentPattern;
    // 提前还款额
    double m_dMoneyAdvancePayment;
    // 处理方式
    CString m_strProcessPattern;
    // 原月还款额
    double m_dPayPerMonth;
    // 己还总额
    double m_dPaymentTotal;
    // 已还利息
    double m_dPayInterest;
    // 该月一次还款额
    double m_dPayThisMonth;
    // 下月起月还款额
    double m_dPayNew;
    // 节省利息支出
    double m_nInterestDecrease;
    // 新的最后还款日期(年)
    int m_iNewLastYear;
    // 新的最后还款日期(月)
    int m_dNewLastMonth;
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    CComboBox m_cbbPaymentPattern;
    CComboBox m_cbbProcessPattern;
    // 提前还款额
    CEdit m_edtMoneyAdvancePayment;
    afx_msg void OnCbnSelchangeComboPaymentPattern();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
