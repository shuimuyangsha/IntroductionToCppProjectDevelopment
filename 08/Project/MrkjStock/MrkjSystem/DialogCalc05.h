#pragma once
#include "afxwin.h"


// CDialogCalc05 对话框

class CDialogCalc05 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc05)

public:
    CDialogCalc05(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc05();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG5 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonCalc();
    afx_msg void OnCbnSelchangeCombo1();
    // 收入类型
    CComboBox m_cbbPattern;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
