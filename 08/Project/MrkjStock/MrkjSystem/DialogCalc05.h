#pragma once
#include "afxwin.h"


// CDialogCalc05 �Ի���

class CDialogCalc05 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc05)

public:
    CDialogCalc05(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogCalc05();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG5 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonCalc();
    afx_msg void OnCbnSelchangeCombo1();
    // ��������
    CComboBox m_cbbPattern;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
