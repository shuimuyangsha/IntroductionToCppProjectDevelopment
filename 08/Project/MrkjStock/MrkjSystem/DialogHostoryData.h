#pragma once
#include "afxcmn.h"
#include "ATLComTime.h"

#include "DialogHistorySub01.h"
#include "DialogHistorySub02.h"
#include "afxwin.h"


// CDialogHostoryData 对话框

class CDialogHostoryData : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogHostoryData)

public:
    CDialogHostoryData(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogHostoryData();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HOSTORY_DATA };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

private:
    CDialogHistorySub01* m_pDlg01{new CDialogHistorySub01()};
    CDialogHistorySub02* m_pDlg02{new CDialogHistorySub02()};
public:
    //
    CTabCtrl m_tab;
    // 开始日期
    COleDateTime m_odtDateStart;
    // 结束日期
    COleDateTime m_odtDateEnd;

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonQuery();
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

    // 股票代码或者名称
    CString m_strCode;
    // 股票代码
    CComboBox m_cbbStockCode;
    // 清理数据
    void ClearComboBox();

    // 重命名数据类型,方便本类内使用
    typedef CMRKJDatabase::TupleStockInfo INFO, *PINFO;
    // 更新股票代码
    void UpdateStockInfoComboBox();
    // 获得选中的股票代码
    bool GetSelStockCode(CString& strCode);
    // 设置选中项
    void SetSelByStockCode(const CString& strCode);
    // 使界面显示此数据
    void SetDataAndUpdate(const CString& stockCode, const COleDateTime& m_odtDateStart, const COleDateTime& m_odtDateEnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
