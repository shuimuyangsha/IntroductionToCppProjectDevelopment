
// DialogMain.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopToys.h"
#include "DialogMain.h"
#include "afxdialogex.h"
#include "ScreenTools.h"

#include "Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialogMain 对话框



CDialogMain::CDialogMain(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DESKTOPTOYS_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialogMain::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogMain, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CDialogMain 消息处理程序

BOOL CDialogMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // 获得屏幕分辨率
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // 布满全屏,并窗口透明,无标题标栏
    {
        // 设置本窗口大小, 遮挡整个屏幕
        MoveWindow(0, 0, w, y);
        //创建一个子窗口，用来画个动的东西
        m_dlg = make_shared<CDesktopToysDlg>(this);
        m_dlg->Create(IDD_DESKTOPTOYS_DIALOG, this);
        m_dlg->MoveWindow(0, 0, w, y);
        m_dlg->ShowWindow(SW_SHOW);
    }

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDialogMain::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDialogMain::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


BOOL CDialogMain::PreTranslateMessage(MSG *pMsg)
{
    //// 按下 'ESC'键时
    //if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
    //    if (m_pGame->OnESC()) {
    //        return TRUE;
    //    }
    //    else {
    //        // 继续本窗口处理,退出,游戏结束
    //    }
    //}
    return CDialogEx::PreTranslateMessage(pMsg);
}

#if 0

void CDialogMain::OnTimer(UINT_PTR nIDEvent)
{
    {
        static DWORD dwLastTime = 0;
        DWORD dwTime = GetTickCount();
        m_pGame->EnterFrame(dwTime);
        dwLastTime = dwTime;
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CDialogMain::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDialogMain::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonUp(nFlags, point);
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CDialogMain::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonDblClk(nFlags, point);
    CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CDialogMain::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonDown(nFlags, point);
    CDialogEx::OnRButtonDown(nFlags, point);
}

void CDialogMain::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonUp(nFlags, point);
    CDialogEx::OnRButtonUp(nFlags, point);
}

void CDialogMain::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonDblClk(nFlags, point);
    CDialogEx::OnRButtonDblClk(nFlags, point);
}

void CDialogMain::OnMouseMove(UINT nFlags, CPoint point)
{
    m_pGame->OnMouseMove(nFlags, point);
    CDialogEx::OnMouseMove(nFlags, point);
}

#endif
