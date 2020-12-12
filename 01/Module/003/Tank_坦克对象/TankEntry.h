#pragma once
#include "GameEntry.h"
#include "Bullet.h"
#include <list>
#include "GameTimer.h"


class CTankEntry : public CGameEntryMoveable
{
public:
    CTankEntry();
    CTankEntry(float x, float y, PCTSTR szImgName);
    virtual ~CTankEntry();
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // 设置开枪间隔时间
    void SetFireTimeval(unsigned long timeval);

    // 开火 : 如果时间到达，且有子弹，就发射，并返回 true 否则返回false
    bool Fire(CBullet &blt);

    // 增加子弹
    void AddBullet(CBullet &blt);

    // 获得枪的位置
    virtual PointF GetGunPosition() const;

    // 获取子弹 出枪口时的初始位置
    virtual PointF GetBulletFirstStepPos(float bulletRadius) const;

    // 使坦克爆炸
    void Bomb();

    // 是否有效
    bool IsActive() const;

    // 是否爆炸结束(如果根本有爆炸，同样返回false)
    virtual bool IsBombEnd() const;

    // 判断是否被子弹击中
    virtual bool IsHitted(const CBullet &blt) const;

    // 获取碰撞信息, pDelay：如果发生碰撞，返回碰撞时间
    bool GetHitInfo(const CBullet &blt, float *pDelay = nullptr) const;

    // 对坦克的方向进行微调
    virtual void ChangeDirection(bool bForward);

private:
    // 画正常的坦克
    void DrawTank(Graphics &gh) const;

    // 绘制尾焰
    void DrawTankTail(Graphics &gh) const;

    // 画爆炸的坦克
    void DrawExplosion(Graphics &gh) const;

    // 当前状态: 1,正常,2:爆炸
    int m_status{1};

    // 坦克图片 正常时
    Image *m_imgTank;

    // 坦克图片 爆炸时
    Image *m_imgExplosion[12];
    mutable int m_explosionIndex{ 0 };

    // 子弹
    std::list<CBullet> m_lstBullets;

    // 开火间隔时间
    CGameTimeval m_timer_fire;

    // 爆炸时间
    CGameTimer m_timerBomb;
};

