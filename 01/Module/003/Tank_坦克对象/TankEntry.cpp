#include "stdafx.h"
#include "TankEntry.h"
#include "GameHit.h"


CTankEntry::CTankEntry()
{
}

CTankEntry::CTankEntry(float x, float y, PCTSTR szImgName)
{
    m_imgTank = Image::FromFile(szImgName);
    for(int i = 0; i < _countof(m_imgExplosion); ++i) {
        TCHAR szPath[MAX_PATH];
        _stprintf_s(szPath, _T("%d.gif"), i);
        m_imgExplosion[i] = Image::FromFile(szPath);
    }
    int width = m_imgTank->GetWidth();
    int height = m_imgTank->GetHeight();

    // ���� ̹�� ǰռ�õ�����
    RectF r(static_cast<float>(x)
            , static_cast<float>(y)
            , static_cast<float>(width)
            , static_cast<float>(height));
    SetRect(r);

    // �ٶ�
    SetSpeed(10);
    // ��ǰ�ĽǶ�
    SetDirection(0);
    // ÿ�ΰ���ת 30��
    SetDirectionTurn(30);
    // Ĭ��Ϊ��Ч
    SetActive(true);
    // �ӵ���Ϣ
    for(int i = 0; i < 5; ++i) {
        m_lstBullets.push_back(CBullet(0, 0, 12, 0));
    }
}

CTankEntry::~CTankEntry()
{
}

// ���Լ�
void CTankEntry::Draw(Gdiplus::Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }

    //��ת��ͼƽ��
    PointF center = GetCenterPoint();

    // 1.ƽ�Ʊ任 : �ƶ�����㵽 ̹������
    gh.TranslateTransform(center.X, center.Y);
    // 2.��ת�任 : ʹ����ϵ����̹�˵ĽǶ�һ��
    gh.RotateTransform(GetDirection());
    // 3.�ָ�ԭ��
    gh.TranslateTransform(-center.X, -center.Y);

    switch(m_status) {
        case 1: {
            //����̹��
            DrawTank(gh);
            DrawTankTail(gh);
            break;
        }
        case 2: {
            // ���Ʊ��Ƶ�̹��
            DrawExplosion(gh);
            break;
        }
        default:
            break;
    };


#if 1
    // ��������任 : �����Ƿ��أ�
    // 1.�ƶ������ĵ�
    gh.TranslateTransform(center.X, center.Y);
    // 2.����ת
    gh.RotateTransform(-GetDirection());
    // 3.���ƶ���ȥ
    gh.TranslateTransform(-center.X, -center.Y);
    //ScaleTransform : ����
#else
    // �ڶ��ָֻ��ķ�ʽ : ȱ�㣺ֱ�����ã����ð�
    gh.ResetTransform();
#endif
}

// ���ÿ�ǹ���ʱ��
void CTankEntry::SetFireTimeval(unsigned long timeval)
{
    m_timer_fire.SetTimeval(timeval);
}

// ���ǹ��λ��
PointF CTankEntry::GetGunPosition() const
{
    return GetHeadPos();
}

// ��ȡ�ӵ� ��ǹ��ʱ�ĳ�ʼλ��
PointF CTankEntry::GetBulletFirstStepPos(float bulletRadius) const
{
    //ǹ�ڵ�λ��
    PointF p = GetGunPosition();
    // ��ǰ���ȥһ���,�����һ�η���ʱ���ʹ��Լ�
    p.X += static_cast<Gdiplus::REAL>((bulletRadius) * sin(GetDirectionArc()));
    p.Y -= static_cast<Gdiplus::REAL>((bulletRadius) * cos(GetDirectionArc()));
    return p;
}

// ʹ̹�˱�ը
void CTankEntry::Bomb()
{
    m_timerBomb.SetTimeout(3000);
    m_timerBomb.StartTimer();
    m_status = 2;
}

// �Ƿ���Ч
bool CTankEntry::IsActive() const
{
    return (CGameEntryMoveable::IsActive() && m_explosionIndex <= _countof(m_imgExplosion));
}

// �Ƿ�ը����(��������б�ը��ͬ������false)
bool CTankEntry::IsBombEnd() const
{
    return m_status == 2 && m_timerBomb.IsTimeout();
}

// ��̹�˵ķ������΢��
void CTankEntry::ChangeDirection(bool bForward)
{
    return; // ��Ҫ��
    float dir = GetDirection();
    int _d = int(dir);
    if(bForward) {
        if(0 < _d && _d < 180) {
            dir += 1;
        }
        else {
            dir -= 1;
        }
    }
    else {
        if(0 < _d && _d < 180) {
            dir -= 1;
        }
        else {
            dir += 1;
        }
    }
    SetDirection(dir);
}

