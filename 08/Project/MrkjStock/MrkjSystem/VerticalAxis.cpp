#include "stdafx.h"
#include "StockChart.h"
#include "VerticalAxis.h"



CVerticalAxis::CVerticalAxis(CStockChart* pParent)
    : m_pParent(pParent)
{
    // 设置 自定义的画刷
    {
        REAL dashVals[] = { 1.0f, 3.0f };
        m_pPenGrid->SetDashPattern(dashVals, _countof(dashVals));
    }
    UpdateCtrl();
}
CVerticalAxis::~CVerticalAxis()
{
    SAFE_DELETE(m_pBrush);
    SAFE_DELETE(m_pPenGrid);
    SAFE_DELETE(m_pPenLine);
    SAFE_DELETE(m_pFont);
}

// 更新位置信息．大小等等,主要用于父窗口变大之时
void CVerticalAxis::UpdateCtrl()
{
    RECT rect;
    if(m_pParent->GetSafeHwnd()) {
        m_pParent->GetClientRect(&rect);
        m_nXBegin = rect.right - m_nOffsetRight;
        m_nYBegin = rect.top;
        m_nXEnd = m_nXBegin;
        m_nYEnd = rect.bottom - m_nOffsetBottom;
    }
}

// 当鼠标移动时
void CVerticalAxis::OnMouseMove(CPoint pt)
{
}

// 画自己
void CVerticalAxis::Draw(Gdiplus::Graphics& gh)
{
    // 画底部横线
    gh.DrawLine(m_pPenLine, m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd);
    // 画网格线和文字
    {
        const int nCount = 16;
        float dif = (GetRealMaxPrice() - GetRealMinPrice()) / nCount;
        float price = GetRealMinPrice();
        while(price < GetRealMaxPrice()) {
            int y = PriceToYPos(price);
            // 网格线
            gh.DrawLine(m_pPenGrid, 0, y, m_nXEnd, y);
            // 标签线
            gh.DrawLine(m_pPenLine, m_nXEnd, y, m_nXEnd + 4, y);
            //文字
            PointF origin(static_cast<float>(m_nXEnd + 4), static_cast<float>(y - 5));
            std::wstring str = StringHelper::ToString(price, 2);
            gh.DrawString(str.c_str(), -1, m_pFont, origin, m_pBrush);
            price += dif;
        }
    }
}

// 根据y坐标，计算价格
float CVerticalAxis::YPosToPrice(int y)  const
{
    // TODO
    return 0;
}

// 根据价格，计算：y坐标
int CVerticalAxis::PriceToYPos(float price) const
{
    /*
        计算注意:
        最下面的是确定的 m_minPrice对应的坐标是确定的
    */
    float y = GetRealMaxPrice() - price;
    return static_cast<int>(y * GetLenOf1Yuan());
}

// 计算1元可以占用的像素数
int CVerticalAxis::GetLenOf1Yuan() const
{
    // 价格范围
    float pr_dif = GetRealMaxPrice() - GetRealMinPrice();
    float dy = GetLenOfAxis() / pr_dif; //每单位价格占用的长度
    int idy = static_cast<int>(std::ceil(dy));
    return idy;
}

// 计算坐标轴的长度
int CVerticalAxis::GetLenOfAxis() const
{
    return m_nYEnd - m_nYBegin;
}

// 获得最下面/上面的作坐标对应的价格,这个是调整过后的
float CVerticalAxis::GetRealMinPrice() const
{
    // 把最小值向下取整(使得范围变大)
    float nMin = std::floor(m_minPrice);
    return nMin;
}
float CVerticalAxis::GetRealMaxPrice() const
{
    // 把最大值向上取整(使得范围变大)
    float nMax = std::ceil(m_maxPrice);
    return nMax;
}

// 本得 横轴本身占用的 RECT
RECT CVerticalAxis::GetRectAxis() const
{
    RECT r{ m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd };
    return r;
}
