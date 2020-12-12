#pragma once
class CKLineItem
{
public:
    CKLineItem(float rate, const std::wstring& date, const std::wstring& open, const std::wstring& close, const std::wstring& max, const std::wstring& min);
    ~CKLineItem();
    void Draw(Graphics& gh, int x, int y);
private:
    float m_rate;
    std::wstring m_date;
    std::wstring m_open;
    std::wstring m_close;
    std::wstring m_max;
    std::wstring m_min;
};

