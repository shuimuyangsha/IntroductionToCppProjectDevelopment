#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{
}


CGame::~CGame()
{
}

// ����������ڵľ��
void CGame::SetHandle(HWND hWnd)
{
    m_hWnd = hWnd;
}

// ������Ϸ֡
bool CGame::EnterFrame(DWORD dwTime)
{
    return false;
}

// ��������ƶ��¼�
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
}

// �������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
}
