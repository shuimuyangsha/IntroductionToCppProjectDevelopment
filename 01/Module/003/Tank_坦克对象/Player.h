#pragma once
#include "TankEntry.h"
class CPlayer :
    public CTankEntry
{
public:
    CPlayer();
    CPlayer(float x, float y, PCTSTR szImgName);
    virtual ~CPlayer();
    // 左右转 : 两次转动之间有时间限制。不能转得太快
    virtual void RotateRight();
    
    virtual void RotateLeft();

private:
    // 检查两次动作之间的时间
    CGameTimeval m_timer{100};
};

