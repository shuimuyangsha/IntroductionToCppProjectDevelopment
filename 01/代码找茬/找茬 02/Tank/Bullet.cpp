#include "stdafx.h"
#include "GameEntry.h"
#include "Bullet.h"


CBullet::CBullet()
{
}

CBullet::CBullet(float x, float y, float speed, float direction)
{
    SetSpeed(speed);
    SetDirection(direction);
    SetRect(RectF(x, y, m_iDiameter, m_iDiameter));
    m_step = 0;
}

CBullet::~CBullet()
{
}

CBullet::CBullet(const CBullet &rhs)
    : CGameEntryMoveable(rhs)
{
    if(this != &rhs) {
        m_step = rhs.m_step;
        // 直径
        m_iDiameter = rhs.m_iDiameter;
        // 子弹存在的时间 : 默认10秒
        m_keepTime = rhs.m_keepTime;
        // 记录子弹发射时间
        m_fireTime = rhs.m_fireTime;
        // 属于谁的
        m_pOwner = rhs.m_pOwner;
    }
}

CBullet &CBullet::operator=(const CBullet &rhs)
{
    if(this != &rhs) {
        CGameEntryMoveable::operator=(rhs);
        m_step = rhs.m_step;
        // 直径
        m_iDiameter = rhs.m_iDiameter;
        // 子弹存在的时间 : 默认10秒
        m_keepTime = rhs.m_keepTime;
        // 记录子弹发射时间
        m_fireTime = rhs.m_fireTime;
        // 属于谁的
        m_pOwner = rhs.m_pOwner;
    }
    return *this;
}

// 是否超时了
bool CBullet::IsTimeout() const
{
    return (GetTickCount() - m_fireTime >= m_keepTime);
}

// 设置子弹位置
void CBullet::SetPositionForCenter(PointF pfCenter)
{
    // 子弹的位置
    RectF rect{
        pfCenter.X - m_iDiameter / 2,
        pfCenter.Y - m_iDiameter / 2,
        m_iDiameter, m_iDiameter
    };
    SetRect(rect);
}

// 画自己
void CBullet::Draw(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }

    // 画一个圆...
    gh.FillEllipse(&SolidBrush(Color::OrangeRed), GetRect());
}

// 设置子弹所有者(子弹是由谁发出的)
void CBullet::SetOwner(CTankEntry *pOwner)
{
    m_pOwner = pOwner;
}

// 获得所有者(子弹是由谁发出的)
CTankEntry *CBullet::GetOwner() const
{
    return m_pOwner;
}

// 设置存在时间
void CBullet::SetKeepTime(unsigned long keepTime)
{
    m_keepTime = keepTime;
}

// 设置开始时间，并设置为有效
void CBullet::SetFireTime(unsigned long fireTime)
{
    m_fireTime = fireTime;
    SetActive(true);
};

// 获得直径
float CBullet::GetDiameter() const
{
    return m_iDiameter;
}

// 设置直径
void CBullet::SetDiameter(float diameter)
{
    m_iDiameter = diameter;
}

// 设置子弹为首次发出
void CBullet::SetFirst()
{
    m_step = 0;
};

// 是否是首次发出
bool CBullet::IsFirstFire() const
{
    return m_step == 0;
}

// 移动子弹
void CBullet::Move()
{
    if(!IsActive()) {
        return;
    }
    ++m_step;
    // 子弹 只能向前进
    Forward();
}


