#pragma once
class CDrawKLineImagePanel
{
public:
    explicit CDrawKLineImagePanel();
    virtual ~CDrawKLineImagePanel();
    // ��ʵʱ����K��ͼ
    void Update(Gdiplus::Graphics& gh, int x, int y, PCTSTR szFilename);
};

