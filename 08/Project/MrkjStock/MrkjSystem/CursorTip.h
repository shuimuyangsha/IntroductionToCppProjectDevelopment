#pragma once

class CStockChart;

class CCursorTip
{
public:
    CCursorTip(CStockChart* pParent);
    ~CCursorTip();

    // 更新位置信息．大小等等
    void UpdateCtrl();

    // 画自己
    void Draw(Gdiplus::Graphics& gh);

    // 当鼠标移动时,调用此函数; 付责重绘自己
    void OnMouseMove(CPoint pt);
private:
    // 父控件
    CStockChart* m_pParent;
    // 鼠标当前位置
    int m_x;
    int m_y;
    // 白色画笔
    Pen *m_pPen{ new Pen(Color(0xFF, 0xFF, 0xFF, 0xFF))};
    // 最后一次的位置
    POINT m_ptLast{ -1, -1};
};

