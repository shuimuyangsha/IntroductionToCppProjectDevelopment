#pragma once

#include "wall.h"
#include "Bullet.h"
#include "Player.h"
#include "Bot.h"
#include "GameMap.h"
#include "GameMenuPanel.h"
#include "GameMenuPanel2.h"
#include "KeyMenuPlayer02.h"
#include "KeyMenuPlayer01.h"
#include "GameMenuBackground.h"


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

    // ��ǰ��Ϸ�����Ľ׶�
    enum EGameType {
        EGameTypeMenu = 0, // ѡ��׶�
        EGameTypeOne2BotMenu, //���˶Ե��� �˵��׶�
        EGameTypeOne2Bot, //���˶Ե���
        EGameTypeOne2BotEnd, //���˶Ե��� ����
        EGameTypeOne2OneMenu, // ˫�˶�ս �˵��׶�
        EGameTypeOne2One, // ˫�˶�ս
        EGameTypeOne2OneEnd, // ˫�˶�ս ����
        EGameTypeCount, // = 5
    };

    // ���õ�ǰ��Ϸ�����Ľ׶�
    // �����ݲ��� ��ʼ��
    void SetStep(CGame::EGameType step);

private:
    // ����
    HWND m_hWnd;

    /* ��Ϸ��ʼ��
    ���� ��Ϸ���󣬳�ʼ����ͼ������λ�õ�
    */
    bool GameInit();

    // ��Ϸ��ʼ�� : ѡ��׶�
    bool GameInitMenu();

    // ��Ϸ��ʼ�� : ���˶Ե��� �˵��׶�
    bool GameInitOne2BotMenu();

    // ��Ϸ��ʼ�� : ���˶Ե���
    bool GameInitOne2Bot();

    // ��Ϸ��ʼ�� : ���˶Ե��� ����
    bool GameInitOne2BotEnd();

    // ��Ϸ��ʼ�� : ˫�˶�ս �˵��׶�
    bool GameInitOne2OneMenu();

    // ��Ϸ��ʼ�� : ˫�˶�ս
    bool GameInitOne2One();

    // ��Ϸ��ʼ�� : ˫�˶�ս ����
    bool GameInitOne2OneEnd();

    // �����������������飬�������
    bool (CGame::*m_initFunc[EGameTypeCount])() = {
        &CGame::GameInitMenu, // ѡ��׶�
        &CGame::GameInitOne2BotMenu, //
        &CGame::GameInitOne2Bot, // ���˶Ե���
        &CGame::GameInitOne2BotEnd, // ���˶Ե��� ����
        &CGame::GameInitOne2OneMenu, //
        &CGame::GameInitOne2One, // ˫�˶�ս
        &CGame::GameInitOne2OneEnd // ˫�˶�ս ����
    };

    /* ��Ϸ�߼�����:
    1. ά���ӵ�״̬
    2. ά�� ������AI���Զ��ƶ�,�Զ������ӵ�
    3. ά�� ��� ̹�˵�״̬
    �Բ�������� ײǽ���ӵ�����̹��...*/
    void GameRunLogic();

    // ��Ϸ�߼����� : ѡ��׶�
    void GameRunLogicOnMenu();

    // ��Ϸ�߼����� : ���˶Ե��� �˵��׶�
    void GameRunLogicOnOne2BotMenu();

    // ��Ϸ�߼����� : ���˶Ե���
    void GameRunLogicOnOne2Bot();

    // ��Ϸ�߼����� : ���˶Ե��� ����
    void GameRunLogicOnOne2BotEnd();

    // ��Ϸ�߼����� : ˫�˶�ս �˵��׶�
    void GameRunLogicOnOne2OneMenu();

    // ��Ϸ�߼����� : ˫�˶�ս
    void GameRunLogicOnOne2One();

    // ��Ϸ�߼����� : ˫�˶�ս ����
    void GameRunLogicOnOne2OneEnd();

    // �����������������飬�������
    void(CGame::*m_logicFunc[EGameTypeCount])() = {
        &CGame::GameRunLogicOnMenu, // ѡ��׶�
        &CGame::GameRunLogicOnOne2BotMenu, //
        &CGame::GameRunLogicOnOne2Bot, //���˶Ե���
        &CGame::GameRunLogicOnOne2BotEnd, //���˶Ե��� ����
        &CGame::GameRunLogicOnOne2OneMenu, // 
        &CGame::GameRunLogicOnOne2One, // ˫�˶�ս
        &CGame::GameRunLogicOnOne2OneEnd // ˫�˶�ս ����
    };

    /* ��Ϸ��ͼ����
    ���� �滭 ��Ϸ�еĶ���
    */
    void GameRunDraw();

    // ��Ϸ��ͼ���� : ѡ��׶�
    void GameRunDrawOnMenu(Graphics &gh);

    // ��Ϸ��ͼ���� : ���˶Ե��� �˵��׶�
    void GameRunDrawOnOne2BotMenu(Graphics &gh);

    // ��Ϸ��ͼ���� : ���˶Ե���
    void GameRunDrawOnOne2Bot(Graphics &gh);

    // ��Ϸ��ͼ���� : ���˶Ե��� ����
    void GameRunDrawOnOne2BotEnd(Graphics &gh);

    // ��Ϸ��ͼ���� : ˫�˶�ս �˵��׶�
    void GameRunDrawOnOne2OneMenu(Graphics &gh);

    // ��Ϸ��ͼ���� : ˫�˶�ս
    void GameRunDrawOnOne2One(Graphics &gh);

    // ��Ϸ��ͼ���� : ˫�˶�ս ����
    void GameRunDrawOnOne2OneEnd(Graphics &gh);

    // �����������������飬�������
    void(CGame::*m_drawFunc[EGameTypeCount])(Graphics &) = {
        &CGame::GameRunDrawOnMenu, // ѡ��׶�
        &CGame::GameRunDrawOnOne2BotMenu, //
        &CGame::GameRunDrawOnOne2Bot, //���˶Ե���
        &CGame::GameRunDrawOnOne2BotEnd, //���˶Ե��� ����
        &CGame::GameRunDrawOnOne2OneMenu, // 
        &CGame::GameRunDrawOnOne2One, // ˫�˶�ս
        &CGame::GameRunDrawOnOne2OneEnd // ˫�˶�ս ����
    };

