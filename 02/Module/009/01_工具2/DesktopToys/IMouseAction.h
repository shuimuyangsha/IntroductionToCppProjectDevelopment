#pragma once

/*
    可以响映鼠标事件的对象,子类可覆盖本类的方法，实现差异化处理
*/
class IMouseAction
{
public:
    IMouseAction();
    virtual ~IMouseAction();

    /// ////////////////////////////////////////////////////////////////////////
    /// 处理鼠标消息 : 需要在子类中处理
    /// 如果 不处理，则返回false,父类可以具此判断是否继续进行处理
    // 鼠标左键按下
    virtual bool OnLButtonDown(UINT nFlags, CPoint point) { return false; }
    // 鼠标左键抬起
    virtual bool OnLButtonUp(UINT nFlags, CPoint point) { return false; }
    // 鼠标左键双击
    virtual bool OnLButtonDblClk(UINT nFlags, CPoint point) { return false; }
    // 鼠标右键按下
    virtual bool OnRButtonDown(UINT nFlags, CPoint point) { return false; }
    // 鼠标右键抬起
    virtual bool OnRButtonUp(UINT nFlags, CPoint point) { return false; }
    // 鼠标右键双击
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point) { return false; }
    // 鼠标移动
    virtual bool OnMouseMove(UINT nFlags, CPoint point) { return false; }
};

