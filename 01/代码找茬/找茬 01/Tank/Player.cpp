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

// ����ת : ����ת��֮����ʱ�����ơ�����ת��̫��
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

