#pragma once
#include "KLineElement.h"
class CKLinePanelElement :
    public CKLineElement
{
public:
    CKLinePanelElement(
        int x, int y, //ÆðÊ¼Î»ÖÃ
        double panelW, double panelH,
        double maxPrice, double minPrice,
        const Gdiplus::Color& clrBackground = Gdiplus::Color(0xFF, 0, 0, 0),
        const Gdiplus::Color& clrRect = Gdiplus::Color(0xFF, 0xFF, 0, 0)
        );
    virtual ~CKLinePanelElement();
    virtual void Draw(Gdiplus::Graphics& gh);
private:
    int m_x, m_y;
    double m_panelH, m_panelW;
    double m_maxPrice, m_minPrice;

    Gdiplus::Color m_clrBackground;
    Gdiplus::Color m_clrRect;
};

