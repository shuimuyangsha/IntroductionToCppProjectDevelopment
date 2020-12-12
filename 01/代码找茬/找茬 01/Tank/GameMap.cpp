#include "stdafx.h"
#include "GameMap.h"
#include "Bullet.h"
#include "GamePathFinder.h"

CGameMapElement::CGameMapElement()
{
}

CGameMapElement::~CGameMapElement()
{

}

CGameMapElement::CGameMapElement(float x, float y
                                 , bool bLeft, bool bTop, bool bRight, bool bBottom
                                 , float wallWidth, float spaceLen)
    : m_bLeft(bLeft), m_bTop(bTop), m_bRight(bRight), m_bBottom(bBottom)
    , m_wallWidth(wallWidth)
    , m_spaceLen(spaceLen)
{
    RectF rectf(x, y, spaceLen + 2 * wallWidth, spaceLen + 2 * wallWidth);
    SetRect(rectf);

    // 四面的墙的初始化
    {
        {
            CWall &w = m_wallArray[EWLeft];
            w.SetRect(RectF(x, y, wallWidth, spaceLen + 2 * wallWidth));
            w.SetDir(CWall::EDirectionY);
        }
        {
            CWall &w = m_wallArray[EWRight];
            w.SetRect(RectF(x + wallWidth + spaceLen, y, wallWidth, spaceLen + 2 * wallWidth));
            w.SetDir(CWall::EDirectionY);
        }
        {
            CWall &w = m_wallArray[EWTop];
            w.SetRect(RectF(x, y, spaceLen + 2 * wallWidth, wallWidth));
            w.SetDir(CWall::EDirectionX);
        }
        {
            CWall &w = m_wallArray[EWBottom];
            w.SetRect(RectF(x, y + wallWidth + spaceLen, spaceLen + 2 * wallWidth, wallWidth));
            w.SetDir(CWall::EDirectionX);
        }
    }
}

void CGameMapElement::Draw(Graphics &gh) const
{
    // 画空位置
    {
        SolidBrush spaceBrush(m_sapceClr);
        RectF rect(GetRect().X + m_wallWidth, GetRect().Y + m_wallWidth, m_spaceLen + m_wallWidth, m_spaceLen + m_wallWidth);
        gh.FillRectangle(&spaceBrush, rect);
    }

    // 画四周的墙
    if(m_bLeft) {
        m_wallArray[EWLeft].Draw(gh);
    }

    if(m_bTop) {
        m_wallArray[EWTop].Draw(gh);
    }

    if(m_bRight) {
        m_wallArray[EWRight].Draw(gh);
    }

    if(m_bBottom) {
        m_wallArray[EWBottom].Draw(gh);
    }
}


/// ////////////////////////////////////////////////////////////////////////////////

CGameMap::CGameMap()
{
    LoadMap();
}

CGameMap::CGameMap(int x, int y, int width, int height) : m_rectMap(x, y, width, height)
{
    m_finder = nullptr;
    LoadMap();
}

CGameMap::~CGameMap()
{
    SAFE_DEL_PTR(m_finder);
}

// 载入地图
void CGameMap::LoadMap()
{
    // 一个占用的空间宽高
    int Space1Element = m_wallWidth + m_spaceLen;
    // 计算多少行， 多少列
    int m_row = 0, m_col = 0;
    {
        m_row = m_rectMap.Height / Space1Element;
        m_col = m_rectMap.Width / Space1Element;
    }

    if(m_row <= 0 || m_col <= 0) {
        AfxMessageBox(_T("地图数据不正"));
        return;
    }
    //生成地图
    srand(GetTickCount());
    m_arr.clear();
    int step = Space1Element;
    for(int r = 0; r < m_row; ++r) {
        // 一行地图空间
        vector<CGameMapElement> vRow;
        // 列数据
        for(int c = 0; c < m_col; ++c) {
            vRow.push_back(CGameMapElement(
                               static_cast<float>(m_rectMap.X + c * step)
                               , static_cast<float>(m_rectMap.Y + r * step)
                               , c == 0 ? true : (1 == rand() % 5)
                               , r == 0 ? true : (1 == rand() % 5)
                               , c == m_col - 1 ? true : (1 == rand() % 5)
                               , r == m_row - 1 ? true : (1 == rand() % 5)
                               , static_cast<float>(m_wallWidth), static_cast<float>(m_spaceLen)
                           ));
        }
        // 追加一行
        m_arr.push_back(vRow);
    }

    // 由于四周的墙是随机生成的，所以会有重复的;
    // 对地图进行处理。去掉重复的墙
    for(size_t m_row = 1; m_row < m_arr.size(); ++m_row) {
        for(size_t m_col = 1; m_col < m_arr[m_row].size(); ++m_col) {
            auto &a = m_arr[m_row][m_col];
            // 上面的
            auto const &top = m_arr[m_row - 1][m_col];
            // 左边的
            auto const &left = m_arr[m_row][m_col - 1];
            if(top.m_bBottom && a.m_bTop) {
                a.m_bTop = false;
            }
            if(left.m_bRight && a.m_bLeft) {
                a.m_bLeft = false;
            }
        }
    }

    // 对地图进行处理，去掉 完全封闭的区域


    // 初始化查找工具
    SAFE_DEL_PTR(m_finder);
    m_finder = new CPathFinderBFS(m_arr);
}

