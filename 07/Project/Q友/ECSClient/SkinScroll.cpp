// SkinScroll.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "SkinScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinScroll

CSkinScroll::CSkinScroll()
{
	memset(&m_ScrollInfo,0,sizeof(SCROLLINFO));
	m_ScrollInfo.nTrackPos = -1;
	m_Pos          = 0;
	m_Pressed      = FALSE;
	m_DragPos      = 0;
	m_ClickState   = caNone;
}

CSkinScroll::~CSkinScroll()
{
}


BEGIN_MESSAGE_MAP(CSkinScroll, CScrollBar)
	//{{AFX_MSG_MAP(CSkinScroll)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinScroll message handlers

void CSkinScroll::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CBitmap bmp;
	BOOL type = IsVert(); 
	// 绘制左箭头

	if (type) // 水平滚动条

		bmp.LoadBitmap(IDB_UPARROW);
	else
		bmp.LoadBitmap(IDB_LEFTARROW);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bmp);
	BITMAP bInfo;
	bmp.GetBitmap(&bInfo);
	int x = bInfo.bmWidth;
	int y = bInfo.bmHeight;

	CRect rc;
	GetClientRect(rc);

	if (type)
		m_HLeftArrow.CopyRect(CRect(0,0,x,y));
	else
		m_HLeftArrow.CopyRect(CRect(-1,0,x,y));

	dc.BitBlt(m_HLeftArrow.left,m_HLeftArrow.top,m_HLeftArrow.Width(),m_HLeftArrow.Height(),&memDC,0,0,SRCCOPY);
	bmp.DeleteObject();
	// 绘制右箭头

	if (type)
		bmp.LoadBitmap(IDB_DOWNARROW);
	else
		bmp.LoadBitmap(IDB_RIGHTARROW);
	memDC.SelectObject(bmp);
	
	if (type)
		m_HRightArrow.CopyRect(CRect(0,rc.Height()-16,x,rc.Height()));
	else
		m_HRightArrow.CopyRect(CRect(rc.right-16,0,rc.right-16+x,y));

	dc.StretchBlt(m_HRightArrow.left,m_HRightArrow.top,m_HRightArrow.Width(),m_HRightArrow.Height(),&memDC,0,0,x,y,SRCCOPY);

	bmp.DeleteObject();
	// 绘制通道

	if (type)
		bmp.LoadBitmap(IDB_VCHANEL);
	else
		bmp.LoadBitmap(IDB_CHANEL);
	memDC.SelectObject(bmp);
	bmp.GetBitmap(&bInfo);
	x = bInfo.bmWidth;
	y = bInfo.bmHeight;

	if (type)
		dc.StretchBlt(1,16,x,rc.Height()-16*2,&memDC,0,0,x,y,SRCCOPY);
	else
		dc.StretchBlt(16,0,rc.right-32,y,&memDC,0,0,x,y,SRCCOPY);
	
	bmp.DeleteObject();

	if (type)
		bmp.LoadBitmap(IDB_VTHUMB);
	else
		bmp.LoadBitmap(IDB_HTHUMB);

	bmp.GetBitmap(&bInfo);
	memDC.SelectObject(&bmp);
	x = bInfo.bmWidth;
	y = bInfo.bmHeight;
	// 获取滚动块去除箭头的宽度或高度

	
	int width = 0;
	if (type)
		width = rc.Height()-16*2;
	else
		width = rc.Width()-16*2;

	// 滚动块宽度

	m_ThumbWidth = m_ScrollInfo.nPage*width/(m_ScrollInfo.nMax-m_ScrollInfo.nMin+1);
	int pos = m_Pos*(width)/(m_ScrollInfo.nMax-m_ScrollInfo.nMin+1)+16;

	if (type)
		m_ThumbRC.CopyRect(CRect(0,pos,16,pos+m_ThumbWidth));	
	else
		m_ThumbRC.CopyRect(CRect(pos,0,pos+m_ThumbWidth,16));

	if (m_ScrollInfo.nPage<=m_ScrollInfo.nMax-m_ScrollInfo.nMin)
		dc.StretchBlt(m_ThumbRC.left,m_ThumbRC.top,m_ThumbRC.Width(),m_ThumbRC.Height(),&memDC,0,0,x,y,SRCCOPY);

	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CSkinScroll::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_Point = point;
	if (m_ThumbRC.PtInRect(m_Point))
		m_Pressed = TRUE;
	else
		m_Pressed = FALSE;
	m_DragPos = m_ScrollInfo.nPos;
	m_ClickState = caNone;
	BOOL type = IsVert();
	// 单击滚动条箭头时设置滚动块位置

	if (m_HLeftArrow.PtInRect(point))
	{
		if (type)
			GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,0),(LPARAM)m_hWnd);
		else
			GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_LINELEFT,0),(LPARAM)m_hWnd);
		m_ClickState = caLeft;	
		SetTimer(1,100,NULL);
	}
	else if (m_HRightArrow.PtInRect(point))
	{
		if (type)
			GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,0),(LPARAM)m_hWnd);
		else
			GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_LINERIGHT,0),(LPARAM)m_hWnd);
		m_ClickState = caRight;
		SetTimer(1,100,NULL);
	}
	else if (m_Pressed == FALSE) // 单击滚动区域时设置滚动块的位置

	{
		if (type)
		{
			if (m_ThumbRC.top>point.y)          // 单击滚动块左边的区域

				GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,0),(LPARAM)m_hWnd);
			else if ((m_ThumbRC.top<point.y))   // 单击滚动块右边的区域

				GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,0),(LPARAM)m_hWnd);		
		}
		else
		{
			if (m_ThumbRC.left>point.x)         // 单击滚动块左边的区域

				GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_PAGELEFT,0),(LPARAM)m_hWnd);
			else if ((m_ThumbRC.left<point.x))  // 单击滚动块右边的区域

				GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_PAGERIGHT,0),(LPARAM)m_hWnd);
		}
	}
}

