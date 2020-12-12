#pragma once

#include "GameMenuPanel.h"
#include "GameMenuBackground.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include "GameMap.h"


class CGame
{
public:
    CGame();
    ~CGame();

    // ����������ڵľ��
    void SetHandle(HWND hWnd);

    // ������Ϸ֡
    bool EnterFrame(DWORD dwTime);

    // ��������ƶ��¼�
    void OnMouseMove(UINT nFlags, CPoint point);

    // �������̧���¼�
    void OnLButtonUp(UINT nFlags, CPoint point);

private:
    // ����
    HWND m_hWnd;

    /* ��Ϸ��ͼ����
    ���� �滭 ��Ϸ�еĶ���
    */
    void GameRunDraw();

    /* ��Ϸ�߼�����:
     * 1. ά���ӵ�״̬
     * 2. ά�� ������AI���Զ��ƶ�,�Զ������ӵ�
     * 3. ά�� ��� ̹�˵�״̬
     * �Բ�������� ײǽ���ӵ�����̹��...
    **/
    void GameRunLogic();


    // ��ʼ�˵�
    CGameMenuPanel m_menuSelect;

    // ��ʼ�˵�����ͼ
    CGameMenuBackground m_menu;

    // ������Ҷ���
    CPlayer m_player01{100, 100, _T("tank_player1.png")};

    // ��Ҷ���2: ˫�˶�սʱ�Ż��õ�
    CPlayer m_player02{200, 100, _T("tank_player2.png")};

    // һ������: �˻���սʱ�õ�
    CBot m_bot{300, 200, _T("tank_bot.png")};

    // �����ڵ�ͼ�����е� �ӵ���������
    std::list<CBullet> m_lstBullets;

    // ��ͼ����
    CGameMap m_map{ 10, 10, 780, 580 };

    // ��Ϸ��ʼ�� : ���˶Ե���
    bool CGame::GameInitOne2Bot();
};

