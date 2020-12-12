
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
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
        WS_MINIMIZEBOX | FWS_ADDTOTITLE)
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
        RECT rcFrame = { 0, 0,
                         m_iWidth + m_iWidth - rcCli.right,
                         m_iHeight + m_iHeight - rcCli.bottom
                       };
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

    return 0;
}
