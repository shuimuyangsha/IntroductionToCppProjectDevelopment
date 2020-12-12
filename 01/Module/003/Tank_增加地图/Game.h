#pragma once

#include "GameMenuPanel.h"
#include "GameMenuBackground.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include "GameMap.h"


class CGame
{
public:
    CGame();
    ~CGame();

    // 设置输出窗口的句柄
    void SetHandle(HWND hWnd);

    // 进入游戏帧
    bool EnterFrame(DWORD dwTime);

    // 处理鼠标移动事件
    void OnMouseMove(UINT nFlags, CPoint point);

    // 处理左键抬起事件
    void OnLButtonUp(UINT nFlags, CPoint point);

private:
    // 窗口
    HWND m_hWnd;

    /* 游戏绘图处理
    负责 绘画 游戏中的对象
    */
    void GameRunDraw();

    /* 游戏逻辑处理:
     * 1. 维护子弹状态
     * 2. 维护 机器人AI的自动移动,自动发射子弹
     * 3. 维护 玩家 坦克的状态
     * 以测检测包括： 撞墙，子弹命中坦克...
    **/
    void GameRunLogic();


    // 开始菜单
    CGameMenuPanel m_menuSelect;

    // 开始菜单背景图
    CGameMenuBackground m_menu;

    // 两个玩家对象
    CPlayer m_player01{100, 100, _T("tank_player1.png")};

    // 玩家对象2: 双人对战时才会用到
    CPlayer m_player02{200, 100, _T("tank_player2.png")};

    // 一个电脑: 人机对战时用到
    CBot m_bot{300, 200, _T("tank_bot.png")};

    // 存在于地图场景中的 子弹对象数组
    std::list<CBullet> m_lstBullets;

    // 地图对象
    CGameMap m_map{ 10, 10, 780, 580 };

    // 游戏初始化 : 单人对电脑
    bool CGame::GameInitOne2Bot();
};

