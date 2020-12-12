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
}

// 处理左键抬起事件
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
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
    DrawFps(gh);

    /*下面的注释一定要写,否则后面插入代码时,找不到位置*/
    // 画出一幅图片
    {
        // 获得 游戏窗口大小
        CRect rc;
        GetClientRect(m_hWnd, rc);

        // 载入要绘制的图片
        Gdiplus::Image * img = Image::FromFile(_T("menu_background.png"));

        //画出图片
        gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());
    }


    // 拷贝到屏幕
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // 释放
    dc->DeleteDC();
    return;
}

// 画 fps
void CGame::DrawFps(Graphics &gh)
{
    static int fps = 0;
    // 记录己经画了多少帧
    m_fps++;
    // 记录上次输出fps的时间
    static DWORD dwLast = GetTickCount();
    // 判数时间是否超过 1秒，如果超过，输出 fps
    if(GetTickCount() - dwLast >= 1000) {
        fps = m_fps;
        // 清零，方便对帧进行重新记数
        m_fps = 0;
        // 记录本次输出的时间
        dwLast = GetTickCount();
    }

    // 输出fps
    {
        // 将fsp 格式化到 字符串
        CString s;
        s.Format(_T("FPS:%d"), fps);
        // 创建红色的画刷
        SolidBrush brush(Color(0x00, 0x00, 0xFF));
        // 创建输出的字体
        Gdiplus::Font font(_T("宋体"), 10.0);
        // 获得输出窗口的大小，用来定位 文字的输出位置
        CRect rc;
        ::GetClientRect(m_hWnd, &rc);
        // 在右上角显示
        PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
        // 正式出文字
        gh.DrawString(s.GetString(), -1, &font, origin, &brush);
    }
}








