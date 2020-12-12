#pragma once

class CStockChart;

class CCursorTip
{
public:
    CCursorTip(CStockChart* pParent);
    ~CCursorTip();

    // ����λ����Ϣ����С�ȵ�
    void UpdateCtrl();

    // ���Լ�
    void Draw(Gdiplus::Graphics& gh);

    // ������ƶ�ʱ,���ô˺���; �����ػ��Լ�
    void OnMouseMove(CPoint pt);
private:
    // ���ؼ�
    CStockChart* m_pParent;
    // ��굱ǰλ��
    int m_x;
    int m_y;
    // ��ɫ����
    Pen *m_pPen{ new Pen(Color(0xFF, 0xFF, 0xFF, 0xFF))};
    // ���һ�ε�λ��
    POINT m_ptLast{ -1, -1};
};

