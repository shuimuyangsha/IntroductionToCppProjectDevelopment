#pragma once
#include "IDrawable.h"
/*
    ����Ϊ ���� ���(���䣬ͿĨ����Ļ����Ķ���������Ч��)�ĸ���,
    ��һ��������   
*/
class IMark :
    public IDrawable
{
public:
    IMark();
    virtual ~IMark();
    // ��Ǳ��൱ǰ�Ƿ��Ƕ�̬��
    virtual bool IsChanging() const = 0;
};

