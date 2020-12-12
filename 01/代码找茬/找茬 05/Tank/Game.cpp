#include "stdafx.h"
#include "Game.h"

#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

CGame::CGame()
{
    m_menuSelect.m_pParent = this;
    m_menuBackup.m_pParent = this;
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
    // 选择阶段
    if (m_eStep == EGameTypeMenu) {
        // 选择游戏类型
        m_menuSelect.OnMouseMove(nFlags, point);
    }
    else {
        // 返回主菜单
        m_menuBackup.OnMouseMove(nFlags, point);
    }
}

// 处理左键抬起事件
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // 选择阶段
    if (m_eStep == EGameTypeMenu) {
        // 选择游戏类型
        m_menuSelect.OnLButtonUp(nFlags, point);
    }
    else {
        // 返回主菜单
        m_menuBackup.OnLButtonUp(nFlags, point);
    }
}

// 设置当前游戏所处的阶段
// 并根据步聚 初始化
void CGame::SetStep(CGame::EGameType step)
{
    m_eStep = step;
    GameInit();
}

/* 游戏初始化
生成 游戏对象，初始化地图，对象位置等
*/
bool CGame::GameInit()
{
    //  初始化随机数生成器
    srand(GetTickCount());
    // 根据不同阶段调用不同的处理函数
    return (this->*m_initFunc[m_eStep])();
}

// 游戏初始化 : 选择阶段
bool CGame::GameInitMenu()
{
    return true;
}

// 游戏初始化 : 单人对电脑 菜单阶段
bool CGame::GameInitOne2BotMenu()
{
    // 设置 玩家一的 keyMenu 位置 : 屏幕正中间
    RECT rc;
    GetWindowRect(m_hWnd, &rc);
    PointF pt;
    pt.X = rc.left + (rc.right - rc.left) / 2.0f;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu01.SetCenterPoint(pt);
    m_keymenu01.SetStop(false);
    return true;
}

// 游戏初始化 : 单人对电脑
bool CGame::GameInitOne2Bot()
{
    for (; ;) {
        // 地图
        m_map.LoadMap();
        // 玩家一
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("调整Player01位置失败"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }

        // 敌军
        {
            m_bot = CBot(0, 0, _T("tank_bot.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("调整Bot位置失败"));
            }
            else {
                m_bot.SetCenterPoint(ptCenter);
            }
        }
        // 子弹
        m_lstBullets.clear();

        // 判断是否合法
        {
            // 机器人，玩家 所在的位置
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_bot, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                AfxMessageBox(_T("获取坦克位置发生错误"));
                goto __Init_End;
            }
            VPath path;
            m_map.FindPath(startX, startY, targetX, targetY, path);
            if (!path.empty()) {
                goto __Init_End;
            }
        }
    }
__Init_End:
    return true;
}

// 游戏初始化 : 单人对电脑 结束
bool CGame::GameInitOne2BotEnd()
{

    return true;
}

// 游戏初始化 : 双人对战 菜单阶段
bool CGame::GameInitOne2OneMenu()
{
    // 设置 两个玩家的 keyMenu 位置 : 屏幕正中间
    RECT rc;
    GetWindowRect(m_hWnd, &rc);
    PointF pt;
    pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu01.SetCenterPoint(pt);
    m_keymenu01.SetStop(false);


    pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu02.SetCenterPoint(pt);
    m_keymenu02.SetStop(false);

    return true;
}

// 游戏初始化 : 双人对战
bool CGame::GameInitOne2One()
{
    for (;;) {
        // 地图
        m_map.LoadMap();
        //中间放置坦克
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("调整Player01位置失败"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }
        {
            m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("调整Player02位置失败"));
            }
            else {
                m_player02.SetCenterPoint(ptCenter);
            }
        }

        // 子弹
        m_lstBullets.clear();
        // 判断是否合法
        {
            // 机器人，玩家 所在的位置
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_player02, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                AfxMessageBox(_T("获取坦克位置发生错误"));
                break;
            }
            VPath path;
            m_map.FindPath(startX, startY, targetX, targetY, path);
            if (!path.empty()) {
                break;
            }
        }
    }
    return true;
}

// 游戏初始化 : 双人对战 结束
bool CGame::GameInitOne2OneEnd()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////

