
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/********************************************************************************/
// 全局变量
#define GAME_WIN_W (800)
#define GAME_WIN_H (600)

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | FWS_ADDTOTITLE)
    // 创建窗口
    Create(NULL, _T("明日科技.坦克大战"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
    // 设置客户区大小
    {
        // 获得客户区的大小
        CRect rcCli;
        GetClientRect(rcCli);
        TRACE("%d,%d,%d,%d\r\n", rcCli.left, rcCli.top, rcCli.right, rcCli.bottom);
        assert(rcCli.left == 0);
        assert(rcCli.top == 0);

        // 计算边框的大小，并设置
        RECT rcFrame = { 0, 0, m_iWidth + m_iWidth - rcCli.right, m_iHeight + m_iHeight - rcCli.bottom };
        MoveWindow(&rcFrame, TRUE);
    }
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }
    // 启动定时器 每次都会进入游戏帧
    SetTimer(ETimerIdGameLoop, 0, NULL);
    // 设置游戏主窗口句柄
    m_game.SetHandle(GetSafeHwnd());

    return 0;
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent) {
        // 游戏循环ID
        case ETimerIdGameLoop: {
            // 记录本次时刻
            static DWORD dwLastUpdate = GetTickCount();
            // 判断时间隔
            if(GetTickCount() - dwLastUpdate >= 20) {
                // 进入游戏帧处理
                m_game.EnterFrame(GetTickCount());
                // 记录时间间隔
                dwLastUpdate = GetTickCount();
            }
            // 否则什么都不做
        }
        default:
            break;
    }

    CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
    // 直接把鼠标消息转给 CGame对象
    m_game.OnMouseMove(nFlags, point);
    CFrameWnd::OnMouseMove(nFlags, point);
}


void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // 直接把鼠标消息转给 CGame对象
    m_game.OnLButtonUp(nFlags, point);
    CFrameWnd::OnLButtonUp(nFlags, point);
}
