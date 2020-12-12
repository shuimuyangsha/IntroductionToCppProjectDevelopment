#include "stdafx.h"
#include "MenuItem.h"


CMenuItem::CMenuItem()
{
}

CMenuItem::CMenuItem(float x, float y, PCTSTR szImg, PCTSTR szTips)
{
    m_img = Image::FromFile(szImg);

    int width = m_img->GetWidth();
    int height = m_img->GetHeight();

    // 计算 坦克 前占用的区域
    RectF r(static_cast<float>(x)
            , static_cast<float>(y)
            , static_cast<float>(width)
            , static_cast<float>(height));
    SetRect(r);
    m_sizeInit.Width = (float)width;
    m_sizeInit.Height = (float)height;

    m_pTips = make_shared<CTipStringChangeColor>(szTips);
}


CMenuItem::~CMenuItem()
{
}

// 根据 自己当前的属性画自己
void CMenuItem::Draw(Gdiplus::Graphics &gh)
{
    if(m_indexAnimate >= m_vAnimateInfo.size()) {
        // 不进行缩放移动位置等操作
        m_indexAnimate = m_vAnimateInfo.size();
        if(m_indexAnimate == 0) {
            return;
        }
        m_indexAnimate--;
    }

    {
        auto const &info = m_vAnimateInfo[m_indexAnimate++];
        // 移动位置
        SetCenterPos(info.pos);
        // 缩放
        SetSize(info.size.Width, info.size.Height);
    }
    gh.DrawImage(m_img, GetRect());

    // 画提示文字
    m_pTips->Draw(gh);
}

// 鼠标左键按下
bool CMenuItem::OnLButtonDown(UINT nFlags, CPoint point)
{
    // 查看命中了哪个菜单
    if(this->GetRect().Contains((float)point.x, (float)point.y)) {
        return true;
    }
    else {
        return false;
    }
}

// 鼠标右键按下
bool CMenuItem::OnRButtonDown(UINT nFlags, CPoint point)
{
    // 什么也不做
    return false;
}

// 鼠标移动
bool CMenuItem::OnMouseMove(UINT nFlags, CPoint point)
{
    if(this->GetRect().Contains((float)point.x, (float)point.y)) {
        m_pTips->SetPos(point);
        return true;
    }
    else {
        m_pTips->SetEnd();
    }
    return false;
}

// 开始播放动画;
void CMenuItem::StartAnimate()
{

}

// 设置动画信息:飞出，散布在四周
void CMenuItem::InitAnimateInfo0(float x, float y, float dir)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // 初始
    m_indexAnimate = 0;
    // 运动路径信息清空
    m_vAnimateInfo.clear();
    // 初始大小
    float w = m_sizeInit.Width * 0.01f, h = m_sizeInit.Height * 0.01f;
    // 原始大小
    float ww = m_sizeInit.Width, hh = m_sizeInit.Height;

    // 最大
    float www = m_sizeInit.Width * 2.0f, hhh = m_sizeInit.Height * 2.0f;

    SAnimateInfo info;

    // 第一个位置
    {
        info.size.Width = w;
        info.size.Height = h;
        info.pos.X = x;
        info.pos.Y = y;
        m_vAnimateInfo.push_back(info);
    }
    float distance = 1.0f;
    // 达到最大值
    while(true) {
        // 大小每次增加 10%
        w *= (1 + 0.1f);
        h *= (1 + 0.1f);
        if(w > www && h > hhh) {
            break;
        }
        // 位置每次移动 Distance 像素
        x += distance * cos(PI(2) - dir);
        y += distance * sin(PI(2) - dir);

        {

            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }

    distance = 4.0f;
    // 恢复到原始值
    while(true) {
        // 大小每次增加 10%
        w *= (1.0f - 0.03f);
        h *= (1.0f - 0.03f);
        if(w <= ww && h <= hh) {
            break;
        }
        // 位置每次移动 Distance 像素
        x += distance * cos(PI(2) - dir);
        y += distance * sin(PI(2) - dir);
        {
            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }
    // 放入初始大小

    info.size = m_sizeInit;
    info.pos.X = x;
    info.pos.Y = y;
    m_vAnimateInfo.push_back(info);
}

// 设置动画信息:以中心点作翻转
void CMenuItem::InitAnimateInfo1(float x, float y, float dir)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // 初始
    m_indexAnimate = 0;
    // 运动路径信息清空
    m_vAnimateInfo.clear();
    // 根据初始位置，和方向，生成新的路径
    // 到最大
    float w = m_sizeInit.Width, h = m_sizeInit.Height;
    SAnimateInfo info;

    // 第一个位置
    {
        info.size.Width = w;
        info.size.Height = h;
        info.pos.X = x;
        info.pos.Y = y;
        m_vAnimateInfo.push_back(info);
    }

    for(int i = 0; i < 25; ++i) {
        // 高度每次增加 10%
        h *= (1 - 0.05f);
        {

            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }
}

// 设置动画信息:以中心点作翻转,逆向翻传
void CMenuItem::InitAnimateInfoReverse()
{
    if(m_bAnimate2) {
        return;
    }
    else {
        m_bAnimate2 = true;
    }
    // 初始
    m_indexAnimate = 0;
    // 把播放的 信息数组逆序排列一下
    std::reverse(m_vAnimateInfo.begin(), m_vAnimateInfo.end());
}

// 设置动画信息:什么也不干
void CMenuItem::InitAnimateInfo3(float x, float y)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // 初始
    m_indexAnimate = 0;
    // 运动路径信息清空
    m_vAnimateInfo.clear();
    // 根据初始位置，和方向，生成新的路径
    // 到最大
    float w = m_sizeInit.Width * 0.01f, h = m_sizeInit.Height * 0.01f;
    SAnimateInfo info;
    // 放入初始大小
    info.size = m_sizeInit;
    info.pos.X = x;
    info.pos.Y = y;
    m_vAnimateInfo.push_back(info);
}


