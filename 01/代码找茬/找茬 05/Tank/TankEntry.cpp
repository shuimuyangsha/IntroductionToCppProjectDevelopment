#include "stdafx.h"
#include "TankEntry.h"
#include "GameHit.h"


CTankEntry::CTankEntry()
{
}

CTankEntry::CTankEntry(float x, float y, PCTSTR szImgName)
{
    m_imgTank = Image::FromFile(szImgName);
    for(int i = 0; i < _countof(m_imgExplosion); ++i) {
        TCHAR szPath[MAX_PATH];
        _stprintf_s(szPath, _T("%d.gif"), i);
        m_imgExplosion[i] = Image::FromFile(szPath);
    }
    int width = m_imgTank->GetWidth();
    int height = m_imgTank->GetHeight();

    // 计算 坦克 前占用的区域
    RectF r(static_cast<float>(x)
            , static_cast<float>(y)
            , static_cast<float>(width)
            , static_cast<float>(height));
    SetRect(r);

    // 速度
    SetSpeed(10);
    // 当前的角度
    SetDirection(0);
    // 每次按键转 30度
    SetDirectionTurn(30);
    // 默认为有效
    SetActive(true);
    // 子弹信息
    for(int i = 0; i < 5; ++i) {
        m_lstBullets.push_back(CBullet(0, 0, 12, 0));
    }
}

CTankEntry::~CTankEntry()
{
}

// 画自己
void CTankEntry::Draw(Gdiplus::Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }

    //旋转绘图平面
    PointF center = GetCenterPoint();

    // 1.平移变换 : 移动坐标点到 坦克中心
    gh.TranslateTransform(center.X, center.Y);
    // 2.旋转变换 : 使作标系，跟坦克的角度一致
    gh.RotateTransform(GetDirection());
    // 3.恢复原点
    gh.TranslateTransform(-center.X, -center.Y);

    switch(m_status) {
        case 1: {
            //绘制坦克
            DrawTank(gh);
            DrawTankTail(gh);
            break;
        }
        case 2: {
            // 绘制爆制的坦克
            DrawExplosion(gh);
            break;
        }
        default:
            break;
    };


#if 1
    // 重置坐标变换 : 精度是否够呢？
    // 1.移动到中心点
    gh.TranslateTransform(center.X, center.Y);
    // 2.逆向转
    gh.RotateTransform(-GetDirection());
    // 3.再移动回去
    gh.TranslateTransform(-center.X, -center.Y);
    //ScaleTransform : 缩放
#else
    // 第二种恢复的方式 : 缺点：直接重置，不好啊
    gh.ResetTransform();
#endif
}

// 设置开枪间隔时间
void CTankEntry::SetFireTimeval(unsigned long timeval)
{
    m_timer_fire.SetTimeval(timeval);
}

// 获得枪的位置
PointF CTankEntry::GetGunPosition() const
{
    return GetHeadPos();
}

// 获取子弹 出枪口时的初始位置
PointF CTankEntry::GetBulletFirstStepPos(float bulletRadius) const
{
    //枪口的位置
    PointF p = GetGunPosition();
    // 向前申出去一点儿,避免第一次发射时，就打爆自己
    p.X += static_cast<Gdiplus::REAL>((bulletRadius) * sin(GetDirectionArc()));
    p.Y -= static_cast<Gdiplus::REAL>((bulletRadius) * cos(GetDirectionArc()));
    return p;
}

// 使坦克爆炸
void CTankEntry::Bomb()
{
    m_timerBomb.SetTimeout(3000);
    m_timerBomb.StartTimer();
    m_status = 2;
}

// 是否有效
bool CTankEntry::IsActive() const
{
    return (CGameEntryMoveable::IsActive() && m_explosionIndex <= _countof(m_imgExplosion));
}

// 是否爆炸结束(如果根本有爆炸，同样返回false)
bool CTankEntry::IsBombEnd() const
{
    return m_status == 2 && m_timerBomb.IsTimeout();
}

// 对坦克的方向进行微调
void CTankEntry::ChangeDirection(bool bForward)
{
    return; // 不要了
    float dir = GetDirection();
    int _d = int(dir);
    if(bForward) {
        if(0 < _d && _d < 180) {
            dir += 1;
        }
        else {
            dir -= 1;
        }
    }
    else {
        if(0 < _d && _d < 180) {
            dir -= 1;
        }
        else {
            dir += 1;
        }
    }
    SetDirection(dir);
}

