#pragma once



namespace data_http
{
    class CHttpHelper
    {
    public:
        CHttpHelper(void);
        ~CHttpHelper(void);
    public:
        // 全局初始化:多线程环境 应该 先在主线程中调用一次
        bool GlobalInit(long flags = CURL_GLOBAL_WIN32);
        // 初始化 easyhandle
        bool EasyInit(void);
        // 设置
        bool EasySetopt(CURLoption option, void* parameter);
        // 设置URL
        bool SetURL(char const * const szURL);
        // 查询:之前必须设置URL,调用SetURL
        bool Get(std::string& strResult, long* length = nullptr);
    public:
        // 查询
        bool Get(std::string& strURL, std::string& strResult, long* length = nullptr);

    private:
        // 清理
        void EasyCleanup(void);
        void GlobalCleanup(void);

        CURL* m_pCurl;
    };
};

