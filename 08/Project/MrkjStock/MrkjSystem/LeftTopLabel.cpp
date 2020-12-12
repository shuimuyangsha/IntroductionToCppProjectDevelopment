#include "stdafx.h"
#include "StockChart.h"
#include "LeftTopLabel.h"

#define SAFE_DELETE(ptr) do { if(ptr) {delete ptr; ptr = nullptr;} }while(0)

CLeftTopLabel::CLeftTopLabel(CStockChart* pParent)
    : m_pParent(pParent)
{
    UpdateCtrl();
}
CLeftTopLabel::~CLeftTopLabel()
{
    SAFE_DELETE(m_pFont);
    SAFE_DELETE(m_pColor);
}

// 更新位置信息．大小等等
void CLeftTopLabel::UpdateCtrl()
{
    RECT rect;
    if(m_pParent->GetSafeHwnd()) {
        m_pParent->GetClientRect(&rect);
        m_nXBegin = rect.left;
        m_nYBegin = rect.top;
    }
}

// 画自己呀
void CLeftTopLabel::Draw(Gdiplus::Graphics& gh)
{
    Gdiplus::PointF origin(static_cast<float>(m_nXBegin), static_cast<float>(m_nYBegin));
    SolidBrush brush(*m_pColor);
    gh.DrawString(m_strLabel.c_str(), -1, m_pFont, origin, &brush);
}

// 设置显示的文本
void CLeftTopLabel::SetText(const wchar_t *szLbl)
{
    m_strLabel = szLbl;
}


