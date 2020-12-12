#pragma once


// CDialogCalc01 对话框

class CDialogCalc01 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc01)

public:
    CDialogCalc01(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc01();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CString m_strCode;
    float m_fSellPrice;
    CString m_strName;
    float m_fRate;
    float m_fPriceBuy;
    float m_fLossMoney;
    int m_iBuyCount;
    float m_fProfRate;
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnEnKillfocusEdit1();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
