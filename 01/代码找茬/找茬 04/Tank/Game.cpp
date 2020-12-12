#include "stdafx.h"
#include "Game.h"

#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

CGame::CGame()
{
    m_menuSelect.m_pParent = this;
    m_menuBackup.m_pParent = this;
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
    // ѡ��׶�
    if (m_eStep == EGameTypeMenu) {
        // ѡ����Ϸ����
        m_menuSelect.OnMouseMove(nFlags, point);
    }
    else {
        // �������˵�
        m_menuBackup.OnMouseMove(nFlags, point);
    }
}

// �������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
    // ѡ��׶�
    if (m_eStep == EGameTypeMenu) {
        // ѡ����Ϸ����
        m_menuSelect.OnLButtonUp(nFlags, point);
    }
    else {
        // �������˵�
        m_menuBackup.OnLButtonUp(nFlags, point);
    }
}

// ���õ�ǰ��Ϸ�����Ľ׶�
// �����ݲ��� ��ʼ��
void CGame::SetStep(CGame::EGameType step)
{
    m_eStep = step;
    GameInit();
}

/* ��Ϸ��ʼ��
���� ��Ϸ���󣬳�ʼ����ͼ������λ�õ�
*/
bool CGame::GameInit()
{
    //  ��ʼ�������������
    srand(GetTickCount());
    // ���ݲ�ͬ�׶ε��ò�ͬ�Ĵ�����
    return (this->*m_initFunc[m_eStep])();
}

// ��Ϸ��ʼ�� : ѡ��׶�
bool CGame::GameInitMenu()
{
    return true;
}

// ��Ϸ��ʼ�� : ���˶Ե��� �˵��׶�
bool CGame::GameInitOne2BotMenu()
{
    // ���� ���һ�� keyMenu λ�� : ��Ļ���м�
    RECT rc;
    GetWindowRect(m_hWnd, &rc);
    PointF pt;
    pt.X = rc.left + (rc.right - rc.left) / 2.0f;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu01.SetCenterPoint(pt);
    m_keymenu01.SetStop(false);
    return true;
}

// ��Ϸ��ʼ�� : ���˶Ե���
bool CGame::GameInitOne2Bot()
{
    for (; ;) {
        // ��ͼ
        m_map.LoadMap();
        // ���һ
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("����Player01λ��ʧ��"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }

        // �о�
        {
            m_bot = CBot(0, 0, _T("tank_bot.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("����Botλ��ʧ��"));
            }
            else {
                m_bot.SetCenterPoint(ptCenter);
            }
        }
        // �ӵ�
        m_lstBullets.clear();

        // �ж��Ƿ�Ϸ�
        {
            // �����ˣ���� ���ڵ�λ��
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_bot, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                AfxMessageBox(_T("��ȡ̹��λ�÷�������"));
                goto __Init_End;
            }
            VPath path;
            m_map.FindPath(startX, startY, targetX, targetY, path);
            if (!path.empty()) {
                goto __Init_End;
            }
        }
    }
__Init_End:
    return true;
}

// ��Ϸ��ʼ�� : ���˶Ե��� ����
bool CGame::GameInitOne2BotEnd()
{

    return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս �˵��׶�
bool CGame::GameInitOne2OneMenu()
{
    // ���� ������ҵ� keyMenu λ�� : ��Ļ���м�
    RECT rc;
    GetWindowRect(m_hWnd, &rc);
    PointF pt;
    pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu01.SetCenterPoint(pt);
    m_keymenu01.SetStop(false);


    pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
    pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
    m_keymenu02.SetCenterPoint(pt);
    m_keymenu02.SetStop(false);

    return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս
bool CGame::GameInitOne2One()
{
    for (;;) {
        // ��ͼ
        m_map.LoadMap();
        //�м����̹��
        {
            m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("����Player01λ��ʧ��"));
            }
            else {
                m_player01.SetCenterPoint(ptCenter);
            }
        }
        {
            m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
            PointF ptCenter;
            if (!m_map.FindRandomPosition(ptCenter)) {
                AfxMessageBox(_T("����Player02λ��ʧ��"));
            }
            else {
                m_player02.SetCenterPoint(ptCenter);
            }
        }

        // �ӵ�
        m_lstBullets.clear();
        // �ж��Ƿ�Ϸ�
        {
            // �����ˣ���� ���ڵ�λ��
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_player02, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                AfxMessageBox(_T("��ȡ̹��λ�÷�������"));
                break;
            }
            VPath path;
            m_map.FindPath(startX, startY, targetX, targetY, path);
            if (!path.empty()) {
                break;
            }
        }
    }
    return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս ����
bool CGame::GameInitOne2OneEnd()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////

