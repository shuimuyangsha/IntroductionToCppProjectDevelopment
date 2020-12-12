#if !defined(AFX_SCROLLPARENT_H__CBC2665C_F52E_4455_AAEF_94AEC113E7F0__INCLUDED_)
#define AFX_SCROLLPARENT_H__CBC2665C_F52E_4455_AAEF_94AEC113E7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollParent.h : header file

#include "SkinScroll.h"
/////////////////////////////////////////////////////////////////////////////
// CScrollParent window
// ������������
class CScrollParent : public CWnd
{
// Construction
public:
	CScrollParent();

	// ����ˮƽ�������봹ֱ������
	CSkinScroll m_VerScroll;
	CSkinScroll m_HorScroll;
	// ����һ���Ӵ���
	CWnd  m_ChildWnd;

	WNDPROC m_OldProc;     // ��¼�ؼ�ԭ���Ĺ�����
	int     m_ScrollWidth; // ���ƵĹ��������
	int		m_Type;        // �Ƿ���Ҫ���ƹ��������洦�ľ�������,
						   // ��ˮƽ�������ʹ�ֱ������ͬʱ����ʱ,
						   // ��Ҫ����,���Ҵ�ֱ���������󷽺��ҷ�,
						   // ���������ǲ�ͬ��,��ֵΪ1,��ʾ��������
						   // ����,Ϊ2,�����������ҷ�,Ϊ0,����Ҫ��������
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
