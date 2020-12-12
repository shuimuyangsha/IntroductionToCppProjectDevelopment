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

    // ��������
    CHttpDataEntry::CHttpDataEntry(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
            this->name = rhs.name; // ��Ʊ����
            this->open = rhs.open; // ���տ��̼�
            this->close = rhs.close; // �������̼�
            this->price = rhs.price; // ��ǰ�۸�
            this->maxPrice = rhs.maxPrice; // ������߼�
            this->minPrice = rhs.minPrice; // ������ͼ�
            this->dealCount = rhs.dealCount; // �ɽ��Ĺ�Ʊ��(��λ:��)
            this->dealMoney = rhs.dealMoney; // �ɽ��Ĺ�Ʊ��(��λ:Ԫ)
            this->buyCount01 = rhs.buyCount01; // ��1����(��λ:��, 1��=100��)
            this->buyPrice01 = rhs.buyPrice01; // ��1����
            this->buyCount02 = rhs.buyCount02; // ��2����(��λ:��, 1��=100��)
            this->buyPrice02 = rhs.buyPrice02; // ��2����
            this->buyCount03 = rhs.buyCount03; // ��3����(��λ:��, 1��=100��)
            this->buyPrice03 = rhs.buyPrice03; // ��3����
            this->buyCount04 = rhs.buyCount04; // ��4����(��λ:��, 1��=100��)
            this->buyPrice04 = rhs.buyPrice04; // ��4����
            this->buyCount05 = rhs.buyCount05; // ��5����(��λ:��, 1��=100��)
            this->buyPrice05 = rhs.buyPrice05; // ��5����
            this->sellCount01 = rhs.sellCount01; // ��1����(��λ:��, 1��=100��)
            this->sellPrice01 = rhs.sellPrice01; // ��1����
            this->sellCount02 = rhs.sellCount02; // ��2����(��λ:��, 1��=100��)
            this->sellPrice02 = rhs.sellPrice02; // ��2����
            this->sellCount03 = rhs.sellCount03; // ��3����(��λ:��, 1��=100��)
            this->sellPrice03 = rhs.sellPrice03; // ��3����
            this->sellCount04 = rhs.sellCount04; // ��4����(��λ:��, 1��=100��)
            this->sellPrice04 = rhs.sellPrice04; // ��4����
            this->sellCount05 = rhs.sellCount05; // ��5����(��λ:��, 1��=100��)
            this->sellPrice05 = rhs.sellPrice05; // ��5����
            this->date = rhs.date; // ����(2008-01-11)
            this->time = rhs.time; // ʱ��(15:05:32)
        }
    }
    // ��ֵ
    CHttpDataEntry& CHttpDataEntry::operator=(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
            this->name = rhs.name; // ��Ʊ����
            this->open = rhs.open; // ���տ��̼�
            this->close = rhs.close; // �������̼�
            this->price = rhs.price; // ��ǰ�۸�
            this->maxPrice = rhs.maxPrice; // ������߼�
            this->minPrice = rhs.minPrice; // ������ͼ�
            this->dealCount = rhs.dealCount; // �ɽ��Ĺ�Ʊ��(��λ:��)
            this->dealMoney = rhs.dealMoney; // �ɽ��Ĺ�Ʊ��(��λ:Ԫ)
            this->buyCount01 = rhs.buyCount01; // ��1����(��λ:��, 1��=100��)
            this->buyPrice01 = rhs.buyPrice01; // ��1����
            this->buyCount02 = rhs.buyCount02; // ��2����(��λ:��, 1��=100��)
            this->buyPrice02 = rhs.buyPrice02; // ��2����
            this->buyCount03 = rhs.buyCount03; // ��3����(��λ:��, 1��=100��)
            this->buyPrice03 = rhs.buyPrice03; // ��3����
            this->buyCount04 = rhs.buyCount04; // ��4����(��λ:��, 1��=100��)
            this->buyPrice04 = rhs.buyPrice04; // ��4����
            this->buyCount05 = rhs.buyCount05; // ��5����(��λ:��, 1��=100��)
            this->buyPrice05 = rhs.buyPrice05; // ��5����
            this->sellCount01 = rhs.sellCount01; // ��1����(��λ:��, 1��=100��)
            this->sellPrice01 = rhs.sellPrice01; // ��1����
            this->sellCount02 = rhs.sellCount02; // ��2����(��λ:��, 1��=100��)
            this->sellPrice02 = rhs.sellPrice02; // ��2����
            this->sellCount03 = rhs.sellCount03; // ��3����(��λ:��, 1��=100��)
            this->sellPrice03 = rhs.sellPrice03; // ��3����
            this->sellCount04 = rhs.sellCount04; // ��4����(��λ:��, 1��=100��)
            this->sellPrice04 = rhs.sellPrice04; // ��4����
            this->sellCount05 = rhs.sellCount05; // ��5����(��λ:��, 1��=100��)
            this->sellPrice05 = rhs.sellPrice05; // ��5����
            this->date = rhs.date; // ����(2008-01-11)
            this->time = rhs.time; // ʱ��(15:05:32)
        }
        return *this;
    }
}

