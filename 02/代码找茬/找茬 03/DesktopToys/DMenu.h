#pragma once

#include "MenuItem.h"
#include "IMouseAction.h"
#include "TipStringFall.h"

/*
    选择工具的菜单界面
*/

class CDMenu
    : public IDrawable
    , public IMouseAction
{
public:
    CDMenu(HWND hWnd);
    virtual ~CDMenu();

    // 重新初始化各菜单子项，及位置信息
    void InitAnimateInfo0();

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

private:
    HWND m_hWnd;
    // 菜单子项
    // 箱子
    std::shared_ptr<CMenuItem> m_box;
    // 盖子1:打开
    std::shared_ptr<CMenuItem> m_cover;
    // 盖子2:关闭
    std::shared_ptr<CMenuItem> m_cover2;
    // 选项
    std::shared_ptr<CMenuItem> m_item0;
    std::shared_ptr<CMenuItem> m_item1;
    std::shared_ptr<CMenuItem> m_item2;
    std::shared_ptr<CMenuItem> m_item3;

    // 结束动画播放
    void EndAnimate();

    typedef enum {EStatusOverCover1,
                  EStatusItem0, // 播放动画
                  EStatusItem1, // 播放动画
                  EStatusItem2,	// 播放动画
                  EStatusItem3,	// 播放动画
                  EStatusOverCover2, // 播放关盖子的动画
                  EStatusAll
                 } EStatus;
    EStatus m_eStatus{ EStatusOverCover1 };

    // 提示文字 : 请选择一个工具
    std::shared_ptr<CTipStringFall> m_pTips;
};

