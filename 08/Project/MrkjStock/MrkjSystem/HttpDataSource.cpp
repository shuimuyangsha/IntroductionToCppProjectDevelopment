#include "StdAfx.h"
#include "StringHelper.h"

#include "HttpDataSource.h"
#include "HttpHelper.h"
#include "HttpDataEntry.h"

#include <regex>

namespace data_http
{

    // 查询 实时数据 URL
    const std::string CHttpDataSource::m_strQueryURL = "http://hq.sinajs.cn/list="; //+sh601616

    // 查询 分钟线 K线图 URL
    const std::string CHttpDataSource::m_strQueryURLGif = "http://image.sinajs.cn/newchart/min/n/"; // +sh601006.gif

    CHttpDataSource::CHttpDataSource(void)
        : m_http(nullptr)
    {
        Init();
    }


    CHttpDataSource::~CHttpDataSource(void)
    {
        Release();
    }

    // 初始化数据库
    bool CHttpDataSource::Init(void)
    {
        try {
            // http访问工具初始化
            m_http = new CHttpHelper;
            return true;
        }
        catch(const std::bad_alloc& e) {
            TRACE("%s", e.what());
        }
        catch(const std::exception& e) {
            TRACE("%s", e.what());
        }
        catch(...) {
            TRACE("发生未知异常");
        }
        return false;
    }

    // 清理资源
    void CHttpDataSource::Release(void)
    {
        SAFE_DELETE(m_http);
    }

    // 查询股票实时数据
    bool CHttpDataSource::QueryStockRealTimeData(const std::string& stockCode, CHttpDataEntry& data)
    {
        using namespace std;
        //1.获取网页信息
        // 拼接成完整的 URL: http://hq.sinajs.cn/list=stockCode   stockCode=sh601006
        std::string strURL = m_strQueryURL;
        strURL.append(stockCode);
        // 获取数据
        std::string strContent;
        if(!m_http->Get(strURL, strContent)) {
            return false;
        }
        //2.拆分获取,内容
        /* 结果内容: var hq_str_s_sh601006="大秦铁路,6.810,...,2016-03-09,10:27:36,00";
           使用正则表达式进行拆分
           v[0] = "var hq_str_s_sh601006"
           v[1] = "大秦铁路"
           v[2] = "6.810"
           ...
           v[33] = "00" */
        regex reg("=|,|;|\""); // 以'=' or ',' or '"' or ';' 作为分割符进行拆分
        sregex_token_iterator it(strContent.begin(), strContent.end(), reg, -1);
        sregex_token_iterator end;
        std::vector<std::string> v;
        while(it != end) {
            //提取不为空的项,存储
            if(*it != "") {
                v.push_back(*it);
            }
            ++it;
        }
        if(v.size() < 33) {
            return false;
        }
        //3.给返回的结构体赋值
        data.name = v[1]; // 股票名字
        data.open = v[2]; // 今日开盘价
        data.close = v[3]; // 昨日收盘价
        data.price = v[4]; // 当前价格
        data.maxPrice = v[5]; // 今日最高价
        data.minPrice = v[6]; // 今日最低价
        //data.= v[7]; // 竟买一数量j
        //data.= v[8]; // 竟卖一报价
        data.dealCount = v[9]; // 成交的股票数(单位:股)
        data.dealMoney = v[10]; // 成交的股票数(单位:元)
        data.buyCount01 = v[11]; // 买1数量(单位:手, 1手=100股)
        data.buyPrice01 = v[12]; // 买1报价
        data.buyCount02 = v[13]; // 买2数量(单位:手, 1手=100股)
        data.buyPrice02 = v[14]; // 买2报价
        data.buyCount03 = v[15]; // 买3数量(单位:手, 1手=100股)
        data.buyPrice03 = v[16]; // 买3报价
        data.buyCount04 = v[17]; // 买4数量(单位:手, 1手=100股)
        data.buyPrice04 = v[18]; // 买4报价
        data.buyCount05 = v[19]; // 买5数量(单位:手, 1手=100股)
        data.buyPrice05 = v[20]; // 买5报价
        data.sellCount01 = v[21]; // 卖1数量(单位:手, 1手=100股)
        data.sellPrice01 = v[22]; // 卖1报价
        data.sellCount02 = v[23]; // 卖2数量(单位:手, 1手=100股)
        data.sellPrice02 = v[24]; // 卖2报价
        data.sellCount03 = v[25]; // 卖3数量(单位:手, 1手=100股)
        data.sellPrice03 = v[26]; // 卖3报价
        data.sellCount04 = v[27]; // 卖4数量(单位:手, 1手=100股)
        data.sellPrice04 = v[28]; // 卖4报价
        data.sellCount05 = v[29]; // 卖5数量(单位:手, 1手=100股)
        data.sellPrice05 = v[30]; // 卖5报价
        data.date = v[31]; // 日期(2008-01-11)
        data.time = v[32]; // 时间(15:05:32)
        // TODO: 检查数据格式是否合法
        return true;
    }

    // 下载股票分钟K线图
    bool CHttpDataSource::QueryStockKLineMin(const std::string& stockCode, const std::string& filename)
    {
        using namespace std;
        //1.获取网页信息
        // 拼接成完整的 URL:http://image.sinajs.cn/newchart/min/n/+stockCode+.gif
        std::string strURL = m_strQueryURLGif;
        strURL.append(stockCode);
        strURL.append(".gif");
        // 获取数据
        std::string content;
        if(!m_http->Get(strURL, content, nullptr)) {
            return false;
        }
        // 2.保存文件内容到filename指定位置
        try {
            // 如果文件存存,则删除文件重建
            ofstream file(filename, ios::out | ios::binary);
            if(file) {
                file.write(content.c_str(), content.size());
                file.close();
            }
        }
        catch(const std::exception& e) {
            TRACE("%s", e.what());
        }
        catch(...) {
            TRACE("发生未知异常");
        }
        return true;
    }
}
