#include "stdafx.h"
#include "StockChart.h"
#include "HorizontalAxis.h"

CHorizontalAxis::CHorizontalAxis(CStockChart* pParent)
    : m_pParent(pParent)
{
    UpdateCtrl();
}
CHorizontalAxis::~CHorizontalAxis()
{
    SAFE_DELETE(m_pPenLine);
    SAFE_DELETE(m_pBrush);
    SAFE_DELETE(m_pFont);
}

// ����λ����Ϣ����С�ȵ�,��Ҫ���ڸ����ڱ��֮ʱ
void CHorizontalAxis::UpdateCtrl()
{
    RECT rect;
    if(m_pParent->GetSafeHwnd()) {
        m_pParent->GetClientRect(&rect);
        m_nXBegin = rect.left;
        m_nYBegin = rect.bottom - m_nOffsetBottom;
        m_nXEnd = rect.right - m_nOffsetRight;
        m_nYEnd = m_nYBegin;
    }
}

// �����������,��С����
void CHorizontalAxis::SetTimeMaxMin()
{
    // �������ֵ
    const auto it_max = max_element(m_vDate.begin(), m_vDate.end());
    // ������Сֵ
    const auto it_min = min_element(m_vDate.begin(), m_vDate.end());
    if(it_max != m_vDate.end() && it_min != m_vDate.end()) {
        m_maxDt = *it_max;
        m_minDt = *it_min;
    }
}

// ���ú�������
void CHorizontalAxis::SetDateList(const vector<COleDateTime>& vDate)
{
    m_vDate.clear();
    m_vDate = vDate;
    SetTimeMaxMin();
}

// ����� n����x����
int CHorizontalAxis::GetXPos(int iIndex/*[0,)*/) const
{
    int iWidth = GetLenOf1Day();
    int iLength = GetLenOfAxis();
    //�����˶���
    int iLeft = iLength - (iLength / iWidth) * iWidth;
    //��i�����λ����
    return  iLeft + iWidth * iIndex + m_nXBegin;
}

// ����� dt �յ� x����
int CHorizontalAxis::GetXPos(const COleDateTime& dt) const
{
    int iIndex = -1;
    for(auto date : m_vDate) {
        if(dt <= date) {
            ++iIndex;
        }
        if(dt > date) {
            break;
        }
    }
    return GetXPos(iIndex);
}

// ������ƶ�
void CHorizontalAxis::OnMouseMove(CPoint pt)
{
#if 0
    // ������ǰ����
    COleDateTime dt = XPosToDate(pt.x);
    //�������ײ��ñ�������
    CClientDC aDc(m_pParent);
    Graphics gh(aDc.GetSafeHdc());
    SolidBrush blackBrush(Color::Black);
    const int len = 5;
    // �ñ���ɫ���
    gh.FillRectangle(&blackBrush, m_nXBegin, m_nYBegin + len, m_nXEnd + m_nOffsetRight, m_nYBegin + len + 20);
    // ������������
    Gdiplus::PointF origin;
    origin.X = static_cast<float>(pt.x);
    origin.Y = static_cast<float>(m_nYBegin + len);
    gh.DrawString(dt.Format(_T("%Y/%m/%d")).GetString(), -1, m_pFont, origin, m_pBrush);
    DrawStartEndDate(m_nYBegin + len, gh);
#endif
}

void CHorizontalAxis::DrawStartEndDate(int yEnd, Gdiplus::Graphics &gh)
{
    if(!m_vDate.empty()) {
        int i = 0;
        Gdiplus::PointF origin{
            static_cast<float>(GetXPos(i)),
            static_cast<float>(yEnd + 4)
        };
        gh.DrawString(m_vDate[0].Format(_T("%Y/%m/%d")), -1, m_pFont, origin, m_pBrush);
        i = m_vDate.size() - 1;
        origin = {
            static_cast<float>(GetXPos(i)),
            static_cast<float>(yEnd + 4)
        };
        gh.DrawString(m_vDate[m_vDate.size() - 1].Format(_T("%Y/%m/%d")), -1, m_pFont, origin, m_pBrush);
    }
}
// ���Լ�
void CHorizontalAxis::Draw(Gdiplus::Graphics& gh)
{
    // ����
    gh.DrawLine(m_pPenLine , m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd);
    // ���̶���
    int yBegin = m_nYBegin;
    int yEnd   = m_nYBegin + 3;
    for(unsigned i = 0; i < m_vDate.size(); ++i) {
        int x = GetXPos(i);
        gh.DrawLine(m_pPenLine, x, yBegin, x, yEnd);
    }
    // д��ʼ,������������
    DrawStartEndDate(yEnd, gh);
}

// ����x���꣬�������ǰ������
COleDateTime CHorizontalAxis::XPosToDate(int x) const
{
    int iWidth = GetLenOf1Day();
    int iLength = GetLenOfAxis();
    //�����˶���
    int iLeft = iLength - (iLength / iWidth) * iWidth;
    unsigned nIndex = static_cast<unsigned>((x - iLeft - m_nXBegin) / iWidth);
    if(nIndex >= 0 && nIndex < m_vDate.size()) {
        return m_vDate[nIndex];
    }
    return COleDateTime(1999, 1, 1, 0, 0, 0);
}

// �������ڣ������x����
int CHorizontalAxis::DateToXPos(const COleDateTime& date) const
{
    int nX = GetXPos(0);
    unsigned i = 0;
    for(i = 0; i < m_vDate.size(); ++i) {
        if(date <= m_vDate[i]) {
            break;
        }
    }
    return GetXPos(i);
}

// ����ÿһ�����ռ�õ�������
int CHorizontalAxis::GetLenOf1Day() const
{
    if(m_vDate.empty()) {
        return -1;
    }
    int days = m_vDate.size();
    // ������ֱ��ȡ����...
    return std::max<int>(GetLenOfAxis() / days, 1);
}

// ����������ĳ���
int CHorizontalAxis::GetLenOfAxis() const
{
    return m_nXEnd - m_nXBegin;
}

// ���� ���᱾��ռ�õ� RECT
RECT CHorizontalAxis::GetRectAxis() const
{
    RECT r{ m_nXBegin, m_nYBegin, m_nXEnd, m_nYEnd };
    return r;
}


