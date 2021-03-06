#pragma once

#include "GameMenuPanel.h"
#include "GameMenuBackground.h"

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

    // 开始菜单
    CGameMenuPanel m_menuSelect;

    // 开始菜单背景图
    CGameMenuBackground m_menu;

};

