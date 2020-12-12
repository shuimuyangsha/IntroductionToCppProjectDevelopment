#pragma once
#include "IMark.h"

/*
    机器人，激光在窗口上留下的痕迹
*/

class CShooter1Mark :
    public IMark
{
public:
    CShooter1Mark(float x, float y, int index=0);
    virtual ~CShooter1Mark();
    // 画自己
    virtual void Draw(Gdiplus::Graphics &gh);
    // 状态是否变化中
    virtual bool IsChanging() const;


private:
    // 图片
    std::vector<Image*> m_img;
    //第几幅图片
    size_t m_index{0}; 
};

