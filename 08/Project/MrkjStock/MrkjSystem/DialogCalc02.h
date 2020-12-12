#pragma once


// CDialogCalc02 对话框

class CDialogCalc02 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc02)

public:
    CDialogCalc02(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc02();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG2 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CString m_strCode;
    CString m_strName;
    afx_msg void OnEnKillfocusEdit1();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton4();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
