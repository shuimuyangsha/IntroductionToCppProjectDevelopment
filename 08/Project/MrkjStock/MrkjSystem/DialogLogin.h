#pragma once


// CDialogLogin 对话框

class CDialogLogin : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogLogin)

public:
    CDialogLogin(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogLogin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedCheckRecord();
	// 用户名
    CString m_strUsername;
	// 密码
    CString m_strPassword;
	// 是否记住 密码
    BOOL m_bRecordPass;
	// 记录信息 到配置文件中
    void RecordInfo();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
