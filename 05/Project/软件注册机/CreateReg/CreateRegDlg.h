// CreateRegDlg.h : header file
//

#if !defined(AFX_CREATEREGDLG_H__6991AA3A_212B_4A6F_8CF0_8DC0D463B1AD__INCLUDED_)
#define AFX_CREATEREGDLG_H__6991AA3A_212B_4A6F_8CF0_8DC0D463B1AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCreateRegDlg dialog

class CCreateRegDlg : public CDialog
{
// Construction
public:
	CString GetMacAddress();
	CString GetDiskNum();
	CString GetCPUNum();
	CCreateRegDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateRegDlg)
	enum { IDD = IDD_CREATEREG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateRegDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCreateRegDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEREGDLG_H__6991AA3A_212B_4A6F_8CF0_8DC0D463B1AD__INCLUDED_)
