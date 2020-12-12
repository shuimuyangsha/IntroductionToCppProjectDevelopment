// StockChart.cpp : 实现文件
//

#include "stdafx.h"
#include "StockChart.h"
using namespace std;

#define MRKJ_STOCK_CHART _T("MRKJ_STOCK_CHART")

// CStockChart

IMPLEMENT_DYNAMIC(CStockChart, CWnd)

CStockChart::CStockChart()
{
    // 注册自身的窗口类
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
                AfxMessageBox(_T("创建 控件类失败"));
            }
        }
    }
}


CStockChart::~CStockChart()
{
    // 价格轴(纵坐标)
    SAFE_DELETE(m_pVerAxis);
    // 时间轴(横坐标)
    SAFE_DELETE(m_pHorAxis);
    // 左上解的 Label
    SAFE_DELETE(m_pLTLabel);
    // 一堆蜡烛
    SAFE_DELETE(m_pSeries);
    // 鼠标移动时，跟随鼠标的指示文字，十字线等
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

// CStockChart 消息处理程序

void CStockChart::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CWnd::OnPaint()
    {
        RECT rc;
        GetClientRect(&rc);
        //创建一个bmp
        Bitmap bmp(int(rc.right), int(rc.bottom));
        //创建bmp的 gh
        Graphics bmpGraphics(&bmp);
        bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
        // 在些bmp上面画图
        Graphics &gh = bmpGraphics;
        //画横轴
        m_pVerAxis->Draw(gh);
        // 画纵轴,及文字
        m_pHorAxis->Draw(gh);
        // 画左上角标题
        m_pLTLabel->Draw(gh);
        // 画 蜡烛图
        m_pSeries->Draw(gh);
        // 画 鼠标线
        m_pCursor->Draw(gh);
        // 创建dc的 gdi对象
        Graphics graphics(dc.GetSafeHdc());
        CachedBitmap cachedBmp(&bmp, &graphics);
        // 将 bmp 画到真正的设备上面
        graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
    }
}

BOOL CStockChart::OnEraseBkgnd(CDC *pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return CWnd::OnEraseBkgnd(pDC);
}


void CStockChart::OnSize(UINT nType, int cx, int cy)
{
    //更新各个子控件的位置,大小等信息
    UpdateCtrl();
    CWnd::OnSize(nType, cx, cy);
}


void CStockChart::OnMouseMove(UINT nFlags, CPoint point)
{
    //鼠标移动时,绘制十字线
    m_pCursor->OnMouseMove(point);
    m_pVerAxis->OnMouseMove(point);
    m_pHorAxis->OnMouseMove(point);
    CWnd::OnMouseMove(nFlags, point);
}



void CStockChart::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnLButtonDown(nFlags, point);
}


void CStockChart::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnLButtonUp(nFlags, point);
}


void CStockChart::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnLButtonDblClk(nFlags, point);
}


void CStockChart::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnRButtonDown(nFlags, point);
}


void CStockChart::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnRButtonUp(nFlags, point);
}


void CStockChart::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnRButtonDblClk(nFlags, point);
}


void CStockChart::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CStockChart::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 更新控件的位置等信息
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
    //更新各个子控件的位置,大小等信息
    UpdateCtrl();
    CWnd::PreSubclassWindow();
}

void CStockChart::ReSetData(const CMRKJDatabase::TupleStockInfo &info, const VDayDataEntry &v)
{
    // 价格最大值
    auto it_max_price = max_element(v.begin(), v.end()
                                    , [](auto lhs, auto rhs)->bool { return lhs.GetMax() < rhs.GetMax(); });
    // 价格最小值
    auto it_min_price = min_element(v.begin(), v.end()
                                    , [](auto lhs, auto rhs)->bool { return lhs.GetMin() < rhs.GetMin(); });
    // 设定纵轴价格区间
    m_pVerAxis->SetPriceMaxMin(static_cast<float>(it_max_price->GetMax()), static_cast<float>(it_min_price->GetMin()));
    // 横轴要设置日期序列
    {
        vector<COleDateTime> vDate;
        std::transform(v.begin(), v.end(),
                       back_inserter(vDate),
                       [](const VDayDataEntry::value_type & obj)->COleDateTime {return obj.GetDate(); });
        m_pHorAxis->SetDateList(vDate);
    }
    // 设置左上角显示文字
    {
        wchar_t szBuf[1024] = {};
        swprintf_s(szBuf, _countof(szBuf), L"%s(%s-%s)%.2f, %.2f"
                   , get<0>(info).c_str()
                   , get<1>(info).c_str()
                   , get<3>(info).c_str()
                   , it_max_price->GetMax(), it_min_price->GetMin());
        m_pLTLabel->SetText(szBuf);
    }
    // 增加要画的每日数据
    m_pSeries->RemoveAllPoints();
    for each(auto obj in v) {
        m_pSeries->AddPoint(obj.GetDate()
                            , static_cast<float>(obj.GetMax())
                            , static_cast<float>(obj.GetOpen())
                            , static_cast<float>(obj.GetClose())
                            , static_cast<float>(obj.GetMin()));
    }
}

