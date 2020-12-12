#pragma once

#include "Wall.h"
#include "Bullet.h"

class CGamePathFinder;

/*
    �����е�ͼ�е�һ������
    ���ܿ�����ǽ����ͨ���� �м��ǿյؿ���ͨ��
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
    // �ĸ����������Ƿ���ǽ
    bool m_bLeft{ false };
    bool m_bTop{ false };
    bool m_bRight{ false };
    bool m_bBottom{ false };

    // �����ĸ�����
    typedef enum { EWLeft = 0, EWTop, EWRight, EWBottom, EWSize } EWallIndex;

    // ��������� ����������һ��ȡ�����ؿ��ǽ
    const CWall &operator[](CGameMapElement::EWallIndex eIndex) const
    {
        return m_wallArray[eIndex];
    };

    // ���ÿյص���ɫ
    void SetSpaceColor(Color clr)
    {
        m_sapceClr = clr;
    }

private:
    // �յص���ɫ
    Color m_sapceClr{ Color(0xE6, 0xE6, 0xE6) };

    // ǽ�Ŀ��
    float m_wallWidth{ 10 };

    // �м�յصĿ��
    float m_spaceLen{ 80 };

    // ���������ǽ : ��������, ע��˳��
    CWall m_wallArray[4];
};

/* ����������ͼ
*/
class CGameMap : public CGameEntryMoveable
{
public:
    CGameMap();
    CGameMap(int x, int y, int width, int height);
    virtual ~CGameMap();

    // �����ͼ
    void LoadMap();

    // ����ͼ
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // �ӵ�ײǽ����
    bool HitWallProcess(const CBullet &blt) const;

    // �ж��Ƿ�ײ��ǽ�����ײ���ˣ����� ����ǣ�ײǽ��ʱ��
    bool GetHitWallInfo(const CBullet &blt, float *pTime = nullptr, float *pDir = nullptr) const;

    // ̹���Ƿ�ײǽ
    bool IsHitTheWall(const CTankEntry &tank, bool bForward) const;

    // ���һ�����Է�����Ʒ��λ�ã�λ���������
    bool FindRandomPosition(PointF &ptCenter) const;

    // �жϵ�ǰ�����ڵ�����, x : �����ڵڼ���,y : �����ڵڼ���
    bool FindObjPosition(const CGameEntryMoveable &obj, int &row, int &col) const;

    /// ///////////////////////////////////////////////////////////////////////////////
    /// ·�� ����
    //  path ����·�� [��㣬�������� �յ�]
    void FindPath(int startX, int startY, int targetX, int targetY, VPath &path) const;

    // �����һ�ε�ǰ������
    bool FindNextDirection(float *dir, int startX, int startY, int targetX, int targetY) const;

    // ���ĳ������ĵ�λ��
    PointF GetElementAreaCenter(int row, int col) const;

    // �ж����ڷ����ӵ��Ƿ���Դ����
    bool IsCanKillTarget(const CGameEntryMoveable &bot, const CGameEntryMoveable &target, float *dir = nullptr) const;

private:
    // �潲���еĵ�ͼ
    std::vector<std::vector<CGameMapElement>> m_arr;

    // ������ͼ�ķ�Χ
    Gdiplus::Rect m_rectMap{ 0, 0, 800, 600 };

    // ��ͼ��ǽ�Ŀ��
    int m_wallWidth{ 7 };

    // ��ͼ���м��λ�Ŀ��
    int m_spaceLen{ 42 };
private:
    // ·������
    CGamePathFinder *m_finder{ nullptr };
};

