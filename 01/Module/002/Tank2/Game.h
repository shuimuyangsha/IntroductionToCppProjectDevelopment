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

    //��Ϸ��ͼ����
    //���� �滭 ��Ϸ�еĶ���
    void GameRunDraw();

    // ���fps
    void DrawFps(Graphics &gh);

    // ��¼ ��Ϸÿ�����֡
    int m_fps{ 0 };
};

