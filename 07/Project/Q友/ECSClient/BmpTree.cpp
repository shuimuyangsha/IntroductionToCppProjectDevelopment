// BmpTree.cpp : implementation file
//

#include "stdafx.h"
#include "ecsclient.h"
#include "BmpTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpTree

CBmpTree::CBmpTree()
{
	m_Bitmap.LoadBitmap(IDB_TREEBMP);
	m_IsScroll = FALSE;
}

CBmpTree::~CBmpTree()
{
}


BEGIN_MESSAGE_MAP(CBmpTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CBmpTree)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpTree message handlers

BOOL CBmpTree::OnEraseBkgnd(CDC* pDC) 
{
	// 防止背景闪烁
	return true;	
	// return CTreeCtrl::OnEraseBkgnd(pDC);
}

LRESULT CBmpTree::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message==TVM_INSERTITEM ||message==TVM_DELETEITEM )
	{
		SetRedraw(FALSE);
		SetRedraw();
	}	
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

BOOL CBmpTree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SetRedraw(FALSE);
	SetRedraw();	
	return CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CBmpTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!iswalnum(nChar))
		SetRedraw();	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBmpTree::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SetRedraw(FALSE);
	SetRedraw();	
	CTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBmpTree::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting

	CRect ClientRC;
	GetClientRect(&ClientRC);
	my_help::CMemDC bkDC(&dc,ClientRC);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);	
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, ClientRC.Width(), ClientRC.Height());
	MemDC.SelectObject( &bitmap );
	// 获取当前窗口的位图信息
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)MemDC.m_hDC,0);

	CDC MaskDC;
	MaskDC.CreateCompatibleDC(&dc);
	CBitmap mkBmp;
	mkBmp.CreateBitmap(ClientRC.Width(),ClientRC.Height(),1,1,NULL);
	MaskDC.SelectObject( &mkBmp );
	MaskDC.BitBlt( 0,0,ClientRC.Width(),ClientRC.Height(),&MemDC, 
		ClientRC.left,ClientRC.top,SRCCOPY);
	// 首先在填充中填充位图
	CBrush Brush;
	Brush.CreatePatternBrush(&m_Bitmap);
	bkDC.FillRect(ClientRC, &Brush);	
	
	MemDC.SetBkColor(RGB(0,0,0));          
	MemDC.SetTextColor(RGB(255,255,255));  
	MemDC.BitBlt(ClientRC.left,ClientRC.top, ClientRC.Width(),ClientRC.Height(), 
            &MaskDC,ClientRC.left,ClientRC.top, SRCAND);	

	bkDC.SetBkColor(RGB(255,255,255));
	bkDC.SetTextColor(RGB(0,0,0));
	bkDC.BitBlt(	ClientRC.left,ClientRC.top, ClientRC.Width(),ClientRC.Height(), 
             &MaskDC,ClientRC.left,ClientRC.top, SRCAND);
	
	bkDC.BitBlt(	ClientRC.left,ClientRC.top, ClientRC.Width(),ClientRC.Height(), 
             &MemDC,ClientRC.left,ClientRC.top,SRCPAINT);
	Brush.DeleteObject();
	

}

void CBmpTree::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SetRedraw(FALSE);
	SetRedraw();	
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBmpTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	SetRedraw(FALSE);
	*pResult = 0;	
}

void CBmpTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	SetRedraw();	
	*pResult = 0;
}
