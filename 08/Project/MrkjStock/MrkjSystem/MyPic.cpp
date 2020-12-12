// MyPic.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "MyPic.h"

#include "ScreenTools.h"

// CMyPic

IMPLEMENT_DYNAMIC(CMyPic, CWnd)

CMyPic::CMyPic()
{

}

CMyPic::~CMyPic()
{
}


BEGIN_MESSAGE_MAP(CMyPic, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMyPic 消息处理程序


void CMyPic::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	HCURSOR hc = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_SHOT_CUR));
	::SetCursor(hc);
	HICON hicon2 = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	this->SetIcon(hicon2);
	SetTimer(1, 300, nullptr);
	CStatic::OnLButtonDown(nFlags, point);
}

void CMyPic::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture(); //释放鼠标捕获
	HICON hicon1 = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON_SHOT));
	this->SetIcon(hicon1);
	KillTimer(1);
		
	//获得鼠标所在位置的窗口的图片
	if(0){
		POINT pt;
		::GetCursorPos(&pt);
		HWND hWnd = ::WindowFromPoint(pt);
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		HBITMAP hBmp = CScreenTools::CopyScreenToBitmap(&rc);
		CScreenTools::SaveBitmapToFile(hBmp, _T("C:\\aaa.bmp"));
	}
	if(0){
		POINT pt;
		::GetCursorPos(&pt);
		HWND hWnd = ::WindowFromPoint(pt);
		HBITMAP hBmp = CScreenTools::PrintWindow(hWnd);
		CScreenTools::SaveBitmapToFile(hBmp, _T("C:\\bbb.bmp"));
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CMyPic::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags == MK_LBUTTON) {

	}
	else {
		static POINT ptLast = { -1, -1 };
		if (ptLast.x >= 0) {
			CScreenTools::DawMouse(ptLast);
		}
			POINT pt;
			GetCursorPos(&pt);
			CScreenTools::DawMouse(pt);
			ptLast = pt;
		//画出鼠标十字星
	}
	CStatic::OnMouseMove(nFlags, point);
}


void CMyPic::OnTimer(UINT_PTR nIDEvent)
{
	CStatic::OnTimer(nIDEvent);
	return;

	switch(nIDEvent){
	case 1: {
		POINT pnt;
		RECT rc;
		HWND DeskHwnd = ::GetDesktopWindow(); //取得桌面句柄
		HDC DeskDC = ::GetWindowDC(DeskHwnd); //取得桌面设备场景
		int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);
		::GetCursorPos(&pnt); //取得鼠标坐标
		HWND UnHwnd = ::WindowFromPoint(pnt); //取得鼠标指针处窗口句柄
		::GetWindowRect(UnHwnd, &rc); //获得窗口矩形
		if (rc.left < 0) rc.left = 0;
		if (rc.top < 0) rc.top = 0;
		HPEN newPen = ::CreatePen(0, 3, 0); //建立新画笔,载入DeskDC
		HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
		for (int i = 0; i < 3; ++i) {
			if (i != 0) {
				Sleep(100);
			}
			::Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom); //在窗口周围显示闪烁矩形
			Sleep(100); //设置闪烁时间间隔
			::Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom);
		}
		::SetROP2(DeskDC, oldRop2);
		::SelectObject(DeskDC, oldPen);
		::DeleteObject(newPen);
		::ReleaseDC(DeskHwnd, DeskDC);
		DeskDC = NULL;
	}
	default:
		break;
	}
	CStatic::OnTimer(nIDEvent);
}