/* 游戏逻辑处理:
1. 维护子弹状态
2. 维护 机器人AI的自动移动,自动发射子弹
3. 维护 玩家 坦克的状态
以测检测包括： 撞墙，子弹命中坦克...*/
void CGame::GameRunLogic()
{
    // 根据不同阶段调用不同的处理函数
    (this->*m_logicFunc[m_eStep])();
}

// 游戏逻辑处理 : 选择阶段
void CGame::GameRunLogicOnMenu()
{
    // 什么也不做，还没开始游戏
}

// 游戏逻辑处理 : 单人对电脑 菜单阶段
void CGame::GameRunLogicOnOne2BotMenu()
{
    // 如果按下了M键，停止动画状态
    if (KEYDOWN('M')) {
        m_keymenu01.SetStop();
    }

    // 如果都按下了，正式开始游戏
    if (m_keymenu01.GetStop()) {
        SetStep(EGameTypeOne2Bot);
    }
}

// 游戏逻辑处理 : 单人对电脑
void CGame::GameRunLogicOnOne2Bot()
{
    // 状态维护
    // 移动除列表中无效的子弹,并给相应的坦克增加子弹
    RemoveTimeoutBullets();

    // 检查子弹是否击中坦克 :  击中要使坦克爆炸(子弹可不用移除了，
    // 因为游戏都结束了)
    for (auto &blt : m_lstBullets) {
        // 击中机器人
        if (m_bot.IsHitted(blt)) {
            m_bot.Bomb();
            // 游戏结束
            m_eStep = EGameTypeOne2BotEnd;
            blt.SetActive(false);
        }
        // 击中玩家1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
            // 游戏结束
            m_eStep = EGameTypeOne2BotEnd;
            blt.SetActive(false);
        }
        break;
    }

    // 子弹运动维护
    ProcessHitBullets();

    AI();

    //按键处理
    {
        if (KEYDOWN(VK_LEFT)) {
            m_player01.RotateLeft();
        }
        if (KEYDOWN(VK_RIGHT)) {
            m_player01.RotateRight();
        }
        if (KEYDOWN(VK_UP)) {
            // 坦克撞墙检测试
            {
                if (m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if (KEYDOWN(VK_DOWN)) {
            {
                // 坦克撞墙检测试
                {
                    if (m_map.IsHitTheWall(m_player01, false)) {
                        m_player01.ChangeDirection(true);
                    }
                    else {
                        m_player01.Backward();
                    }
                }
            }
        }
        if (KEYDOWN('M')) {
            CBullet blt;
            if (m_player01.Fire(blt)) {
                // 加入到地图列表中
                m_lstBullets.push_back(blt);
            }
        }
        if (KEYDOWN('I')) {
            // 机器人，玩家 所在的位置
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_bot, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                return;
            }
            float fDirNext = 0; //机器人下一步的方向
            if (!m_map.FindNextDirection(&fDirNext, startX, startY, 
                targetX, targetY)) {
                return;
            }

            PointF ptTankCenter = m_bot.GetCenterPoint();
            PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
            RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


            // 判断 坦克是否己经走到了中心点位置了
            if (!rc.Contains(ptTankCenter)) {
                m_bot.Forward(); // 没有到达中心点，继续前进
                return;
            }
            else {
                m_bot.SetDirection(fDirNext);
                m_bot.Forward();
            }
        }
    }

}

// 游戏逻辑处理 : 单人对电脑 结束
void CGame::GameRunLogicOnOne2BotEnd()
{
    //按键处理
    // 不再接受按键


    // 状态维护
    // 移动除列表中无效的子弹,并给相应的坦克增加子弹
    RemoveTimeoutBullets();


    // 子弹是否撞上墙 : 如果撞上了，改变方向等等
    ProcessHitBullets();
}

// 游戏逻辑处理 : 双人对战 菜单阶段
void CGame::GameRunLogicOnOne2OneMenu()
{
    // 如果按下了M键，停止动画状态
    if (KEYDOWN('M')) {
        m_keymenu01.SetStop();
    }
    // 如果按下了Q键，停止动画状态
    if (KEYDOWN('Q')) {
        m_keymenu02.SetStop();
    }

    // 如果都按下了，正式开始游戏
    if (m_keymenu01.GetStop() && m_keymenu02.GetStop()) {
        SetStep(EGameTypeOne2One);
    }
}

// 游戏逻辑处理 : 双人对战
void CGame::GameRunLogicOnOne2One()
{
    //按键处理
    {
        // 玩家一
        if (KEYDOWN(VK_LEFT)) {
            m_player01.RotateLeft();
        }
        if (KEYDOWN(VK_RIGHT)) {
            m_player01.RotateRight();
        }
        if (KEYDOWN(VK_UP)) {
            // 坦克撞墙检测试
            {
                if (m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if (KEYDOWN(VK_DOWN)) {
            {
                // 坦克撞墙检测试
                {
                    if (m_map.IsHitTheWall(m_player01, false)) {
                        m_player01.ChangeDirection(false);
                    }
                    else {
                        m_player01.Backward();
                    }
                }
            }
        }
        //if (KEYDOWN('M')) {
        //    CBullet blt;
        //    if (m_player01.Fire(blt)) {
        //        m_lstBullets.push_back(blt);
        //    }
        //}
        // 玩家二
        if (KEYDOWN('A')) {
            m_player02.RotateLeft();
        }
        if (KEYDOWN('D')) {
            m_player02.RotateRight();
        }
        if (KEYDOWN('W')) {
            // 坦克撞墙检测试
            {
                if (m_map.IsHitTheWall(m_player02, true)) {
                    m_player02.ChangeDirection(true);
                }
                else {
                    m_player02.Forward();
                }
            }
        }
        if (KEYDOWN('S')) {
            {
                // 坦克撞墙检测试
                {
                    if (m_map.IsHitTheWall(m_player02, false)) {
                        m_player02.ChangeDirection(false);
                    }
                    else {
                        m_player02.Backward();
                    }
                }
            }
        }
        if (KEYDOWN('Q')) {
            CBullet blt;
            if (m_player02.Fire(blt)) {
                m_lstBullets.push_back(blt);
            }
        }
        if (KEYDOWN('Z')) {
            if (m_map.IsCanKillTarget(m_player01, m_player02)) {
                AfxMessageBox(_T("可以打到"));
            }
        }
    }

    // 先判断状态
    // 移动除列表中无效的子弹,并给相应的坦克增加子弹
    RemoveTimeoutBullets();

    // 检查子弹是否击中坦克 :  击中要使坦克爆炸(子弹可不用移除了，因为游戏都结束了)
    for (auto &blt : m_lstBullets) {
        if (!blt.IsActive()) {
            continue;
        }
        // 击中玩家1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
            // 游戏结束
            m_eStep = EGameTypeOne2OneEnd;
            blt.SetActive(false);
        }
        // 击中玩家2
        if (m_player02.IsHitted(blt)) {
            m_player02.Bomb();
            // 游戏结束
            m_eStep = EGameTypeOne2OneEnd;
            blt.SetActive(false);
        }
    }

    // 撞墙
    ProcessHitBullets();
}

// 游戏逻辑处理 : 双人对战 结束
void CGame::GameRunLogicOnOne2OneEnd()
{
    //按键处理
    // 不需要按键处理

    // 移动除列表中无效的子弹,并给相应的坦克增加子弹
    RemoveTimeoutBullets();

    // 撞墙
    ProcessHitBullets();
}

////////////////////////////////////////////////////////////////////////////////

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
    (this->*m_drawFunc[m_eStep])(gh);

    // 拷贝到屏幕
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), 
        m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // 释放
    ::ReleaseDC(m_hWnd, hdc);
    return;
}

// 选择阶段
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
    m_menuSelect.Draw(gh);
}

