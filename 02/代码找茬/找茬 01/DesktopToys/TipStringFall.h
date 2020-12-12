#pragma once
#include "IDrawable.h"
#include "GameTimer.h"

/*
    �������ʾ����
*/
class CTipStringFall :
    public IDrawable
{
public:
    CTipStringFall(float x, float y, PCTSTR szText);
    virtual ~CTipStringFall();

    // ������
    virtual void Draw(Gdiplus::Graphics &gh);
    // ����Ϊ��ʼ״̬
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
    // �����ֵ�λ��(ԭʼλ�ñ��ֲ���)
    PointF m_pos;
};

