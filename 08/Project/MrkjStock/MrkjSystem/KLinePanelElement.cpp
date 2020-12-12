#include "stdafx.h"
#include "KLinePanelElement.h"


CKLinePanelElement::CKLinePanelElement(
        int x, int y, //ÆðÊ¼Î»ÖÃ
        double panelW, double panelH,
        double maxPrice, double minPrice,
        const Gdiplus::Color& clrBackground/* = Gdiplus::Color(0xFF, 0, 0, 0)*/,
        const Gdiplus::Color& clrRect/* = Gdiplus::Color(0xFF, 0xFF, 0, 0)*/
    )
    : m_x(x)
    , m_y(y)
    , m_panelH(panelH)
    , m_panelW(panelW)
    , m_maxPrice(maxPrice)
    , m_minPrice(minPrice)
    , m_clrBackground(clrBackground)
    , m_clrRect(clrRect)
{
}


CKLinePanelElement::~CKLinePanelElement()
{
}

void CKLinePanelElement::Draw(Gdiplus::Graphics& gh)
{
    /*
    */
    Gdiplus::SolidBrush brush(m_clrBackground);
    Gdiplus::SolidBrush brush2(m_clrRect);
    Gdiplus::Pen pen(&brush);
    Gdiplus::Pen pen2(&brush);

    gh.FillRectangle(&brush, static_cast<int>(m_x), static_cast<int>(m_y), static_cast<int>(m_panelW), static_cast<int>(m_panelH));
    gh.DrawRectangle(&pen2, static_cast<int>(m_x), static_cast<int>(m_y), static_cast<int>(m_panelW), static_cast<int>(m_panelH));
}


