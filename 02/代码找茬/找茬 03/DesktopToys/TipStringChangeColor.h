#pragma once
#include "IDrawable.h"
#include "GameTimer.h"

/*
    �������ʾ����
*/
class CTipStringChangeColor:
    public IDrawable
{
public:
    CTipStringChangeColor(PCTSTR szText);
    virtual ~CTipStringChangeColor();

    // ������
    virtual void Draw(Gdiplus::Graphics &gh);

    // ����Ϊ��ʼ״̬
    void SetPos(CPoint pt, DWORD dwTimeval = 300)
    {
        m_timeval.SetTimeval(dwTimeval);
        m_bFlag = true;
        m_pos.X = (float)pt.x;
        m_pos.Y = (float)pt.y - 30.0f;
    }

    // ����״̬
    void SetEnd()
    {
        m_bFlag = false;
    }
private:
    CString m_str;
    CGameTimeval m_timeval;
    bool m_bFlag{false};
    // �����ֵ�λ��(�����䶯);
    PointF m_pos;
    std::vector<Color> m_vColor;
    size_t m_index{0};
};

