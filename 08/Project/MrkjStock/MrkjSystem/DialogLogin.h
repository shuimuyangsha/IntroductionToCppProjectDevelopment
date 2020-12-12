#pragma once


// CDialogLogin �Ի���

class CDialogLogin : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogLogin)

public:
    CDialogLogin(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogLogin();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedCheckRecord();
	// �û���
    CString m_strUsername;
	// ����
    CString m_strPassword;
	// �Ƿ��ס ����
    BOOL m_bRecordPass;
	// ��¼��Ϣ �������ļ���
    void RecordInfo();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
