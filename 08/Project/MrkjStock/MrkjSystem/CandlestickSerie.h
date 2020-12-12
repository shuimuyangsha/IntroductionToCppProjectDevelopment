#pragma once



class CCandlestickPoint
{
public:
    CCandlestickPoint() {}
    /*
        构造函数:传入 日期,最高价,开盘价,收盘价,最低价
    */
    CCandlestickPoint(const COleDateTime& time, float high, float open, float close, float low)
        : m_time(time)
        , m_low(low)
        , m_high(high)
        , m_open(open)
        , m_close(close)
    { }
    // 拷贝构造函数
    CCandlestickPoint(const CCandlestickPoint& rhs)
    {
        if(this == &rhs) {
            return;
        }
        m_time = rhs.m_time;
        m_high = rhs.m_high;
        m_open = rhs.m_open;
        m_close = rhs.m_close;
        m_low = rhs.m_low;
    }

    // 重载赋值函数
    CCandlestickPoint& operator=(const CCandlestickPoint& rhs)
    {
        if(this != &rhs) {
            m_time = rhs.m_time;
            m_high = rhs.m_high;
            m_open = rhs.m_open;
            m_close = rhs.m_close;
            m_low = rhs.m_low;
        }
        return *this;
    }

public:
    COleDateTime m_time; //日期
    float m_high; // 最高价
    float m_open; // 开盘价
    float m_close; // 收盘价
    float m_low; // 最低价
};

typedef std::vector<CCandlestickPoint> VPoints;

// 前向声明
class CStockChart;

class CCandlestickSerie
{
public:
    CCandlestickSerie(CStockChart* pParent);
    ~CCandlestickSerie();

    // 更新位置信息．大小等等
    void UpdateCtrl();

    // 画自己
    void Draw(Gdiplus::Graphics& gh);

    // 删除所有的数据
    void RemoveAllPoints()
    {
        m_vPoints.clear();
        m_minIndex = -1;
        m_maxIndex = -1;
    }

    // 增加数据
    void AddPoint(const COleDateTime& date, float high, float open, float close, float low);
private:
    // 画一日的数据
    void DrawPoint(Gdiplus::Graphics& gh, size_t index);
    //标记最高价,最低价文字
    void DrawHighLowText(Graphics& gh);
    // 获取 整个 占用的空间
    void GetCancleRect(CRect& rcBody, CRect& rcLine, size_t index) const;
    CStockChart *m_pParent; //父控件
    VPoints m_vPoints; // 存讲所有的数据
    int m_minIndex{ -1 }; //最小数据 下标
    int m_maxIndex{ -1 }; //最小数据 下标

    // 涨跌时 颜色不同:
    // 涨
    SolidBrush * m_pBrushUp{ new SolidBrush(Gdiplus::Color(0xFF, 0xFF, 0x32, 0x32)) };
    Pen * m_pPenUp{ new Pen(Gdiplus::Color(0xFF, 0xFF, 0x32, 0x32)) };
    // 跌
    SolidBrush * m_pBrushDown{ new SolidBrush(Gdiplus::Color(0xFF, 0x54, 0xFF, 0xFF)) };
    Pen * m_pPenDown{ new Pen(Gdiplus::Color(0xFF, 0x54, 0xFF, 0xFF)) };

    // 标示 最大值,最小值的文字及颜色
    Gdiplus::Font * m_pFontMaxMinLabel {new  Gdiplus::Font(_T("宋体"), 8.0f)};
    Pen * m_pPenMaxMinLabel{ new Pen(Gdiplus::Color(0xFF, 0xFF, 0xFF, 0xFF))};
    SolidBrush * m_pBrushMaxMinLabel{ new SolidBrush(Gdiplus::Color(0xFF, 0xFF, 0xFF, 0xFF)) };
};
