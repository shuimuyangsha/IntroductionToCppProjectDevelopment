#pragma once
namespace data_http
{
    using namespace std;
    class CHttpDataEntry
    {
    public:
        CHttpDataEntry();
        ~CHttpDataEntry();

        // ��������
        CHttpDataEntry(const CHttpDataEntry& rhs);

        // ���ظ�ֵ������
        CHttpDataEntry& operator=(const CHttpDataEntry& rhs);

    public:
        string name; // ��Ʊ����
        string open; // ���տ��̼�
        string close; // �������̼�
        string price; // ��ǰ�۸�
        string maxPrice; // ������߼�
        string minPrice; // ������ͼ�
        //string // ����һ
        //string // �����
        string dealCount; // �ɽ��Ĺ�Ʊ��(��λ:��)
        string dealMoney; // �ɽ��Ĺ�Ʊ��(��λ:Ԫ)
        string buyCount01; // ��1����(��λ:��, 1��=100��)
        string buyPrice01; // ��1����
        string buyCount02; // ��2����(��λ:��, 1��=100��)
        string buyPrice02; // ��2����
        string buyCount03; // ��3����(��λ:��, 1��=100��)
        string buyPrice03; // ��3����
        string buyCount04; // ��4����(��λ:��, 1��=100��)
        string buyPrice04; // ��4����
        string buyCount05; // ��5����(��λ:��, 1��=100��)
        string buyPrice05; // ��5����

        string sellCount01; // ��1����(��λ:��, 1��=100��)
        string sellPrice01; // ��1����
        string sellCount02; // ��2����(��λ:��, 1��=100��)
        string sellPrice02; // ��2����
        string sellCount03; // ��3����(��λ:��, 1��=100��)
        string sellPrice03; // ��3����
        string sellCount04; // ��4����(��λ:��, 1��=100��)
        string sellPrice04; // ��4����
        string sellCount05; // ��5����(��λ:��, 1��=100��)
        string sellPrice05; // ��5����

        string date; // ����(2008-01-11)
        string time; // ʱ��(15:05:32)

    };
}



