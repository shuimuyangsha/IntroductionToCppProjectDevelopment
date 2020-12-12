#pragma once
#include "afxwin.h"


// CDialogCalc04 �Ի���

class CDialogCalc04 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc04)

public:
    CDialogCalc04(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogCalc04();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG4 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonCalc();
    afx_msg void OnBnClickedButtonClear();
    // �����ܶ�
    double m_dTotal;
    // ԭ��������
    double m_totalYear;
    // �״λ�������(��)
    int m_iYearFirst;
    // �״λ�������(��)
    int m_iMonthFirst;
    // ��ǰ��������(��)
    int m_iYearAdvance;
    // ��ǰ��������(��)
    int m_iMonthAdvance;
    // ����������
    double m_dRateYear;
    // ���ʽ
    CString m_strPaymentPattern;
    // ��ǰ�����
    double m_dMoneyAdvancePayment;
    // ����ʽ
    CString m_strProcessPattern;
    // ԭ�»����
    double m_dPayPerMonth;
    // �����ܶ�
    double m_dPaymentTotal;
    // �ѻ���Ϣ
    double m_dPayInterest;
    // ����һ�λ����
    double m_dPayThisMonth;
    // �������»����
    double m_dPayNew;
    // ��ʡ��Ϣ֧��
    double m_nInterestDecrease;
    // �µ���󻹿�����(��)
    int m_iNewLastYear;
    // �µ���󻹿�����(��)
    int m_dNewLastMonth;
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    CComboBox m_cbbPaymentPattern;
    CComboBox m_cbbProcessPattern;
    // ��ǰ�����
    CEdit m_edtMoneyAdvancePayment;
    afx_msg void OnCbnSelchangeComboPaymentPattern();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
