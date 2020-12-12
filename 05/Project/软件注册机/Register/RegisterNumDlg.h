#if !defined(AFX_REGISTERNUMDLG_H__9E499DBD_3F9F_4A3B_82FA_52AB5DD50428__INCLUDED_)
#define AFX_REGISTERNUMDLG_H__9E499DBD_3F9F_4A3B_82FA_52AB5DD50428__INCLUDED_
#include "RegEdit.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterNumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegisterNumDlg dialog

class CRegisterNumDlg : public CDialog
{
// Construction
public:
	CRegisterNumDlg(CWnd* pParent = NULL);   // standard constructor
	HWND hwnd;

// Dialog Data
	//{{AFX_DATA(CRegisterNumDlg)
	enum { IDD = IDD_REGISTERNUM_DIALOG };
	CButton	m_Backoff;
	CButton	m_Advance;
	CEdit	m_edtRegisterCode;
	CEdit	m_edtName; // ×¢²áÃû
	CString	m_Name;
	CString	m_strRegisterCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterNumDlg)
public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegisterNumDlg)
	afx_msg void OnBackoff();
	afx_msg void OnAdvance();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnChangeEdit1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERNUMDLG_H__9E499DBD_3F9F_4A3B_82FA_52AB5DD50428__INCLUDED_)