// ��������̹��
void CTankEntry::DrawTank(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    gh.DrawImage(m_imgTank, GetRect());
}

// ����β��
void CTankEntry::DrawTankTail(Graphics &gh) const
{
}

// ����ը��̹��
void CTankEntry::DrawExplosion(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    //���Ʊ�ըͼƬ
    auto p = m_imgExplosion[m_explosionIndex];
    if(p) {
        RectF rect;
        // �Լ������ĵ�
        PointF pCenter = GetCenterPoint();
        rect.Width = static_cast<float>(p->GetWidth());
        rect.Height = static_cast<float>(p->GetHeight());
        rect.X = 0;
        rect.Y = 0;
        rect.Offset(pCenter.X - rect.Width / 2, pCenter.Y - rect.Height / 2);
        gh.DrawImage(p, rect);
    }
    // ��һ����ͼƬ
    static unsigned long last = GetTickCount();
    if(GetTickCount() - last > 50) {
        m_explosionIndex++;
        if(m_explosionIndex >= _countof(m_imgExplosion)) {
            m_explosionIndex = _countof(m_imgExplosion) - 1;
        }
    }
}

// ����
bool CTankEntry::Fire(CBullet &_blt)
{
    if(!IsActive()) {
        return false;
    }
    // ��⿪ǹ���ʱ��
    if(!m_timer_fire.IsTimeval()) {
        return false;
    }

    // ��ȡͷ��λ��
    PointF front = GetGunPosition();// GetBulletFirstStepPos(_blt.GetDiameter() / 2.0f);
    //��� �ӵ���Ϣ
    //��������ӵ�������ͷ������,���Ҵ��б����Ƴ����ӵ���һ���б�
    if(!m_lstBullets.empty()) {
        CBullet &blt = m_lstBullets.back();
        // ���ó�ʼ ����
        blt.SetPositionForCenter(front);
        // ���� �˶�����
        blt.SetDirection(GetDirection());
        // ���ó�ʼ�ٶ�
        blt.SetSpeed(5);
        // ����Ϊ��Ч
        blt.SetActive(true);
        // ���÷���ʱ��
        blt.SetFireTime(GetTickCount());
        // ����������
        blt.SetOwner(this);
        // ����Ϊ�״η���
        blt.SetFirst();
        // ���뵽��ͼ�б��б���
        _blt = blt;
        // ����ɾ��
        m_lstBullets.pop_back();
        // ��¼��ǹʱ��
        m_timer_fire.SetLastTime();
        return true;
    }
    return false;
}

// �����ӵ�
void CTankEntry::AddBullet(CBullet &blt)
{
    if(!IsActive()) {
        return;
    }
    m_lstBullets.push_back(blt);
}

// �ж��Ƿ��ӵ�����
bool CTankEntry::IsHitted(const CBullet &blt) const
{
    if(blt.IsFirstFire() && blt.GetOwner() == this) {
        return false;
    }
    return GetHitInfo(blt);
}

// ��ȡ��ײ��Ϣ
bool CTankEntry::GetHitInfo(const CBullet &blt, float *pDelay /*= nullptr*/) const
{
    // �ӵ� ���η��䣬���ܰ��Լ���
    if(blt.IsFirstFire() && blt.GetOwner() == this) {
        return false;
    }
    // ����Ƿ�����ײ
    //�����ӵ���ǰ��λ��
    game_hit::Circle A;
    A.r = blt.GetDiameter() / 2.0f;
    A.x = blt.GetCenterPoint().X;
    A.y = blt.GetCenterPoint().Y;

    // ��С����ĳ�����ƶ�
    auto move_step = [](game_hit::Circle & A, float dir, float distance) {
        A.x += distance * sin(dir);
        A.y -= distance * cos(dir);
    };

    // �Լ�
    RectF B = GetRect();

    // Ϊ�˷�ֹ�ӵ��������µ���ײ���ʧЧ
    // ʹ�ӵ��������н������ϣ�һ�����أ�һ�����ص�ǰ������������ײ���
    for(int step = 0; step < blt.GetSpeed(); ++step) {
        move_step(A, blt.GetDirectionArc(), step);
        if(game_hit::check_collision(A, B, GetDirectionArc())) {
            if(pDelay) {
                *pDelay = step;
            }
            return true;
        }
    }
    return false;
}
