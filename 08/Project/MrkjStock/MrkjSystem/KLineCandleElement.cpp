#include "stdafx.h"
#include "KLineCandleElement.h"


CKLineCandleElement::CKLineCandleElement(int x, int y, 
                        double panelW, double panelH,
                        double maxPrice, double minPrice,
                        int count, int index, 
        double max, double min, double open, double close,
        const Gdiplus::Color& clrUp/* = Gdiplus::Color(0xFF, 0xFF, 0, 0)*/,
        const Gdiplus::Color& clrDown/* = Gdiplus::Color(0xFF, 0, 0xFF, 0)*/)
    : m_x(x)
    , m_y(y)
    , m_panelH(panelH)
    , m_panelW(panelW)
    , m_maxPrice (maxPrice)
    , m_minPrice (minPrice)
    , m_count(count)
    , m_index(index)
    , m_max(max)
    , m_min(min)
    , m_open(open)
    , m_close(close)
    , m_clrUp(clrUp)
    , m_clrDown(clrDown)
{
}


CKLineCandleElement::~CKLineCandleElement()
{

}

void CKLineCandleElement::Draw(Gdiplus::Graphics& gh)
{
	/*
	  ----------------------> x
	  |      |
	  |     |||
	  |      |
	  y

	*/

	// 每一个可以占用的宽度,包括空隙
	double dw = m_panelW / m_count;
	// 价格 高度比(每一元,可以占用多少高度)
	double dy = m_panelH / (m_maxPrice - m_minPrice);

	// 1. 根据比例关系,算出实际绘画的大小
	// 坚线的开始,结束点
	double lineX{ (0.5 + m_index) * dw };
	double lineY{ (m_maxPrice - m_max)*dy };
	// 坚线的结束点
	double lineXX{ lineX };
	double lineYY{ (m_maxPrice - m_min)*dy };
	// 矩型的开始,结束点
	double bodyX{ m_index * dw };
	double bodyY{ (m_maxPrice - (std::max)(m_open, m_close))*dy };
	double bodyW{ dw };
	double bodyH{ dy * std::abs(m_open - m_close) };

	// 3.划影线,实体,注意顺序
	Gdiplus::Color clr(m_close > m_open ? m_clrUp : m_clrDown);
	Gdiplus::SolidBrush brush(clr);
	Gdiplus::Pen pen(&brush);

	gh.DrawLine(&pen, static_cast<int>(lineX), static_cast<int>(lineY), static_cast<int>(lineXX), static_cast<int>(lineYY));
	gh.DrawRectangle(&pen, static_cast<int>(bodyX), static_cast<int>(bodyY), static_cast<int>(bodyW), static_cast<int>(bodyH));
	gh.FillRectangle(&brush, static_cast<int>(bodyX), static_cast<int>(bodyY), static_cast<int>(bodyW), static_cast<int>(bodyH));
}


