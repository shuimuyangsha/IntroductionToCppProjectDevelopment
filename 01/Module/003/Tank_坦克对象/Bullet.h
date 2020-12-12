#pragma once
#include "GameEntry.h"

class CTankEntry;

class CBullet : public CGameEntryMoveable
{
public:
    CBullet();
    CBullet(float x, float y, float speed, float direction);
    virtual ~CBullet();

    CBullet(const CBullet &rhs);

    CBullet &operator=(const CBullet &rhs);

    // 画自己
    virtual void Draw(Graphics &gh) const;

    // 移动子弹
    void Move();

    // 是否超时了
    bool IsTimeout() const;

    // 设置子弹位置
    void SetPositionForCenter(PointF pfCenter);

    // 设置子弹所有者(子弹是由谁发出的)
    void SetOwner(CTankEntry *pOwner);

    // 获得所有者(子弹是由谁发出的)
    CTankEntry *GetOwner() const;

    // 设置存在时间
    void SetKeepTime(unsigned long keepTime);

    // 设置开始时间，并设置为有效
    void SetFireTime(unsigned long fireTime);

    // 获得直径
    float GetDiameter() const;

    // 设置直径
    void SetDiameter(float diameter);

    // 设置子弹为首次发出
    void SetFirst();

    // 是否是首次发出
    bool IsFirstFire() const;

private:
    int m_step{0};
    // 直径
    float m_iDiameter{10};
    // 子弹存在的时间 : 默认10秒
    unsigned long m_keepTime{10 * 1000};
    // 记录子弹发射时间
    unsigned long m_fireTime{0};
    // 属于谁的
    CTankEntry *m_pOwner{nullptr};
};
