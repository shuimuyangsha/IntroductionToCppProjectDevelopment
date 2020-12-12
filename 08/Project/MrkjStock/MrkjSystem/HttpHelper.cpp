#include "StdAfx.h"
#include "HttpHelper.h"
#include  "StringHelper.h"

namespace data_http
{
    using namespace std;

    //libcurl �ص�����
    static size_t LibCurlCallback(void *buffer, size_t size, size_t nmemb, void *userp)
    {
        long sizes = size * nmemb;
        std::string temp((char*)buffer, sizes);
        *(std::string*)userp += temp;
        return sizes;
    }

    CHttpHelper::CHttpHelper(void)
        : m_pCurl(NULL)
    {
        try {
            if(!GlobalInit() || !EasyInit() || !EasySetopt(::CURLOPT_WRITEFUNCTION, &LibCurlCallback)) {
                throw "��ʼ��ʧ��";
            }
        }
        catch(const std::exception& e) {
            TRACE("%s", e.what());
        }
        catch(...) {
            TRACE("����δ֪�쳣");
        }
    }

    CHttpHelper::~CHttpHelper(void)
    {
        EasyCleanup();
        GlobalCleanup();
    }

    // ȫ�ֳ�ʼ��
    bool CHttpHelper::GlobalInit(long flags/*=CURL_GLOBAL_WIN32*/)
    {
        CURLcode cc = ::curl_global_init(CURL_GLOBAL_WIN32);
        return CURLE_OK == cc;
    }

    // ��ʼ��
    bool CHttpHelper::EasyInit(void)
    {
        m_pCurl = ::curl_easy_init();
        return NULL != m_pCurl;
    }

    // ����
    bool CHttpHelper::EasySetopt(CURLoption option, void* parameter)
    {
        CURLcode cc = ::curl_easy_setopt(m_pCurl, option, parameter);
        return CURLE_OK == cc;
    }

    // ����URL
    bool CHttpHelper::SetURL(char const * const szURL)
    {
        return EasySetopt(::CURLOPT_URL, (void*)szURL);
    }

    // ��ѯ
    bool CHttpHelper::Get(std::string& strResult, long* length/*=nullptr*/)
    {
        strResult.clear();
        if(!EasySetopt(CURLOPT_WRITEDATA, &strResult)) {
            return false;
        }
        CURLcode cc = ::curl_easy_perform(m_pCurl);
        if(cc != CURLE_OK) {
            return false;
        }
        return CURLE_OK == cc;
    }

    // ��ѯ
    bool CHttpHelper::Get(std::string& strURL, std::string& strResult, long* length/*=nullptr*/)
    {
        return SetURL(strURL.c_str()) && Get(strResult, length);
    }

    // ����
    void CHttpHelper::EasyCleanup(void)
    {
        ::curl_easy_cleanup(m_pCurl);
        return;
    }

    void CHttpHelper::GlobalCleanup(void)
    {
        ::curl_global_cleanup();
        return;
    }
}
