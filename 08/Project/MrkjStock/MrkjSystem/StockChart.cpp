// StockChart.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StockChart.h"
using namespace std;

#define MRKJ_STOCK_CHART _T("MRKJ_STOCK_CHART")

// CStockChart

IMPLEMENT_DYNAMIC(CStockChart, CWnd)

CStockChart::CStockChart()
{
    // ע������Ĵ�����
    {
        HINSTANCE hInst = AfxGetInstanceHandle();
        WNDCLASS windowClass;
        if(!(::GetClassInfo(hInst, MRKJ_STOCK_CHART, &windowClass))) {
            memset(&windowClass, 0, sizeof(windowClass));
            windowClass.hInstance = hInst;
            windowClass.lpfnWndProc = ::DefWindowProc;
            windowClass.hCursor = nullptr;
            windowClass.hIcon = nullptr;
            windowClass.lpszMenuName = nullptr;
            windowClass.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
            windowClass.style = CS_DBLCLKS;
            windowClass.cbClsExtra = 0;
            windowClass.cbWndExtra = 0;
            windowClass.lpszClassName = MRKJ_STOCK_CHART;
            if(!AfxRegisterClass(&windowClass)) {
                AfxMessageBox(_T("���� �ؼ���ʧ��"));
            }
        }
    }
}


CStockChart::~CStockChart()
{
    // �۸���(������)
    SAFE_DELETE(m_pVerAxis);
    // ʱ����(������)
    SAFE_DELETE(m_pHorAxis);
    // ���Ͻ�� Label
    SAFE_DELETE(m_pLTLabel);
    // һ������
    SAFE_DELETE(m_pSeries);
    // ����ƶ�ʱ����������ָʾ���֣�ʮ���ߵ�
    SAFE_DELETE(m_pCursor);
}


BEGIN_MESSAGE_MAP(CStockChart, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SHOWWINDOW()
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CStockChart ��Ϣ�������

void CStockChart::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
    {
        RECT rc;
        GetClientRect(&rc);
        //����һ��bmp
        Bitmap bmp(int(rc.right), int(rc.bottom));
        //����bmp�� gh
        Graphics bmpGraphics(&bmp);
        bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
        // ��Щbmp���滭ͼ
        Graphics &gh = bmpGraphics;
        //������
        m_pVerAxis->Draw(gh);
        // ������,������
        m_pHorAxis->Draw(gh);
        // �����ϽǱ���
        m_pLTLabel->Draw(gh);
        // �� ����ͼ
        m_pSeries->Draw(gh);
        // �� �����
        m_pCursor->Draw(gh);
        // ����dc�� gdi����
        Graphics graphics(dc.GetSafeHdc());
        CachedBitmap cachedBmp(&bmp, &graphics);
        // �� bmp �����������豸����
        graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
    }
}

BOOL CStockChart::OnEraseBkgnd(CDC *pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    return CWnd::OnEraseBkgnd(pDC);
}


void CStockChart::OnSize(UINT nType, int cx, int cy)
{
    //���¸����ӿؼ���λ��,��С����Ϣ
    UpdateCtrl();
    CWnd::OnSize(nType, cx, cy);
}


void CStockChart::OnMouseMove(UINT nFlags, CPoint point)
{
    //����ƶ�ʱ,����ʮ����
    m_pCursor->OnMouseMove(point);
    m_pVerAxis->OnMouseMove(point);
    m_pHorAxis->OnMouseMove(point);
    CWnd::OnMouseMove(nFlags, point);
}



void CStockChart::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnLButtonDown(nFlags, point);
}


void CStockChart::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnLButtonUp(nFlags, point);
}


void CStockChart::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnLButtonDblClk(nFlags, point);
}


void CStockChart::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnRButtonDown(nFlags, point);
}


void CStockChart::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnRButtonUp(nFlags, point);
}


void CStockChart::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnRButtonDblClk(nFlags, point);
}


void CStockChart::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CStockChart::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

// ���¿ؼ���λ�õ���Ϣ
void CStockChart::UpdateCtrl()
{
    m_pVerAxis->UpdateCtrl();
    m_pHorAxis->UpdateCtrl();
    m_pLTLabel->UpdateCtrl();
    m_pSeries->UpdateCtrl();
    m_pCursor->UpdateCtrl();
}

void CStockChart::PreSubclassWindow()
{
    //���¸����ӿؼ���λ��,��С����Ϣ
    UpdateCtrl();
    CWnd::PreSubclassWindow();
}

void CStockChart::ReSetData(const CMRKJDatabase::TupleStockInfo &info, const VDayDataEntry &v)
{
    // �۸����ֵ
    auto it_max_price = max_element(v.begin(), v.end()
                                    , [](auto lhs, auto rhs)->bool { return lhs.GetMax() < rhs.GetMax(); });
    // �۸���Сֵ
    auto it_min_price = min_element(v.begin(), v.end()
                                    , [](auto lhs, auto rhs)->bool { return lhs.GetMin() < rhs.GetMin(); });
    // �趨����۸�����
    m_pVerAxis->SetPriceMaxMin(static_cast<float>(it_max_price->GetMax()), static_cast<float>(it_min_price->GetMin()));
    // ����Ҫ������������
    {
        vector<COleDateTime> vDate;
        std::transform(v.begin(), v.end(),
                       back_inserter(vDate),
                       [](const VDayDataEntry::value_type & obj)->COleDateTime {return obj.GetDate(); });
        m_pHorAxis->SetDateList(vDate);
    }
    // �������Ͻ���ʾ����
    {
        wchar_t szBuf[1024] = {};
        swprintf_s(szBuf, _countof(szBuf), L"%s(%s-%s)%.2f, %.2f"
                   , get<0>(info).c_str()
                   , get<1>(info).c_str()
                   , get<3>(info).c_str()
                   , it_max_price->GetMax(), it_min_price->GetMin());
        m_pLTLabel->SetText(szBuf);
    }
    // ����Ҫ����ÿ������
    m_pSeries->RemoveAllPoints();
    for each(auto obj in v) {
        m_pSeries->AddPoint(obj.GetDate()
                            , static_cast<float>(obj.GetMax())
                            , static_cast<float>(obj.GetOpen())
                            , static_cast<float>(obj.GetClose())
                            , static_cast<float>(obj.GetMin()));
    }
}

