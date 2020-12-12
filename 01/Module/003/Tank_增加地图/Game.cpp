#include "stdafx.h"
#include "Game.h"
#include <algorithm>


CGame::CGame()
{
    GameInitOne2Bot();
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
    GameRunLogic();
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
        // ������ͼ
        m_map.Draw(gh);
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
                if(m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if(KEYDOWN(VK_DOWN))
        {
            // ̹��ײǽ�����
            {
                if(m_map.IsHitTheWall(m_player01, false)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Backward();
                }
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
        blt.Move(); // �ӵ���ǰ�ƶ�
    }

    // �Ƴ���ʱ���ӵ�
    {
        auto itRemove = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(), [](CBullet & blt)->bool {return blt.IsTimeout(); });
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

    // �ӵ�ײǽ����
    {
        // �ӵ��Ƿ�ײ��ǽ : ���ײ���ˣ��ı䷽��ȵ�
        for(auto &blt : m_lstBullets) {
            // ����ײǽ����
            m_map.HitWallProcess(blt);
            blt.Move();
        }
    }

    // ����ӵ��Ƿ����̹�� :  ����Ҫʹ̹�˱�ը(�ӵ��ɲ����Ƴ��ˣ���Ϊ��Ϸ��������)
    for (auto &blt : m_lstBullets) {
        // �������1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
        }
        break;
    }
}


// ��Ϸ��ʼ�� : ���˶Ե���
bool CGame::GameInitOne2Bot()
{
    {
        // ��ͼ
        m_map.LoadMap();
        // ���һ
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if(!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("����Player01λ��ʧ��"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }

        // �ӵ�
        m_lstBullets.clear();
    }
    return true;
}




