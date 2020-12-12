#pragma once
#include "Shooter.h"

/*
    橡皮人类
*/


class CShooter3 :
    public CShooter
{
public:
    CShooter3();
    virtual ~CShooter3();
    // 根据 自己当前的属性画自己
    virtual void Draw(Gdiplus::Graphics &gh);
    /// ////////////////////////////////////////////////////////////////////////
    /// 处理鼠标消息 : 需要在子类中处理
    /// 如果 不处理，则返回false,父类可以具此判断是否继续进行处理
    // 鼠标左键按下
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);

    // 鼠标左键抬起
    virtual bool OnLButtonUp(UINT nFlags, CPoint point);

    // 鼠标左键双击
    virtual bool OnLButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // 鼠标右键按下
    virtual bool OnRButtonDown(UINT nFlags, CPoint point);

    // 鼠标右键抬起
    virtual bool OnRButtonUp(UINT nFlags, CPoint point);

    // 鼠标右键双击
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // 鼠标移动
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

private:

    enum class EStatus {EStatusLeftUp/*左键抬起*/, EStatusLeftDown/*左键按下*/};
    EStatus m_status{EStatus::EStatusLeftUp};
    bool m_bRightDown{ false };

    // 当前鼠标所在的位置
    PointF m_mousePos{0.0f, 0.0f};

    // 鼠标左键抬起图片
    std::vector<Image *> m_vImgUp;

    // 鼠标左键按下图片
    std::vector<Image *> m_vImgDown;
};

