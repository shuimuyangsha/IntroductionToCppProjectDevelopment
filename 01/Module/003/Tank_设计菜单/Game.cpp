#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{
}


CGame::~CGame()
{
}

// 设置输出窗口的句柄
void CGame::SetHandle(HWND hWnd)
{
    m_hWnd = hWnd;
}

// 进入游戏帧
bool CGame::EnterFrame(DWORD dwTime)
{
    GameRunDraw();
    return false;
}

// 处理鼠标移动事件
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
    // 选择游戏类型
    m_menuSelect.OnMouseMove(nFlags, point);
}

// 处理左键抬起事件
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // 选择游戏类型
    m_menuSelect.OnLButtonUp(nFlags, point);
}

// 游戏绘图
void CGame::GameRunDraw()
{
    HDC hdc = ::GetDC(m_hWnd);
    // 客户区的大小
    CRect rc;
    GetClientRect(m_hWnd, &rc);

    CDC *dc = CClientDC::FromHandle(hdc);

    // 双缓冲绘图用
    CDC m_dcMemory;
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
    m_dcMemory.CreateCompatibleDC(dc);
    CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

    // 构造对象
    Graphics gh(m_dcMemory.GetSafeHdc());
    // 清除背景
    gh.Clear(Color::White);
    gh.ResetClip();

    // 画入内存
    {
        // 画背景
        m_menu.Draw(gh);

        // 画菜单
        m_menuSelect.Draw(gh);
    }

    // 拷贝到屏幕
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // 释放
    dc->DeleteDC();
    return;
}

