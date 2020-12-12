#include "stdafx.h"
#include "GameMenuPanel2.h"
#include "Game.h"


CGameMenuPanel2::CGameMenuPanel2()
{
    m_img = Image::FromFile(_T("menu_backup.png"));
    if(m_img) {
        RectF rc = {800 - 100, 600 - 40
                    , static_cast<float>(m_img->GetWidth())
                    , static_cast<float>(m_img->GetHeight())
                   };
        SetRect(rc);
    }
}

CGameMenuPanel2::~CGameMenuPanel2()
{

}

void CGameMenuPanel2::Draw(Graphics &gh) const
{
    if(m_bNormal) {
        gh.DrawImage(m_img, GetRect());
    }
    else {
        // 放大绘图
        RectF rc = GetRect();
        rc.Inflate(rc.Width * 0.3f, rc.Height * 0.3f);
        gh.DrawImage(m_img, rc);
    }
}

// 处理 鼠标移动事件
void CGameMenuPanel2::OnMouseMove(UINT nFlags, CPoint point)
{
    RectF rcf = GetRect();
    if(rcf.Contains(static_cast<float>(point.x), static_cast<float>(point.y))) {
        m_bNormal = false;
    }
    else {
        m_bNormal = true;
    }
    return;
}

// 处理 鼠标左键抬起事件
void CGameMenuPanel2::OnLButtonUp(UINT nFlags, CPoint point)
{
    RectF rcf = GetRect();
    if(rcf.Contains((float)point.x, (float)point.y)) {
        m_pParent->SetStep(CGame::EGameTypeMenu);
    }
}
