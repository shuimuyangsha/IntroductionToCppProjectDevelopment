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
        this->code = rhs.code;                      // 股票代码
        this->date = rhs.date;                      // 日期
        this->open = rhs.open;                      // 开盘
        this->max = rhs.max;                        // 最高
        this->min = rhs.min;                        // 最低
        this->close = rhs.close;                    // 收盘
        this->turnover = rhs.turnover;              // 成交量
        this->tradingVolume = rhs.tradingVolume;    // 成交额
    }
}

CDayDataEntry& CDayDataEntry::operator= (const CDayDataEntry& rhs)
{
    if(this != &rhs) {
        this->code = rhs.code;                      // 股票代码
        this->date = rhs.date;                      // 日期
        this->open = rhs.open;                      // 开盘
        this->max = rhs.max;                        // 最高
        this->min = rhs.min;                        // 最低
        this->close = rhs.close;                    // 收盘
        this->turnover = rhs.turnover;              // 成交量
        this->tradingVolume = rhs.tradingVolume;    // 成交额
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

double CDayDataEntry::GetOpen() const           // 开盘
{
    return ToFloat(this->open);
}
double CDayDataEntry::GetMax() const            // 最高
{
    return ToFloat(this->max);
}
double CDayDataEntry::GetMin() const            // 最低
{
    return ToFloat(this->min);
}
double CDayDataEntry::GetClose() const          // 收盘
{
    return ToFloat(this->close);
}
double CDayDataEntry::GetTurnover() const       // 成交量
{
    return ToFloat(this->turnover);
}
double CDayDataEntry::GetTradingVolume() const  // 成交额
{
    return ToFloat(this->tradingVolume);
}







