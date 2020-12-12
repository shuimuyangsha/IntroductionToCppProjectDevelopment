#pragma once
#include "afxcmn.h"


// CDialogDataKind 对话框

class CDialogDataKind : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDataKind)

public:
    CDialogDataKind(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogDataKind();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DATA_KIND };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    virtual BOOL OnInitDialog();
    // 品种
    CTreeCtrl m_treeKind;
    CString m_strKindName;

    void UpdateTree();

    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnTvnSelchangedTreeKind(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