// 画地图
void CGameMap::Draw(Gdiplus::Graphics &gh) const
{
    int m_col = m_arr[0].size();
    int m_row = m_arr.size();
    for(int y = 0; y != m_col; ++y) {
        for(int x = 0; x != m_row; ++x) {
            m_arr[x][y].Draw(gh);
        }
    }
}

// 子弹撞墙处理
bool CGameMap::HitWallProcess(const CBullet &blt) const
{
    float delay, dir;
    bool b = GetHitWallInfo(blt, &delay, &dir);
    // 改变方向
    if(b) {
        const_cast<CBullet &>(blt).SetDirectionArc(dir);
    }

    return b;
}

// 判断是否撞上墙，如查撞上了，返回 返射角，撞墙的时间
bool CGameMap::GetHitWallInfo(const CBullet &blt, float *pTime /*= nullptr*/, float *pDir /*= nullptr*/) const
{
    // 查找产生碰撞的墙
    struct info {
        CWall const &w; // 相交的墙
        PointF ptHit; // 相交点
        float dir; // 返射速度
        float delay; // 相交时间
    };

    vector<struct info> v;
    for(auto &line : m_arr) {  // 行
        for(auto &ele : line) {  // 列
            // 有墙，且产生碰撞
            PointF ptHit;
            float dir;
            float delay;
            if(ele.m_bLeft && ele[CGameMapElement::EWLeft].CheckHit(blt, &ptHit, &dir, &delay)) {
                struct info s {
                    ele[CGameMapElement::EWLeft], ptHit, dir, delay
                };
                v.push_back(s);
            }
            if(ele.m_bTop && ele[CGameMapElement::EWTop].CheckHit(blt, &ptHit, &dir, &delay)) {
                struct info s {
                    ele[CGameMapElement::EWTop], ptHit, dir, delay
                };
                v.push_back(s);
            }
            if(ele.m_bRight && ele[CGameMapElement::EWRight].CheckHit(blt, &ptHit, &dir, &delay)) {
                struct info s {
                    ele[CGameMapElement::EWRight], ptHit, dir, delay
                };
                v.push_back(s);
            }
            if(ele.m_bBottom && ele[CGameMapElement::EWBottom].CheckHit(blt, &ptHit, &dir, &delay)) {
                struct info s {
                    ele[CGameMapElement::EWBottom], ptHit, dir, delay
                };
                v.push_back(s);
            }
        }
    }
    if(v.empty()) {
        return false;
    }
    // 查找最先撞击的墙面
    auto it = std::min_element(v.begin(), v.end(), [](auto & lhs, auto & rhs)->bool {
        return lhs.delay < rhs.delay;
    });

    // 返回撞墙信息
    if(pTime) {
        *pTime = it->delay;
    }

    if(*pDir) {
        *pDir = it->dir;
    }
    return true;
}

// 判断
bool CGameMap::IsHitTheWall(const CTankEntry &tank, bool bForward) const
{
    // 判断所有的墙，查看是否产生碰撞
    for(auto &line : m_arr) {  // 行
        for(auto &ele : line) {  // 列
            // 有墙，且产生碰撞
            if(ele.m_bLeft && ele[CGameMapElement::EWLeft].IsWillHit(tank, bForward)) {
                return true;
            }
            if(ele.m_bTop && ele[CGameMapElement::EWTop].IsWillHit(tank, bForward)) {
                return true;
            }
            if(ele.m_bRight && ele[CGameMapElement::EWRight].IsWillHit(tank, bForward)) {
                return true;
            }
            if(ele.m_bBottom && ele[CGameMapElement::EWBottom].IsWillHit(tank, bForward)) {
                return true;
            }
        }
    }
    return false;
}


// 获得一个可以放置物品的位置，位置是随机的
bool CGameMap::FindRandomPosition(PointF &ptCenter) const
{
    if(m_arr.empty() || m_arr[0].empty()) {
        return false;
    }
    try {
        // 随机产生 行，列
        size_t m_row, m_col;
        m_row = static_cast<size_t>(rand()) % m_arr.size();
        m_col = static_cast<size_t>(rand()) % m_arr[0].size();
        // 获得选中的 地图块的 中心点
        ptCenter = m_arr[m_row][m_col].GetCenterPoint();
    }
    catch(...) {
        return false;
    }
    return true;
}

