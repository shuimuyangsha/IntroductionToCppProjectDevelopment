#if !defined(AFX_ICONBTN_H__02A1419C_0E38_4922_ABE2_3D6D23E7A6B7__INCLUDED_)
#define AFX_ICONBTN_H__02A1419C_0E38_4922_ABE2_3D6D23E7A6B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconBtn window

class CIconBtn : public CButton
{
// Construction
public:
	CIconBtn();
	CImageList  m_ImageList;
	CImageList* m_pImageList;	// 图像列表指针
	int			m_ImageIndex;	// 图标索引
	BOOL		IsPressed;		// 按钮是否被按下
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetImageIndex(UINT Index);
	virtual ~CIconBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONBTN_H__02A1419C_0E38_4922_ABE2_3D6D23E7A6B7__INCLUDED_)
