#pragma once
#include "Shooter.h"

/*
    ��Ƥ����
*/


class CShooter3 :
    public CShooter
{
public:
    CShooter3();
    virtual ~CShooter3();
    // ���� �Լ���ǰ�����Ի��Լ�
    virtual void Draw(Gdiplus::Graphics &gh);
    /// ////////////////////////////////////////////////////////////////////////
    /// ���������Ϣ : ��Ҫ�������д���
    /// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
    // ����������
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);

    // ������̧��
    virtual bool OnLButtonUp(UINT nFlags, CPoint point);

    // ������˫��
    virtual bool OnLButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // ����Ҽ�����
    virtual bool OnRButtonDown(UINT nFlags, CPoint point);

    // ����Ҽ�̧��
    virtual bool OnRButtonUp(UINT nFlags, CPoint point);

    // ����Ҽ�˫��
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // ����ƶ�
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

private:

    enum class EStatus {EStatusLeftUp/*���̧��*/, EStatusLeftDown/*�������*/};
    EStatus m_status{EStatus::EStatusLeftUp};
    bool m_bRightDown{ false };

    // ��ǰ������ڵ�λ��
    PointF m_mousePos{0.0f, 0.0f};

    // ������̧��ͼƬ
    std::vector<Image *> m_vImgUp;

    // ����������ͼƬ
    std::vector<Image *> m_vImgDown;
};

