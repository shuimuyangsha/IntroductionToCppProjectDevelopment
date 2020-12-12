#pragma once
#include "afxcmn.h"


// CDialogFilterKind �Ի���

class CDialogFilterKind : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogFilterKind)

public:
    CDialogFilterKind(CWnd *pParent = NULL);   // ��׼���캯��
    virtual ~CDialogFilterKind();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_STOCK_ROUND };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    // ��Ʊ����
    CListCtrl m_lst;
    afx_msg void OnBnClickedButtonOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonCancel2();

    // ѡ���
    std::vector<std::tuple<int, std::wstring>> m_vKinds;
};
