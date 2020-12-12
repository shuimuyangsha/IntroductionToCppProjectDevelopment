// ScrollParent.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "ScrollParent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CM_MOVE  WM_USER+10
/////////////////////////////////////////////////////////////////////////////
// CScrollParent

BOOL DestroyScrollBar(CWnd *pWnd)
{
	CScrollParent *pPanel=(CScrollParent *)GetWindowLong(pWnd->m_hWnd,GWL_USERDATA);
	if(pPanel) 
	{
		RECT rc;
		CWnd *pParent=pPanel->GetParent();
		pPanel->GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		SetWindowLong(pWnd->m_hWnd,GWL_WNDPROC,(LONG)pPanel->m_OldProc);
		SetWindowLong(pWnd->m_hWnd,GWL_USERDATA,0);
		pWnd->SetParent(pParent);
		pWnd->MoveWindow(&rc);
		pPanel->DestroyWindow();
		delete pPanel;
	}
	return TRUE;
}

CScrollParent* SkinScrollBar(CWnd *pWnd)
{
	CScrollParent *pPanel=new CScrollParent;
	pPanel->WindowSkin(pWnd);
	return pPanel;
}

// 子类化回调函数

static LRESULT CALLBACK
NewProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CScrollParent *pSkin=(CScrollParent*)GetWindowLong(hwnd,GWL_USERDATA);
	if(msg==WM_DESTROY)
	{
		WNDPROC procOld=pSkin->m_OldProc;
		DestroyScrollBar(CWnd::FromHandle(hwnd));
		return ::CallWindowProc(procOld,hwnd,msg,wp,lp);
	}
	// 执行原来的回调函数

	LRESULT lr=::CallWindowProc(pSkin->m_OldProc,hwnd,msg,wp,lp);

	if(msg==WM_ERASEBKGND)
	{
			SCROLLINFO si;
			DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
			if(dwStyle&WS_VSCROLL)
			{
				memset(&si,0,sizeof(si));
				si.cbSize=sizeof(si);
				si.fMask=SIF_ALL;
				// 获取滚动条信息

				::GetScrollInfo(hwnd,SB_VERT,&si);
				if(si.nMax!=pSkin->m_VerScroll.m_ScrollInfo.nMax 
					||si.nMin!=pSkin->m_VerScroll.m_ScrollInfo.nMin
					||si.nPos!=pSkin->m_VerScroll.m_ScrollInfo.nPos
					||si.nPage!=pSkin->m_VerScroll.m_ScrollInfo.nPage)
				{
					pSkin->m_VerScroll.SetScrollInfo(&si);
					pSkin->m_VerScroll.m_ScrollInfo = si;

				}
			}
			if(dwStyle&WS_HSCROLL)
			{
				memset(&si,0,sizeof(si));
				si.cbSize=sizeof(si);
				si.fMask=SIF_ALL;
				::GetScrollInfo(hwnd,SB_HORZ,&si);
				if(si.nMax!=pSkin->m_HorScroll.m_ScrollInfo.nMax 
					||si.nMin!=pSkin->m_HorScroll.m_ScrollInfo.nMin
					||si.nPos!=pSkin->m_HorScroll.m_ScrollInfo.nPos
					||si.nPage!=pSkin->m_HorScroll.m_ScrollInfo.nPage)
				{
					pSkin->m_HorScroll.SetScrollInfo(&si);
					pSkin->m_HorScroll.EnableWindow(si.nMax>=si.nPage);
				}
			}
	}
	// 应用程序需要显示合法的客户区域

	else if(msg==WM_NCCALCSIZE && wp)
	{

			LPNCCALCSIZE_PARAMS pNcCalcSizeParam=(LPNCCALCSIZE_PARAMS)lp;
			DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
			DWORD dwExStyle=::GetWindowLong(hwnd,GWL_EXSTYLE);
			BOOL  bLeftScroll=dwExStyle&WS_EX_LEFTSCROLLBAR;

			int nWid=::GetSystemMetrics(SM_CXVSCROLL);

			if(dwStyle&WS_VSCROLL) 
			{
				// 垂直滚动条在左边

				if(bLeftScroll)
					// 由于图片与滚动条的宽度不一定相同,需要适当调整客户区域大小

					pNcCalcSizeParam->rgrc[0].left-=nWid-pSkin->m_ScrollWidth;
				else
					// 由于图片与滚动条的宽度不一定相同,需要适当调整客户区域大小

					pNcCalcSizeParam->rgrc[0].right+=nWid-pSkin->m_ScrollWidth;
			}

			if(dwStyle&WS_HSCROLL)
				pNcCalcSizeParam->rgrc[0].bottom+=nWid-pSkin->m_ScrollWidth;
			
			RECT rcVert,rcHorz;  // 记录滚动条的位置

			
			// 图片宽度

			nWid=pSkin->m_ScrollWidth;
			if(bLeftScroll)
			{
				int nLeft=pNcCalcSizeParam->rgrc[0].left;
				int nBottom=pNcCalcSizeParam->rgrc[0].bottom;
				rcVert.right=nLeft;
				rcVert.left=nLeft-nWid;
				rcVert.top=0;
				rcVert.bottom=nBottom;
				rcHorz.left=nLeft;
				rcHorz.right=pNcCalcSizeParam->rgrc[0].right;
				rcHorz.top=nBottom;
				rcHorz.bottom=nBottom+nWid;
			}else
			{
				int nRight=pNcCalcSizeParam->rgrc[0].right;
				int nBottom=pNcCalcSizeParam->rgrc[0].bottom;
				rcVert.left=nRight;
				rcVert.right=nRight+nWid;
				rcVert.top=0;
				rcVert.bottom=nBottom;
				rcHorz.left=0;
				rcHorz.right=nRight;
				rcHorz.top=nBottom;
				rcHorz.bottom=nBottom+nWid;
			}
			if(dwStyle&WS_VSCROLL && dwStyle&WS_HSCROLL)
			{
				pSkin->m_Type=bLeftScroll?1:2;
			}else
			{
				pSkin->m_Type=0;
			}
			// 显示或隐藏滚动条

			if(dwStyle&WS_VSCROLL)
			{
				pSkin->m_VerScroll.MoveWindow(&rcVert);
				pSkin->m_VerScroll.ShowWindow(SW_SHOW);
			}else
			{
				pSkin->m_VerScroll.ShowWindow(SW_HIDE);
			}
			if(dwStyle&WS_HSCROLL)
			{
				pSkin->m_HorScroll.MoveWindow(&rcHorz);
				pSkin->m_HorScroll.ShowWindow(SW_SHOW);
			}else
			{
				pSkin->m_HorScroll.ShowWindow(SW_HIDE);
			}
			pSkin->PostMessage(CM_MOVE,dwStyle&WS_VSCROLL,bLeftScroll);
	}
	return lr;
}

