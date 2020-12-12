#pragma once
#include "TankEntry.h"
class CPlayer :
    public CTankEntry
{
public:
    CPlayer();
    CPlayer(float x, float y, PCTSTR szImgName);
    virtual ~CPlayer();
    // ����ת : ����ת��֮����ʱ�����ơ�����ת��̫��
    virtual void RotateRight();
    
    virtual void RotateLeft();

private:
    // ������ζ���֮���ʱ��
    CGameTimeval m_timer{100};
};