// 画正常的坦克
void CTankEntry::DrawTank(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    gh.DrawImage(m_imgTank, GetRect());
}

// 绘制尾焰
void CTankEntry::DrawTankTail(Graphics &gh) const
{
}

// 画爆炸的坦克
void CTankEntry::DrawExplosion(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    //绘制爆炸图片
    auto p = m_imgExplosion[m_explosionIndex];
    if(p) {
        RectF rect;
        // 自己的中心点
        PointF pCenter = GetCenterPoint();
        rect.Width = static_cast<float>(p->GetWidth());
        rect.Height = static_cast<float>(p->GetHeight());
        rect.X = 0;
        rect.Y = 0;
        rect.Offset(pCenter.X - rect.Width / 2, pCenter.Y - rect.Height / 2);
        gh.DrawImage(p, rect);
    }
    // 换一下张图片
    static unsigned long last = GetTickCount();
    if(GetTickCount() - last > 50) {
        m_explosionIndex++;
        if(m_explosionIndex >= _countof(m_imgExplosion)) {
            m_explosionIndex = _countof(m_imgExplosion) - 1;
        }
    }
}

// 开火
bool CTankEntry::Fire(CBullet &_blt)
{
    if(!IsActive()) {
        return false;
    }
    // 检测开枪间隔时间
    if(!m_timer_fire.IsTimeval()) {
        return false;
    }

    // 获取头部位置
    PointF front = GetGunPosition();// GetBulletFirstStepPos(_blt.GetDiameter() / 2.0f);
    //检查 子弹信息
    //如果还有子弹，就在头部发出,并且从列表中移除，加到另一个列表
    if(!m_lstBullets.empty()) {
        CBullet &blt = m_lstBullets.back();
        // 设置初始 坐标
        blt.SetPositionForCenter(front);
        // 设置 运动方向
        blt.SetDirection(GetDirection());
        // 设置初始速度
        blt.SetSpeed(5);
        // 设置为有效
        blt.SetActive(true);
        // 设置发射时间
        blt.SetFireTime(GetTickCount());
        // 设置所有者
        blt.SetOwner(this);
        // 设置为首次发射
        blt.SetFirst();
        // 加入到地图列表列表中
        _blt = blt;
        // 本地删除
        m_lstBullets.pop_back();
        // 记录开枪时间
        m_timer_fire.SetLastTime();
        return true;
    }
    return false;
}

// 增加子弹
void CTankEntry::AddBullet(CBullet &blt)
{
    if(!IsActive()) {
        return;
    }
    m_lstBullets.push_back(blt);
}

// 判断是否被子弹击中
bool CTankEntry::IsHitted(const CBullet &blt) const
{
    if(blt.IsFirstFire() && blt.GetOwner() == this) {
        return false;
    }
    return GetHitInfo(blt);
}

// 获取碰撞信息
bool CTankEntry::GetHitInfo(const CBullet &blt, float *pDelay /*= nullptr*/) const
{
    // 子弹 初次发射，不能把自己打爆
    if(blt.IsFirstFire() && blt.GetOwner() == this) {
        return false;
    }
    // 检查是否发生碰撞
    //代表子弹当前的位置
    game_hit::Circle A;
    A.r = blt.GetDiameter() / 2.0f;
    A.x = blt.GetCenterPoint().X;
    A.y = blt.GetCenterPoint().Y;

    // 把小球向某方向移动
    auto move_step = [](game_hit::Circle & A, float dir, float distance) {
        A.x += distance * sin(dir);
        A.y -= distance * cos(dir);
    };

    // 自己
    RectF B = GetRect();

    // 为了防止子弹穿过导致的碰撞检测失效
    // 使子弹对象在行进方向上，一个像素，一个像素的前进，并进行碰撞检测
    for(int step = 0; step < blt.GetSpeed(); ++step) {
        move_step(A, blt.GetDirectionArc(), step);
        if(game_hit::check_collision(A, B, GetDirectionArc())) {
            if(pDelay) {
                *pDelay = step;
            }
            return true;
        }
    }
    return false;
}