/* ��Ϸ�߼�����:
1. ά���ӵ�״̬
2. ά�� ������AI���Զ��ƶ�,�Զ������ӵ�
3. ά�� ��� ̹�˵�״̬
�Բ�������� ײǽ���ӵ�����̹��...*/
void CGame::GameRunLogic()
{
    // ���ݲ�ͬ�׶ε��ò�ͬ�Ĵ�����
    (this->*m_logicFunc[m_eStep])();
}

// ��Ϸ�߼����� : ѡ��׶�
void CGame::GameRunLogicOnMenu()
{
    // ʲôҲ��������û��ʼ��Ϸ
}

// ��Ϸ�߼����� : ���˶Ե��� �˵��׶�
void CGame::GameRunLogicOnOne2BotMenu()
{
    // ���������M����ֹͣ����״̬
    if (KEYDOWN('M')) {
        m_keymenu01.SetStop();
    }

    // ����������ˣ���ʽ��ʼ��Ϸ
    if (m_keymenu01.GetStop()) {
        SetStep(EGameTypeOne2Bot);
    }
}

// ��Ϸ�߼����� : ���˶Ե���
void CGame::GameRunLogicOnOne2Bot()
{
    // ״̬ά��
    // �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
    RemoveTimeoutBullets();

    // ����ӵ��Ƿ����̹�� :  ����Ҫʹ̹�˱�ը(�ӵ��ɲ����Ƴ��ˣ�
    // ��Ϊ��Ϸ��������)
    for (auto &blt : m_lstBullets) {
        // ���л�����
        if (m_bot.IsHitted(blt)) {
            m_bot.Bomb();
            // ��Ϸ����
            m_eStep = EGameTypeOne2BotEnd;
            blt.SetActive(false);
        }
        // �������1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
            // ��Ϸ����
            m_eStep = EGameTypeOne2BotEnd;
            blt.SetActive(false);
        }
        break;
    }

    // �ӵ��˶�ά��
    ProcessHitBullets();

    AI();

    //��������
    {
        if (KEYDOWN(VK_LEFT)) {
            m_player01.RotateLeft();
        }
        if (KEYDOWN(VK_RIGHT)) {
            m_player01.RotateRight();
        }
        if (KEYDOWN(VK_UP)) {
            // ̹��ײǽ�����
            {
                if (m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if (KEYDOWN(VK_DOWN)) {
            {
                // ̹��ײǽ�����
                {
                    if (m_map.IsHitTheWall(m_player01, false)) {
                        m_player01.ChangeDirection(true);
                    }
                    else {
                        m_player01.Backward();
                    }
                }
            }
        }
        if (KEYDOWN('M')) {
            CBullet blt;
            if (m_player01.Fire(blt)) {
                // ���뵽��ͼ�б���
                m_lstBullets.push_back(blt);
            }
        }
        if (KEYDOWN('I')) {
            // �����ˣ���� ���ڵ�λ��
            int startX, startY, targetX, targetY;
            if (!m_map.FindObjPosition(m_bot, startX, startY) || 
                !m_map.FindObjPosition(m_player01, targetX, targetY)) {
                return;
            }
            float fDirNext = 0; //��������һ���ķ���
            if (!m_map.FindNextDirection(&fDirNext, startX, startY, 
                targetX, targetY)) {
                return;
            }

            PointF ptTankCenter = m_bot.GetCenterPoint();
            PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
            RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


            // �ж� ̹���Ƿ񼺾��ߵ������ĵ�λ����
            if (!rc.Contains(ptTankCenter)) {
                m_bot.Forward(); // û�е������ĵ㣬����ǰ��
                return;
            }
            else {
                m_bot.SetDirection(fDirNext);
                m_bot.Forward();
            }
        }
    }

}

// ��Ϸ�߼����� : ���˶Ե��� ����
void CGame::GameRunLogicOnOne2BotEnd()
{
    //��������
    // ���ٽ��ܰ���


    // ״̬ά��
    // �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
    RemoveTimeoutBullets();


    // �ӵ��Ƿ�ײ��ǽ : ���ײ���ˣ��ı䷽��ȵ�
    ProcessHitBullets();
}

// ��Ϸ�߼����� : ˫�˶�ս �˵��׶�
void CGame::GameRunLogicOnOne2OneMenu()
{
    // ���������M����ֹͣ����״̬
    if (KEYDOWN('M')) {
        m_keymenu01.SetStop();
    }
    // ���������Q����ֹͣ����״̬
    if (KEYDOWN('Q')) {
        m_keymenu02.SetStop();
    }

    // ����������ˣ���ʽ��ʼ��Ϸ
    if (m_keymenu01.GetStop() && m_keymenu02.GetStop()) {
        SetStep(EGameTypeOne2One);
    }
}

// ��Ϸ�߼����� : ˫�˶�ս
void CGame::GameRunLogicOnOne2One()
{
    //��������
    {
        // ���һ
        if (KEYDOWN(VK_LEFT)) {
            m_player01.RotateLeft();
        }
        if (KEYDOWN(VK_RIGHT)) {
            m_player01.RotateRight();
        }
        if (KEYDOWN(VK_UP)) {
            // ̹��ײǽ�����
            {
                if (m_map.IsHitTheWall(m_player01, true)) {
                    m_player01.ChangeDirection(true);
                }
                else {
                    m_player01.Forward();
                }
            }
        }
        if (KEYDOWN(VK_DOWN)) {
            {
                // ̹��ײǽ�����
                {
                    if (m_map.IsHitTheWall(m_player01, false)) {
                        m_player01.ChangeDirection(false);
                    }
                    else {
                        m_player01.Backward();
                    }
                }
            }
        }
        if (KEYDOWN('M')) {
            CBullet blt;
            if (m_player01.Fire(blt)) {
                m_lstBullets.push_back(blt);
            }
        }
        // ��Ҷ�
        if (KEYDOWN('A')) {
            m_player02.RotateLeft();
        }
        if (KEYDOWN('D')) {
            m_player02.RotateRight();
        }
        if (KEYDOWN('W')) {
            // ̹��ײǽ�����
            {
                if (m_map.IsHitTheWall(m_player02, true)) {
                    m_player02.ChangeDirection(true);
                }
                else {
                    m_player02.Forward();
                }
            }
        }
        if (KEYDOWN('S')) {
            {
                // ̹��ײǽ�����
                {
                    if (m_map.IsHitTheWall(m_player02, false)) {
                        m_player02.ChangeDirection(false);
                    }
                    else {
                        m_player02.Backward();
                    }
                }
            }
        }
        if (KEYDOWN('Q')) {
            CBullet blt;
            if (m_player02.Fire(blt)) {
                m_lstBullets.push_back(blt);
            }
        }
        if (KEYDOWN('Z')) {
            if (m_map.IsCanKillTarget(m_player01, m_player02)) {
                AfxMessageBox(_T("���Դ�"));
            }
        }
    }

    // ���ж�״̬
    // �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
    RemoveTimeoutBullets();

    // ����ӵ��Ƿ����̹�� :  ����Ҫʹ̹�˱�ը(�ӵ��ɲ����Ƴ��ˣ���Ϊ��Ϸ��������)
    for (auto &blt : m_lstBullets) {
        if (!blt.IsActive()) {
            continue;
        }
        // �������1
        if (m_player01.IsHitted(blt)) {
            m_player01.Bomb();
            // ��Ϸ����
            m_eStep = EGameTypeOne2OneEnd;
            blt.SetActive(false);
        }
        // �������2
        if (m_player02.IsHitted(blt)) {
            m_player02.Bomb();
            // ��Ϸ����
            m_eStep = EGameTypeOne2OneEnd;
            blt.SetActive(false);
        }
    }

    // ײǽ
    ProcessHitBullets();
}

// ��Ϸ�߼����� : ˫�˶�ս ����
void CGame::GameRunLogicOnOne2OneEnd()
{
    //��������
    // ����Ҫ��������

    // �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
    RemoveTimeoutBullets();

    // ײǽ
    ProcessHitBullets();
}

////////////////////////////////////////////////////////////////////////////////

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
    (this->*m_drawFunc[m_eStep])(gh);

    // ��������Ļ
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), 
        m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // �ͷ�
    ::ReleaseDC(m_hWnd, hdc);
    return;
}

