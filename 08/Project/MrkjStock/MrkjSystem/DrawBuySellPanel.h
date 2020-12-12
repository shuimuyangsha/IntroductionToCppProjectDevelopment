#pragma once

// 前向声明
namespace data_http
{
    class CHttpDataEntry;
}

class CDrawBuySellPanel
{
public:
    explicit CDrawBuySellPanel();
    virtual ~CDrawBuySellPanel();
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
    void Update(Gdiplus::Graphics& gh, int x, int y, const data_http::CHttpDataEntry& data);

private:
    // 更新 画面: 真正的画图,供 public: Update调用
    void Update(Gdiplus::Graphics& gh, int x, int y, const std::vector<std::string>& vSellBuy, const std::string& open);

    // 保存目标
    HDC m_hdc;
};

