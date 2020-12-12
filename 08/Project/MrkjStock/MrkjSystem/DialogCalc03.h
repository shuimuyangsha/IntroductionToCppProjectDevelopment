#pragma once


// CDialogCalc03 对话框

class CDialogCalc03 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc03)

public:
    CDialogCalc03(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc03();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG3 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonCalc();

    // 获取控件的值,并传化为 double
    double GetDlgItemValueD(UINT ID);
    // 设置控件的值
    void SetDlgItemValueD(UINT ID, double val);

    virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
