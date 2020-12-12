#pragma once
#include "GameEntry.h"
#include "Bullet.h"

class CWall : public CGameEntryMoveable
{

public:
    enum EWallDirection {EDirectionX /*横向*/, EDirectionY/*纵向*/};

public:
    CWall();
    CWall(float x, float y, float width, float length, CWall::EWallDirection dir);
    virtual ~CWall();

    // 画自己
    virtual void Draw(Graphics &gh) const;

    // 设置方向
    void SetDir(CWall::EWallDirection dir);

    // 获得方向
    CWall::EWallDirection GetDir() const;

public:
    // 如果 小球撞上墙,则改变方向和位置(调整为刚好不撞墙的位置)
    bool HitAction(CBullet &moveObj) const;

    // 判断是否碰撞，如果发生碰撞，返回碰撞点，及反射角
    bool CheckHit(const CBullet &moveObj, PointF *ptHit = nullptr, float *dir = nullptr, float *pDelay = nullptr) const;

    // 下一步是否会撞上墙
    bool IsWillHit(const CTankEntry &tank, bool bForward) const;

private:
    // 判断小球撞上墙的哪一边,如果相撞，返回最先碰撞的那一边的碰撞信息
    bool CheckHitEx(const CBullet &moveObj, PointF *ptHit = nullptr, float *dir = nullptr, float *pDelay = nullptr) const;

private:


    //墙的方向:只有两种方向 横向，坚向
    EWallDirection m_direction;

    // 墙的颜色
    mutable Color m_clr{0x4D, 0x4D, 0x4D};
};

