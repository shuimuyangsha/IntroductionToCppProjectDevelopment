#include "stdafx.h"
#include "StockChart.h"
#include "CursorTip.h"

CCursorTip::CCursorTip(CStockChart* pParent)
    : m_pParent(pParent)
{
    UpdateCtrl();
}


CCursorTip::~CCursorTip()
{
    SAFE_DELETE(m_pPen);
}


// 更新位置信息．大小等等
void CCursorTip::UpdateCtrl()
{
}

// 更新自己的相关,并重绘自己?
void CCursorTip::OnMouseMove(CPoint point)
{
    // 记录当前位置
    m_x = point.x;
    m_y = point.y;
    {
        CClientDC aDc(m_pParent);
        auto pVer = m_pParent->GetVerticalAxis();
        auto pHor = m_pParent->GetHorizonTalAxis();
        //画线 横线
        POINT pt = { m_x, m_y };
        RECT rcHor = pHor->GetRectAxis();
        RECT rcVer = pVer->GetRectAxis();
        // 重绘旧图形(擦除上一次画的图型)
        aDc.SetROP2(R2_NOTXORPEN);
        if(m_ptLast.x < 0 || m_ptLast.y < 0) {
            // 没有旧图形什么也不做,
        }
        else {
            // 横线
            aDc.MoveTo(rcHor.left, m_ptLast.y);
            aDc.LineTo(rcHor.right, m_ptLast.y);
            //坚线
            aDc.MoveTo(m_ptLast.x, rcVer.top);
            aDc.LineTo(m_ptLast.x, rcVer.bottom);
        }
        //绘制本次的
        {
            //限制边界: 不要画到坐标轴外面去
            pt.x = std::min<int>(pt.x, pVer->GetRectAxis().right);
            pt.y = std::min<int>(pt.y, pHor->GetRectAxis().bottom);
            {
                // 横线
                aDc.MoveTo(rcHor.left, pt.y);
                aDc.LineTo(rcHor.right, pt.y);
                //坚线
                aDc.MoveTo(pt.x, rcVer.top);
                aDc.LineTo(pt.x, rcVer.bottom);
            }
            //保存本次的位置
            m_ptLast = pt;
        }
    }
    // 绘制日期
}

// 画自己呀
void CCursorTip::Draw(Gdiplus::Graphics& gh)
{
    //重置上次的位置
    m_ptLast.x = -1;
    m_ptLast.y = -1;
}