// 判断当前物所在的坐标, x : 返回在第几行,y : 返回在第几列
bool CGameMap::FindObjPosition(const CGameEntryMoveable &obj, int &row, int &col) const
{
    PointF ptCenter = obj.GetCenterPoint();
    size_t m_row = m_arr.size();
    size_t m_col = m_arr[0].size();
    for(size_t _x = 0; _x != m_row; ++_x) {
        for(size_t _y = 0; _y != m_col; ++_y) {
            RectF rc = m_arr[_x][_y].GetRect();
            if(rc.Contains(ptCenter)) {
                row = _x, col = _y;
                return true;
            }
        }
    }
    // 没有找到？
    return false;
}

//  path 返回路径 [起点，。。。， 终点]
void CGameMap::FindPath(int startX, int startY, int targetX, int targetY, VPath &path) const
{
    path.clear();
    m_finder->FindPath(startX, startY, targetX, targetY, path);
}

// 获得下一次的前进方向
bool CGameMap::FindNextDirection(float *dir, int startX, int startY, int targetX, int targetY) const
{
    VPath path;
    FindPath(startX, startY, targetX, targetY, path);
    if(path.empty()) {
        return false;
    }

    // 获得第一个位置
    auto it = path.begin();
    it++;

    if(it == path.end()) {
        return false;
    }

    it->x;
    it->y;
    // 判断在哪个方向？
    if(startX == it->x) {
        // 同一行
        if(startY < it->y) {
            // 左侧
            *dir = 90;
        }
        else if(startY > it->y) {
            // 右侧
            *dir = 270;
        }
        else {

        }
    }
    else if(startY == it->y) {
        // 同一列
        if(startX < it->x) {
            // 上方
            *dir = 180;
        }
        if(startX > it->x) {
            // 下方
            *dir = 0;
        }
    }
    else {

    }
    return true;
}

// 获得某块的中心点位置
PointF CGameMap::GetElementAreaCenter(int row, int col) const
{
    try {
        return m_arr[row][col].GetCenterPoint();
    }
    catch(...) {
        return PointF(-1, -1);
    }
}

// 判断现在发射子弹是否可以打到玩家
bool CGameMap::IsCanKillTarget(const CGameEntryMoveable &bot, const CGameEntryMoveable &target, float *dir/* = nullptr*/) const
{
    // 机器人，玩家 所在的位置
    int startRow, startCol, targetRow, targetCol;
    if(!FindObjPosition(bot, startRow, startCol) || !FindObjPosition(target, targetRow, targetCol)) {
        return false;
    }
    // 横坚都不在一个方向上
    if(startRow != targetRow && startCol != targetCol) {
        return false;
    }
    // 查看四方向，是否可以打到目标
    if(startCol == targetCol && startRow == targetRow) {
        if(dir) {
            *dir = 0;
        }
        return true;
    }
    // 坚向查看
    if(startCol == targetCol) {
        // 向下查看
        if(startRow < targetRow) {
            int col = startCol;
            for(int row = startRow; row <= targetRow; ++row) {
                auto &ele = m_arr[row][col];
                if(row == startRow) {
                    if(ele.m_bBottom) {
                        return false;
                    }
                }
                else if(row == targetRow) {
                    if(ele.m_bTop) {
                        return false;
                    }
                }
                else {
                    if(ele.m_bBottom || ele.m_bTop) {
                        return false;
                    }
                }
            }
            if(dir) {
                *dir = PI;
            }
            return true;
        }
        // 向上查看
        else {
            int col = startCol;
            for(int row = startRow; row >= targetRow; --row) {
                auto &ele = m_arr[row][col];
                if(row == targetRow) {
                    if(ele.m_bBottom) {
                        return false;
                    }
                }
                else if(row == startRow) {
                    if(ele.m_bTop) {
                        return false;
                    }
                }
                else {
                    if(ele.m_bBottom || ele.m_bTop) {
                        return false;
                    }
                }
            }
            if(dir) {
                *dir = 0;
            }
            return true;
        }
    }
    // 横向查看
    if(startRow == targetRow) {
        // 向右查看
        if(startCol < targetCol) {
            int row = startRow;
            for(int col = startCol; col <= targetCol; ++col) {
                auto &ele = m_arr[row][col];
                if(col == startCol) {
                    if(ele.m_bRight) {
                        return false;
                    }
                }
                else if(col == targetCol) {
                    if(ele.m_bLeft) {
                        return false;
                    }
                }
                else {
                    if(ele.m_bLeft || ele.m_bRight) {
                        return false;
                    }
                }
            }
            if(dir) {
                *dir = PI / 2.0f;
            }
            return true;
        }
        // 向左查看
        else {
            int row = startRow;
            for(int col = startCol; col >= targetCol; --col) {
                auto &ele = m_arr[row][col];
                if(col == targetCol) {
                    if(ele.m_bRight) {
                        return false;
                    }
                }
                else if(col == startCol) {
                    if(ele.m_bLeft) {
                        return false;
                    }
                }
                else {
                    if(ele.m_bLeft || ele.m_bRight) {
                        return false;
                    }
                }
            }

            if(dir) {
                *dir = PI * 1.5f;
            }
            return true;
        }
    }

    return false;
}

//.