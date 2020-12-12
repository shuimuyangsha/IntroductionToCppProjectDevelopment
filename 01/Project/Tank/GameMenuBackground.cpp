#include "stdafx.h"
#include "GameMenuBackground.h"
#include "Game.h"


CGameMenuBackground::CGameMenuBackground()
{
    // ±³¾°Í¼
    m_imgBackground = Image::FromFile(_T("menu_background.png"));
}

CGameMenuBackground::~CGameMenuBackground()
{

}

void CGameMenuBackground::Draw(Graphics &gh) const
{
    gh.DrawImage(m_imgBackground, m_rect);
}

