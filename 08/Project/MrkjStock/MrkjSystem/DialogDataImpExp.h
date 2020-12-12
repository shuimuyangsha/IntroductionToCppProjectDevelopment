#pragma once
#include "afxcmn.h"


// CDialogDataImpExp 对话框

class CDialogDataImpExp : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDataImpExp)

public:
    CDialogDataImpExp(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogDataImpExp();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DATA_IMPEXP };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    virtual BOOL OnInitDialog();
    // 导入文件路径
    CString m_strImportFilePath;
    // 导出文件路径
    CString m_strExportFilePath;
    // 股票代码
    CString m_strCode;
    // 股票的历史数据
    CListCtrl m_lst;
    enum {
        EListIndexCode = 0,
        EListIndexName,
        EListIndexUpPercent,
        EListIndexPrice,
        EListIndexOpen,
        EListIndexClose,
        EListIndexMax,
        EListIndexMin,
        EListIndexTurnover,
        EListIndexTradingVolume,
        EListIndexMaxLimit,
    };

    // 更新m_lst中的数据
    void UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info);
    // 获得某行数据
    void GetListData(std::vector<CString>& data, int index);

    afx_msg void OnBnClickedButtonLookupImport();
    afx_msg void OnBnClickedButtonLookupExport();
    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnBnClickedButtonImport();
    afx_msg void OnBnClickedButtonExport();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonImportTdx();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
