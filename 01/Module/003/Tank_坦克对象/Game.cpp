#include "stdafx.h"
#include "Game.h"
#include <algorithm>


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
    GameRunLogic(); // ��Ϸ�߼�����
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
        // ��̹��(���һ)
        m_player01.Draw(gh);
        // ���ӵ�(���д����ڵ�ͼ�ϵ�̹��)
        for(auto &blt : m_lstBullets) {
            blt.Draw(gh);
        }
    }

    // ��������Ļ
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // �ͷ�
    dc->DeleteDC();
    return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
    //��������
    {
        if(KEYDOWN(VK_LEFT))
        {
            m_player01.RotateLeft();
        }
        if(KEYDOWN(VK_RIGHT))
        {
            m_player01.RotateRight();
        }
        if(KEYDOWN(VK_UP))
        {
            // ̹��ײǽ�����
            {
                m_player01.Forward();
            }
        }
        if(KEYDOWN(VK_DOWN))
        {
            {
                m_player01.Backward();
            }
        }
        if(KEYDOWN('M'))
        {
            CBullet blt;
            if(m_player01.Fire(blt)) {
                // ���뵽��ͼ�б���
                m_lstBullets.push_back(blt);
            }
        }
    }

    // �����ӵ�������ƶ�
    for(auto &blt : m_lstBullets) {
        blt.Move();  // �ӵ���ǰ�ƶ�
    }

    // �Ƴ���ʱ���ӵ�
    {
        auto itRemove = std::remove_if(m_lstBullets.begin(),
                                       m_lstBullets.end(),
            [] (CBullet & blt)->bool {
                return blt.IsTimeout(); 
            });
        for(auto it = itRemove; it != m_lstBullets.end(); ++it) {
            // ����Ϊ��Ч
            it->SetActive(false);
            // �ӵ�ͼ�б����Ƴ�
            // ����Ӧ��̹�������ӵ�
            it->GetOwner()->AddBullet(*it);
        }
        // �ӱ��� ɾ�� �ӵ�
        m_lstBullets.erase(itRemove, m_lstBullets.end());
    }
}

