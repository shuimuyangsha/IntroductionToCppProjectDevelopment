#pragma once

#include "vector"
using namespace std;

// 向量及相关运算
namespace game_hit
{

#if !defined(PI)
#  define PI (3.14159265357)
#endif


    // 个向量表示的各种形状，及向量类
    // 二维向量，及相关计算
    // 二维向量
    class CVector2D
    {
    public:
        float x, y;
        CVector2D() {}
        CVector2D(float _x, float _y) : x(_x), y(_y) {}
        CVector2D(const CVector2D &rhs) : x(rhs.x), y(rhs.y) {}
        CVector2D &operator = (const CVector2D &a)
        {
            x = a.x;
            y = a.y;
            return *this;
        }
        bool operator == (const CVector2D &a)
        {
            return x == a.x  && y == a.y;
        }
        bool operator != (const CVector2D &a)
        {
            return x != a.x || y != a.y;
        }

        /// 向量运算

        //置为0向量
        void Zero()
        {
            x = y = 0.0f;
        }

        // 重载 '-' 求负向量
        CVector2D operator - () const
        {
            return CVector2D(-x, -y);
        }

        CVector2D operator + (const CVector2D &a) const
        {
            return CVector2D(x + a.x, y + a.y);
        }

        // 减法
        CVector2D operator - (const CVector2D &a) const
        {
            return CVector2D(x - a.x, y - a.y);
        }

        // 与标量的 乘除法
        CVector2D operator *(float a) const
        {
            return CVector2D(x * a, y * a);
        }

        // 与标量的 乘除法
        CVector2D operator /(float a) const
        {
            return CVector2D(x / a, y / a);
        }

        // +=
        CVector2D &operator+=(const CVector2D &a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }
        // -=
        CVector2D &operator-=(const CVector2D &a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }
        // *=
        CVector2D &operator*=(const CVector2D &a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }
        // /=
        CVector2D &operator/=(const CVector2D &a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }
        // 求向量长度
        float magintude() const
        {
            return sqrt(x * x + y * y);
        }
        // 求两个向量之间的夹角
        float angleBetween(const CVector2D &vector2)
        {
            CVector2D vector3 = *this - vector2;
            auto m1 = this->magintude();
            auto m2 = vector2.magintude();
            auto m3 = vector3.magintude();
            assert(m1 != 0 && m2 != 0);
            if(m3 == 0) {
                return 0;
            }
            else {
                return acos((m2 * m2 + m1 * m1 - m3 * m3) / (2 * m1 * m2));
            }
        }

        // 向量标准化
        void Normal()
        {
            // 求向量长度
            float magSq = x * x + y * y;
            if(magSq > 0.000001f) {
                float oneOverMag = 1.0f / sqrt(magSq);
                x *= oneOverMag;
                y *= oneOverMag;
            }
        }

        // 求法向量 : 重直于本向量的向量 点积 = 0 可得 fx * vx  + fy* vy = 0 , 最简单的解为:
        //    fx = -vy, fy = vx
        // or fx =  vy, fy = -vx
        CVector2D NormalizeLeft()
        {
            CVector2D f = *this;
            f.Normal();
            return CVector2D(f.y, -f.x);
        }

        CVector2D NormalizeRight()
        {
            CVector2D f = *this;
            f.Normal();
            return CVector2D(-f.y, f.x);
        }

        // 向量的点乘
        float operator * (const CVector2D &a) const
        {
            return x * a.x + y * a.y;
        }

    };

