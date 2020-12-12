#pragma once
#include "GameEntry.h"
#include "Bullet.h"

class CWall : public CGameEntryMoveable
{

public:
    enum EWallDirection {EDirectionX /*����*/, EDirectionY/*����*/};

public:
    CWall();
    CWall(float x, float y, float width, float length, CWall::EWallDirection dir);
    virtual ~CWall();

    // ���Լ�
    virtual void Draw(Graphics &gh) const;

    // ���÷���
    void SetDir(CWall::EWallDirection dir);

    // ��÷���
    CWall::EWallDirection GetDir() const;

public:
    // ��� С��ײ��ǽ,��ı䷽���λ��(����Ϊ�պò�ײǽ��λ��)
    bool HitAction(CBullet &moveObj) const;

    // �ж��Ƿ���ײ�����������ײ��������ײ�㣬�������
    bool CheckHit(const CBullet &moveObj, PointF *ptHit = nullptr, float *dir = nullptr, float *pDelay = nullptr) const;

    // ��һ���Ƿ��ײ��ǽ
    bool IsWillHit(const CTankEntry &tank, bool bForward) const;

private:
    // �ж�С��ײ��ǽ����һ��,�����ײ������������ײ����һ�ߵ���ײ��Ϣ
    bool CheckHitEx(const CBullet &moveObj, PointF *ptHit = nullptr, float *dir = nullptr, float *pDelay = nullptr) const;

private:


    //ǽ�ķ���:ֻ�����ַ��� ���򣬼���
    EWallDirection m_direction;

    // ǽ����ɫ
    mutable Color m_clr{0x4D, 0x4D, 0x4D};
};

