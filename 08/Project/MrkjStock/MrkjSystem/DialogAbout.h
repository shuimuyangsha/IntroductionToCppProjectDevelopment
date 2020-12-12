#pragma once


// CDialogAbout �Ի���

class CDialogAbout : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogAbout)

public:
    CDialogAbout(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogAbout();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ABOUT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnOK();
    virtual void OnCancel();

	// �ı�
    CString m_strAboutText;
	// ��Ȩ��Ϣ
    CString m_strCopyright;
	// �绰
    CString m_strPhone;
	// ��ַ
    CString m_strUrl;

public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
