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

    // ���ÿ�ǹ���ʱ��
    void SetFireTimeval(unsigned long timeval);

    // ���� : ���ʱ�䵽������ӵ����ͷ��䣬������ true ���򷵻�false
    bool Fire(CBullet &blt);

    // �����ӵ�
    void AddBullet(CBullet &blt);

    // ���ǹ��λ��
    virtual PointF GetGunPosition() const;

    // ��ȡ�ӵ� ��ǹ��ʱ�ĳ�ʼλ��
    virtual PointF GetBulletFirstStepPos(float bulletRadius) const;

    // ʹ̹�˱�ը
    void Bomb();

    // �Ƿ���Ч
    bool IsActive() const;

    // �Ƿ�ը����(��������б�ը��ͬ������false)
    virtual bool IsBombEnd() const;

    // �ж��Ƿ��ӵ�����
    virtual bool IsHitted(const CBullet &blt) const;

    // ��ȡ��ײ��Ϣ, pDelay�����������ײ��������ײʱ��
    bool GetHitInfo(const CBullet &blt, float *pDelay = nullptr) const;

    // ��̹�˵ķ������΢��
    virtual void ChangeDirection(bool bForward);

private:
    // ��������̹��
    void DrawTank(Graphics &gh) const;

    // ����β��
    void DrawTankTail(Graphics &gh) const;

    // ����ը��̹��
    void DrawExplosion(Graphics &gh) const;

    // ��ǰ״̬: 1,����,2:��ը
    int m_status{1};

    // ̹��ͼƬ ����ʱ
    Image *m_imgTank;

    // ̹��ͼƬ ��ըʱ
    Image *m_imgExplosion[12];
    mutable int m_explosionIndex{ 0 };

    // �ӵ�
    std::list<CBullet> m_lstBullets;

    // ������ʱ��
    CGameTimeval m_timer_fire;

    // ��ըʱ��
    CGameTimer m_timerBomb;
};

