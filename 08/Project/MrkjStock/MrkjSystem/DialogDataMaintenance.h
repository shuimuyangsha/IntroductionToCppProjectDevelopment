#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxcmn.h"


// CDialogDataMaintenance 对话框

class CDialogDataMaintenance : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDataMaintenance)

public:
    CDialogDataMaintenance(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogDataMaintenance();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DATA_MAINTENANCE };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    // 股票代码
    CEdit m_edtCode;
    CString m_strCode;

    // 股票名称
    CEdit m_edtName;
    CString m_strName;

    // 开始日期
    CDateTimeCtrl m_dtcDateStart;
    COleDateTime m_odtDateStart;

    // 结束日期
    CDateTimeCtrl m_dtcDateEnd;
    COleDateTime m_odtDateEnd;

    // 股票品种
    CComboBox m_cbbKind;
    CString m_strKind;

    // 开盘价
    CEdit m_edtOpen;
    CString m_strOpen;

    // 收盘价
    CEdit m_edtClose;
    CString m_strClose;

    // 最高价
    CEdit m_edtHigh;
    CString m_strHigh;

    // 最低价
    CEdit m_edtLow;
    CString m_strLow;

    // 成交量
    CEdit m_edtTurnover;
    CString m_strTurnover;

    // 成交额
    CEdit m_edtTradingvolume;
    CString m_strTradingvolume;
    // 股票的历史数据
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

    // 保存旧的数据
    CString m_strOldDate{_T("")};
    // 修改按钮
    CButton m_btnUpdate;
    // 保存按钮
    CButton m_btnSave;

	// 更新种类信息
	void UpdateKindList();
    // 更新m_lst中的数据
    void UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info);
    // 获得某行数据
    void GetListData(CDayDataEntry& data, int index);
    // 更新股票信息
    void UpdateStockInfo(const CMRKJDatabase::TupleStockInfo& info);
    // 获得当前选择的 种类id
    bool GetKindIdSel(int& id);
    // 检查控件变量的合法性 : 日线数据
    bool IsInputValid();

    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg void OnBnClickedButtonSave();
    afx_msg void OnBnClickedButtonDel();
    afx_msg void OnBnClickedButtonQueryStockInfo();
    afx_msg void OnBnClickedButtonAddStockInfo();
    afx_msg void OnBnClickedButtonUpdateStockInfo();
    afx_msg void OnBnClickedButtonDelteStockInfo();
    afx_msg void OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
