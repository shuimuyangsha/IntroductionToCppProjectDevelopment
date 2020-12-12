#pragma once

class CDayDataEntry
{
public:
    CDayDataEntry();
    virtual ~CDayDataEntry();
    CDayDataEntry(const CDayDataEntry& rhs);
    CDayDataEntry& operator= (const CDayDataEntry& rhs);

    std::wstring code;               // ��Ʊ����
    std::wstring date;               // ����
    std::wstring open;               // ����
    std::wstring max;                // ���
    std::wstring min;                // ���
    std::wstring close;              // ����
    std::wstring turnover;           // �ɽ���
    std::wstring tradingVolume;      // �ɽ���

    COleDateTime GetDate() const;
    double GetOpen() const;          // ����
    double GetMax() const;           // ���
    double GetMin() const;           // ���
    double GetClose() const;         // ����
    double GetTurnover() const;      // �ɽ���
    double GetTradingVolume() const; // �ɽ���
};

typedef std::vector<CDayDataEntry> VDayDataEntry;