private:

    // �Ƴ���ʱ�ӵ�,������Ӧ��̹��װ��
    void RemoveTimeoutBullets();

    // ά���ӵ������У���ײǽ����
    void ProcessHitBullets();

    // ά�����Ե��Զ�Ѱ·����
    void AI();

    // ���fps
    void DrawFps(Graphics &gh);

private:
    // ��¼ ��Ϸÿ�����֡
    int m_fps{ 0 };

    // ��ǰ�׶�:�˵�ѡ��׶�
    EGameType m_eStep{ EGameTypeMenu };

    // ������Ҷ���
    CPlayer m_player01;

    // ��Ҷ���2: ˫�˶�սʱ�Ż��õ�
    CPlayer m_player02;

    // һ������: �˻���սʱ�õ�
    CBot m_bot;

    // �����ڵ�ͼ�����е� �ӵ���������
    std::list<CBullet> m_lstBullets;

    // ��ͼ����
    CGameMap m_map{ 10, 10, 780, 580 };

    // ��ʼ�˵�
    CGameMenuPanel m_menuSelect;

    // ��ʼ�˵�����ͼ
    CGameMenuBackground m_menu;

    // ���ز˵�
    CGameMenuPanel2 m_menuBackup;

    // ��ʾ�����Ĳ˵�
    CKeyMenuPlayer01 m_keymenu01;
    CKeyMenuPlayer02 m_keymenu02;
};

