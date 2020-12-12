#include "stdafx.h"
#include "TipStringChangeColor.h"


CTipStringChangeColor::CTipStringChangeColor(PCTSTR szText) : m_str(szText)
{
    m_vColor.push_back(Color(0xFF, 0x00, 0x00));
    m_vColor.push_back(Color(0x00, 0xFF, 0x00));
    m_vColor.push_back(Color(0x00, 0x00, 0xFF));
    m_vColor.push_back(Color(0xFF, 0xFF, 0xFf));
}


CTipStringChangeColor::~CTipStringChangeColor()
{
}

// »­ÎÄ×Ö
void CTipStringChangeColor::Draw(Gdiplus::Graphics &gh)
{
    if(!m_bFlag) {
        return;
    }

    if(m_timeval.IsTimeval()) {
        m_index++;
        if(m_index >= m_vColor.size()) {
            m_index = 0;
        }
    }

    SolidBrush m_brush(m_vColor[m_index]);
    Gdiplus::Font m_font(_T("ËÎÌå"), 10.0);
    PointF origin = m_pos;
    CDC dc;
    gh.DrawString(m_str.GetString(), -1, &m_font, origin, &m_brush);
}


