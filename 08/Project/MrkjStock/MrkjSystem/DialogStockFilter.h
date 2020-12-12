#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CDialogStockFilter 对话框

class CDialogStockFilter : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogStockFilter)

public:
    CDialogStockFilter(CWnd *pParent = NULL);   // 标准构造函数
    virtual ~CDialogStockFilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_STOCK_FILTER };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    // 条件选择列表框
    CComboBox m_comboFilter;
    // 选择不同的combobox选项，显示不同的页面
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
    // 选中的条件
    CListBox m_listFilters;
    afx_msg void OnBnClickedButtonFilterDel();
    afx_msg void OnBnClickedButtonDo();
    BOOL m_bFilter{FALSE};
    CDateTimeCtrl m_dtStart;
    CDateTimeCtrl m_dtEnd;
    // 过滤进程
    CProgressCtrl m_process;

    // 获得条件，并启动线程
    BOOL DoFilter(BOOL bFilter);

    // 条件选股选择出来的股票
    std::vector<std::wstring> m_vFilterStocks;

    // 处理其它线要程的消息
    afx_msg LRESULT OnOtherThreadMessage(WPARAM, LPARAM);
    afx_msg void OnBnClickedButtonOk();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedButtonFilterRound();
    // 股票范围
    CListBox m_lstBoxKinds;

    // 获取股票范围
    void GetKinds(std::vector<int>& v);
};