// ѡ��׶�
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
    m_menuSelect.Draw(gh);
}

//���˶Ե��� : �˵��׶�
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
    // ������
    m_menu.Draw(gh);

    // ���˵�
    m_keymenu01.Draw(gh);
}

//���˶Ե���
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
    // �˵�
    m_menuBackup.Draw(gh);
    // ǽ
    m_map.Draw(gh);

    // ���
    m_player01.Draw(gh);

    // ������
    m_bot.Draw(gh);

    // ���ӵ� :  ���������
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }

    // ��� : FPS
    DrawFps(gh);
}

//���˶Ե��� ����
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
    // �˵�
    m_menuBackup.Draw(gh);
    // ǽ
    m_map.Draw(gh);
    // ���
    m_player01.Draw(gh);
    // ������
    m_bot.Draw(gh);
    // ���ӵ� :  ���������
    //for(auto b : m_lstBullets) {
    //    b.Draw(gh);
    //}

    // ��� : FPS
    DrawFps(gh);

    // �ж���Ϸ�������
    if (m_player01.IsBombEnd() || m_bot.IsBombEnd()) {
        m_eStep = EGameTypeMenu;
    }
}

// ˫�˶�ս : �˵��׶�
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
    // ������
    m_menu.Draw(gh);

    // ���˵�
    m_keymenu01.Draw(gh);
    m_keymenu02.Draw(gh);
}

