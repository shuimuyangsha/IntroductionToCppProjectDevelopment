#if !defined(AFX_TASKDLG_H__A4B6965E_ABA5_4DDE_93E9_AF53624F08C6__INCLUDED_)
#define AFX_TASKDLG_H__A4B6965E_ABA5_4DDE_93E9_AF53624F08C6__INCLUDED_
#include "CustomMenu.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg dialog

class CTaskDlg : public CDialog
{
// Construction
public:
	void ShowList(int num);
	CTaskDlg(CWnd* pParent = NULL);   // standard constructor
	CCustomMenu m_Menu;	// 自定义菜单


// Dialog Data
	//{{AFX_DATA(CTaskDlg)
	enum { IDD = IDD_TASK_DIALOG };
	CListCtrl	m_Grid;
	CTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuref();
	afx_msg void OnMenustop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDLG_H__A4B6965E_ABA5_4DDE_93E9_AF53624F08C6__INCLUDED_)
