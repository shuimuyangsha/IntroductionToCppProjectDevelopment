
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/********************************************************************************/
// ȫ�ֱ���
#define GAME_WIN_W (800)
#define GAME_WIN_H (600)

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
        WS_MINIMIZEBOX | FWS_ADDTOTITLE)
    // ��������
    Create(NULL, _T("���տƼ�.̹�˴�ս"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
    // ���ÿͻ�����С
    {
        // ��ÿͻ����Ĵ�С
        CRect rcCli;
        GetClientRect(rcCli);
        TRACE("%d,%d,%d,%d\r\n", rcCli.left, rcCli.top, rcCli.right, rcCli.bottom);
        assert(rcCli.left == 0);
        assert(rcCli.top == 0);

        // ����߿�Ĵ�С��������
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
