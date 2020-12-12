#pragma once
#include "afxcmn.h"


// CDialogFilterKind 对话框

class CDialogFilterKind : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogFilterKind)

public:
    CDialogFilterKind(CWnd *pParent = NULL);   // 标准构造函数
    virtual ~CDialogFilterKind();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_STOCK_ROUND };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    // 股票种类
    CListCtrl m_lst;
    afx_msg void OnBnClickedButtonOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonCancel2();

    // 选择的
    std::vector<std::tuple<int, std::wstring>> m_vKinds;
};
