#pragma once


// CDialogCalc03 �Ի���

class CDialogCalc03 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc03)

public:
    CDialogCalc03(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogCalc03();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG3 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonCalc();

    // ��ȡ�ؼ���ֵ,������Ϊ double
    double GetDlgItemValueD(UINT ID);
    // ���ÿؼ���ֵ
    void SetDlgItemValueD(UINT ID, double val);

    virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
