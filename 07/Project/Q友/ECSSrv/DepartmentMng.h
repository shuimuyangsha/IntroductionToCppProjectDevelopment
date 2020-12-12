#if !defined(AFX_DEPARTMENTMNG_H__54D5DFBE_A3A9_4AC9_B457_31CF6C4236C8__INCLUDED_)
#define AFX_DEPARTMENTMNG_H__54D5DFBE_A3A9_4AC9_B457_31CF6C4236C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DepartmentMng.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDepartmentMng dialog
#include "DataManage.h"
class CDepartmentMng : public CDialog
{
// Construction
public:
	void FindNode(HTREEITEM hParent, int ParentID);
	void LoadDepartInfo();
	CDepartmentMng(CWnd* pParent = NULL);   // standard constructor
	CDataManage m_DBMng;
	BOOL m_InitSucess;
	int  m_CurrentID;
	CImageList  m_ImageList;
	// Dialog Data
	//{{AFX_DATA(CDepartmentMng)
	enum { IDD = IDD_DEPARTMENTMNG_DIALOG };
	CEdit	m_CurrentNode;
	CEdit	m_DPName;
	CTreeCtrl	m_DPList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepartmentMng)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDepartmentMng)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtadd();
	afx_msg void OnSelchangedDplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtdelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEPARTMENTMNG_H__54D5DFBE_A3A9_4AC9_B457_31CF6C4236C8__INCLUDED_)
