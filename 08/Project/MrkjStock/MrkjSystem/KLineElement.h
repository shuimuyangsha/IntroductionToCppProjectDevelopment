#pragma once
class CKLineElement
{
public:

    CKLineElement()
    {
    }

    virtual ~CKLineElement()
    {
    }

    // 一个虚函数,各个部件 自绘
    virtual void Draw(Gdiplus::Graphics& gh) = 0;
};
