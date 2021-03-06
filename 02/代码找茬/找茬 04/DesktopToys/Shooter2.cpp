#include "stdafx.h"
#include "Shooter2.h"
#include "Shooter2Mark.h"
#include "Shooter2MarkSaw.h"


CShooter2::CShooter2()
{
    //  载入图片
    m_img0 = Image::FromFile(_T("res/The Toolbox01.png"));
    m_img1 = Image::FromFile(_T("res/The Toolbox02.png"));
    m_img2 = Image::FromFile(_T("res/The Toolbox03.png"));
}

CShooter2::~CShooter2()
{
}

// 顺时针旋转绘图
void DrawImageRotate(PointF mousePt, Graphics &gh, Image *img, float degree)
{

    //旋转绘图平面
    PointF center = mousePt;
    center.X += img->GetWidth() / 2.0f;
    center.Y += img->GetHeight() / 2.0f;

    // 1.平移变换 : 移动坐标点到 坦克中心
    gh.TranslateTransform(center.X, center.Y);
    // 2.旋转变换 : 使作标系，跟坦克的角度一致
    gh.RotateTransform(degree);
    // 3.恢复原点
    gh.TranslateTransform(-center.X, -center.Y);

    // 函数退出时恢复
    ON_SCOPE_EXIT([&]() {
        // 重置坐标变换 : 精度是否够呢？
        // 1.移动到中心点
        gh.TranslateTransform(center.X, center.Y);
        // 2.逆向转
        gh.RotateTransform(-degree);
        // 3.再移动回去
        gh.TranslateTransform(-center.X, -center.Y);
        //ScaleTransform : 缩放
    });


    // 画出图像
    {
        RectF rc;
        rc.X = mousePt.X;
        rc.Y = mousePt.Y;
        rc.Width = (float)img->GetWidth();
        rc.Height = (float)img->GetHeight();
        gh.DrawImage(img, rc);
    }
}

// 根据 自己当前的属性画自己
void CShooter2::Draw(Gdiplus::Graphics &gh)
{
    switch(m_status) {
        // 锤子
        case EStatus::EStatusHammer: {
            switch(m_eStatusHammer) {
                // 鼠标左键第一次点击 :  发射子弹,同时锤子图片变化
                case EStatusHammer::EStatusHammerDownFirst: {
                    // 发射一个子弹
                    g_game->Append(std::make_shared<CShooter2Mark>(m_mousePos.X, m_mousePos.Y));
                    m_eStatusHammer = EStatusHammer::EStatusHammerDown;
                    /*break; // 注意这里没有break;会接下来执行下面一个case部分*/
                }
                // 鼠标左键点击
                case EStatusHammer::EStatusHammerDown: {
                    auto img = m_img1;
                    RectF rc;
                    rc.X = m_mousePos.X;
                    rc.Y = m_mousePos.Y;
                    rc.Width = (float)img->GetWidth();
                    rc.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rc);
                    break;
                }
                // 鼠标键左键抬起
                case EStatusHammer::EStatusHammerUp: {
                    auto img = m_img2;
                    RectF rc;
                    rc.X = m_mousePos.X;
                    rc.Y = m_mousePos.Y;
                    rc.Width = (float)img->GetWidth();
                    rc.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rc);
                    break;
                }
                default: {
                    // 不会走到这里
                    assert(false);
                    break;
                }
            }
            break;
        }
        //电锯
        case EStatus::EStatusSaw: {
            // 电锯图片
            auto img = m_img0;
            // 计算电锯的位置,和大小
            RectF rc;
            rc.X = m_sawPos.X;
            rc.Y = m_sawPos.Y;
            rc.Width = (float)img->GetWidth();
            rc.Height = (float)img->GetHeight();
            // 绘制电锯图片
            DrawImageRotate(m_sawPos, gh, img, m_sawDegree);

            switch(m_eStatusSaw) {

                // 如果是开始状态,画下切割效果
                case CShooter2::EStatusSaw::EStatusSawStart: {
                    // 什么也不做，也不跳出，直接跳到下一个case 处理
                }
                // 保持移动状态, 电锯移动
                case CShooter2::EStatusSaw::EStatusMoving: {
                    float radian = Degree2Radian(m_sawDegree);

                    // 发射一个子弹
                    g_game->Append(std::make_shared<CShooter2MarkSaw>(m_mousePos.X, m_mousePos.Y));

                    // 计算电锯当前的位置
                    const float speed = 5.0f;
                    auto newX = m_sawPos.X + speed * cos(radian);
                    auto newY = m_sawPos.Y + speed * sin(radian);

                    if(newX >= 0.0f && newX <= g_game->GetWidth() && newY >= 0.0f && newY <= g_game->GetHeigth()) {
                        m_sawPos.X = newX;
                        m_sawPos.Y = newY;
                        // 使鼠标位置跟随变化
                        SetCursorPos(static_cast<int>(m_sawPos.X), static_cast<int>(m_sawPos.Y));
                    }
                    break;
                }
                // 停止
                case CShooter2::EStatusSaw::EStatusSawStop: {
                    break;
                }
                default:
                    break;
            }
            if(m_eStatusSaw == EStatusSaw::EStatusSawStart) {
                // 傍止状态，什么也不做
            }
            break;
        }
        default : {
            break;
        }
    }
}

