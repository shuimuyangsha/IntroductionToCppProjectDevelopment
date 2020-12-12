#include "stdafx.h"
#include "DrawKLineImagePanel.h"
#include "StringHelper.h"


CDrawKLineImagePanel::CDrawKLineImagePanel()
{
}


CDrawKLineImagePanel::~CDrawKLineImagePanel()
{
}

// 画实时分钟K线图
void CDrawKLineImagePanel::Update(Gdiplus::Graphics& gh, int x, int y, PCTSTR szFilename)
{
    using namespace Gdiplus;
    //  1.载入图片
    Image* image = new Image(szFilename);
    //  2.画白色背景
    {
        SolidBrush brush(Color(0xFF, 0xFF, 0xFF, 0xFF));
        gh.FillRectangle(&brush, Rect(x, y, image->GetWidth(), image->GetHeight()));
    }
    //  3.画图片
    gh.DrawImage(image, x, y, image->GetWidth(), image->GetHeight());
    //  4.清除资源
    SAFE_DELETE(image);
}
