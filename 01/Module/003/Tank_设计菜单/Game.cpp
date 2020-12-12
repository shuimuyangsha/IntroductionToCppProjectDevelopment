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
    GameRunDraw();
    return false;
}

// ��������ƶ��¼�
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
    // ѡ����Ϸ����
    m_menuSelect.OnMouseMove(nFlags, point);
}

// �������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // ѡ����Ϸ����
    m_menuSelect.OnLButtonUp(nFlags, point);
}

// ��Ϸ��ͼ
void CGame::GameRunDraw()
{
    HDC hdc = ::GetDC(m_hWnd);
    // �ͻ����Ĵ�С
    CRect rc;
    GetClientRect(m_hWnd, &rc);

    CDC *dc = CClientDC::FromHandle(hdc);

    // ˫�����ͼ��
    CDC m_dcMemory;
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
    m_dcMemory.CreateCompatibleDC(dc);
    CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

    // �������
    Graphics gh(m_dcMemory.GetSafeHdc());
    // �������
    gh.Clear(Color::White);
    gh.ResetClip();

    // �����ڴ�
    {
        // ������
        m_menu.Draw(gh);

        // ���˵�
        m_menuSelect.Draw(gh);
    }

    // ��������Ļ
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // �ͷ�
    dc->DeleteDC();
    return;
}

