#pragma once


/*
    可以移动的对象
*/

class IMoveable
{
public:
    IMoveable();
    virtual ~IMoveable() = 0;

    // 前进
    virtual bool Forward() = 0;
    // 后退
    virtual bool Backward() = 0;
    // 转向
    virtual bool Turn() = 0;
    // 转向 某方向
    virtual bool Turn(float fDirection) = 0;
};

