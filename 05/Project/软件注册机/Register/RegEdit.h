#if !defined(AFX_REGEDIT_H__0705DAD7_10D6_4EF8_A168_6C605500212F__INCLUDED_)
#define AFX_REGEDIT_H__0705DAD7_10D6_4EF8_A168_6C605500212F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegEdit window

class CRegEdit : public CEdit
{
// Construction
public:
	CRegEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRegEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRegEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGEDIT_H__0705DAD7_10D6_4EF8_A168_6C605500212F__INCLUDED_)