// ˫�˶�ս
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
    // �˵�
    m_menuBackup.Draw(gh);
    // ǽ
    m_map.Draw(gh);

    // ���һ
    m_player01.Draw(gh);

    // ��Ҷ�
    m_player02.Draw(gh);

    // ���ӵ� :  ���������
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }

    // ��� : FPS
    DrawFps(gh);
}

// ˫�˶�ս ����
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
    // �˵�
    m_menuBackup.Draw(gh);
    // ǽ
    m_map.Draw(gh);
    // ���һ
    m_player01.Draw(gh);
    // ��Ҷ�
    m_player02.Draw(gh);
    // ���ӵ� :  ���������
    for (auto b : m_lstBullets) {
        b.Draw(gh);
    }
    // ��� : FPS
    DrawFps(gh);

    // �ж���Ϸ�������
    if (m_player01.IsBombEnd() || m_player02.IsBombEnd()) {
        m_eStep = EGameTypeMenu;
    }
}

// �� fps
void CGame::DrawFps(Graphics &gh)
{
    static int fps = 0;
    m_fps++;
    static DWORD dwLast = GetTickCount();
    if (GetTickCount() - dwLast >= 1000) {
        fps = m_fps;
        m_fps = 0;
        dwLast = GetTickCount();
    }

    // ���fps
    {
        CString s;
        s.Format(_T("FPS:%d"), fps);
        SolidBrush brush(Color(0x00, 0x00, 0xFF));
        Gdiplus::Font font(_T("����"), 10.0);
        CRect rc;
        ::GetClientRect(m_hWnd, &rc);
        // �����Ͻ���ʾ
        PointF origin(static_cast<float>(rc.right - 50), 
            static_cast<float>(rc.top + 2));
        gh.DrawString(s.GetString(), -1, &font, origin, &brush);
    }
}

// �Ƴ���ʱ�ӵ�,������Ӧ��̹��װ��
void CGame::RemoveTimeoutBullets()
{
    // ������Һ���
    auto itRemove = std::remove_if(m_lstBullets.begin(), 
        m_lstBullets.end(),
        [](CBullet & blt)->bool {return blt.IsTimeout(); });

    // ���ӵ��Ƴ�,������Ӧ��̹�������ӵ�,
    for (auto it = itRemove; it != m_lstBullets.end(); ++it) {
        // ����Ϊ��Ч
        it->SetActive(false);
        // �ӵ�ͼ�б����Ƴ�
        // ����Ӧ��̹�������ӵ�
        it->GetOwner()->AddBullet(*it);
    }
    // �ӱ��� ɾ�� �ӵ�
    m_lstBullets.erase(itRemove, m_lstBullets.end());
}

// �ӵ��˶���ά��:ײǽ����
void CGame::ProcessHitBullets()
{
    // �ӵ��Ƿ�ײ��ǽ : ���ײ���ˣ��ı䷽��ȵ�
    for (auto &blt : m_lstBullets) {
        // ����ײǽ����
        m_map.HitWallProcess(blt);
        blt.Move();
    }
}

// ά�����Ե��Զ�Ѱ·����
void CGame::AI()
{
    // �����˶�״̬̬ά��
    static CGameTimer acTimer(-1, 150);
    if (acTimer.IsTimeval()) {
        // �����ˣ���� ���ڵ�λ��
        int startX, startY, targetX, targetY;
        if (!m_map.FindObjPosition(m_bot, startX, startY) || 
            !m_map.FindObjPosition(m_player01, targetX, targetY)) {
            return;
        }
        float fDirNext = 0; //��������һ���ķ���
        if (!m_map.FindNextDirection(&fDirNext, 
            startX, startY, 
            targetX, targetY)) {
            return;
        }

        PointF ptTankCenter = m_bot.GetCenterPoint();
        PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
        RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


        // �ж� ̹���Ƿ񼺾��ߵ������ĵ�λ����
        if (!rc.Contains(ptTankCenter)) {
            m_bot.Forward(); // û�е������ĵ㣬����ǰ��
            return;
        }
        else {
            m_bot.SetDirection(fDirNext);
            float dir;
            if (m_map.IsCanKillTarget(m_bot, m_player01, &dir)) {
                CBullet blt;
                if (m_bot.Fire(blt)) {
                    m_lstBullets.push_back(blt);
                }
                return;
            }
            m_bot.Forward();
        }
    }
}
