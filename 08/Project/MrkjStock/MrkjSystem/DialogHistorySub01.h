#pragma once
#include "afxcmn.h"


// CDialogHistorySub01 对话框

class CDialogHistorySub01 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHistorySub01)

public:
    CDialogHistorySub01(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogHistorySub01();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HISTORY_SUB_01 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CListCtrl m_lst;
    enum {
        EListIndexCode = 0,
        EListIndexName,
        EListIndexUpPercent,
        EListIndexPrice,
        EListIndexMax,
        EListIndexOpen,
        EListIndexClose,
        EListIndexMin,
        EListIndexTurnover,
        EListIndexTradingVolume,
        EListIndexMaxLimit,
    };

    typedef tuple<CString, CString, CString, CString, CString, CString, CString, CString, CString, CString> TupleLineData;
    typedef vector<TupleLineData> VLineData;

    // 更新m_lst中的数据
    void UpdateListData(const VLineData& v);

    // 获得某行数据
    void GetListData(TupleLineData& data, int index);

public:
    //提供给父类进行调用的接口
    void UpdateStockData(const CString& stockCode, const CString& strDateStart, const CString strDateEnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
