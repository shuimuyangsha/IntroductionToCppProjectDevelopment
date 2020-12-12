#if !defined(AFX_SKINSCROLL_H__1F5A56A8_39BD_4BF0_8979_1313BFF5E225__INCLUDED_)
#define AFX_SKINSCROLL_H__1F5A56A8_39BD_4BF0_8979_1313BFF5E225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinScroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinScroll window
// �����ͷ��ťʱ��״̬
enum ClickArrow  {caNone,caLeft,caRight};

class CSkinScroll : public CScrollBar
{
// Construction
public:
	CSkinScroll();

// Attributes
public:
	SCROLLINFO	m_ScrollInfo;  // ��������Ϣ
	BOOL        m_Pressed;     // ����Ƿ񱻰���
	int         m_ThumbWidth;  // ��������
	int         m_Pos;         // �����鵱ǰλ��
	CPoint      m_Point;       // ��갴��ʱ�������
	CRect       m_HLeftArrow;  // ��������/�ϼ�ͷ����
	CRect       m_HRightArrow; // ��������/�¼�ͷ����
	CRect       m_ThumbRC;     // ��������������
	int         m_DragPos;     // �϶�������ʱ��λ��
	ClickArrow  m_ClickState;  // ������������ť��״̬
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
