#include "stdafx.h"
#include "StockChart.h"
#include "CandlestickSerie.h"

CCandlestickSerie::CCandlestickSerie(CStockChart* pParent)
    : m_pParent(pParent)
{
    // 设置画线风格
    m_pPenMaxMinLabel->SetDashStyle(Gdiplus::DashStyleDashDotDot);
    // 设置端点(箭头)
    m_pPenMaxMinLabel->SetStartCap(Gdiplus::LineCapArrowAnchor);
    UpdateCtrl();
}

CCandlestickSerie::~CCandlestickSerie()
{
    SAFE_DELETE(m_pBrushMaxMinLabel);
    SAFE_DELETE(m_pPenMaxMinLabel);
    SAFE_DELETE(m_pFontMaxMinLabel);
    SAFE_DELETE(m_pPenDown);
    SAFE_DELETE(m_pBrushDown);
    SAFE_DELETE(m_pPenUp);
    SAFE_DELETE(m_pBrushUp);
}

// 更新位置信息．大小等等
void CCandlestickSerie::UpdateCtrl()
{
}

// 画自己呀
void CCandlestickSerie::Draw(Gdiplus::Graphics& gh)
{
    //画小蜡烛
    for(size_t index = 0; index != m_vPoints.size(); ++index) {
        DrawPoint(gh, index);
    }
    //标记最高价,最低价文字
    DrawHighLowText(gh);
}

void CCandlestickSerie::AddPoint(const COleDateTime& date, float high, float open, float close, float low)
{
	unsigned nCount = m_pParent->GetHorizonTalAxis()->GetLenOfAxis();
	if (m_vPoints.size() >= nCount) {
		return;
	}
    m_vPoints.push_back(CCandlestickPoint(date, high, open, close, low));
    // 更新最小值
    if(m_minIndex == -1) {
        m_minIndex = 0;
    }
    else {
        if(m_vPoints[m_minIndex].m_low > low) {
            m_minIndex = m_vPoints.size() - 1;
        }
    }
    // 更新最大值
    if(m_maxIndex == -1) {
        m_maxIndex = 0;
    }
    else {
        // 比较最大值
        if(m_vPoints[m_maxIndex].m_high < high) {
            m_maxIndex = m_vPoints.size() - 1;
        }
    }
}

void CCandlestickSerie::DrawPoint(Gdiplus::Graphics& gh, size_t index)
{
    SolidBrush* pBrush = nullptr;
    Pen* pPen = nullptr;
    const CCandlestickPoint & data = m_vPoints[index];
    //根据开盘价/收盘价 判数颜色及开盘价/收盘价位置
    if(data.m_close > data.m_open) {
        pBrush = m_pBrushUp;
        pPen = m_pPenUp;
    }
    else {
        pBrush = m_pBrushDown;
        pPen = m_pPenDown;
    }
    // 获取 蜡烛 的大小
    CRect rcBody, rcLine;
    GetCancleRect(rcBody, rcLine, index);
    // 画体
    Gdiplus::Rect r = { rcBody.left, rcBody.top, rcBody.Width(), rcBody.Height() };
    gh.DrawRectangle(pPen, r);
    gh.FillRectangle(pBrush, r);
    //画线
    gh.DrawLine(pPen, rcLine.left, rcLine.top, rcLine.right, rcLine.bottom);
}

//标记最高价,最低价文字
void CCandlestickSerie::DrawHighLowText(Graphics& gh)
{
    if(m_vPoints.empty()) {
        return;
    }
    // 获取 坐标轴对象
    auto pHor = m_pParent->GetHorizonTalAxis();
    auto pVer = m_pParent->GetVerticalAxis();
    const int line = 20; // 标记线的长度
    //画最大值
    {
        CRect rcBody, rcLine;
        GetCancleRect(rcBody, rcLine, m_maxIndex);
        int x = rcLine.left;
        int y = rcLine.top;
        // 画一条白线
        gh.DrawLine(m_pPenMaxMinLabel, x, y, x + line, y);
        // 画数字
        CString str;
        str.Format(_T("%.2f"), m_vPoints[m_maxIndex].m_high);
        Gdiplus::PointF origin{ static_cast<float>(x + line), static_cast<float>(y) };
        gh.DrawString(str.GetString(), -1, m_pFontMaxMinLabel, origin, m_pBrushMaxMinLabel);
    }
    // 最小值
    {
        CRect rcBody, rcLine;
        GetCancleRect(rcBody, rcLine, m_minIndex);
        int x = rcLine.right;
        int y = rcLine.bottom;
        // 画一条白线
        gh.DrawLine(m_pPenMaxMinLabel, x, y, x + line, y);
        // 画数字
        CString str;
        str.Format(_T("%.2f"), m_vPoints[m_minIndex].m_low);
        Gdiplus::PointF origin{ static_cast<float>(x + line), static_cast<float>(y) };
        gh.DrawString(str.GetString(), -1, m_pFontMaxMinLabel, origin, m_pBrushMaxMinLabel);
    }
}

void CCandlestickSerie::GetCancleRect(CRect& rcBody, CRect& rcLine, size_t index) const
{
    // 获取 坐标轴
    auto pHor = m_pParent->GetHorizonTalAxis();
    auto pVer = m_pParent->GetVerticalAxis();
    assert(pHor);
    assert(pVer);
    // 距型的位置,大小
    int x_body, y_body, width_body, height_body;
    // 第 index  项数据
    const CCandlestickPoint & data = m_vPoints[index];
    // 位置
    int X = pHor->DateToXPos(data.m_time);
    //所画的宽度(最小是1)
    int nWidth = pHor->GetLenOf1Day();
    //TRACE("第%03d个点起始位置:%d,宽度:%d\r\n", index, X, nWidth);
    x_body = X;
    width_body = std::max<int>(nWidth - 1, 1);
    //确定body上下位置位置
    if(data.m_close > data.m_open) {
        y_body = pVer->PriceToYPos(data.m_close); //收盘价在上
        height_body = std::max<int>(pVer->PriceToYPos(data.m_open) - y_body - 1, 1);
    }
    else {
        y_body = pVer->PriceToYPos(data.m_open); //开盘价在上
        height_body = std::max<int>(pVer->PriceToYPos(data.m_close) - y_body - 1, 1);
    }
    rcBody.SetRect(x_body, y_body, x_body + width_body, y_body + height_body);
    //限制最大宽度
    CRect rP;
    m_pParent->GetClientRect(rP);
    if(rcBody.Width() >= rP.Width() / 10) {
        rcBody.right = rcBody.left + rP.Width() / 10;
    }
    // 每条右边留一个像素的空白
    if(rcBody.Width() >= 2) {
        rcBody.right -= 1;
    }
    // 计算线的位置
    // 最高价
    int y_high = pVer->PriceToYPos(data.m_high);
    // 最低价
    int y_low = pVer->PriceToYPos(data.m_low);
    // x位置
    int x_high_low = X + rcBody.Width() / 2;
    rcLine.SetRect(x_high_low, y_high, x_high_low, y_low);
}


