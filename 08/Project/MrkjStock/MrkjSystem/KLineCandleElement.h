#pragma once
#include "KLineElement.h"
class CKLineCandleElement :
    public CKLineElement
{
public:
    // �������۸�/��С�۸�, ����۸�.�Զ�ȷ�������λ��,�ʹ�С
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
    // �������Ŀ��,��С�����ĸ���
    double m_panelH{};
    double m_panelW{};

    //
    int m_count{ 1 };
    int m_index{ 1 };

    // 
    int m_x{};
    int m_y{};

    // ��߼�,��ͼ�
    double m_maxPrice{};
    double m_minPrice{};

    // ��ɫ : ��,Ĭ���Ǻ�ɫ
    Gdiplus::Color m_clrUp{ 0xFF, 0xFF, 0, 0 };
    // ��ɫ : ��,Ĭ������ɫ
    Gdiplus::Color m_clrDown{ 0xFF, 0, 0xFF, 0 };

    // ���ռ۸�
    double m_max{};
    double m_min{};
    double m_open{};
    double m_close{};
};