CScrollParent::CScrollParent()
{
	m_OldProc     = NULL;
	m_ScrollWidth = 0;
}

CScrollParent::~CScrollParent()
{


}


BEGIN_MESSAGE_MAP(CScrollParent, CWnd)
	//{{AFX_MSG_MAP(CScrollParent)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_MESSAGE(CM_MOVE,OnScrollMove)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollParent message handlers

void CScrollParent::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if(cx==0 || cy==0) 
		return;
	if(IsWindow( m_ChildWnd.m_hWnd))
	{
		CWnd *pWnd=m_ChildWnd.GetWindow(GW_CHILD);
		pWnd->MoveWindow(0,0,cx,cy);
	}
}

void CScrollParent::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd *pWnd=m_ChildWnd.GetWindow(GW_CHILD);
	pWnd->SendMessage(WM_HSCROLL,MAKELONG(nSBCode,nPos),0);

	if(nSBCode==SB_THUMBTRACK) return;
	SCROLLINFO si={0};
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	pWnd->GetScrollInfo(SB_HORZ,&si);
	pScrollBar->SetScrollInfo(&si,TRUE);	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScrollParent::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd *pWnd=m_ChildWnd.GetWindow(GW_CHILD);

	pWnd->SendMessage(WM_VSCROLL,MAKELONG(nSBCode,nPos),0);

	if(nSBCode==SB_THUMBTRACK) return;
	SCROLLINFO si={0};
	si.cbSize=sizeof(si);
	si.fMask=SIF_ALL;
	pWnd->GetScrollInfo(SB_VERT,&si);
	pScrollBar->SetScrollInfo(&si,TRUE);	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 当在控件中利用方向键致使滚动块移动时,更新父窗口中的滚动条控件

void CScrollParent::OnTimer(UINT nIDEvent) 
{
	CWnd *pWnd=m_ChildWnd.GetWindow(GW_CHILD);
	SCROLLINFO si1={0};
	si1.cbSize=sizeof(SCROLLINFO);
	si1.fMask=SIF_ALL;
	SCROLLINFO si2={0};
	if(GetWindowLong(m_VerScroll.m_hWnd,GWL_STYLE)&WS_VISIBLE )
	{
		pWnd->GetScrollInfo(SB_VERT,&si1);
		m_VerScroll.SetScrollInfo(&si1);
	}
	if(GetWindowLong(m_HorScroll.m_hWnd,GWL_STYLE)&WS_VISIBLE )
	{
		pWnd->GetScrollInfo(SB_HORZ,&si1);
		m_HorScroll.SetScrollInfo(&si1);
	}	
}

