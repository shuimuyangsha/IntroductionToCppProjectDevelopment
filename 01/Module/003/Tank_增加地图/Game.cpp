#include "stdafx.h"
#include "Game.h"
#include <algorithm>


CGame::CGame()
{
    GameInitOne2Bot();
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
    GameRunLogic();
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
        // 画出地图
        m_map.Draw(gh);
        // 画坦克(玩家一)
        m_player01.Draw(gh);
        // 画子弹(所有存在于地图上的坦克)
        for(auto &blt : m_lstBullets) {
            blt.Draw(gh);
        }
    }

    // 拷贝到屏幕
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // 释放
    dc->DeleteDC();
    return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
    //按键处理
    {
        if(KEYDOWN(VK_LEFT))
        {
            m_player01.RotateLeft();
        }
        if(KEYDOWN(VK_RIGHT))
        {
            m_player01.RotateRight();
        }
        if(KEYDOWN(VK_UP))
        {
            // 坦克撞墙检测试
            {
                if(m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if(KEYDOWN(VK_DOWN))
        {
            // 坦克撞墙检测试
            {
                if(m_map.IsHitTheWall(m_player01, false)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Backward();
                }
            }
        }
        if(KEYDOWN('M'))
        {
            CBullet blt;
            if(m_player01.Fire(blt)) {
                // 加入到地图列表中
                m_lstBullets.push_back(blt);
            }
        }
    }

    // 处理子弹对象的移动
    for(auto &blt : m_lstBullets) {
        blt.Move(); // 子弹向前移动
    }

    // 移除超时的子弹
    {
        auto itRemove = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(), [](CBullet & blt)->bool {return blt.IsTimeout(); });
        for(auto it = itRemove; it != m_lstBullets.end(); ++it) {
            // 设置为无效
            it->SetActive(false);
            // 从地图列表中移除
            // 给对应的坦克增加子弹
            it->GetOwner()->AddBullet(*it);
        }
        // 从本地 删除 子弹
        m_lstBullets.erase(itRemove, m_lstBullets.end());
    }

    // 子弹撞墙处理
    {
        // 子弹是否撞上墙 : 如果撞上了，改变方向等等
        for(auto &blt : m_lstBullets) {
            // 进行撞墙处理
            m_map.HitWallProcess(blt);
            blt.Move();
        }
    }

    // 检查子弹是否击中坦克 :  击中要使坦克爆炸(子弹可不用移除了，因为游戏都结束了)
    for (auto &blt : m_lstBullets) {
        // 击中玩家1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
        }
        break;
    }
}


// 游戏初始化 : 单人对电脑
bool CGame::GameInitOne2Bot()
{
    {
        // 地图
        m_map.LoadMap();
        // 玩家一
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if(!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("调整Player01位置失败"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }

        // 子弹
        m_lstBullets.clear();
    }
    return true;
}




