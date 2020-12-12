#pragma once
#include "afxcmn.h"


// CDialogKindSelect 对话框

class CDialogKindSelect : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogKindSelect)

public:
    CDialogKindSelect(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogKindSelect();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_KIND_SELECT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

private:
    CBitmap m_bmpTree;
    CImageList m_imgListTree;
    CImageList m_imgListList;
    CBitmap m_bmpList;
    CString GetSelectStockCode() const;

    // 确定时 指定选中的股票
    static CString m_strSelectCode;
public:
    CTreeCtrl m_treeKind;
    CListCtrl m_lst;
    afx_msg void OnBnClickedButtonOk();
    afx_msg void OnBnClickedButtonCancel();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();

    void UpdateTree();

    afx_msg void OnNMClickTreeKind(NMHDR *pNMHDR, LRESULT *pResult);

    // 更新
    void UpdateList(const vector<CMRKJDatabase::TupleStockInfo>& v);
    // 清除列表中的数据
    void ClearList();
    // 获得选中的,要查询的股票代码
    CString GetTargetStockCode() const
    {
        return m_strSelectCode;
    };
    afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
