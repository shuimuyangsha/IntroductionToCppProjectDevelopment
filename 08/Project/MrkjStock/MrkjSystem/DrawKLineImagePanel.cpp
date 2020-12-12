#include "stdafx.h"
#include "DrawKLineImagePanel.h"
#include "StringHelper.h"


CDrawKLineImagePanel::CDrawKLineImagePanel()
{
}


CDrawKLineImagePanel::~CDrawKLineImagePanel()
{
}

// ��ʵʱ����K��ͼ
void CDrawKLineImagePanel::Update(Gdiplus::Graphics& gh, int x, int y, PCTSTR szFilename)
{
    using namespace Gdiplus;
    //  1.����ͼƬ
    Image* image = new Image(szFilename);
    //  2.����ɫ����
    {
        SolidBrush brush(Color(0xFF, 0xFF, 0xFF, 0xFF));
        gh.FillRectangle(&brush, Rect(x, y, image->GetWidth(), image->GetHeight()));
    }
    //  3.��ͼƬ
    gh.DrawImage(image, x, y, image->GetWidth(), image->GetHeight());
    //  4.�����Դ
    SAFE_DELETE(image);
}
