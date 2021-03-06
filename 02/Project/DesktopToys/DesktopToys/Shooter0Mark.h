#pragma once
#include "IMark.h"

/*
    机器人，激光在窗口上留下的痕迹
*/

class CShooter0Mark :
    public IMark
{
public:
    CShooter0Mark(float x, float y);
    virtual ~CShooter0Mark();

    // 画自己
    virtual void Draw(Gdiplus::Graphics &gh);

    // 状态是否在改变中
    virtual bool IsChanging() const;

private:
    // 图片
    std::vector<Image *> m_img;
    //第几幅图片
    size_t m_index{ 0 };
    //旋转的角度
    float m_dir;
};

