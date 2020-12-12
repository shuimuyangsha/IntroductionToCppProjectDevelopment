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

	// ÿһ������ռ�õĿ��,������϶
	double dw = m_panelW / m_count;
	// �۸� �߶ȱ�(ÿһԪ,����ռ�ö��ٸ߶�)
	double dy = m_panelH / (m_maxPrice - m_minPrice);

	// 1. ���ݱ�����ϵ,���ʵ�ʻ滭�Ĵ�С
	// ���ߵĿ�ʼ,������
	double lineX{ (0.5 + m_index) * dw };
	double lineY{ (m_maxPrice - m_max)*dy };
	// ���ߵĽ�����
	double lineXX{ lineX };
	double lineYY{ (m_maxPrice - m_min)*dy };
	// ���͵Ŀ�ʼ,������
	double bodyX{ m_index * dw };
	double bodyY{ (m_maxPrice - (std::max)(m_open, m_close))*dy };
	double bodyW{ dw };
	double bodyH{ dy * std::abs(m_open - m_close) };

	// 3.��Ӱ��,ʵ��,ע��˳��
	Gdiplus::Color clr(m_close > m_open ? m_clrUp : m_clrDown);
	Gdiplus::SolidBrush brush(clr);
	Gdiplus::Pen pen(&brush);

	gh.DrawLine(&pen, static_cast<int>(lineX), static_cast<int>(lineY), static_cast<int>(lineXX), static_cast<int>(lineYY));
	gh.DrawRectangle(&pen, static_cast<int>(bodyX), static_cast<int>(bodyY), static_cast<int>(bodyW), static_cast<int>(bodyH));
	gh.FillRectangle(&brush, static_cast<int>(bodyX), static_cast<int>(bodyY), static_cast<int>(bodyW), static_cast<int>(bodyH));
}


