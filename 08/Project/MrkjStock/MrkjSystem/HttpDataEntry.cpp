#include "stdafx.h"
#include "HttpDataEntry.h"


namespace data_http
{
    CHttpDataEntry::CHttpDataEntry()
    {
    }


    CHttpDataEntry::~CHttpDataEntry()
    {
    }

    // 拷贝构造
    CHttpDataEntry::CHttpDataEntry(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
            this->name = rhs.name; // 股票名字
            this->open = rhs.open; // 今日开盘价
            this->close = rhs.close; // 昨日收盘价
            this->price = rhs.price; // 当前价格
            this->maxPrice = rhs.maxPrice; // 今日最高价
            this->minPrice = rhs.minPrice; // 今日最低价
            this->dealCount = rhs.dealCount; // 成交的股票数(单位:股)
            this->dealMoney = rhs.dealMoney; // 成交的股票数(单位:元)
            this->buyCount01 = rhs.buyCount01; // 买1数量(单位:手, 1手=100股)
            this->buyPrice01 = rhs.buyPrice01; // 买1报价
            this->buyCount02 = rhs.buyCount02; // 买2数量(单位:手, 1手=100股)
            this->buyPrice02 = rhs.buyPrice02; // 买2报价
            this->buyCount03 = rhs.buyCount03; // 买3数量(单位:手, 1手=100股)
            this->buyPrice03 = rhs.buyPrice03; // 买3报价
            this->buyCount04 = rhs.buyCount04; // 买4数量(单位:手, 1手=100股)
            this->buyPrice04 = rhs.buyPrice04; // 买4报价
            this->buyCount05 = rhs.buyCount05; // 买5数量(单位:手, 1手=100股)
            this->buyPrice05 = rhs.buyPrice05; // 买5报价
            this->sellCount01 = rhs.sellCount01; // 卖1数量(单位:手, 1手=100股)
            this->sellPrice01 = rhs.sellPrice01; // 卖1报价
            this->sellCount02 = rhs.sellCount02; // 卖2数量(单位:手, 1手=100股)
            this->sellPrice02 = rhs.sellPrice02; // 卖2报价
            this->sellCount03 = rhs.sellCount03; // 卖3数量(单位:手, 1手=100股)
            this->sellPrice03 = rhs.sellPrice03; // 卖3报价
            this->sellCount04 = rhs.sellCount04; // 卖4数量(单位:手, 1手=100股)
            this->sellPrice04 = rhs.sellPrice04; // 卖4报价
            this->sellCount05 = rhs.sellCount05; // 卖5数量(单位:手, 1手=100股)
            this->sellPrice05 = rhs.sellPrice05; // 卖5报价
            this->date = rhs.date; // 日期(2008-01-11)
            this->time = rhs.time; // 时间(15:05:32)
        }
    }
    // 赋值
    CHttpDataEntry& CHttpDataEntry::operator=(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
            this->name = rhs.name; // 股票名字
            this->open = rhs.open; // 今日开盘价
            this->close = rhs.close; // 昨日收盘价
            this->price = rhs.price; // 当前价格
            this->maxPrice = rhs.maxPrice; // 今日最高价
            this->minPrice = rhs.minPrice; // 今日最低价
            this->dealCount = rhs.dealCount; // 成交的股票数(单位:股)
            this->dealMoney = rhs.dealMoney; // 成交的股票数(单位:元)
            this->buyCount01 = rhs.buyCount01; // 买1数量(单位:手, 1手=100股)
            this->buyPrice01 = rhs.buyPrice01; // 买1报价
            this->buyCount02 = rhs.buyCount02; // 买2数量(单位:手, 1手=100股)
            this->buyPrice02 = rhs.buyPrice02; // 买2报价
            this->buyCount03 = rhs.buyCount03; // 买3数量(单位:手, 1手=100股)
            this->buyPrice03 = rhs.buyPrice03; // 买3报价
            this->buyCount04 = rhs.buyCount04; // 买4数量(单位:手, 1手=100股)
            this->buyPrice04 = rhs.buyPrice04; // 买4报价
            this->buyCount05 = rhs.buyCount05; // 买5数量(单位:手, 1手=100股)
            this->buyPrice05 = rhs.buyPrice05; // 买5报价
            this->sellCount01 = rhs.sellCount01; // 卖1数量(单位:手, 1手=100股)
            this->sellPrice01 = rhs.sellPrice01; // 卖1报价
            this->sellCount02 = rhs.sellCount02; // 卖2数量(单位:手, 1手=100股)
            this->sellPrice02 = rhs.sellPrice02; // 卖2报价
            this->sellCount03 = rhs.sellCount03; // 卖3数量(单位:手, 1手=100股)
            this->sellPrice03 = rhs.sellPrice03; // 卖3报价
            this->sellCount04 = rhs.sellCount04; // 卖4数量(单位:手, 1手=100股)
            this->sellPrice04 = rhs.sellPrice04; // 卖4报价
            this->sellCount05 = rhs.sellCount05; // 卖5数量(单位:手, 1手=100股)
            this->sellPrice05 = rhs.sellPrice05; // 卖5报价
            this->date = rhs.date; // 日期(2008-01-11)
            this->time = rhs.time; // 时间(15:05:32)
        }
        return *this;
    }
}

