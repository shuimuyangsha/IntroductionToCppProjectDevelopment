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
}

// �������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
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
    DrawFps(gh);

    /*�����ע��һ��Ҫд,�������������ʱ,�Ҳ���λ��*/
    // ����һ��ͼƬ
    {
        // ��� ��Ϸ���ڴ�С
        CRect rc;
        GetClientRect(m_hWnd, rc);

        // ����Ҫ���Ƶ�ͼƬ
        Gdiplus::Image * img = Image::FromFile(_T("menu_background.png"));

        //����ͼƬ
        gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());
    }


    // ��������Ļ
    ::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
    // �ͷ�
    dc->DeleteDC();
    return;
}

// �� fps
void CGame::DrawFps(Graphics &gh)
{
    static int fps = 0;
    // ��¼�������˶���֡
    m_fps++;
    // ��¼�ϴ����fps��ʱ��
    static DWORD dwLast = GetTickCount();
    // ����ʱ���Ƿ񳬹� 1�룬������������ fps
    if(GetTickCount() - dwLast >= 1000) {
        fps = m_fps;
        // ���㣬�����֡�������¼���
        m_fps = 0;
        // ��¼���������ʱ��
        dwLast = GetTickCount();
    }

    // ���fps
    {
        // ��fsp ��ʽ���� �ַ���
        CString s;
        s.Format(_T("FPS:%d"), fps);
        // ������ɫ�Ļ�ˢ
        SolidBrush brush(Color(0x00, 0x00, 0xFF));
        // �������������
        Gdiplus::Font font(_T("����"), 10.0);
        // ���������ڵĴ�С��������λ ���ֵ����λ��
        CRect rc;
        ::GetClientRect(m_hWnd, &rc);
        // �����Ͻ���ʾ
        PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
        // ��ʽ������
        gh.DrawString(s.GetString(), -1, &font, origin, &brush);
    }
}








