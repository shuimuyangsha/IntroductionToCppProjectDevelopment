#include "stdafx.h"
#include "StockChart.h"
#include "VerticalAxis.h"



CVerticalAxis::CVerticalAxis(CStockChart* pParent)
    : m_pParent(pParent)
{
    // ���� �Զ���Ļ�ˢ
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

// ����λ����Ϣ����С�ȵ�,��Ҫ���ڸ����ڱ��֮ʱ
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

// ������ƶ�ʱ
void CVerticalAxis::OnMouseMove(CPoint pt)
{
}

// ���Լ�
void CVerticalAxis::Draw(Gdiplus::Graphics& gh)
{
    // ���ײ�����
    gh.DrawLine(m_pPenLine, m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd);
    // �������ߺ�����
    {
        const int nCount = 16;
        float dif = (GetRealMaxPrice() - GetRealMinPrice()) / nCount;
        float price = GetRealMinPrice();
        while(price < GetRealMaxPrice()) {
            int y = PriceToYPos(price);
            // ������
            gh.DrawLine(m_pPenGrid, 0, y, m_nXEnd, y);
            // ��ǩ��
            gh.DrawLine(m_pPenLine, m_nXEnd, y, m_nXEnd + 4, y);
            //����
            PointF origin(static_cast<float>(m_nXEnd + 4), static_cast<float>(y - 5));
            std::wstring str = StringHelper::ToString(price, 2);
            gh.DrawString(str.c_str(), -1, m_pFont, origin, m_pBrush);
            price += dif;
        }
    }
}

// ����y���꣬����۸�
float CVerticalAxis::YPosToPrice(int y)  const
{
    // TODO
    return 0;
}

// ���ݼ۸񣬼��㣺y����
int CVerticalAxis::PriceToYPos(float price) const
{
    /*
        ����ע��:
        ���������ȷ���� m_minPrice��Ӧ��������ȷ����
    */
    float y = GetRealMaxPrice() - price;
    return static_cast<int>(y * GetLenOf1Yuan());
}

// ����1Ԫ����ռ�õ�������
int CVerticalAxis::GetLenOf1Yuan() const
{
    // �۸�Χ
    float pr_dif = GetRealMaxPrice() - GetRealMinPrice();
    float dy = GetLenOfAxis() / pr_dif; //ÿ��λ�۸�ռ�õĳ���
    int idy = static_cast<int>(std::ceil(dy));
    return idy;
}

// ����������ĳ���
int CVerticalAxis::GetLenOfAxis() const
{
    return m_nYEnd - m_nYBegin;
}

// ���������/������������Ӧ�ļ۸�,����ǵ��������
float CVerticalAxis::GetRealMinPrice() const
{
    // ����Сֵ����ȡ��(ʹ�÷�Χ���)
    float nMin = std::floor(m_minPrice);
    return nMin;
}
float CVerticalAxis::GetRealMaxPrice() const
{
    // �����ֵ����ȡ��(ʹ�÷�Χ���)
    float nMax = std::ceil(m_maxPrice);
    return nMax;
}

// ���� ���᱾��ռ�õ� RECT
RECT CVerticalAxis::GetRectAxis() const
{
    RECT r{ m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd };
    return r;
}
