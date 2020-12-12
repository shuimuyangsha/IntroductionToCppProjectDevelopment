#pragma once
#include "StockChart.h"


// CDialogHistorySub02 对话框

class CDialogHistorySub02 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHistorySub02)

public:
    CDialogHistorySub02(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogHistorySub02();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HISTORY_SUB_02 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CStockChart m_stockChart;
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    // 提供给父窗口调用的接口
    void UpdateKLine(const CString& strCode, const CString& strDateStart, const CString& strDateEnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
