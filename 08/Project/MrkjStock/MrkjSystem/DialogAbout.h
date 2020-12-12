#pragma once


// CDialogAbout 对话框

class CDialogAbout : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogAbout)

public:
    CDialogAbout(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogAbout();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ABOUT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnOK();
    virtual void OnCancel();

	// 文本
    CString m_strAboutText;
	// 版权信息
    CString m_strCopyright;
	// 电话
    CString m_strPhone;
	// 网址
    CString m_strUrl;

public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
