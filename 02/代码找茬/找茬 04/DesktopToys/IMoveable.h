#pragma once


/*
    �����ƶ��Ķ���
*/

class IMoveable
{
public:
    IMoveable();
    virtual ~IMoveable() = 0;

    // ǰ��
    virtual bool Forward() = 0;
    // ����
    virtual bool Backward() = 0;
    // ת��
    virtual bool Turn() = 0;
    // ת�� ĳ����
    virtual bool Turn(float fDirection) = 0;
};

