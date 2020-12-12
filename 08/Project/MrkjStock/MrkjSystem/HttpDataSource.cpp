#include "StdAfx.h"
#include "StringHelper.h"

#include "HttpDataSource.h"
#include "HttpHelper.h"
#include "HttpDataEntry.h"

#include <regex>

namespace data_http
{

    // ��ѯ ʵʱ���� URL
    const std::string CHttpDataSource::m_strQueryURL = "http://hq.sinajs.cn/list="; //+sh601616

    // ��ѯ ������ K��ͼ URL
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

    // ��ʼ�����ݿ�
    bool CHttpDataSource::Init(void)
    {
        try {
            // http���ʹ��߳�ʼ��
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
            TRACE("����δ֪�쳣");
        }
        return false;
    }

    // ������Դ
    void CHttpDataSource::Release(void)
    {
        SAFE_DELETE(m_http);
    }

    // ��ѯ��Ʊʵʱ����
    bool CHttpDataSource::QueryStockRealTimeData(const std::string& stockCode, CHttpDataEntry& data)
    {
        using namespace std;
        //1.��ȡ��ҳ��Ϣ
        // ƴ�ӳ������� URL: http://hq.sinajs.cn/list=stockCode   stockCode=sh601006
        std::string strURL = m_strQueryURL;
        strURL.append(stockCode);
        // ��ȡ����
        std::string strContent;
        if(!m_http->Get(strURL, strContent)) {
            return false;
        }
        //2.��ֻ�ȡ,����
        /* �������: var hq_str_s_sh601006="������·,6.810,...,2016-03-09,10:27:36,00";
           ʹ��������ʽ���в��
           v[0] = "var hq_str_s_sh601006"
           v[1] = "������·"
           v[2] = "6.810"
           ...
           v[33] = "00" */
        regex reg("=|,|;|\""); // ��'=' or ',' or '"' or ';' ��Ϊ�ָ�����в��
        sregex_token_iterator it(strContent.begin(), strContent.end(), reg, -1);
        sregex_token_iterator end;
        std::vector<std::string> v;
        while(it != end) {
            //��ȡ��Ϊ�յ���,�洢
            if(*it != "") {
                v.push_back(*it);
            }
            ++it;
        }
        if(v.size() < 33) {
            return false;
        }
        //3.�����صĽṹ�帳ֵ
        data.name = v[1]; // ��Ʊ����
        data.open = v[2]; // ���տ��̼�
        data.close = v[3]; // �������̼�
        data.price = v[4]; // ��ǰ�۸�
        data.maxPrice = v[5]; // ������߼�
        data.minPrice = v[6]; // ������ͼ�
        //data.= v[7]; // ����һ����j
        //data.= v[8]; // ����һ����
        data.dealCount = v[9]; // �ɽ��Ĺ�Ʊ��(��λ:��)
        data.dealMoney = v[10]; // �ɽ��Ĺ�Ʊ��(��λ:Ԫ)
        data.buyCount01 = v[11]; // ��1����(��λ:��, 1��=100��)
        data.buyPrice01 = v[12]; // ��1����
        data.buyCount02 = v[13]; // ��2����(��λ:��, 1��=100��)
        data.buyPrice02 = v[14]; // ��2����
        data.buyCount03 = v[15]; // ��3����(��λ:��, 1��=100��)
        data.buyPrice03 = v[16]; // ��3����
        data.buyCount04 = v[17]; // ��4����(��λ:��, 1��=100��)
        data.buyPrice04 = v[18]; // ��4����
        data.buyCount05 = v[19]; // ��5����(��λ:��, 1��=100��)
        data.buyPrice05 = v[20]; // ��5����
        data.sellCount01 = v[21]; // ��1����(��λ:��, 1��=100��)
        data.sellPrice01 = v[22]; // ��1����
        data.sellCount02 = v[23]; // ��2����(��λ:��, 1��=100��)
        data.sellPrice02 = v[24]; // ��2����
        data.sellCount03 = v[25]; // ��3����(��λ:��, 1��=100��)
        data.sellPrice03 = v[26]; // ��3����
        data.sellCount04 = v[27]; // ��4����(��λ:��, 1��=100��)
        data.sellPrice04 = v[28]; // ��4����
        data.sellCount05 = v[29]; // ��5����(��λ:��, 1��=100��)
        data.sellPrice05 = v[30]; // ��5����
        data.date = v[31]; // ����(2008-01-11)
        data.time = v[32]; // ʱ��(15:05:32)
        // TODO: ������ݸ�ʽ�Ƿ�Ϸ�
        return true;
    }

    // ���ع�Ʊ����K��ͼ
    bool CHttpDataSource::QueryStockKLineMin(const std::string& stockCode, const std::string& filename)
    {
        using namespace std;
        //1.��ȡ��ҳ��Ϣ
        // ƴ�ӳ������� URL:http://image.sinajs.cn/newchart/min/n/+stockCode+.gif
        std::string strURL = m_strQueryURLGif;
        strURL.append(stockCode);
        strURL.append(".gif");
        // ��ȡ����
        std::string content;
        if(!m_http->Get(strURL, content, nullptr)) {
            return false;
        }
        // 2.�����ļ����ݵ�filenameָ��λ��
        try {
            // ����ļ����,��ɾ���ļ��ؽ�
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
            TRACE("����δ֪�쳣");
        }
        return true;
    }
}
