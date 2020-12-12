#pragma once



class CCandlestickPoint
{
public:
    CCandlestickPoint() {}
    /*
        ���캯��:���� ����,��߼�,���̼�,���̼�,��ͼ�
    */
    CCandlestickPoint(const COleDateTime& time, float high, float open, float close, float low)
        : m_time(time)
        , m_low(low)
        , m_high(high)
        , m_open(open)
        , m_close(close)
    { }
    // �������캯��
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

    // ���ظ�ֵ����
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
    COleDateTime m_time; //����
    float m_high; // ��߼�
    float m_open; // ���̼�
    float m_close; // ���̼�
    float m_low; // ��ͼ�
};

typedef std::vector<CCandlestickPoint> VPoints;

// ǰ������
class CStockChart;

class CCandlestickSerie
{
public:
    CCandlestickSerie(CStockChart* pParent);
    ~CCandlestickSerie();

    // ����λ����Ϣ����С�ȵ�
    void UpdateCtrl();

    // ���Լ�
    void Draw(Gdiplus::Graphics& gh);

    // ɾ�����е�����
    void RemoveAllPoints()
    {
        m_vPoints.clear();
        m_minIndex = -1;
        m_maxIndex = -1;
    }

    // ��������
    void AddPoint(const COleDateTime& date, float high, float open, float close, float low);
private:
    // ��һ�յ�����
    void DrawPoint(Gdiplus::Graphics& gh, size_t index);
    //�����߼�,��ͼ�����
    void DrawHighLowText(Graphics& gh);
    // ��ȡ ���� ռ�õĿռ�
    void GetCancleRect(CRect& rcBody, CRect& rcLine, size_t index) const;
    CStockChart *m_pParent; //���ؼ�
    VPoints m_vPoints; // �潲���е�����
    int m_minIndex{ -1 }; //��С���� �±�
    int m_maxIndex{ -1 }; //��С���� �±�

    // �ǵ�ʱ ��ɫ��ͬ:
    // ��
    SolidBrush * m_pBrushUp{ new SolidBrush(Gdiplus::Color(0xFF, 0xFF, 0x32, 0x32)) };
    Pen * m_pPenUp{ new Pen(Gdiplus::Color(0xFF, 0xFF, 0x32, 0x32)) };
    // ��
    SolidBrush * m_pBrushDown{ new SolidBrush(Gdiplus::Color(0xFF, 0x54, 0xFF, 0xFF)) };
    Pen * m_pPenDown{ new Pen(Gdiplus::Color(0xFF, 0x54, 0xFF, 0xFF)) };

    // ��ʾ ���ֵ,��Сֵ�����ּ���ɫ
    Gdiplus::Font * m_pFontMaxMinLabel {new  Gdiplus::Font(_T("����"), 8.0f)};
    Pen * m_pPenMaxMinLabel{ new Pen(Gdiplus::Color(0xFF, 0xFF, 0xFF, 0xFF))};
    SolidBrush * m_pBrushMaxMinLabel{ new SolidBrush(Gdiplus::Color(0xFF, 0xFF, 0xFF, 0xFF)) };
};
