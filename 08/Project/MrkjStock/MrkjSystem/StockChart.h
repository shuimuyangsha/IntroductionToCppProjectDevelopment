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
    /* �����������ĵ���Ϣ */
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
        �������ñ��ؼ�,���ݴ��������
        1.�������(ʱ��)
        2.��������(�۸�)
        3.���Ͻ�����
    */
    void ReSetData(const CMRKJDatabase::TupleStockInfo& info, const VDayDataEntry& v);

    // �۸���(������)
    const CVerticalAxis * GetVerticalAxis() const
    {
        return m_pVerAxis;
    }
    // ʱ����(������)
    const CHorizontalAxis * GetHorizonTalAxis() const
    {
        return m_pHorAxis;
    };

    // ���¿ؼ���λ�õ���Ϣ
    void UpdateCtrl();
private:
    // �۸���(������)
    CVerticalAxis *m_pVerAxis{ new CVerticalAxis(this) };
    // ʱ����(������)
    CHorizontalAxis *m_pHorAxis{ new CHorizontalAxis(this) };
    // ���Ͻ�� Label
    CLeftTopLabel *m_pLTLabel{ new CLeftTopLabel(this) };
    // һ������
    CCandlestickSerie *m_pSeries{ new CCandlestickSerie(this) };
    // ����ƶ�ʱ����������ָʾ���֣�ʮ���ߵ�
    CCursorTip *m_pCursor{ new CCursorTip(this) };

    // Ϊ�˸��¿ؼ��Ĵ�С����Ϣ
    virtual void PreSubclassWindow();
};



