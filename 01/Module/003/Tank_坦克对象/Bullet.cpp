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
        // ֱ��
        m_iDiameter = rhs.m_iDiameter;
        // �ӵ����ڵ�ʱ�� : Ĭ��10��
        m_keepTime = rhs.m_keepTime;
        // ��¼�ӵ�����ʱ��
        m_fireTime = rhs.m_fireTime;
        // ����˭��
        m_pOwner = rhs.m_pOwner;
    }
}

CBullet &CBullet::operator=(const CBullet &rhs)
{
    if(this != &rhs) {
        CGameEntryMoveable::operator=(rhs);
        m_step = rhs.m_step;
        // ֱ��
        m_iDiameter = rhs.m_iDiameter;
        // �ӵ����ڵ�ʱ�� : Ĭ��10��
        m_keepTime = rhs.m_keepTime;
        // ��¼�ӵ�����ʱ��
        m_fireTime = rhs.m_fireTime;
        // ����˭��
        m_pOwner = rhs.m_pOwner;
    }
    return *this;
}

// �Ƿ�ʱ��
bool CBullet::IsTimeout() const
{
    return (GetTickCount() - m_fireTime >= m_keepTime);
}

// �����ӵ�λ��
void CBullet::SetPositionForCenter(PointF pfCenter)
{
    // �ӵ���λ��
    RectF rect{
        pfCenter.X - m_iDiameter / 2,
        pfCenter.Y - m_iDiameter / 2,
        m_iDiameter, m_iDiameter
    };
    SetRect(rect);
}

// ���Լ�
void CBullet::Draw(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }

    // ��һ��Բ...
    gh.FillEllipse(&SolidBrush(Color::OrangeRed), GetRect());
}

// �����ӵ�������(�ӵ�����˭������)
void CBullet::SetOwner(CTankEntry *pOwner)
{
    m_pOwner = pOwner;
}

// ���������(�ӵ�����˭������)
CTankEntry *CBullet::GetOwner() const
{
    return m_pOwner;
}

// ���ô���ʱ��
void CBullet::SetKeepTime(unsigned long keepTime)
{
    m_keepTime = keepTime;
}

// ���ÿ�ʼʱ�䣬������Ϊ��Ч
void CBullet::SetFireTime(unsigned long fireTime)
{
    m_fireTime = fireTime;
    SetActive(true);
};

// ���ֱ��
float CBullet::GetDiameter() const
{
    return m_iDiameter;
}

// ����ֱ��
void CBullet::SetDiameter(float diameter)
{
    m_iDiameter = diameter;
}

// �����ӵ�Ϊ�״η���
void CBullet::SetFirst()
{
    m_step = 0;
};

// �Ƿ����״η���
bool CBullet::IsFirstFire() const
{
    return m_step == 0;
}

// �ƶ��ӵ�
void CBullet::Move()
{
    if(!IsActive()) {
        return;
    }
    ++m_step;
    // �ӵ� ֻ����ǰ��
    Forward();
}


