#include "stdafx.h"
#include "Shooter3.h"
#include "Shooter3Mark.h"
#include "GameTimer.h"


CShooter3::CShooter3()
{
    // 载入图片
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 0.png")));
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 1.png")));
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 2.png")));
    m_vImgDown.push_back(Image::FromFile(_T("res/Eraser Ed 00.png")));
    m_vImgDown.push_back(Image::FromFile(_T("res/Eraser Ed 01.png")));
}


CShooter3::~CShooter3()
{
}

// 根据 自己当前的属性画自己
void CShooter3::Draw(Gdiplus::Graphics &gh)
{
    switch(m_status) {
        // 右键按下，眨眼
        case CShooter3::EStatus::EStatusLeftUp: {
            if(!m_bRightDown) {
                // 反复显示三幅图片：眨眼睛
                static size_t index = 0;
                if(index >= 3) {
                    index = 0;
                }
                // 画当前图片
                {
                    auto img = m_vImgUp[index];
                    RectF rect;
                    rect.X = m_mousePos.X;
                    rect.Y = m_mousePos.Y;
                    rect.Width = (float)img->GetWidth();
                    rect.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rect);
                }
                //到达时间间隔，增加
                static CGameTimeval val(200);
                if(val.IsTimeval()) {
                    index++;
                }
                // 判断是否需要眨眼
                if(index == 2) {
                    if((0 == rand() % 4)) {
                        // 不改动index， 此时会闭眼
                    }
                    else {
                        index = 0;
                    }
                }
            }
            else {
                // 只显示闭眼的图片
                size_t index = 2;
                {
                    auto img = m_vImgUp[index];
                    RectF rect;
                    rect.X = m_mousePos.X;
                    rect.Y = m_mousePos.Y;
                    rect.Width = (float)img->GetWidth();
                    rect.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rect);
                }
            }
            break;
        }
        // 左键按下， 擦屏幕
        case CShooter3::EStatus::EStatusLeftDown: {
            // 反复显示两幅图片
            static size_t index = 0;
            if(index > 1) {
                index = 0;
            }
            auto img = m_vImgDown[index];
            RectF rect;
            rect.X = m_mousePos.X;
            rect.Y = m_mousePos.Y;
            rect.Width = (float)img->GetWidth();
            rect.Height = (float)img->GetHeight();
            gh.DrawImage(img, rect);
            if(index == 1) {
                // 释放碎屑
                g_game->Append(std::make_shared<CShooter3Mark>(m_mousePos.X, m_mousePos.Y));
            }
            // 30豪秒画一次
            static CGameTimeval val(30);
            if(val.IsTimeval()) {
                index++;
            }
            break;
        }
        default:
            break;
    }
}


/// ////////////////////////////////////////////////////////////////////////
/// 处理鼠标消息 : 需要在子类中处理
/// 如果 不处理，则返回false,父类可以具此判断是否继续进行处理
// 鼠标左键按下
bool CShooter3::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_status = EStatus::EStatusLeftDown;
    return true;
}

// 鼠标左键抬起
bool CShooter3::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_status = EStatus::EStatusLeftUp;
    return true;
}

// 鼠标右键按下
bool CShooter3::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_bRightDown = true;
    return true;
}

// 鼠标右键抬起
bool CShooter3::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_bRightDown = false;
    return true;
}

// 鼠标移动
bool CShooter3::OnMouseMove(UINT nFlags, CPoint point)
{
    // 记录鼠标位置
    m_mousePos.X = (float)point.x;
    m_mousePos.Y = (float)point.y;
    return true;
}





