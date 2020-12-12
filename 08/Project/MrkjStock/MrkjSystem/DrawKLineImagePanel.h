#pragma once
class CDrawKLineImagePanel
{
public:
    explicit CDrawKLineImagePanel();
    virtual ~CDrawKLineImagePanel();
    // 画实时分钟K线图
    void Update(Gdiplus::Graphics& gh, int x, int y, PCTSTR szFilename);
};

