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

    // �����ǽ�ĳ�ʼ��
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
    // ����λ��
    {
        SolidBrush spaceBrush(m_sapceClr);
        RectF rect(GetRect().X + m_wallWidth, GetRect().Y + m_wallWidth, m_spaceLen + m_wallWidth, m_spaceLen + m_wallWidth);
        gh.FillRectangle(&spaceBrush, rect);
    }

    // �����ܵ�ǽ
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

// �����ͼ
void CGameMap::LoadMap()
{
    // һ��ռ�õĿռ���
    int Space1Element = m_wallWidth + m_spaceLen;
    // ��������У� ������
    int m_row = 0, m_col = 0;
    {
        m_row = m_rectMap.Height / Space1Element;
        m_col = m_rectMap.Width / Space1Element;
    }

    if(m_row <= 0 || m_col <= 0) {
        AfxMessageBox(_T("��ͼ���ݲ���"));
        return;
    }
    //���ɵ�ͼ
    srand(GetTickCount());
    m_arr.clear();
    int step = Space1Element;
    for(int r = 0; r < m_row; ++r) {
        // һ�е�ͼ�ռ�
        vector<CGameMapElement> vRow;
        // ������
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
        // ׷��һ��
        m_arr.push_back(vRow);
    }

    // �������ܵ�ǽ��������ɵģ����Ի����ظ���;
    // �Ե�ͼ���д���ȥ���ظ���ǽ
    for(size_t m_row = 1; m_row < m_arr.size(); ++m_row) {
        for(size_t m_col = 1; m_col < m_arr[m_row].size(); ++m_col) {
            auto &a = m_arr[m_row][m_col];
            // �����
            auto const &top = m_arr[m_row - 1][m_col];
            // ��ߵ�
            auto const &left = m_arr[m_row][m_col - 1];
            if(top.m_bBottom && a.m_bTop) {
                a.m_bTop = false;
            }
            if(left.m_bRight && a.m_bLeft) {
                a.m_bLeft = false;
            }
        }
    }

    // �Ե�ͼ���д���ȥ�� ��ȫ��յ�����


    // ��ʼ�����ҹ���
    SAFE_DEL_PTR(m_finder);
    m_finder = new CPathFinderBFS(m_arr);
}

// ����ͼ
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

// �ӵ�ײǽ����
bool CGameMap::HitWallProcess(const CBullet &blt) const
{
    float delay, dir;
    bool b = GetHitWallInfo(blt, &delay, &dir);
    // �ı䷽��
    if(b) {
        const_cast<CBullet &>(blt).SetDirectionArc(dir);
    }

    return b;
}

// �ж��Ƿ�ײ��ǽ�����ײ���ˣ����� ����ǣ�ײǽ��ʱ��
bool CGameMap::GetHitWallInfo(const CBullet &blt, float *pTime /*= nullptr*/, float *pDir /*= nullptr*/) const
{
    // ���Ҳ�����ײ��ǽ
    struct info {
        CWall const &w; // �ཻ��ǽ
        PointF ptHit; // �ཻ��
        float dir; // �����ٶ�
        float delay; // �ཻʱ��
    };

    vector<struct info> v;
    for(auto &line : m_arr) {  // ��
        for(auto &ele : line) {  // ��
            // ��ǽ���Ҳ�����ײ
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
    // ��������ײ����ǽ��
    auto it = std::min_element(v.begin(), v.end(), [](auto & lhs, auto & rhs)->bool {
        return lhs.delay < rhs.delay;
    });

    // ����ײǽ��Ϣ
    if(pTime) {
        *pTime = it->delay;
    }

    if(*pDir) {
        *pDir = it->dir;
    }
    return true;
}

// �ж�
bool CGameMap::IsHitTheWall(const CTankEntry &tank, bool bForward) const
{
    // �ж����е�ǽ���鿴�Ƿ������ײ
    for(auto &line : m_arr) {  // ��
        for(auto &ele : line) {  // ��
            // ��ǽ���Ҳ�����ײ
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


// ���һ�����Է�����Ʒ��λ�ã�λ���������
bool CGameMap::FindRandomPosition(PointF &ptCenter) const
{
    if(m_arr.empty() || m_arr[0].empty()) {
        return false;
    }
    try {
        // ������� �У���
        size_t m_row, m_col;
        m_row = static_cast<size_t>(rand()) % m_arr.size();
        m_col = static_cast<size_t>(rand()) % m_arr[0].size();
        // ���ѡ�е� ��ͼ��� ���ĵ�
        ptCenter = m_arr[m_row][m_col].GetCenterPoint();
    }
    catch(...) {
        return false;
    }
    return true;
}

// �жϵ�ǰ�����ڵ�����, x : �����ڵڼ���,y : �����ڵڼ���
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
    // û���ҵ���
    return false;
}

//  path ����·�� [��㣬�������� �յ�]
void CGameMap::FindPath(int startX, int startY, int targetX, int targetY, VPath &path) const
{
    path.clear();
    m_finder->FindPath(startX, startY, targetX, targetY, path);
}

// �����һ�ε�ǰ������
bool CGameMap::FindNextDirection(float *dir, int startX, int startY, int targetX, int targetY) const
{
    VPath path;
    FindPath(startX, startY, targetX, targetY, path);
    if(path.empty()) {
        return false;
    }

    // ��õ�һ��λ��
    auto it = path.begin();
    it++;

    if(it == path.end()) {
        return false;
    }

    it->x;
    it->y;
    // �ж����ĸ�����
    if(startX == it->x) {
        // ͬһ��
        if(startY < it->y) {
            // ���
            *dir = 90;
        }
        else if(startY > it->y) {
            // �Ҳ�
            *dir = 270;
        }
        else {

        }
    }
    else if(startY == it->y) {
        // ͬһ��
        if(startX < it->x) {
            // �Ϸ�
            *dir = 180;
        }
        if(startX > it->x) {
            // �·�
            *dir = 0;
        }
    }
    else {

    }
    return true;
}

// ���ĳ������ĵ�λ��
PointF CGameMap::GetElementAreaCenter(int row, int col) const
{
    try {
        return m_arr[row][col].GetCenterPoint();
    }
    catch(...) {
        return PointF(-1, -1);
    }
}

// �ж����ڷ����ӵ��Ƿ���Դ����
bool CGameMap::IsCanKillTarget(const CGameEntryMoveable &bot, const CGameEntryMoveable &target, float *dir/* = nullptr*/) const
{
    // �����ˣ���� ���ڵ�λ��
    int startRow, startCol, targetRow, targetCol;
    if(!FindObjPosition(bot, startRow, startCol) || !FindObjPosition(target, targetRow, targetCol)) {
        return false;
    }
    // ��ᶼ����һ��������
    if(startRow != targetRow && startCol != targetCol) {
        return false;
    }
    // �鿴�ķ����Ƿ���Դ�Ŀ��
    if(startCol == targetCol && startRow == targetRow) {
        if(dir) {
            *dir = 0;
        }
        return true;
    }
    // ����鿴
    if(startCol == targetCol) {
        // ���²鿴
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
        // ���ϲ鿴
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
    // ����鿴
    if(startRow == targetRow) {
        // ���Ҳ鿴
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
        // ����鿴
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