#include "stdafx.h"
#include "DayDataEntry.h"


CDayDataEntry::CDayDataEntry()
{
}


CDayDataEntry::~CDayDataEntry()
{
}

CDayDataEntry::CDayDataEntry(const CDayDataEntry& rhs)
{
    if(this != &rhs) {
        this->code = rhs.code;                      // ��Ʊ����
        this->date = rhs.date;                      // ����
        this->open = rhs.open;                      // ����
        this->max = rhs.max;                        // ���
        this->min = rhs.min;                        // ���
        this->close = rhs.close;                    // ����
        this->turnover = rhs.turnover;              // �ɽ���
        this->tradingVolume = rhs.tradingVolume;    // �ɽ���
    }
}

CDayDataEntry& CDayDataEntry::operator= (const CDayDataEntry& rhs)
{
    if(this != &rhs) {
        this->code = rhs.code;                      // ��Ʊ����
        this->date = rhs.date;                      // ����
        this->open = rhs.open;                      // ����
        this->max = rhs.max;                        // ���
        this->min = rhs.min;                        // ���
        this->close = rhs.close;                    // ����
        this->turnover = rhs.turnover;              // �ɽ���
        this->tradingVolume = rhs.tradingVolume;    // �ɽ���
    }
    return *this;
}

static double ToFloat(const std::wstring& strVal)
{
    using namespace std;
    wstringstream s(strVal);
    double d;
    s >> d;
    return d;
}
static int ToInt(const std::wstring& strVal)
{
    using namespace std;
    wstringstream s(strVal);
    int d;
    s >> d;
    return d;
}

COleDateTime ToDateTime(const std::wstring& strVal)
{
    std::vector<std::wstring> v;
    StringHelper::SplitString(v, strVal, L"/");
    //assert(v.size() == 3);
    return COleDateTime(ToInt(v[0]), ToInt(v[1]), ToInt(v[2]), 0, 0, 0);
}

COleDateTime CDayDataEntry::GetDate() const
{
    return ToDateTime(this->date);
}

double CDayDataEntry::GetOpen() const           // ����
{
    return ToFloat(this->open);
}
double CDayDataEntry::GetMax() const            // ���
{
    return ToFloat(this->max);
}
double CDayDataEntry::GetMin() const            // ���
{
    return ToFloat(this->min);
}
double CDayDataEntry::GetClose() const          // ����
{
    return ToFloat(this->close);
}
double CDayDataEntry::GetTurnover() const       // �ɽ���
{
    return ToFloat(this->turnover);
}
double CDayDataEntry::GetTradingVolume() const  // �ɽ���
{
    return ToFloat(this->tradingVolume);
}







