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

// 更新 画面: 真正的画图,供 public: Update调用
void CDrawBuySellPanel::Update(Gdiplus::Graphics& gh, int x, int y, const std::vector<std::string>& vSellBuy, const std::string& open)
{
    using namespace Gdiplus;
    assert(vSellBuy.size() == 20);
    FontFamily fontFamily(L"宋体");
    Gdiplus::Font font(&fontFamily, 10, FontStyleRegular, UnitPoint);
    SolidBrush redBrush(Color(0xFF, 0xFF, 0, 0));
    SolidBrush greenBrush(Color(0xFF, 0, 0xFF, 0));
    SolidBrush whiteBrush(Color(0xFF, 0xFF, 0xFF, 0xFF));
    SolidBrush yellowBrush(Color(0xFF, 0xFF, 0xFF, 0));
    int dy = 20; //行高
    int line_w = 200; //横线的长度
    int line_h = 11 * dy; //坚线的高度
    int y0 = y; // 第一条线
    int y1 = y + 1 * dy; //第二条线
    int y2 = y + 6 * dy; //中间的一条线
    int y3 = y + 11 * dy; //最下面的线
    //画背景
    {
        // 黑色不透明
        SolidBrush brush(Color(255, 0, 0, 0));
        gh.FillRectangle(&brush, Rect(x, y, line_w, line_h));
    }
    // 画委比,委差
    {
        CString firstLineTextInfo[2];
        firstLineTextInfo[0].LoadStringW(IDS_STRING_DRAW_WEIBI);
        firstLineTextInfo[0].LoadStringW(IDS_STRING_DRAW_WEICHA);
        gh.DrawString(firstLineTextInfo[0].GetString(), -1, &font, PointF(x, y0 + 3), &whiteBrush);
        gh.DrawString(firstLineTextInfo[1].GetString(), -1, &font, PointF(x + 100, y0 + 3), &whiteBrush);
    }
    // 画卖一~卖五,买一~买五
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
    //画出外框及分割线
    {
        Pen pen(&redBrush);
        // 画四条横线
        gh.DrawLine(&pen, Point(x, y0), Point(x + line_w, y0));
        gh.DrawLine(&pen, Point(x, y1), Point(x + line_w, y1));
        gh.DrawLine(&pen, Point(x, y2), Point(x + line_w, y2));
        gh.DrawLine(&pen, Point(x, y3), Point(x + line_w, y3));
        // 画坚线
        gh.DrawLine(&pen, Point(x, y), Point(x, y + line_h));
        gh.DrawLine(&pen, Point(x + line_w, y), Point(x + line_w, y + line_h));
    }
    // 画传入的报价,数量
    for(int index = 0; index != vSellBuy.size(); index += 2) {
        // 报价:需要根据 开盘价,判断颜色(>open红色 <open绿色 ==open白色
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
        // 数量
        gh.DrawString(StringHelper::ANSIToUnicode(vSellBuy[index + 1]).c_str(),
                      -1, &font, PointF(x + 120, y1 + 3 + index / 2 * dy), &yellowBrush);
    }
}


/*更新 画面
  1.x,y 起始位置
  2.vSellBuy 一个长度为20的数组,包含实时信息:
  卖五价, 卖五数量
  卖四价, 卖四数量
  ... ...
  买四价, 买四数量
  买五价, 买五数量
  3. 开盘价:用于判断价格颜色
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
