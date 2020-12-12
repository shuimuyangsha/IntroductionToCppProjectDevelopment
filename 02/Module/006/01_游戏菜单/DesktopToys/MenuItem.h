#pragma once

#include "IDrawable.h"
#include "IMouseAction.h"

/*
    选择工具的菜单界面的菜单项
*/

class CMenuItem
    : public IDrawable
    , public IMouseAction
{
public:
    CMenuItem();

    CMenuItem(float x, float y, PCTSTR szImg, PCTSTR szTips);

    virtual ~CMenuItem();

    // 根据 自己当前的属性画自己
    virtual void Draw(Gdiplus::Graphics &gh);

    /// ////////////////////////////////////////////////////////////////////////
    /// 处理鼠标消息
    /// 如果 不处理，则返回false,父类可以具此判断是否继续进行处理
    // 鼠标左键按下
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    // 鼠标右键按下
    virtual bool OnRButtonDown(UINT nFlags, CPoint point);
    // 鼠标移动
    virtual bool OnMouseMove(UINT nFlags, CPoint point);


    // 开始播放动画;
    void StartAnimate();
    // 设置动画信息:飞出，散布在四周
    void InitAnimateInfo0(float x, float y, float dir);
    // 设置动画信息:以中心点作翻转
    void InitAnimateInfo1(float x, float y, float dir);
    // 设置动画信息:以中心点作翻转,逆向翻传
    void InitAnimateInfoReverse();
    // 设置动画信息:什么也不干
    void InitAnimateInfo3(float x, float y);

    // 傍止播放动画，直接跳到最后(静止下来)
    void EndAnimate()
    {
        m_indexAnimate = m_vAnimateInfo.size();
    }
    // 动画是否播放完毕
    bool IsAnimateEnd() const
    {
        return m_indexAnimate != 0 && m_indexAnimate >= m_vAnimateInfo.size();
    }
private:
    // 图片
    Image *m_img{ nullptr };

    // 最初的大小
    SizeF m_sizeInit;

    // 动画相关信息
    typedef struct {
        SizeF size;
        PointF pos;
    } SAnimateInfo;
    std::vector<SAnimateInfo> m_vAnimateInfo;
    // 当前在第几帧的位置
    size_t m_indexAnimate;
    // 标记当前是飞出动画
    bool m_bAnimate{ false };
    // 标记当前是翻转动画
    bool m_bAnimate2{ false };
};