/// ////////////////////////////////////////////////////////////////////////
/// 处理鼠标消息 : 需要在子类中处理
/// 如果 不处理，则返回false,父类可以具此判断是否继续进行处理
// 鼠标左键按下
bool CShooter2::OnLButtonDown(UINT nFlags, CPoint point)
{
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // 锤子砸下
            m_eStatusHammer = EStatusHammer::EStatusHammerDownFirst;
            break;
        }
        case EStatus::EStatusSaw: {
            // 电锯 开始割东西啦
            m_eStatusSaw = EStatusSaw::EStatusSawStart;
            // 当前位置
            m_sawPos = m_mousePos;
            break;
        }
        default : {
            break;
        }
    }

    return false;
}

// 鼠标左键抬起
bool CShooter2::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // 锤子抬起
            m_eStatusHammer = EStatusHammer::EStatusHammerUp;
            break;
        }
        case EStatus::EStatusSaw: {
            // 电锯停止
            m_eStatusSaw = EStatusSaw::EStatusSawStop;
            break;
        }
        default : {
            break;
        }
    }

    return false;
}

// 鼠标右键按下
bool CShooter2::OnRButtonDown(UINT nFlags, CPoint point)
{
    // 换工具: 如果当前是锤子,切换为电锯
    if(m_status == EStatus::EStatusHammer) {
        m_status = EStatus::EStatusSaw;
    }
    else {
        // 换工具: 如果当前是电锯,切换为锤子
        m_status = EStatus::EStatusHammer;
    }
    return true;
}

// 计算两点间距离
float distance(PointF pt1, PointF pt2)
{
    return std::sqrt(std::pow(pt1.X - pt2.X, 2) + std::pow(pt1.Y - pt2.Y, 2));
}

// 鼠标移动
bool CShooter2::OnMouseMove(UINT nFlags, CPoint point)
{
    // 记录鼠标位置
    m_mousePos.X = (float)point.x;
    m_mousePos.Y = (float)point.y;

    // 根据不同的情况进行处理
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // 锤子，维持原状态
            break;
        }
        case EStatus::EStatusSaw: {
            // 如果没有锯东西，变换角度
            if(m_eStatusSaw == EStatusSaw::EStatusSawStop) {
                // 记录 锯子的位置
                m_sawPos = m_mousePos;
                // 变换角度
                if(distance(m_mousePosLast, m_mousePos) > 10) {
                    // 计算角度
                    float theta = (atan2(m_mousePos.Y - m_mousePosLast.Y, m_mousePos.X - m_mousePosLast.X));
                    if(theta < 0.0f) {
                        theta = PI(2.0f) + theta;
                    }
                    // 角度:单位(度)
                    m_sawDegree = Radian2Degree(theta);
                    // 记住本次位置
                    m_mousePosLast = m_mousePos;
                }
            }
            break;
        }
        default : {
            break;
        }
    }

    return true;
}





