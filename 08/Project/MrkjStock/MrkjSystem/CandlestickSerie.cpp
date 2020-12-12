#include "stdafx.h"
#include "StockChart.h"
#include "CandlestickSerie.h"

CCandlestickSerie::CCandlestickSerie(CStockChart* pParent)
    : m_pParent(pParent)
{
    // ���û��߷��
    m_pPenMaxMinLabel->SetDashStyle(Gdiplus::DashStyleDashDotDot);
    // ���ö˵�(��ͷ)
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

// ����λ����Ϣ����С�ȵ�
void CCandlestickSerie::UpdateCtrl()
{
}

// ���Լ�ѽ
void CCandlestickSerie::Draw(Gdiplus::Graphics& gh)
{
    //��С����
    for(size_t index = 0; index != m_vPoints.size(); ++index) {
        DrawPoint(gh, index);
    }
    //�����߼�,��ͼ�����
    DrawHighLowText(gh);
}

void CCandlestickSerie::AddPoint(const COleDateTime& date, float high, float open, float close, float low)
{
	unsigned nCount = m_pParent->GetHorizonTalAxis()->GetLenOfAxis();
	if (m_vPoints.size() >= nCount) {
		return;
	}
    m_vPoints.push_back(CCandlestickPoint(date, high, open, close, low));
    // ������Сֵ
    if(m_minIndex == -1) {
        m_minIndex = 0;
    }
    else {
        if(m_vPoints[m_minIndex].m_low > low) {
            m_minIndex = m_vPoints.size() - 1;
        }
    }
    // �������ֵ
    if(m_maxIndex == -1) {
        m_maxIndex = 0;
    }
    else {
        // �Ƚ����ֵ
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
    //���ݿ��̼�/���̼� ������ɫ�����̼�/���̼�λ��
    if(data.m_close > data.m_open) {
        pBrush = m_pBrushUp;
        pPen = m_pPenUp;
    }
    else {
        pBrush = m_pBrushDown;
        pPen = m_pPenDown;
    }
    // ��ȡ ���� �Ĵ�С
    CRect rcBody, rcLine;
    GetCancleRect(rcBody, rcLine, index);
    // ����
    Gdiplus::Rect r = { rcBody.left, rcBody.top, rcBody.Width(), rcBody.Height() };
    gh.DrawRectangle(pPen, r);
    gh.FillRectangle(pBrush, r);
    //����
    gh.DrawLine(pPen, rcLine.left, rcLine.top, rcLine.right, rcLine.bottom);
}

//�����߼�,��ͼ�����
void CCandlestickSerie::DrawHighLowText(Graphics& gh)
{
    if(m_vPoints.empty()) {
        return;
    }
    // ��ȡ ���������
    auto pHor = m_pParent->GetHorizonTalAxis();
    auto pVer = m_pParent->GetVerticalAxis();
    const int line = 20; // ����ߵĳ���
    //�����ֵ
    {
        CRect rcBody, rcLine;
        GetCancleRect(rcBody, rcLine, m_maxIndex);
        int x = rcLine.left;
        int y = rcLine.top;
        // ��һ������
        gh.DrawLine(m_pPenMaxMinLabel, x, y, x + line, y);
        // ������
        CString str;
        str.Format(_T("%.2f"), m_vPoints[m_maxIndex].m_high);
        Gdiplus::PointF origin{ static_cast<float>(x + line), static_cast<float>(y) };
        gh.DrawString(str.GetString(), -1, m_pFontMaxMinLabel, origin, m_pBrushMaxMinLabel);
    }
    // ��Сֵ
    {
        CRect rcBody, rcLine;
        GetCancleRect(rcBody, rcLine, m_minIndex);
        int x = rcLine.right;
        int y = rcLine.bottom;
        // ��һ������
        gh.DrawLine(m_pPenMaxMinLabel, x, y, x + line, y);
        // ������
        CString str;
        str.Format(_T("%.2f"), m_vPoints[m_minIndex].m_low);
        Gdiplus::PointF origin{ static_cast<float>(x + line), static_cast<float>(y) };
        gh.DrawString(str.GetString(), -1, m_pFontMaxMinLabel, origin, m_pBrushMaxMinLabel);
    }
}

void CCandlestickSerie::GetCancleRect(CRect& rcBody, CRect& rcLine, size_t index) const
{
    // ��ȡ ������
    auto pHor = m_pParent->GetHorizonTalAxis();
    auto pVer = m_pParent->GetVerticalAxis();
    assert(pHor);
    assert(pVer);
    // ���͵�λ��,��С
    int x_body, y_body, width_body, height_body;
    // �� index  ������
    const CCandlestickPoint & data = m_vPoints[index];
    // λ��
    int X = pHor->DateToXPos(data.m_time);
    //�����Ŀ��(��С��1)
    int nWidth = pHor->GetLenOf1Day();
    //TRACE("��%03d������ʼλ��:%d,���:%d\r\n", index, X, nWidth);
    x_body = X;
    width_body = std::max<int>(nWidth - 1, 1);
    //ȷ��body����λ��λ��
    if(data.m_close > data.m_open) {
        y_body = pVer->PriceToYPos(data.m_close); //���̼�����
        height_body = std::max<int>(pVer->PriceToYPos(data.m_open) - y_body - 1, 1);
    }
    else {
        y_body = pVer->PriceToYPos(data.m_open); //���̼�����
        height_body = std::max<int>(pVer->PriceToYPos(data.m_close) - y_body - 1, 1);
    }
    rcBody.SetRect(x_body, y_body, x_body + width_body, y_body + height_body);
    //���������
    CRect rP;
    m_pParent->GetClientRect(rP);
    if(rcBody.Width() >= rP.Width() / 10) {
        rcBody.right = rcBody.left + rP.Width() / 10;
    }
    // ÿ���ұ���һ�����صĿհ�
    if(rcBody.Width() >= 2) {
        rcBody.right -= 1;
    }
    // �����ߵ�λ��
    // ��߼�
    int y_high = pVer->PriceToYPos(data.m_high);
    // ��ͼ�
    int y_low = pVer->PriceToYPos(data.m_low);
    // xλ��
    int x_high_low = X + rcBody.Width() / 2;
    rcLine.SetRect(x_high_low, y_high, x_high_low, y_low);
}


