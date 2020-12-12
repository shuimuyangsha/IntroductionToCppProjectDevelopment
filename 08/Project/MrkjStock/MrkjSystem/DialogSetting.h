#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDialogSetting �Ի���

class CDialogSetting : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogSetting)

public:
    CDialogSetting(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDialogSetting();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
    // �û��б�
    CListCtrl m_lst;
    enum {
        EListIndexID = 0,
        EListIndexUsername,
        EListIndexPassword,
        EListIndexP001,
        EListIndexP002,
        EListIndexP003,
        EListIndexP004,
        EListIndexP005,
        EListIndexP006,
        EListIndexP007,
        EListIndexP008,
        EListIndexP009,
        EListIndexP010,
        EListIndexMaxLimit,
    };

    // ����m_lst�е�����
    void UpdateListData();
    void UpdateListData(const CMRKJDatabase::VDBEntryUser& v);
    // ���ĳ������
    void GetListData(CMRKJDatabase::DBEntryUser& data, int index);
    // ����m_lst����
    void ClearList();

    // ���
    CString m_strID;
    // �û���
    CString m_strUsername;
    // ����
    CString m_strPassword;
    // ϵͳ
    CButton m_btn001;
    BOOL m_b001;
    // ��������ά��
    CButton m_btn002;
    BOOL m_b002;
    // Ʒ��ά��
    CButton m_btn003;
    BOOL m_b003;
    // ���뵼������
    CButton m_btn004;
    BOOL m_b004;
    // ��ѯ
    CButton m_btn005;
    BOOL m_b005;
    // ��ʱ����
    CButton m_btn006;
    BOOL m_b006;
    // ����
    CButton m_btn007;
    BOOL m_b007;
    // ���ڼ�����
    CButton m_btn008;
    BOOL m_b008;
    // ��Ļ��ͼ
    CButton m_btn009;
    BOOL m_b009;
    // ϵͳ����
    CButton m_btn010;
    BOOL m_b010;
private:
	// ��ȡѡ����
    int GetSelectLineIndex();
public:
    afx_msg void OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedCheckSystem();
    afx_msg void OnBnClickedCheckTools();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnDestroy();
};

