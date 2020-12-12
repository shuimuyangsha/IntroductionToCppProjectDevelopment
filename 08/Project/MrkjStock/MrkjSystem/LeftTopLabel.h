#pragma once


// 前向声明
class CStockChart;

class CLeftTopLabel
{
public:
    CLeftTopLabel(CStockChart* pParent);
    ~CLeftTopLabel();

    // 更新位置信息．大小等等
    void UpdateCtrl();

    // 设置显示的文本
    void SetText(const wchar_t *szLbl);

    // 画自己
    void Draw(Gdiplus::Graphics& gh);

private:
    // 父控件
    CStockChart *m_pParent;
    // 显示的文本
    std::wstring m_strLabel{ L"未设置" };
    // 文本 字体
    Gdiplus::Font* m_pFont{new Gdiplus::Font(L"宋体", 10.0f, FontStyleRegular, UnitPoint) };
    // 文本颜外
    Gdiplus::Color* m_pColor{new Gdiplus::Color(0xFF, 0x00, 0xFF, 0xFF) };
    // 起始位置
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };
};

