#pragma once
#include "IDrawable.h"
#include "GameTimer.h"

/*
    下落的提示文字
*/
class CTipStringChangeColor:
    public IDrawable
{
public:
    CTipStringChangeColor(PCTSTR szText);
    virtual ~CTipStringChangeColor();

    // 画文字
    virtual void Draw(Gdiplus::Graphics &gh);

    // 设置为初始状态
    void SetPos(CPoint pt, DWORD dwTimeval = 300)
    {
        m_timeval.SetTimeval(dwTimeval);
        m_bFlag = true;
        m_pos.X = (float)pt.x;
        m_pos.Y = (float)pt.y - 30.0f;
    }

    // 结束状态
    void SetEnd()
    {
        m_bFlag = false;
    }
private:
    CString m_str;
    CGameTimeval m_timeval;
    bool m_bFlag{false};
    // 画文字的位置(随鼠标变动);
    PointF m_pos;
    std::vector<Color> m_vColor;
    size_t m_index{0};
};

