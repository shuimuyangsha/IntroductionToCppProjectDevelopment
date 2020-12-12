#if !defined(AFX_LITTERDLG_H__5BE912A3_8112_471F_8D04_082D52D0EE80__INCLUDED_)
#define AFX_LITTERDLG_H__5BE912A3_8112_471F_8D04_082D52D0EE80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LitterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLitterDlg dialog

class CLitterDlg : public CDialog
{
// Construction
public:
	void FileDelete(CString FilePath);
	void DeleteLitterFile();
	CLitterDlg(CWnd* pParent = NULL);   // standard constructor
	HANDLE		m_hThread;                // Ïß³Ì¾ä±ú


// Dialog Data
	//{{AFX_DATA(CLitterDlg)
	enum { IDD = IDD_LITTER_DIALOG };
	CListBox	m_List;
	CComboBox	m_Combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLitterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLitterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButselect();
	afx_msg void OnButbegin();
	afx_msg void OnButstop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LITTERDLG_H__5BE912A3_8112_471F_8D04_082D52D0EE80__INCLUDED_)
