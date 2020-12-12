#pragma once

/*
    ��������
*/

using namespace Gdiplus;

// ǰ������
class CStockChart;

class CVerticalAxis
{
public:
    CVerticalAxis(CStockChart* pParent);
    ~CVerticalAxis();

    // ����λ����Ϣ����С�ȵ�,��Ҫ���ڸ����ڱ��֮ʱ
    void UpdateCtrl();

    // ���� �۸�����
    void SetPriceMaxMin(float max, float min)
    {
        m_minPrice = min;
        m_maxPrice = max;
    }

    // ������ƶ�ʱ
    void OnMouseMove(CPoint pt);

    // ���Լ�
    void Draw(Gdiplus::Graphics& gh);

    // ����y���꣬����۸�
    float YPosToPrice(int y) const;

    // ���ݼ۸񣬼��㣺y����
    int PriceToYPos(float price) const;

    // ����1Ԫ����ռ�õ�������
    int GetLenOf1Yuan() const;

    // ����������ĳ���
    int GetLenOfAxis() const;

    // ���������/������������Ӧ�ļ۸�,����ǵ��������
    float GetRealMinPrice() const;
    float GetRealMaxPrice() const;

    // ���� ���᱾��ռ�õ� RECT
    RECT GetRectAxis() const;
private:
    // ���ؼ�
    CStockChart* m_pParent;

    // �۸�Χ
    float m_minPrice{ 10.0 }, m_maxPrice{ 20.0 };

    // ���������蹤��
    Gdiplus::Color m_clr{ Gdiplus::Color(0xFF, 0xFF, 0x00, 0x00) };
    Gdiplus::Font * m_pFont{ new Gdiplus::Font(_T("����"), 8.0f) };
    Gdiplus::Pen * m_pPenLine{ new Gdiplus::Pen(m_clr)};
    Gdiplus::Pen * m_pPenGrid{ new Gdiplus::Pen(m_clr)};
    Gdiplus::SolidBrush * m_pBrush {new SolidBrush(Color(0xFF, 0xFF, 0, 0))};

    // ��ʼλ��
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };

    int m_nXEnd{ 0 };
    int m_nYEnd{ 0 };

    // ���߾���ײ��ľ���
    int m_nOffsetBottom{ 20 };
    // ���߾����Ҳ�ľ���
    int m_nOffsetRight{ 40 };
};

