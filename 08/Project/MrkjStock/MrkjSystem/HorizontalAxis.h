#pragma once

/*
    横轴，日期
*/

// 前向声明
class CStockChart;

class CHorizontalAxis
{
public:
    CHorizontalAxis(CStockChart* pParent);
    ~CHorizontalAxis();

    // 更新位置信息．大小等等,主要用于父窗口变大之时
    void UpdateCtrl();

    // 当鼠标移动
    void OnMouseMove(CPoint pt);

    // 设置横轴日期(列表)
    void SetDateList(const vector<COleDateTime>& vDate);

    // 根据x坐标，计算出当前的日期
    COleDateTime XPosToDate(int x) const;

    // 根据日期，计算出x坐标
    int DateToXPos(const COleDateTime& date) const;

    // 画自己
    void Draw(Gdiplus::Graphics& gh);

    void DrawStartEndDate(int yEnd, Gdiplus::Graphics &gh);

    // 计算每一天可以占用的像素数
    int GetLenOf1Day() const;

    // 本得 横轴本身占用的 RECT
    RECT GetRectAxis() const;

    // 计算坐标轴的长度
    int GetLenOfAxis() const;

private:
    // 计算第 n个的x坐标
    int GetXPos(int iIndex/*[0,)*/) const;

    // 计算第 dt 日的 x坐标
    int GetXPos(const COleDateTime& dt) const;

    // 父控件
    CStockChart* m_pParent;

    //　时间范围
    COleDateTime m_minDt{ 1999, 1, 1, 0, 0, 0 }, m_maxDt{ 2016, 12, 31, 0, 0, 0 };

    // 线的颜色
    Gdiplus::Color m_clrLine { Gdiplus::Color(0xFF, 0xFF, 0x00, 0x00) };

    // 标签文字
    Gdiplus::Font *m_pFont {new Gdiplus::Font(_T("宋体"), 8.0f)};
    Gdiplus::SolidBrush *m_pBrush{ new Gdiplus::SolidBrush(m_clrLine)};

    Pen * m_pPenLine{new Pen(m_clrLine)};


    // 起始位置
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };

    // 结束位置
    int m_nXEnd{ 0 };
    int m_nYEnd{ 0 };

    // 横线距离底部的距离
    int m_nOffsetBottom{ 20 };

    // 横线距离右侧的距离
    int m_nOffsetRight{ 40 };

    // 存讲坐标轴上的日期
    vector<COleDateTime> m_vDate;

    // 设置最大日期,最小日期
    void SetTimeMaxMin();
};

