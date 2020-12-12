#if !defined(AFX_SKINSCROLL_H__1F5A56A8_39BD_4BF0_8979_1313BFF5E225__INCLUDED_)
#define AFX_SKINSCROLL_H__1F5A56A8_39BD_4BF0_8979_1313BFF5E225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinScroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinScroll window
// 打击箭头按钮时的状态
enum ClickArrow  {caNone,caLeft,caRight};

class CSkinScroll : public CScrollBar
{
// Construction
public:
	CSkinScroll();

// Attributes
public:
	SCROLLINFO	m_ScrollInfo;  // 滚动条信息
	BOOL        m_Pressed;     // 鼠标是否被按下
	int         m_ThumbWidth;  // 滚动块宽度
	int         m_Pos;         // 滚动块当前位置
	CPoint      m_Point;       // 鼠标按下时的坐标点
	CRect       m_HLeftArrow;  // 滚动条左/上箭头区域
	CRect       m_HRightArrow; // 滚动条右/下箭头区域
	CRect       m_ThumbRC;     // 滚动块所在区域
	int         m_DragPos;     // 拖动滚动块时的位置
	ClickArrow  m_ClickState;  // 单击滚动条按钮的状态
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinScroll)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsVert();
	virtual ~CSkinScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinScroll)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINSCROLL_H__1F5A56A8_39BD_4BF0_8979_1313BFF5E225__INCLUDED_)
