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


// ����λ����Ϣ����С�ȵ�
void CCursorTip::UpdateCtrl()
{
}

// �����Լ������,���ػ��Լ�?
void CCursorTip::OnMouseMove(CPoint point)
{
    // ��¼��ǰλ��
    m_x = point.x;
    m_y = point.y;
    {
        CClientDC aDc(m_pParent);
        auto pVer = m_pParent->GetVerticalAxis();
        auto pHor = m_pParent->GetHorizonTalAxis();
        //���� ����
        POINT pt = { m_x, m_y };
        RECT rcHor = pHor->GetRectAxis();
        RECT rcVer = pVer->GetRectAxis();
        // �ػ��ͼ��(������һ�λ���ͼ��)
        aDc.SetROP2(R2_NOTXORPEN);
        if(m_ptLast.x < 0 || m_ptLast.y < 0) {
            // û�о�ͼ��ʲôҲ����,
        }
        else {
            // ����
            aDc.MoveTo(rcHor.left, m_ptLast.y);
            aDc.LineTo(rcHor.right, m_ptLast.y);
            //����
            aDc.MoveTo(m_ptLast.x, rcVer.top);
            aDc.LineTo(m_ptLast.x, rcVer.bottom);
        }
        //���Ʊ��ε�
        {
            //���Ʊ߽�: ��Ҫ��������������ȥ
            pt.x = std::min<int>(pt.x, pVer->GetRectAxis().right);
            pt.y = std::min<int>(pt.y, pHor->GetRectAxis().bottom);
            {
                // ����
                aDc.MoveTo(rcHor.left, pt.y);
                aDc.LineTo(rcHor.right, pt.y);
                //����
                aDc.MoveTo(pt.x, rcVer.top);
                aDc.LineTo(pt.x, rcVer.bottom);
            }
            //���汾�ε�λ��
            m_ptLast = pt;
        }
    }
    // ��������
}

// ���Լ�ѽ
void CCursorTip::Draw(Gdiplus::Graphics& gh)
{
    //�����ϴε�λ��
    m_ptLast.x = -1;
    m_ptLast.y = -1;
}