//单人对电脑 : 菜单阶段
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
    // 画背景
    m_menu.Draw(gh);

    // 画菜单
    m_keymenu01.Draw(gh);
}

//单人对电脑
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
    // 菜单
    m_menuBackup.Draw(gh);
    // 墙
    m_map.Draw(gh);

    // 玩家
    m_player01.Draw(gh);

    // 机器人
    m_bot.Draw(gh);

    // 画子弹 :  己经发射的
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }

    // 输出 : FPS
    DrawFps(gh);
}

//单人对电脑 结束
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
    // 菜单
    m_menuBackup.Draw(gh);
    // 墙
    m_map.Draw(gh);
    // 玩家
    m_player01.Draw(gh);
    // 机器人
    m_bot.Draw(gh);
    // 画子弹 :  己经发射的
    //for(auto b : m_lstBullets) {
    //    b.Draw(gh);
    //}

    // 输出 : FPS
    DrawFps(gh);

    // 判断游戏整体结束
    if (m_player01.IsBombEnd() || m_bot.IsBombEnd()) {
        m_eStep = EGameTypeMenu;
    }
}

// 双人对战 : 菜单阶段
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
    // 画背景
    m_menu.Draw(gh);

    // 画菜单
    m_keymenu01.Draw(gh);
    m_keymenu02.Draw(gh);
}