void CSkinScroll::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_ClickState = caNone;
	KillTimer(1);
	BOOL type = IsVert();
	if (m_Pressed)
		if (type)
			GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_THUMBPOSITION,m_Pos),(LPARAM)m_hWnd);
		else
			GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_THUMBPOSITION,m_Pos),(LPARAM)m_hWnd);
	m_Pressed = FALSE;
}

void CSkinScroll::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_Pressed )  // 处于拖动状态

	{
		BOOL type = IsVert();

		CRect rc;
		GetClientRect(rc);
		int width;// 去除滚动条2个按钮的宽度或高度

		if (type)
			width = rc.Height()-16*2;
		else
			width = rc.Width()-16*2;
		int movepos = 0;
		if (type)
			movepos = point.y-m_Point.y;
		else
			movepos = point.x-m_Point.x;
		movepos =(int)( (m_ScrollInfo.nMax-m_ScrollInfo.nMin+1-m_ScrollInfo.nPage)/(double)(width-m_ThumbWidth)*(movepos));
		movepos+=m_DragPos;
		if (movepos< m_ScrollInfo.nMin)
			movepos = m_ScrollInfo.nMin;
		if (movepos> (m_ScrollInfo.nMax-m_ScrollInfo.nMin+1-m_ScrollInfo.nPage))
			movepos = m_ScrollInfo.nMax-m_ScrollInfo.nMin+1-m_ScrollInfo.nPage;
		if (movepos>=0)
			m_Pos = movepos;
		if (type)
			GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_THUMBTRACK,m_Pos),(LPARAM)m_hWnd);
		else
			GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_THUMBTRACK,m_Pos),(LPARAM)m_hWnd);
		Invalidate();
	}
}

void CSkinScroll::OnSize(UINT nType, int cx, int cy) 
{
	CScrollBar::OnSize(nType, cx, cy);
	
	OnPaint();	
}

void CSkinScroll::OnTimer(UINT nIDEvent) 
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	BOOL type = IsVert();
	if (m_ClickState==caLeft)
	{
		if (m_Pos > m_ScrollInfo.nMin)
		if (m_HLeftArrow.PtInRect(pt))
			if (type)
				GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,0),(LPARAM)m_hWnd);
			else
				GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_LINELEFT,0),(LPARAM)m_hWnd);
	}
	else if (m_ClickState ==caRight)
	{
		if (m_Pos< m_ScrollInfo.nMax)
		if (m_HRightArrow.PtInRect(pt))
			if (type)
				GetParent()->SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,0),(LPARAM)m_hWnd);
			else
				GetParent()->SendMessage(WM_HSCROLL,MAKELONG(SB_LINERIGHT,0),(LPARAM)m_hWnd);
	}
}

LRESULT CSkinScroll::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// 阻止滚动条的双击事件

	if(message==WM_LBUTTONDBLCLK)
		return 1;
	if(message==SBM_SETSCROLLINFO)
	{
		BOOL NeedDraw=wParam;
		LPSCROLLINFO pInfo=(LPSCROLLINFO)lParam;
		if(pInfo->fMask&SIF_PAGE)
			m_ScrollInfo.nPage=pInfo->nPage;
		if(pInfo->fMask&SIF_POS)
			m_ScrollInfo.nPos=pInfo->nPos;
		if(pInfo->fMask&SIF_RANGE)
		{
			m_ScrollInfo.nMin=pInfo->nMin;
			m_ScrollInfo.nMax=pInfo->nMax;
		}
		if(NeedDraw)
		{
			m_Pos = m_ScrollInfo.nPos;
			Invalidate();			
		}
		return TRUE;
	}
	if(message==SBM_GETSCROLLINFO)
	{
		LPSCROLLINFO pInfo=(LPSCROLLINFO)lParam;
		int flags=pInfo->fMask;
		if(flags&SIF_PAGE)
			pInfo->nPage=m_ScrollInfo.nPage;
		if(flags&SIF_POS)
			pInfo->nPos=m_ScrollInfo.nPos;
		if(flags&SIF_TRACKPOS)
			pInfo->nTrackPos=m_ScrollInfo.nTrackPos;
		if(flags&SIF_RANGE)
		{
			pInfo->nMin=m_ScrollInfo.nMin;
			pInfo->nMax=m_ScrollInfo.nMax;
		}
		return TRUE;
	}
	return CScrollBar::WindowProc(message, wParam, lParam);
}

BOOL CSkinScroll::IsVert()
{
	return GetStyle()& SBS_VERT; 
}