void CScrollParent::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// 绘制交叉矩形

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_RECT);
	
	HGDIOBJ hOldBmp=memdc.SelectObject(&bmp);
	RECT rc;
	GetClientRect(&rc);
	if(m_Type==1)
		dc.BitBlt(rc.left,rc.bottom-m_ScrollWidth,m_ScrollWidth,m_ScrollWidth,&memdc,0,0,SRCCOPY);
	else if(m_Type==2)
			dc.BitBlt(rc.right-m_ScrollWidth,rc.bottom-m_ScrollWidth,m_ScrollWidth,m_ScrollWidth,&memdc,0,0,SRCCOPY);
	::SelectObject(memdc,hOldBmp);
}

LRESULT CScrollParent::OnScrollMove(WPARAM wParam, LPARAM lParam)
{
	BOOL bVScroll=wParam;
	BOOL bLeftScroll=lParam;
	RECT rcClient;
	GetClientRect(&rcClient);
	RECT rcLimit,rcWnd;
	rcWnd=rcClient;
	rcLimit=rcClient;

	if(GetWindowLong(m_HorScroll.m_hWnd,GWL_STYLE)&WS_VISIBLE)
		rcLimit.bottom-=m_ScrollWidth;

	if(bLeftScroll)
	{
		if(bVScroll)
		{
			rcLimit.left+=m_ScrollWidth;
			OffsetRect(&rcWnd,-m_ScrollWidth,0);
		}
	}else
	{
		if(bVScroll) 
			rcLimit.right-=m_ScrollWidth;
	}
	m_ChildWnd.MoveWindow(&rcLimit);
	m_ChildWnd.GetWindow(GW_CHILD)->SetWindowPos(NULL,rcWnd.left,0,0,0,SWP_NOSIZE|SWP_NOZORDER);
	return 0;
}

BOOL CScrollParent::WindowSkin(CWnd *pWnd)
{
	ASSERT(m_hWnd==NULL);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_LEFTARROW);
	BITMAP bm;
	bmp.GetBitmap(&bm);

	// 滚动条的宽度

	m_ScrollWidth=bm.bmWidth-1;
	// 获取对话框

	CWnd *pParent=pWnd->GetParent();

	ASSERT(pParent);
	RECT rcFrm,rcWnd;
	pWnd->GetWindowRect(&rcFrm);
	// rcFrm为控件在窗口中的位置

	pParent->ScreenToClient(&rcFrm);
	rcWnd=rcFrm;
	// rcFrm记录当前控件的位置,将在该位置创建一个父控件

	// 移动区域左上角被移动到0,0处,使其处于父控件内

	OffsetRect(&rcWnd,-rcWnd.left,-rcWnd.top);

	UINT uID=pWnd->GetDlgCtrlID();
	DWORD dwStyle=pWnd->GetStyle();
	DWORD dwExStyle=pWnd->GetExStyle();
	DWORD dwFrmStyle=WS_CHILD|SS_NOTIFY;
	DWORD dwFrmStyleEx=0;
	if(dwExStyle & WS_EX_TOOLWINDOW)
	{
		dwFrmStyleEx|=WS_EX_TOOLWINDOW;
		dwFrmStyleEx |=WS_EX_TOPMOST;
	}
	if(dwStyle&WS_VISIBLE) dwFrmStyle|=WS_VISIBLE;

	if(dwStyle&WS_BORDER)
	{
		dwFrmStyle|=WS_BORDER;
		// 移除控件的边框

		pWnd->ModifyStyle(WS_BORDER,0);

		int nBorder=::GetSystemMetrics(SM_CXBORDER);
		// 将rcWnd区域扣除边框

		rcWnd.right-=nBorder*2;
		rcWnd.bottom-=nBorder*2;
	}
	if(dwExStyle&WS_EX_CLIENTEDGE)
	{
		pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0);
		int nBorder   =::GetSystemMetrics(SM_CXEDGE);
		rcWnd.right-=nBorder*2;
		rcWnd.bottom-=nBorder*2;
		dwFrmStyleEx|=WS_EX_CLIENTEDGE;
	}
	// 以原控件风格创建一个父控件

	CreateEx(dwFrmStyleEx,AfxRegisterWndClass(NULL),"ScrollBarContainer",dwFrmStyle,rcFrm,pParent,uID);

	m_ChildWnd.Create(NULL,"CHILDWND",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,200);
	m_HorScroll.Create(WS_CHILD|SBS_HORZ,CRect(0,0,0,0),this,100);
	m_VerScroll.Create(WS_CHILD|SBS_VERT,CRect(0,0,0,0),this,101);

	pWnd->SetParent(&m_ChildWnd);
	SetWindowLong(pWnd->m_hWnd,GWL_USERDATA,(LONG)this);
	m_OldProc=(WNDPROC)SetWindowLong(pWnd->m_hWnd,GWL_WNDPROC,(LONG)NewProc);

	pWnd->MoveWindow(&rcWnd);
	SetTimer(1,500,NULL);
	return TRUE;
}
