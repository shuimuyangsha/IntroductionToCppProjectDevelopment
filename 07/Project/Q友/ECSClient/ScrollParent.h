#if !defined(AFX_SCROLLPARENT_H__CBC2665C_F52E_4455_AAEF_94AEC113E7F0__INCLUDED_)
#define AFX_SCROLLPARENT_H__CBC2665C_F52E_4455_AAEF_94AEC113E7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollParent.h : header file

#include "SkinScroll.h"
/////////////////////////////////////////////////////////////////////////////
// CScrollParent window
// 滚动条父窗口
class CScrollParent : public CWnd
{
// Construction
public:
	CScrollParent();

	// 定义水平滚动条与垂直滚动条
	CSkinScroll m_VerScroll;
	CSkinScroll m_HorScroll;
	// 定义一个子窗口
	CWnd  m_ChildWnd;

	WNDPROC m_OldProc;     // 记录控件原来的滚动条
	int     m_ScrollWidth; // 绘制的滚动条宽度
	int		m_Type;        // 是否需要绘制滚动条交叉处的矩形区域,
						   // 当水平滚动条和垂直滚动条同时存在时,
						   // 需要绘制,并且垂直滚动条在左方和右方,
						   // 交叉区域是不同的,该值为1,表示交叉区域
						   // 在左方,为2,交叉区域在右方,为0,不需要交叉区域
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollParent)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL WindowSkin(CWnd *pWnd);

	virtual ~CScrollParent();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollParent)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg	LRESULT OnScrollMove(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLPARENT_H__CBC2665C_F52E_4455_AAEF_94AEC113E7F0__INCLUDED_)
