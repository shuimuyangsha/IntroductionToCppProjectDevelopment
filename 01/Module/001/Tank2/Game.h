#pragma once
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
};

