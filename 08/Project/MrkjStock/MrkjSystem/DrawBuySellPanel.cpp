#include "stdafx.h"
#include "DrawBuySellPanel.h"
#include "StringHelper.h"
#include "HttpDataEntry.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)


CDrawBuySellPanel::CDrawBuySellPanel()
{
}


CDrawBuySellPanel::~CDrawBuySellPanel()
{
}

// ���� ����: �����Ļ�ͼ,�� public: Update����
void CDrawBuySellPanel::Update(Gdiplus::Graphics& gh, int x, int y, const std::vector<std::string>& vSellBuy, const std::string& open)
{
    using namespace Gdiplus;
    assert(vSellBuy.size() == 20);
    FontFamily fontFamily(L"����");
    Gdiplus::Font font(&fontFamily, 10, FontStyleRegular, UnitPoint);
    SolidBrush redBrush(Color(0xFF, 0xFF, 0, 0));
    SolidBrush greenBrush(Color(0xFF, 0, 0xFF, 0));
    SolidBrush whiteBrush(Color(0xFF, 0xFF, 0xFF, 0xFF));
    SolidBrush yellowBrush(Color(0xFF, 0xFF, 0xFF, 0));
    int dy = 20; //�и�
    int line_w = 200; //���ߵĳ���
    int line_h = 11 * dy; //���ߵĸ߶�
    int y0 = y; // ��һ����
    int y1 = y + 1 * dy; //�ڶ�����
    int y2 = y + 6 * dy; //�м��һ����
    int y3 = y + 11 * dy; //���������
    //������
    {
        // ��ɫ��͸��
        SolidBrush brush(Color(255, 0, 0, 0));
        gh.FillRectangle(&brush, Rect(x, y, line_w, line_h));
    }
    // ��ί��,ί��
    {
        CString firstLineTextInfo[2];
        firstLineTextInfo[0].LoadStringW(IDS_STRING_DRAW_WEIBI);
        firstLineTextInfo[0].LoadStringW(IDS_STRING_DRAW_WEICHA);
        gh.DrawString(firstLineTextInfo[0].GetString(), -1, &font, PointF(x, y0 + 3), &whiteBrush);
        gh.DrawString(firstLineTextInfo[1].GetString(), -1, &font, PointF(x + 100, y0 + 3), &whiteBrush);
    }
    // ����һ~����,��һ~����
    {
        CString sellBuyInfo[10];
        sellBuyInfo[0].LoadString(IDS_STRING_DRAW_SELL1);
        sellBuyInfo[1].LoadString(IDS_STRING_DRAW_SELL4);
        sellBuyInfo[2].LoadString(IDS_STRING_DRAW_SELL3);
        sellBuyInfo[3].LoadString(IDS_STRING_DRAW_SELL2);
        sellBuyInfo[4].LoadString(IDS_STRING_DRAW_SELL1);
        sellBuyInfo[5].LoadString(IDS_STRING_DRAW_BUY1);
        sellBuyInfo[6].LoadString(IDS_STRING_DRAW_BUY2);
        sellBuyInfo[7].LoadString(IDS_STRING_DRAW_BUY3);
        sellBuyInfo[8].LoadString(IDS_STRING_DRAW_BUY4);
        sellBuyInfo[9].LoadString(IDS_STRING_DRAW_BUY5);
        for(int index = 0; index < _countof(sellBuyInfo); ++index) {
            gh.DrawString(sellBuyInfo[index].GetString(), -1, &font, PointF(x, y1 + 3 + index * dy), &whiteBrush);
        }
    }
    //������򼰷ָ���
    {
        Pen pen(&redBrush);
        // ����������
        gh.DrawLine(&pen, Point(x, y0), Point(x + line_w, y0));
        gh.DrawLine(&pen, Point(x, y1), Point(x + line_w, y1));
        gh.DrawLine(&pen, Point(x, y2), Point(x + line_w, y2));
        gh.DrawLine(&pen, Point(x, y3), Point(x + line_w, y3));
        // ������
        gh.DrawLine(&pen, Point(x, y), Point(x, y + line_h));
        gh.DrawLine(&pen, Point(x + line_w, y), Point(x + line_w, y + line_h));
    }
    // ������ı���,����
    for(int index = 0; index != vSellBuy.size(); index += 2) {
        // ����:��Ҫ���� ���̼�,�ж���ɫ(>open��ɫ <open��ɫ ==open��ɫ
        SolidBrush* pBrush = nullptr;
        if(vSellBuy[index] > open) {
            pBrush = &redBrush;
        }
        else if(vSellBuy[index] < open) {
            pBrush = &greenBrush;
        }
        else {
            pBrush = &whiteBrush;
        }
        gh.DrawString(StringHelper::ANSIToUnicode(vSellBuy[index]).c_str(), -1,
                      &font, PointF(x + 60, y1 + 3 + index / 2 * dy), pBrush);
        // ����
        gh.DrawString(StringHelper::ANSIToUnicode(vSellBuy[index + 1]).c_str(),
                      -1, &font, PointF(x + 120, y1 + 3 + index / 2 * dy), &yellowBrush);
    }
}


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

void CDrawBuySellPanel::Update(Gdiplus::Graphics& gh, int x, int y, const data_http::CHttpDataEntry& data)
{
    using namespace std;
    vector<string> v;
    v.push_back(data.buyPrice01);
    v.push_back(data.buyCount01);
    v.push_back(data.buyPrice02);
    v.push_back(data.buyCount02);
    v.push_back(data.buyPrice03);
    v.push_back(data.buyCount03);
    v.push_back(data.buyPrice04);
    v.push_back(data.buyCount04);
    v.push_back(data.buyPrice05);
    v.push_back(data.buyCount05);
    v.push_back(data.sellPrice01);
    v.push_back(data.sellCount01);
    v.push_back(data.sellPrice02);
    v.push_back(data.sellCount02);
    v.push_back(data.sellPrice03);
    v.push_back(data.sellCount03);
    v.push_back(data.sellPrice04);
    v.push_back(data.sellCount04);
    v.push_back(data.sellPrice05);
    v.push_back(data.sellCount05);
    Update(gh, x, y, v, data.open);
}
