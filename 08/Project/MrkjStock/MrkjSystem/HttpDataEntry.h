#pragma once
namespace data_http
{
    using namespace std;
    class CHttpDataEntry
    {
    public:
        CHttpDataEntry();
        ~CHttpDataEntry();

        // 拷贝构造
        CHttpDataEntry(const CHttpDataEntry& rhs);

        // 重载赋值操作符
        CHttpDataEntry& operator=(const CHttpDataEntry& rhs);

    public:
        string name; // 股票名字
        string open; // 今日开盘价
        string close; // 昨日收盘价
        string price; // 当前价格
        string maxPrice; // 今日最高价
        string minPrice; // 今日最低价
        //string // 竟买一
        //string // 竟买二
        string dealCount; // 成交的股票数(单位:股)
        string dealMoney; // 成交的股票数(单位:元)
        string buyCount01; // 买1数量(单位:手, 1手=100股)
        string buyPrice01; // 买1报价
        string buyCount02; // 买2数量(单位:手, 1手=100股)
        string buyPrice02; // 买2报价
        string buyCount03; // 买3数量(单位:手, 1手=100股)
        string buyPrice03; // 买3报价
        string buyCount04; // 买4数量(单位:手, 1手=100股)
        string buyPrice04; // 买4报价
        string buyCount05; // 买5数量(单位:手, 1手=100股)
        string buyPrice05; // 买5报价

        string sellCount01; // 卖1数量(单位:手, 1手=100股)
        string sellPrice01; // 卖1报价
        string sellCount02; // 卖2数量(单位:手, 1手=100股)
        string sellPrice02; // 卖2报价
        string sellCount03; // 卖3数量(单位:手, 1手=100股)
        string sellPrice03; // 卖3报价
        string sellCount04; // 卖4数量(单位:手, 1手=100股)
        string sellPrice04; // 卖4报价
        string sellCount05; // 卖5数量(单位:手, 1手=100股)
        string sellPrice05; // 卖5报价

        string date; // 日期(2008-01-11)
        string time; // 时间(15:05:32)

    };
}



