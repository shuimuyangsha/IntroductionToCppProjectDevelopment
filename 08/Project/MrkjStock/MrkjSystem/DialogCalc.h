#pragma once
#include "afxcmn.h"


#include "DialogCalc01.h"
#include "DialogCalc02.h"
#include "DialogCalc03.h"
#include "DialogCalc04.h"
#include "DialogCalc05.h"

// CDialogCalc 对话框

class CDialogCalc : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogCalc)

public:
    CDialogCalc(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDialogCalc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_CALC };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CTreeCtrl m_tree;
    afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    CDialogCalc01 dlg01; // 股票收溢计算器
    CDialogCalc02 dlg02; // 股票涨跌停计算器
    CDialogCalc03 dlg03; // 购房能力评估
    CDialogCalc04 dlg04; // 购房提前还款计算
    CDialogCalc05 dlg05; // 所得税计算
    std::vector<CDialogEx*> m_vDlgPtr;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
