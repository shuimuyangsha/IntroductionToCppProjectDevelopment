#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

CPlayer::CPlayer(float x, float y, PCTSTR szImgName) : CTankEntry(x, y, szImgName)
{

}

// 左右转 : 两次转动之间有时间限制。不能转得太快
void CPlayer::RotateRight()
{
    if(m_timer.IsTimeval()) {
        CTankEntry::RotateRight();
    }
}

void CPlayer::RotateLeft()
{
    if(m_timer.IsTimeval()) {
        CTankEntry::RotateLeft();
    }
}

