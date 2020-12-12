#pragma once
#include "afxwin.h"


// CDialogKeybdQuery �Ի���

class CDialogKeybdQuery : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogKeybdQuery)

public:
    CDialogKeybdQuery(const CString& strInput, int x, int y, CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogKeybdQuery();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_KEYBD_QUERY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
private:
    /*��ʼ��ʱ����*/
    // ��һ������ĵ�һ����ĸ
    CString m_strQueryInput;
    // ������ʾ�����½�λ��
    int m_x, m_y;

    /*�Ի����˳�ʱ����ѡ�е�����*/
    CString m_strQueryResultSel;

public:
    // ����û���ѡ��:��һ����Ʊ����
    CString GetResult()
    {
        return m_strQueryResultSel;
    }

    // ��ѯ������
    CEdit m_edtQueryInput;
    // ��ѯ�Ľ��
    CListBox m_lstBoxQueryResult;

    // �Ի����ʼ��
    virtual BOOL OnInitDialog();
    // �������ݷ����仯
    afx_msg void OnEnChangeEditQueryInput();
    // ˫���˲�ѯ���
    afx_msg void OnLbnDblclkListQueryResult();
    afx_msg void OnEnSetfocusEditQueryInput();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
};
