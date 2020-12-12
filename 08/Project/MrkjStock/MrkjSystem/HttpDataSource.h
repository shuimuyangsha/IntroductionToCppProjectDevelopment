#pragma once
#include "HttpDataEntry.h"

namespace data_http
{
    class CHttpHelper;

    class CHttpDataSource
    {
    public:
        CHttpDataSource(void);
        ~CHttpDataSource(void);
    private:
        // 初始化
        bool Init(void);

        // 清理资源
        void Release(void);

        // Http查询辅助
        CHttpHelper* m_http;

        // 查询实时信息  URL, sina
        static const std::string m_strQueryURL;
        static const std::string m_strQueryURLGif;
    public:
        // 查询股票实时数据
        bool QueryStockRealTimeData(const std::string& stockCode, CHttpDataEntry& data);
        // 下载股票分钟K线图
        bool QueryStockKLineMin(const std::string& stockCode, const std::string& filename);
    };
}
