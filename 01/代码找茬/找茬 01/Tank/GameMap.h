#pragma once

#include "Wall.h"
#include "Bullet.h"

class CGamePathFinder;

/*
    代表中地图中的一个方块
    四周可能有墙不可通过， 中间是空地可以通过
*/
class CGameMapElement : public CGameEntryMoveable
{
public:
    CGameMapElement();
    CGameMapElement(float x, float y
                    , bool bLeft, bool bTop, bool bRight, bool bBottom
                    , float wallLen = 10, float spaceLen = 80);

    ~CGameMapElement();

    virtual void Draw(Graphics &gh) const;
    // 四个方向上面是否有墙
    bool m_bLeft{ false };
    bool m_bTop{ false };
    bool m_bRight{ false };
    bool m_bBottom{ false };

    // 代表四个方向
    typedef enum { EWLeft = 0, EWTop, EWRight, EWBottom, EWSize } EWallIndex;

    // 重载运算符 可以像数组一样取到本地块的墙
    const CWall &operator[](CGameMapElement::EWallIndex eIndex) const
    {
        return m_wallArray[eIndex];
    };

    // 设置空地的颜色
    void SetSpaceColor(Color clr)
    {
        m_sapceClr = clr;
    }

private:
    // 空地的颜色
    Color m_sapceClr{ Color(0xE6, 0xE6, 0xE6) };

    // 墙的宽度
    float m_wallWidth{ 10 };

    // 中间空地的宽度
    float m_spaceLen{ 80 };

    // 代表四面的墙 : 左上右下, 注意顺序
    CWall m_wallArray[4];
};

/* 代表整个地图
*/
class CGameMap : public CGameEntryMoveable
{
public:
    CGameMap();
    CGameMap(int x, int y, int width, int height);
    virtual ~CGameMap();

    // 载入地图
    void LoadMap();

    // 画地图
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // 子弹撞墙处理
    bool HitWallProcess(const CBullet &blt) const;

    // 判断是否撞上墙，如查撞上了，返回 返射角，撞墙的时间
    bool GetHitWallInfo(const CBullet &blt, float *pTime = nullptr, float *pDir = nullptr) const;

    // 坦克是否撞墙
    bool IsHitTheWall(const CTankEntry &tank, bool bForward) const;

    // 获得一个可以放置物品的位置，位置是随机的
    bool FindRandomPosition(PointF &ptCenter) const;

    // 判断当前物所在的坐标, x : 返回在第几行,y : 返回在第几列
    bool FindObjPosition(const CGameEntryMoveable &obj, int &row, int &col) const;

    /// ///////////////////////////////////////////////////////////////////////////////
    /// 路径 搜索
    //  path 返回路径 [起点，。。。， 终点]
    void FindPath(int startX, int startY, int targetX, int targetY, VPath &path) const;

    // 获得下一次的前进方向
    bool FindNextDirection(float *dir, int startX, int startY, int targetX, int targetY) const;

    // 获得某块的中心点位置
    PointF GetElementAreaCenter(int row, int col) const;

    // 判断现在发射子弹是否可以打到玩家
    bool IsCanKillTarget(const CGameEntryMoveable &bot, const CGameEntryMoveable &target, float *dir = nullptr) const;

private:
    // 存讲所有的地图
    std::vector<std::vector<CGameMapElement>> m_arr;

    // 整个地图的范围
    Gdiplus::Rect m_rectMap{ 0, 0, 800, 600 };

    // 地图中墙的宽度
    int m_wallWidth{ 7 };

    // 地图中中间空位的宽度
    int m_spaceLen{ 42 };
private:
    // 路径搜索
    CGamePathFinder *m_finder{ nullptr };
};

