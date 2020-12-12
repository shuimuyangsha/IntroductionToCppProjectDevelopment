#pragma once

/*
    纵坐标轴
*/

using namespace Gdiplus;

// 前向声明
class CStockChart;

class CVerticalAxis
{
public:
    CVerticalAxis(CStockChart* pParent);
    ~CVerticalAxis();

    // 更新位置信息．大小等等,主要用于父窗口变大之时
    void UpdateCtrl();

    // 设置 价格区间
    void SetPriceMaxMin(float max, float min)
    {
        m_minPrice = min;
        m_maxPrice = max;
    }

    // 当鼠标移动时
    void OnMouseMove(CPoint pt);

    // 画自己
    void Draw(Gdiplus::Graphics& gh);

    // 根据y坐标，计算价格
    float YPosToPrice(int y) const;

    // 根据价格，计算：y坐标
    int PriceToYPos(float price) const;

    // 计算1元可以占用的像素数
    int GetLenOf1Yuan() const;

    // 计算坐标轴的长度
    int GetLenOfAxis() const;

    // 获得最下面/上面的作坐标对应的价格,这个是调整过后的
    float GetRealMinPrice() const;
    float GetRealMaxPrice() const;

    // 本得 横轴本身占用的 RECT
    RECT GetRectAxis() const;
private:
    // 窗控件
    CStockChart* m_pParent;

    // 价格范围
    float m_minPrice{ 10.0 }, m_maxPrice{ 20.0 };

    // 画蜡烛所需工具
    Gdiplus::Color m_clr{ Gdiplus::Color(0xFF, 0xFF, 0x00, 0x00) };
    Gdiplus::Font * m_pFont{ new Gdiplus::Font(_T("宋体"), 8.0f) };
    Gdiplus::Pen * m_pPenLine{ new Gdiplus::Pen(m_clr)};
    Gdiplus::Pen * m_pPenGrid{ new Gdiplus::Pen(m_clr)};
    Gdiplus::SolidBrush * m_pBrush {new SolidBrush(Color(0xFF, 0xFF, 0, 0))};

    // 起始位置
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };

    int m_nXEnd{ 0 };
    int m_nYEnd{ 0 };

    // 坚线距离底部的距离
    int m_nOffsetBottom{ 20 };
    // 坚线距离右测的距离
    int m_nOffsetRight{ 40 };
};

