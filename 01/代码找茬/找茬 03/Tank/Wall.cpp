#include "stdafx.h"
#include "Wall.h"
#include "TankEntry.h"
#include "GameHit.h"


CWall::CWall()
{
}


CWall::CWall(float x, float y, float width, float length, CWall::EWallDirection dir) : m_direction(dir)
{
}


CWall::~CWall()
{
}

void CWall::Draw(Graphics &gh) const
{
    SolidBrush brush(m_clr);
    gh.FillRectangle(&brush, GetRect());
}

// ���÷���
void CWall::SetDir(CWall::EWallDirection dir)
{
    m_direction = dir;
}

// ��÷���
CWall::EWallDirection CWall::GetDir() const
{
    return m_direction;
}

// ��� С��ײ��ǽ,��ı䷽���λ��(����Ϊ�պò�ײǽ��λ��)
bool CWall::HitAction(CBullet &blt) const
{
    bool b = false;
    using namespace game_hit;

    auto ptCenter = blt.GetCenterPoint();
    auto speed = blt.GetSpeed();
    // Բ�� + �ٶ�
    CMovePoint ray(ptCenter.X, ptCenter.Y,
                   speed * sin(blt.GetDirectionArc()), // x�������
                   -speed * cos(blt.GetDirectionArc()) // y�������
                  );


    RectF rc = GetRect();
    CLine line;
    // ��ǽ ����� ����
    if(m_direction == EDirectionX) {
        line.startPoint.x = rc.X;
        line.startPoint.y = rc.Y + rc.Height / 2.0f;
        line.endPoint.x = rc.X + rc.Width;
        line.endPoint.y = rc.Y + rc.Height / 2.0f;
    }
    // ��ǽ �����  ����
    else {
        line.startPoint.x = rc.X + rc.Width / 2.0f;
        line.startPoint.y = rc.Y;
        line.endPoint.x = rc.X + rc.Width / 2.0f;
        line.endPoint.y = rc.Y + rc.Height;
    }
    CVector2D newSpeed, crossPoint;
    b = game_hit::IsHit(ray, line, newSpeed, crossPoint);
    if(b) {
        float dir = atan2f(newSpeed.x, -newSpeed.y); // atan2f()
        blt.SetDirection(dir * (180 / PI));
    }
    return b;
}

// �ж��Ƿ���ײ�����������ײ��������ײ�㣬�������
bool CWall::CheckHit(const CBullet &blt, PointF *ptHit/*=nullptr*/, float *dir/*=nullptr*/, float *pDelay/*=nullptr*/) const
{
    return CheckHitEx(blt, ptHit, dir, pDelay);
}

// �ж�С��ײ��ǽ����һ��,�����ײ������������ײ����һ�ߵ���ײ��Ϣ
bool CWall::CheckHitEx(const CBullet &blt, PointF *ptHit/*=nullptr*/, float *dir/*=nullptr*/, float *pDelay/*=nullptr*/) const
{
    using namespace game_hit;

    auto ptCenter = blt.GetCenterPoint();
    auto speed = blt.GetSpeed();
    // Բ�� + �ٶ�
    CMovePoint ray(ptCenter.X, ptCenter.Y,
                   speed * sin(blt.GetDirectionArc()), // x�������
                   -speed * cos(blt.GetDirectionArc()) // y�������
                  );


    RectF rc = GetRect();
    vector<tuple<CVector2D, float, float>> v;
    {
        //  ����
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X;
            line.endPoint.y = rc.Y + rc.Height;

            // ���ٶ�
            CVector2D newSpeed;
            // ײ����
            CVector2D crossPoint;
            // �ཻ����
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // ����Ƕ�
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // ����
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y;

            // ���ٶ�
            CVector2D newSpeed;
            // ײ����
            CVector2D crossPoint;
            // �ཻ����
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // ����Ƕ�
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // ����
        {
            CLine line;
            line.startPoint.x = rc.X + rc.Width;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y + rc.Height;

            // ���ٶ�
            CVector2D newSpeed;
            // ײ����
            CVector2D crossPoint;
            // �ཻ����
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // ����Ƕ�
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // ����
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y + rc.Height;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y + rc.Height;

            // ���ٶ�
            CVector2D newSpeed;
            // ײ����
            CVector2D crossPoint;
            // �ཻ����
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // ����Ƕ�
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }
    }
    // ��������
    if(!v.empty()) {
        auto it = std::min_element(v.begin(), v.end(), [](auto & lhs, auto & rhs)->bool {
            return std::get<1>(lhs) < std::get<1>(rhs);
        });
        if(ptHit) {
            ptHit->X = get<0>(*it).x;
            ptHit->Y = get<0>(*it).y;
        }
        if(pDelay) {
            * pDelay = get<1>(*it);
        }
        if(dir) {
            *dir = get<2>(*it);
        }
        return true;
    }

    return false;
}


// ��һ���Ƿ��ײ��ǽ
bool CWall::IsWillHit(const CTankEntry &tank, bool bForward) const
{
    // ���һ�²���λ�ã����Ҫײ���ˣ��Ͳ��ƶ�
    if(bForward) {
        RectF r = tank.ForwardNextRect();
        if(r.Intersect(GetRect())) {
            return true;
        }
    }
    else {
        RectF r = tank.BackwardNextRect();
        if(r.Intersect(GetRect())) {
            return true;
        }
    }
    return false;
}
