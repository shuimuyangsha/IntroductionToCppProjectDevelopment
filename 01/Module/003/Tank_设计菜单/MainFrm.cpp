
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
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | FWS_ADDTOTITLE)
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
    // ������ʱ�� ÿ�ζ��������Ϸ֡
    SetTimer(ETimerIdGameLoop, 0, NULL);
    // ������Ϸ�����ھ��
    m_game.SetHandle(GetSafeHwnd());

    return 0;
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent) {
        // ��Ϸѭ��ID
        case ETimerIdGameLoop: {
            // ��¼����ʱ��
            static DWORD dwLastUpdate = GetTickCount();
            // �ж�ʱ���
            if(GetTickCount() - dwLastUpdate >= 20) {
                // ������Ϸ֡����
                m_game.EnterFrame(GetTickCount());
                // ��¼ʱ����
                dwLastUpdate = GetTickCount();
            }
            // ����ʲô������
        }
        default:
            break;
    }

    CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
    // ֱ�Ӱ������Ϣת�� CGame����
    m_game.OnMouseMove(nFlags, point);
    CFrameWnd::OnMouseMove(nFlags, point);
}


void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // ֱ�Ӱ������Ϣת�� CGame����
    m_game.OnLButtonUp(nFlags, point);
    CFrameWnd::OnLButtonUp(nFlags, point);
}
