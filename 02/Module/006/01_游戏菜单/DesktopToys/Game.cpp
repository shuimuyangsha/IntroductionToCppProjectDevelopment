#include "stdafx.h"
#include "Game.h"
#include "ScreenTools.h"
#include "IMark.h"

CGame::CGame(HWND hWnd, float x, float y, float w, float h)
    : m_hWnd(hWnd)
    , m_x(x)
    , m_y(y)
    , m_width(w)
    , m_height(h)
{
    // 保存全局指针
    g_game = this;

    // 菜单初始化
    m_menu = std::make_shared<CDMenu> (hWnd);

    {
        // 截图整个屏幕，画在自己的窗口上
        RECT r{(long)x, (long)y, (long)(x + w), (long)(y + h)};
        HBITMAP hBmp = CScreenTools::CopyScreenToBitmap(&r);
        // 保存背景图
        m_imgBk = Bitmap::FromHBITMAP(hBmp, NULL);
    }

    // 获取窗口客户区大小
    GetClientRect(m_hWnd, &m_rcClient);
}


CGame::~CGame()
{
}


// 游戏的一帧
bool CGame::EnterFrame(DWORD dwTime)
{
    Draw();
    return false;
}

// 鼠标左键按下
void CGame::OnLButtonDown(UINT nFlags, CPoint point)
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect: {
            // 菜单 阶段 : 交给 m_menu 菜单处理
            if (m_menu->OnLButtonDown (nFlags, point)) {
                break;
            }
            break;
        }
        case CGame::EGameStatusNormal: {
            // 游戏 阶段
            break;
        }
        default: {
            break;
        }
    }
}

// 鼠标左键抬起
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect: {
            // 菜单 阶段 : 交给 m_menu 菜单处理
            if (m_menu->OnLButtonUp(nFlags, point)) {
                break;
            }
            break;
        }
        case CGame::EGameStatusNormal: {
            // 游戏 阶段
            break;
        }
        default: {
            break;
        }
    }

}

// 鼠标左键双击
void CGame::OnLButtonDblClk(UINT nFlags, CPoint point)
{
}

// 鼠标右键按下
void CGame::OnRButtonDown(UINT nFlags, CPoint point)
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect: {
            // 菜单 阶段 : 交给 m_menu 菜单处理
            if (m_menu->OnRButtonDown (nFlags, point)) {
                break;
            }
            break;
        }
        case CGame::EGameStatusNormal: {
            // 游戏 阶段
            break;
        }
        default: {

            break;
        }
    }
}

// 鼠标右键抬起
void CGame::OnRButtonUp(UINT nFlags, CPoint point)
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect: {
            // 菜单 阶段 : 交给 m_menu 菜单处理
            if (m_menu->OnLButtonUp (nFlags, point)) {
                break;
            }
            break;
        }
        case CGame::EGameStatusNormal: {
            // 游戏 阶段
            break;
        }
        default: {

            break;
        }
    }
}

// 鼠标右键双击
void CGame::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // 什么也不做
}

// 鼠标移动
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect: {
            // 菜单 阶段 : 交给 m_menu 菜单处理
            break;
        }
        case CGame::EGameStatusNormal: {
            // 游戏 阶段
            break;
        }
        default: {
            break;
        }
    }
}

// 按下 ESC 键时的处理
BOOL CGame::OnESC()
{
    switch(m_eStatus) {
        case CGame::EGameStatusSelect:
            // 当前 没有在游戏，不处理
            return FALSE;
        case CGame::EGameStatusNormal:
            // 当前在游戏返回到 菜单
            m_eStatus = EGameStatusSelect;
            // 显示鼠标
            {
                // 初始状态,计数是0,所以保证是0就可以显示了
                int i = ShowCursor(TRUE);
                if(i > 0) {
                    // 减少到0
                    while(ShowCursor(FALSE) > 0) {
                        ;
                    }
                }
                else if(i < 0) {
                    // 增加到0
                    while(ShowCursor(TRUE) < 0) {
                        ;
                    }
                }
            }
            return TRUE;
        default:
            break;
    }

    return FALSE;
}

void CGame::Draw()
{
    // 这个是窗口DC
    HDC hdc = ::GetDC(m_hWnd);
    // 离开函数作用域时，释放DC
    ON_SCOPE_EXIT([&]() {
        ::ReleaseDC(m_hWnd, hdc);
    });


    // 此指针是一个临时指针，不用释放
    CDC *dc = CClientDC::FromHandle(hdc);

    // 双缓冲绘图用
    // 内存DC
    CDC m_dcMemory;
    // 创建兼容位图对象，并选入 内存DC
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(dc, m_rcClient.Width(), m_rcClient.Height());
    m_dcMemory.CreateCompatibleDC(dc);
    // 选入DC
    CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

    // 构造对象
    Graphics gh(m_dcMemory.GetSafeHdc());
    // 设置单位
    gh.SetPageUnit(Gdiplus::UnitPixel);

    // 清除背景
    Color clr;
    clr.SetFromCOLORREF(BACK_GROUND_LAYER);
    gh.Clear(clr);
    gh.ResetClip();

    // 画背景图片
    gh.DrawImage(m_imgBk, m_x, m_y, m_width, m_height);

    // 根据游戏当前阶段，画不同的东西
    {
        switch(m_eStatus) {
            case CGame::EGameStatusSelect:
                // 画菜单
                if (m_menu) {
                    m_menu->Draw (gh);
                }
                break;
            case CGame::EGameStatusNormal:
                // 画出工具
                if (m_pTool) {
                    m_pTool->Draw (gh);
                }
                break;
            default:
                break;
        }
    }

    // 画出FPS
    DrawFps(gh);

    // 拷贝到屏幕
    ::BitBlt(hdc, 0, 0, m_rcClient.Width(), m_rcClient.Height(),
             m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);

    return;
}

// 画 fps
void CGame::DrawFps(Gdiplus::Graphics &gh)
{
    // 记录当前fps
    static int fps = 0;
    // 每帧+1
    m_fps++;
    // 最后一次输出时间
    static DWORD dwLast = GetTickCount();
    // 如果超过 1秒钟，则输出fps,并清零计数器
    if(GetTickCount() - dwLast >= 1000) {
        fps = m_fps;
        m_fps = 0;
        dwLast = GetTickCount();
    }

    // 输出fps
    {
        // 输出的文字
        CString s;
        s.Format(_T("FPS:%d"), fps);
        gh.DrawString(s.GetString(), -1, &m_font, origin, &m_brush);
    }
}

