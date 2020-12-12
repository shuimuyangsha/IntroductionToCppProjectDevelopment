#if !defined(AFX_DISKDLG_H__87987066_DBFD_4910_9966_0724F2A745FA__INCLUDED_)
#define AFX_DISKDLG_H__87987066_DBFD_4910_9966_0724F2A745FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiskDlg dialog

class CDiskDlg : public CDialog
{
// Construction
public:
	void DelFolder(CString path);
	void ClearDisk(int num);
	CDiskDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDiskDlg)
	enum { IDD = IDD_DISK_DIALOG };
	CStatic	m_Text2;
	CStatic	m_Text1;
	CStatic	m_Text;
	CButton	m_CheckAll;
	CListCtrl	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiskDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckll();
	afx_msg void OnButclear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISKDLG_H__87987066_DBFD_4910_9966_0724F2A745FA__INCLUDED_)