    // 求向量的点积
    inline float dotProduct(const CVector2D &a, const CVector2D &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // 标量的左乘
    inline CVector2D operator * (float k, const CVector2D &v)
    {
        return CVector2D(k * v.x, k * v.y);
    }

    //计算两点间的距离
    inline float distance(const CVector2D &a, const CVector2D &b)
    {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    // 给定初始位置，和时间T，计算新位置？
    // 给定向量 v,k,返回4个值 i'(k的标准化版本), j', i'和j'方向上的v 分量 a和b;
    // v = ai' + bj'
    inline void switchBasis(const CVector2D &vector, const CVector2D &directionVector, CVector2D &basis1, CVector2D &basis2, float &a, float &b)
    {
        // 输入的方向向量的标准化版本
        basis1 = directionVector;
        basis1.Normal();
        // 输入的方向向量的法线
        basis2 = basis1;
        basis2.NormalizeLeft();
        auto alpha = atan2(basis1.y, basis1.x);
        auto theta = atan2(vector.y, vector.x);
        auto mag = vector.magintude();
        a = mag * cos(theta - alpha);
        b = mag * sin(theta - alpha);
    }

    // 计算新坐标系中的独立分量的长度
    inline float component(CVector2D &vector, CVector2D &directionVector)
    {
        // 标准化
        auto alpha = atan2(directionVector.y, directionVector.x);
        auto theta = atan2(vector.y, vector.x);
        auto mag = vector.magintude();
        auto a = mag * cos(theta - alpha);
        return a;
    }

    // 计算新坐标系中的独立分量(结果是一个向量,也就是 vector 在 directionVector 上面的投影)
    inline CVector2D componentVector(CVector2D &vector, CVector2D &directionVector)
    {
        // 标准化
        auto v = directionVector;
        v.Normal();
        return component(vector, directionVector) * v;
    }
}

// 定义 需要用到的形状状
namespace game_hit
{
    // 线段
    class CLine
    {
    public:
        CLine() : startPoint(0, 0), endPoint(0, 0) {}
        CLine(float x0, float y0, float x1, float y1) : startPoint(x0, y0), endPoint(x1, y1)
        {
        }
        CVector2D startPoint, endPoint;
    };

    // 运动的点,有当前位置，和速度
    class CMovePoint
    {
    public:
        CMovePoint()
            : pos(0, 0), speed(0, 0)
        {
        }
        CMovePoint(float x, float y, float vx, float vy)
            : pos(x, y), speed(vx, vy)
        {
        }
        CMovePoint(CVector2D _pos, CVector2D _speed)
            : pos(_pos), speed(_speed)
        {
        }

    public:
        // 当前速度(像素/帧)
        CVector2D speed;
        // 当前位置
        CVector2D pos;
    };

    // 代表 圆的结构体
    struct Circle {
        float x, y;
        float r;
    };

}

// 碰撞检测：主要是向量计算
namespace game_hit
{
    // 输入四个向量参数(代表直线 ab, cd),判断是否相交，如果相交，返回交点位置
    // 相交返回 true, 否则返回false;
    inline bool intersectionPoint(const CVector2D &a, const CVector2D &b, const CVector2D &c, const CVector2D &d, CVector2D *pCorssPoint)
    {
        /*
            AB = {a,b}, CD = {c,d}
            // 如果相交 则推出方程:
            OP = OA + t*AB = a + t*(b-a);
            OP = OC + s*CD = c + s*(d-c);

            =>

            a + t * (b - a) = c + s * (d - c);

            =>

            a.x + t*(b.x - a.x) = c.x + s*(d.x - c.x);
            a.y + t*(b.y - a.y) = c.y + s*(d.y - c.y);

            a.x + t* tc1 = c.x + s* sc1;
            a.y + t* tc2 = c.y + s* sc2;
        */
        float tc1 = b.x - a.x;
        float tc2 = b.y - a.y;

        float sc1 = c.x - d.x;
        float sc2 = c.y - d.y;

        float con1 = c.x - a.x;
        float con2 = c.y - a.y;

        float det = tc2 * sc1 - tc1 * sc2;
        if(det == 0) {
            return false;
        }
        float con = tc2 * con1 - tc1 * con2;
        float s = con / det;
        if(pCorssPoint) {
            *pCorssPoint = c + s * (d - c);
        }
        return true;
    }

    // 计算两个直线的相交时间
    // 返回 t 为时间 如果 t = [0,1]
    // t>1 : t 位于 B点一侧
    // t<0 : t 位于 A点一侧
    inline bool intersectionTime(const CVector2D &p1, const CVector2D &v1, const CVector2D &p2, const CVector2D &v2, float *pt = nullptr)
    {
        auto tc1 = v1.x;
        auto tc2 = v1.y;

        auto sc1 = v2.x;
        auto sc2 = v2.y;

        auto con1 = p2.x - p1.x;
        auto con2 = p2.y - p1.y;

        auto det = tc2 * sc1 - tc1 * sc2;

        if(det == 0) {
            return false;
        }
        auto con = sc1 * con2 - sc2 * con1;
        auto t = con / det;
        if(pt) {
            *pt = t;
        }
        return true;
    }

    // 计算两个线段的交点
    inline bool intersection(const CVector2D &a, const CVector2D &b, const CVector2D &c, const CVector2D &d, float *pt = nullptr)
    {
        auto tc1 = b.x - a.x;
        auto tc2 = b.y - a.y;

        auto sc1 = c.x - d.x;
        auto sc2 = c.y - d.y;

        auto con1 = c.x - a.x;
        auto con2 = c.y - a.y;
        auto det = tc2 * sc1 - tc1 * sc2;
        if(det == 0) {
            return false;
        }
        auto con = tc2 * con1 - tc1 * con2;
        auto s = con / det;
        if(s < 0 || s > 1) {
            return false;
        }
        float t = 0;
        if(tc1 != 0) {
            t = (con1 - s * sc1) / tc1;
        }
        else {
            t = (con2 - s * sc2) / tc2;
        }

        if(t < 0 || t > 1) {
            return false;
        }
        if(pt) {
            *pt = t;
        }
        return true;
    }

    // 全局0向量
    extern const CVector2D kZeroVector2D;

    // 计算三角形外积
    inline float signed2DTriArea(CVector2D a, CVector2D b, CVector2D c)
    {
        return ((a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x));
    }

    // 判断两条线段是否相交,如果是，返回交点位置，和相交比例(pIntersectionTime)
    inline bool IntersectLineSegments(const CLine a, const CLine b,
                                      __out CVector2D *pIntersectionPoint = nullptr,
                                      __out float *pIntersectionTime = nullptr)
    {
        auto a1 = signed2DTriArea(a.startPoint, a.endPoint, b.endPoint);
        auto a2 = signed2DTriArea(a.startPoint, a.endPoint, b.startPoint);
        if(a1 * a2 < 0) {
            auto a3 = signed2DTriArea(b.startPoint, b.endPoint, a.startPoint);
            auto a4 = a3 + a2 - a1;
            if(a3 * a4 < 0) {
                auto intersectionTime = a3 / (a3 - a4);
                auto intersectionPoint = CVector2D(a.endPoint.x, a.endPoint.y);
                intersectionPoint -= a.startPoint;
                intersectionPoint = intersectionPoint * intersectionTime;
                intersectionPoint += a.startPoint;
                if(pIntersectionPoint) {
                    *pIntersectionPoint = intersectionPoint;
                }
                if(pIntersectionTime) {
                    *pIntersectionTime = intersectionTime;
                }
                return true;
            }
        }
        return false;
    }

    // 判断移到的点(可以视为射线？)，是否与直线产碰撞，如果碰撞，返回碰撞的点，和反射之后的速度
    inline bool IsHit(const CMovePoint &Ray, const CLine &line,
                      __out CVector2D &newSpeed,
                      __out CVector2D &crossPoint,
        __out float * pHitDelay = nullptr)
    {
        using namespace game_hit;
        // 下一帧之位置
        auto next = Ray.pos + Ray.speed;
        // 两个位置构成一条线段
        CLine a(Ray.pos.x, Ray.pos.y, next.x, next.y);
        // 相交点
        CVector2D hitPoint;
        // 相交时间
        float hitDelay;

        // 判断是否相交
        bool b = IntersectLineSegments(a, line, &hitPoint, &hitDelay);

        if(b) {
            if (pHitDelay) * pHitDelay = hitDelay;
            //设置新的坐标为碰撞位置
            crossPoint = hitPoint;
            // 求 被撞直线的法线
            auto N = (line.endPoint - line.startPoint);
            N = N.NormalizeRight();
            // 新的速度 R = I - 2(I.N)N, I=原速度
            // 求点积
            auto dot = dotProduct(Ray.speed, N);
            // 最终结果
            auto R = Ray.speed - N * 2 * dot;
            newSpeed = R;
            return true;
        }
        else {
            return false;
        }
    }

}

// 碰撞检测 : 圆与旋转矩型,圆与圆是否相交
namespace game_hit
{
    // 计算两点之间的距离
    inline float distance(float x1, float y1, float x2, float y2)
    {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    // 判断两个圆是否相交
    inline bool check_collision(const Circle &A, const Circle &B)
    {
        return true;
    }

    // 圆型 是否与未旋转的矩型相交
    inline bool check_collision(const Circle &A, const RectF &B)
    {
        float cX, cY;
        if(A.x < B.X) {
            cX = B.X;
        }
        else if(A.x > (B.X + B.Width)) {
            cX = B.X + B.Width;
        }
        else {
            cX = A.x;
        }

        if(A.y < B.Y) {
            cY = B.Y;
        }
        else if(A.y > (B.Y + B.Height)) {
            cY = B.Y + B.Height;
        }
        else {
            cY = A.y;
        }


        if(distance(A.x, A.y, cX, cY) < A.r) {
            return true;
        }

        return false;
    }

    // 多个矩型同时判断
    inline bool check_collisions(const Circle &A, const std::vector<RectF> &vRects)
    {
        for(auto r : vRects) {
            if(check_collision(A, r)) {
                return true;
            }
        }

        return false;
    }

    // 获得点在旋转之后的 坐标
    inline PointF GetRotatePoint(const PointF &pt, const float fRadius, const PointF &ptCenter, const float dir)
    {
        PointF ptLeftTop = pt;
        // 原来的角度
        float theta = 2 * PI - atan2(ptLeftTop.Y - ptCenter.Y, ptLeftTop.X - ptCenter.X);
        // 旋转之后的角度
        float alpha = dir - theta;
        float offsetX = ptCenter.X + cos(alpha) * fRadius;
        float offsetY = ptCenter.Y + sin(alpha) * fRadius;
        ptLeftTop.X = offsetX;
        ptLeftTop.Y = offsetY;
        return ptLeftTop;
    }

    // 判断圆与旋转矩型相交
    // theta : 为距型的角度
    inline bool check_collision(const Circle &A, const RectF &B, const float alpha)
    {
        // 这里我们不旋转距型,变成旋转圆
        // 圆的角度 = 距型逆向旋转
        float theta = 2 * PI - alpha;
        // 距型中心：既旋转圆的中心
        Circle A2;
        A2.r = A.r;
        PointF ptCenter(B.X + B.Width / 2.0f, B.Y + B.Height / 2.0f);
        auto pt = GetRotatePoint(PointF(A.x, A.y), distance(A.x, A.y, ptCenter.X, ptCenter.Y), ptCenter, theta);
        A2.x = pt.X;
        A2.y = pt.Y;
        return check_collision(A2, B);
    }

    // 距型旋转一定角度之后， 四个顶点的坐标
    inline void GetRotateRectPoints(const RectF &rect, const float dir, std::vector<PointF> &vPts)
    {
        /*
          一以使用矩阵 进行计算
          这里使用的是三角函数进行计算的
        */
        using namespace std;
        vPts.clear();

        // 中心点坐标
        PointF ptCenter(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
        // 对角线长度的一半
        float fRadius = sqrt(pow(rect.Width / 2, 2) + pow(rect.Height / 2, 2));
        // 计算第一个点
        {
            // 原来的坐标
            PointF ptLeftTop(rect.X, rect.Y);
            vPts.push_back(GetRotatePoint(ptLeftTop, fRadius, ptCenter, dir));
        }
        {
            // 原来的坐标
            PointF ptLeftTop(rect.X + rect.Width, rect.Y);
            vPts.push_back(GetRotatePoint(ptLeftTop, fRadius, ptCenter, dir));
        }
        {
            // 原来的坐标
            PointF ptLeftTop(rect.X + rect.Width, rect.Y + rect.Height);
            vPts.push_back(GetRotatePoint(ptLeftTop, fRadius, ptCenter, dir));
        }
        {
            // 原来的坐标
            PointF ptLeftTop(rect.X, rect.Y + rect.Height);
            vPts.push_back(GetRotatePoint(ptLeftTop, fRadius, ptCenter, dir));
        }
        return ;
    }
}


