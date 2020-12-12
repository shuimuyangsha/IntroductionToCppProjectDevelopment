#pragma once

/*
    ������ӳ����¼��Ķ���,����ɸ��Ǳ���ķ�����ʵ�ֲ��컯����
*/
class IMouseAction
{
public:
    IMouseAction();
    virtual ~IMouseAction();

    /// ////////////////////////////////////////////////////////////////////////
    /// ���������Ϣ : ��Ҫ�������д���
    /// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
    // ����������
    virtual bool OnLButtonDown(UINT nFlags, CPoint point) { return false; }
    // ������̧��
    virtual bool OnLButtonUp(UINT nFlags, CPoint point) { return false; }
    // ������˫��
    virtual bool OnLButtonDblClk(UINT nFlags, CPoint point) { return false; }
    // ����Ҽ�����
    virtual bool OnRButtonDown(UINT nFlags, CPoint point) { return false; }
    // ����Ҽ�̧��
    virtual bool OnRButtonUp(UINT nFlags, CPoint point) { return false; }
    // ����Ҽ�˫��
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point) { return false; }
    // ����ƶ�
    virtual bool OnMouseMove(UINT nFlags, CPoint point) { return false; }
};

