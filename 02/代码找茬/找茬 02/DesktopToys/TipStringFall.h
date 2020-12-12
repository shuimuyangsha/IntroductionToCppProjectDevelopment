#pragma once
#include "IDrawable.h"
#include "GameTimer.h"

/*
    下落的提示文字
*/
class CTipStringFall :
    public IDrawable
{
public:
    CTipStringFall(float x, float y, PCTSTR szText);
    virtual ~CTipStringFall();

    // 画文字
    virtual void Draw(Gdiplus::Graphics &gh);
    // 设置为初始状态
    void Restart(DWORD dwTimeout = 2000, DWORD dwTimeval = 100)
    {
        m_timeval.SetTimeout(dwTimeout);
        m_timeval.StartTimer();
        m_timeval.SetTimeval(dwTimeval);
        m_pos = GetCenterPos();
        m_pos.X -= 50;
        m_bFlag = true;
    }
    void SetEnd()
    {
        m_bFlag = false;
    }
private:
    CString m_str;
    CGameTimer m_timeval;
    bool m_bFlag{false};
    // 画文字的位置(原始位置保持不变)
    PointF m_pos;
};

