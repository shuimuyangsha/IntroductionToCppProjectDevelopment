#pragma once
#include "KLineElement.h"
class CKLineCandleElement :
    public CKLineElement
{
public:
    // 根据最大价格/最小价格, 自身价格.自动确定自身的位置,和大小
    CKLineCandleElement(int x, int y,
        double panelW, double panelH,
        double maxPrice, double minPrice,
        int count, int index, 
        double max, double min, double open, double close,
        const Gdiplus::Color& clrUp = Gdiplus::Color(0xFF, 0xFF, 0, 0),
        const Gdiplus::Color& clrDown = Gdiplus::Color(0xFF, 0, 0xFF, 0));
    virtual ~CKLineCandleElement();
    virtual void Draw(Gdiplus::Graphics& gh);
private:
    // 整个面板的宽高,及小部件的个数
    double m_panelH{};
    double m_panelW{};

    //
    int m_count{ 1 };
    int m_index{ 1 };

    // 
    int m_x{};
    int m_y{};

    // 最高价,最低价
    double m_maxPrice{};
    double m_minPrice{};

    // 颜色 : 涨,默认是红色
    Gdiplus::Color m_clrUp{ 0xFF, 0xFF, 0, 0 };
    // 颜色 : 跌,默认是绿色
    Gdiplus::Color m_clrDown{ 0xFF, 0, 0xFF, 0 };

    // 今日价格
    double m_max{};
    double m_min{};
    double m_open{};
    double m_close{};
};

