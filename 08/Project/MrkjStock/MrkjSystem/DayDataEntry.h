#pragma once

class CDayDataEntry
{
public:
    CDayDataEntry();
    virtual ~CDayDataEntry();
    CDayDataEntry(const CDayDataEntry& rhs);
    CDayDataEntry& operator= (const CDayDataEntry& rhs);

    std::wstring code;               // 股票代码
    std::wstring date;               // 日期
    std::wstring open;               // 开盘
    std::wstring max;                // 最高
    std::wstring min;                // 最低
    std::wstring close;              // 收盘
    std::wstring turnover;           // 成交量
    std::wstring tradingVolume;      // 成交额

    COleDateTime GetDate() const;
    double GetOpen() const;          // 开盘
    double GetMax() const;           // 最高
    double GetMin() const;           // 最低
    double GetClose() const;         // 收盘
    double GetTurnover() const;      // 成交量
    double GetTradingVolume() const; // 成交额
};

typedef std::vector<CDayDataEntry> VDayDataEntry;
