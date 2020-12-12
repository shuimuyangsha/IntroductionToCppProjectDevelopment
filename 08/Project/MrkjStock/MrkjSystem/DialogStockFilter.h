#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CDialogStockFilter �Ի���

class CDialogStockFilter : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogStockFilter)

public:
    CDialogStockFilter(CWnd *pParent = NULL);   // ��׼���캯��
    virtual ~CDialogStockFilter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_STOCK_FILTER };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    // ����ѡ���б��
    CComboBox m_comboFilter;
    // ѡ��ͬ��comboboxѡ���ʾ��ͬ��ҳ��
    enum {IDC_EDIT_UPN_DAYS = 9999,
          IDC_STATIC_UPN_DAYS0,
          IDC_STATIC_UPN_DAYS1,
          IDC_EDIT_DOWNN_DAYS,
          IDC_STATIC_DOWNN_DAYS0,
          IDC_STATIC_DOWNN_DAYS1,
         };

    void UPN(BOOL bShow = TRUE);
    CEdit m_edt_upn_days;
    CStatic m_stt_upn_days0;
    CStatic m_stt_upn_days1;

    void DOWNN(BOOL bShow = TRUE);
    CEdit m_edt_downn_days;
    CStatic m_stt_downn_days0;
    CStatic m_stt_downn_days1;

    afx_msg void OnCbnSelchangeComboFilter();
    afx_msg void OnBnClickedButtonFilterAdd();
    // ѡ�е�����
    CListBox m_listFilters;
    afx_msg void OnBnClickedButtonFilterDel();
    afx_msg void OnBnClickedButtonDo();
    BOOL m_bFilter{FALSE};
    CDateTimeCtrl m_dtStart;
    CDateTimeCtrl m_dtEnd;
    // ���˽���
    CProgressCtrl m_process;

    // ����������������߳�
    BOOL DoFilter(BOOL bFilter);

    // ����ѡ��ѡ������Ĺ�Ʊ
    std::vector<std::wstring> m_vFilterStocks;

    // ����������Ҫ�̵���Ϣ
    afx_msg LRESULT OnOtherThreadMessage(WPARAM, LPARAM);
    afx_msg void OnBnClickedButtonOk();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedButtonFilterRound();
    // ��Ʊ��Χ
    CListBox m_lstBoxKinds;

    // ��ȡ��Ʊ��Χ
    void GetKinds(std::vector<int>& v);
};
