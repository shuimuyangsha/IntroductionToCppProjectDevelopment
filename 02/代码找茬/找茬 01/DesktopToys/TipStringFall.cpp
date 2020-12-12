#include "stdafx.h"
#include "TipStringFall.h"


CTipStringFall::CTipStringFall(float x, float y, PCTSTR szText) : m_str(szText)
{
    SetCenterPos(x, y);
}


CTipStringFall::~CTipStringFall()
{
}

// ������
void CTipStringFall::Draw(Gdiplus::Graphics &gh)
{
    if(!m_bFlag) {
        return;
    }

    if(m_timeval.IsTimeout()) {
        return;
    }
    if(m_timeval.IsTimeval()) {
        // �������ֵ�λ��
        m_pos.Y += 10.0f;
    }
    SolidBrush m_brush(Color(0x00, 0x00, 0xFF));
    Gdiplus::Font m_font(_T("����"), 20.0);
    PointF origin = m_pos;
    CDC dc;
    gh.DrawString(m_str.GetString(), -1, &m_font, origin, &m_brush);
}


