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

    // ���Լ�
    virtual void Draw(Graphics &gh) const;

    // �ƶ��ӵ�
    void Move();

    // �Ƿ�ʱ��
    bool IsTimeout() const;

    // �����ӵ�λ��
    void SetPositionForCenter(PointF pfCenter);

    // �����ӵ�������(�ӵ�����˭������)
    void SetOwner(CTankEntry *pOwner);

    // ���������(�ӵ�����˭������)
    CTankEntry *GetOwner() const;

    // ���ô���ʱ��
    void SetKeepTime(unsigned long keepTime);

    // ���ÿ�ʼʱ�䣬������Ϊ��Ч
    void SetFireTime(unsigned long fireTime);

    // ���ֱ��
    float GetDiameter() const;

    // ����ֱ��
    void SetDiameter(float diameter);

    // �����ӵ�Ϊ�״η���
    void SetFirst();

    // �Ƿ����״η���
    bool IsFirstFire() const;

private:
    int m_step{0};
    // ֱ��
    float m_iDiameter{10};
    // �ӵ����ڵ�ʱ�� : Ĭ��10��
    unsigned long m_keepTime{10 * 1000};
    // ��¼�ӵ�����ʱ��
    unsigned long m_fireTime{0};
    // ����˭��
    CTankEntry *m_pOwner{nullptr};
};
