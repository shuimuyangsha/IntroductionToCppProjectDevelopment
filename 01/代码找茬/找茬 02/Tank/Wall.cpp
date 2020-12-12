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

// 设置方向
void CWall::SetDir(CWall::EWallDirection dir)
{
    m_direction = dir;
}

// 获得方向
CWall::EWallDirection CWall::GetDir() const
{
    return m_direction;
}

// 如果 小球撞上墙,则改变方向和位置(调整为刚好不撞墙的位置)
bool CWall::HitAction(CBullet &blt) const
{
    bool b = false;
    using namespace game_hit;

    auto ptCenter = blt.GetCenterPoint();
    auto speed = blt.GetSpeed();
    // 圆心 + 速度
    CMovePoint ray(ptCenter.X, ptCenter.Y,
                   speed * sin(blt.GetDirectionArc()), // x方向分量
                   -speed * cos(blt.GetDirectionArc()) // y方向分量
                  );


    RectF rc = GetRect();
    CLine line;
    // 横墙 抽像成 横线
    if(m_direction == EDirectionX) {
        line.startPoint.x = rc.X;
        line.startPoint.y = rc.Y + rc.Height / 2.0f;
        line.endPoint.x = rc.X + rc.Width;
        line.endPoint.y = rc.Y + rc.Height / 2.0f;
    }
    // 坚墙 抽像成  坚线
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

// 判断是否碰撞，如果发生碰撞，返回碰撞点，及反射角
bool CWall::CheckHit(const CBullet &blt, PointF *ptHit/*=nullptr*/, float *dir/*=nullptr*/, float *pDelay/*=nullptr*/) const
{
    return CheckHitEx(blt, ptHit, dir, pDelay);
}

// 判断小球撞上墙的哪一边,如果相撞，返回最先碰撞的那一边的碰撞信息
bool CWall::CheckHitEx(const CBullet &blt, PointF *ptHit/*=nullptr*/, float *dir/*=nullptr*/, float *pDelay/*=nullptr*/) const
{
    using namespace game_hit;

    auto ptCenter = blt.GetCenterPoint();
    auto speed = blt.GetSpeed();
    // 圆心 + 速度
    CMovePoint ray(ptCenter.X, ptCenter.Y,
                   speed * sin(blt.GetDirectionArc()), // x方向分量
                   -speed * cos(blt.GetDirectionArc()) // y方向分量
                  );


    RectF rc = GetRect();
    vector<tuple<CVector2D, float, float>> v;
    {
        //  左线
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X;
            line.endPoint.y = rc.Y + rc.Height;

            // 新速度
            CVector2D newSpeed;
            // 撞击点
            CVector2D crossPoint;
            // 相交比例
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // 反射角度
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // 上线
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y;

            // 新速度
            CVector2D newSpeed;
            // 撞击点
            CVector2D crossPoint;
            // 相交比例
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // 反射角度
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // 右线
        {
            CLine line;
            line.startPoint.x = rc.X + rc.Width;
            line.startPoint.y = rc.Y;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y + rc.Height;

            // 新速度
            CVector2D newSpeed;
            // 撞击点
            CVector2D crossPoint;
            // 相交比例
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // 反射角度
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }

        // 下线
        {
            CLine line;
            line.startPoint.x = rc.X;
            line.startPoint.y = rc.Y + rc.Height;
            line.endPoint.x = rc.X + rc.Width;
            line.endPoint.y = rc.Y + rc.Height;

            // 新速度
            CVector2D newSpeed;
            // 撞击点
            CVector2D crossPoint;
            // 相交比例
            float fDelay;
            if(game_hit::IsHit(ray, line, newSpeed, crossPoint, &fDelay)) {
                // 反射角度
                float theta = atan2f(newSpeed.x, -newSpeed.y);
                v.push_back(make_tuple(crossPoint, fDelay, theta));
            }

        }
    }
    // 进行排序
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


// 下一步是否会撞上墙
bool CWall::IsWillHit(const CTankEntry &tank, bool bForward) const
{
    // 获得一下步的位置，如果要撞上了，就不移动
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
