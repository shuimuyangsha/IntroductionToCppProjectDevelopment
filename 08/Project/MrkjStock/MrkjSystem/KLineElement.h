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

    // һ���麯��,�������� �Ի�
    virtual void Draw(Gdiplus::Graphics& gh) = 0;
};