// 双人对战
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
    // 菜单
    m_menuBackup.Draw(gh);
    // 墙
    m_map.Draw(gh);

    // 玩家一
    m_player01.Draw(gh);

    // 玩家二
    m_player02.Draw(gh);

    // 画子弹 :  己经发射的
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }

    // 输出 : FPS
    DrawFps(gh);
}

// 双人对战 结束
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
    // 菜单
    m_menuBackup.Draw(gh);
    // 墙
    m_map.Draw(gh);
    // 玩家一
    m_player01.Draw(gh);
    // 玩家二
    m_player02.Draw(gh);
    // 画子弹 :  己经发射的
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }
    // 输出 : FPS
    DrawFps(gh);

    // 判断游戏整体结束
    if (m_player01.IsBombEnd() || m_player02.IsBombEnd()) {
        m_eStep = EGameTypeMenu;
    }
}

// 画 fps
void CGame::DrawFps(Graphics &gh)
{
    static int fps = 0;
    m_fps++;
    static DWORD dwLast = GetTickCount();
    if (GetTickCount() - dwLast >= 1000) {
        fps = m_fps;
        m_fps = 0;
        dwLast = GetTickCount();
    }

    // 输出fps
    {
        CString s;
        s.Format(_T("FPS:%d"), fps);
        SolidBrush brush(Color(0x00, 0x00, 0xFF));
        Gdiplus::Font font(_T("宋体"), 10.0);
        CRect rc;
        ::GetClientRect(m_hWnd, &rc);
        // 在右上角显示
        PointF origin(static_cast<float>(rc.right - 50), 
            static_cast<float>(rc.top + 2));
        gh.DrawString(s.GetString(), -1, &font, origin, &brush);
    }
}

// 移除超时子弹,并给对应的坦克装弹
void CGame::RemoveTimeoutBullets()
{
    // 定义查找函数
    auto itRemove = std::remove_if(m_lstBullets.begin(), 
        m_lstBullets.end(),
        [](CBullet & blt)->bool {return blt.IsTimeout(); });

    // 把子弹移除,并给对应的坦克增加子弹,
    for (auto it = itRemove; it != m_lstBullets.end(); ++it) {
        // 设置为无效
        it->SetActive(false);
        // 从地图列表中移除
        // 给对应的坦克增加子弹
        it->GetOwner()->AddBullet(*it);
    }
    // 从本地 删除 子弹
    m_lstBullets.erase(itRemove, m_lstBullets.end());
}

// 子弹运动的维护:撞墙拐弯
void CGame::ProcessHitBullets()
{
    // 子弹是否撞上墙 : 如果撞上了，改变方向等等
    for (auto &blt : m_lstBullets) {
        // 进行撞墙处理
        m_map.HitWallProcess(blt);
        blt.Move();
    }
}

// 维护电脑的自动寻路攻击
void CGame::AI()
{
    // 电脑运动状态态维护
    static CGameTimer acTimer(-1, 150);
    if (acTimer.IsTimeval()) {
        // 机器人，玩家 所在的位置
        int startX, startY, targetX, targetY;
        if (!m_map.FindObjPosition(m_bot, startX, startY) || 
            !m_map.FindObjPosition(m_player01, targetX, targetY)) {
            return;
        }
        float fDirNext = 0; //机器人下一步的方向
        if (!m_map.FindNextDirection(&fDirNext, 
            startX, startY, 
            targetX, targetY)) {
            return;
        }

        PointF ptTankCenter = m_bot.GetCenterPoint();
        PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
        RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


        // 判断 坦克是否己经走到了中心点位置了
        if (!rc.Contains(ptTankCenter)) {
            m_bot.Forward(); // 没有到达中心点，继续前进
            return;
        }
        else {
            m_bot.SetDirection(fDirNext);
            float dir;
            if (m_map.IsCanKillTarget(m_bot, m_player01, &dir)) {
                CBullet blt;
                if (m_bot.Fire(blt)) {
                    m_lstBullets.push_back(blt);
                }
                return;
            }
            m_bot.Forward();
        }
    }
}
