#pragma once
#include "Shooter.h"

/*
    锤子/电锯
*/


class CShooter2 :
    public CShooter
{
public:
    CShooter2();
    virtual ~CShooter2();
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
    virtual bool OnRButtonUp(UINT nFlags, CPoint point)
    {
        return false;
    }

    // 鼠标右键双击
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // 鼠标移动
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

private:

    // 当前工具
    enum class EStatus {EStatusHammer, EStatusSaw} ;
    EStatus m_status{EStatus::EStatusHammer};

    // 锤子状态
    enum class EStatusHammer {EStatusHammerDownFirst,/*锤子落下0*/
                              EStatusHammerDown,/*锤子落下*/
                              EStatusHammerUp, /*锤子抬起*/
                             };
    EStatusHammer m_eStatusHammer{ EStatusHammer::EStatusHammerDown };

    // 电锯状态
    enum class EStatusSaw {EStatusSawStart, EStatusMoving, EStatusSawStop};
    EStatusSaw m_eStatusSaw{EStatusSaw::EStatusSawStop};
    // 电锯的运行速度
    float m_sawSpeed{10.0f};
    // 运行方向
    float m_sawDegree{0.0};
    // 当前位置
    PointF m_sawPos{0.0f, 0.0f};

    // 上次变换角度鼠标的位置
    PointF m_mousePosLast{0.0f, 0.0f};

    // 当前鼠标所在的位置
    PointF m_mousePos{0.0f, 0.0f};

    // 工具的图片
    Image *m_img0;
    Image *m_img1;
    Image *m_img2;

    // 输出文字 字体，画刷
    Gdiplus::Font m_font{L"宋体", 20.0f};
    SolidBrush m_brush{Color::Red};

};

