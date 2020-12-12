#pragma once

#include <afxwin.h>

#include "VerticalAxis.h"
#include "HorizontalAxis.h"
#include "LeftTopLabel.h"
#include "CandlestickSerie.h"
#include "CursorTip.h"

// CStockChart

using namespace std;
class CStockChart : public CWnd
{
    DECLARE_DYNAMIC(CStockChart)

public:

    CStockChart();
    virtual ~CStockChart();

protected:
    DECLARE_MESSAGE_MAP()
protected:
    /* 本进程所关心的消息 */
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
    /*
        std::vector<>
        重新设置本控件,根据传入的数据
        1.计算横轴(时间)
        2.计算纵轴(价格)
        3.右上角文字
    */
    void ReSetData(const CMRKJDatabase::TupleStockInfo& info, const VDayDataEntry& v);

    // 价格轴(纵坐标)
    const CVerticalAxis * GetVerticalAxis() const
    {
        return m_pVerAxis;
    }
    // 时间轴(横坐标)
    const CHorizontalAxis * GetHorizonTalAxis() const
    {
        return m_pHorAxis;
    };

    // 更新控件的位置等信息
    void UpdateCtrl();
private:
    // 价格轴(纵坐标)
    CVerticalAxis *m_pVerAxis{ new CVerticalAxis(this) };
    // 时间轴(横坐标)
    CHorizontalAxis *m_pHorAxis{ new CHorizontalAxis(this) };
    // 左上解的 Label
    CLeftTopLabel *m_pLTLabel{ new CLeftTopLabel(this) };
    // 一堆蜡烛
    CCandlestickSerie *m_pSeries{ new CCandlestickSerie(this) };
    // 鼠标移动时，跟随鼠标的指示文字，十字线等
    CCursorTip *m_pCursor{ new CCursorTip(this) };

    // 为了更新控件的大小等信息
    virtual void PreSubclassWindow();
};



