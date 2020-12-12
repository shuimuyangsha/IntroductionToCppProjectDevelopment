#pragma once

// ǰ������
namespace data_http
{
    class CHttpDataEntry;
}

class CDrawBuySellPanel
{
public:
    explicit CDrawBuySellPanel();
    virtual ~CDrawBuySellPanel();
    /*���� ����
      1.x,y ��ʼλ��
      2.vSellBuy һ������Ϊ20������,����ʵʱ��Ϣ:
      �����, ��������
      ���ļ�, ��������
      ... ...
      ���ļ�, ��������
      �����, ��������
      3. ���̼�:�����жϼ۸���ɫ
      */
    void Update(Gdiplus::Graphics& gh, int x, int y, const data_http::CHttpDataEntry& data);

private:
    // ���� ����: �����Ļ�ͼ,�� public: Update����
    void Update(Gdiplus::Graphics& gh, int x, int y, const std::vector<std::string>& vSellBuy, const std::string& open);

    // ����Ŀ��
    HDC m_hdc;
};

