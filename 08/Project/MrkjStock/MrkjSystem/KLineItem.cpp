#include "stdafx.h"
#include "KLineItem.h"


CKLineItem::CKLineItem(float rate, const std::wstring& date, const std::wstring& open, const std::wstring& close, const std::wstring& max, const std::wstring& min)
    : m_rate(rate), m_date(date), m_open(open), m_close(close), m_max(max), m_min(min)
{
}

CKLineItem::~CKLineItem()
{
}

void CKLineItem::Draw(Graphics& gh, int x, int y)
{
}

