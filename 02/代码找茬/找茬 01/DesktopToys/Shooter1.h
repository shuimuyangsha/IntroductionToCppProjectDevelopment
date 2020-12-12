#pragma once
#include "Shooter.h"

/*
    ��ˢ��
*/

class CShooter1 :
    public CShooter
{
public:
    CShooter1(int height);
    virtual ~CShooter1();
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
    virtual bool OnRButtonUp(UINT nFlags, CPoint point)
    {
        return false;
    }

    // ����Ҽ�˫��
    virtual bool OnRButtonDblClk(UINT nFlags, CPoint point)
    {
        return false;
    }

    // ����ƶ�
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

private:

    // ���ӵĸ߶�
    int m_nHeight{1024};


    // 0:����; 1:��,2:�һ�ͷ; 3:��,4���ͷ;5:��ˢ
    int m_status{ 1 };
    // ǰһ�ε�״̬
    int m_statusLast{1};

    // ��ǰ������ڵ�λ��
    PointF m_mousePos{0.0f, 0.0f};

    // ��һ���滭֮ǰ������ڵ�λ��
    PointF m_mousePosLast{0.0f, 0.0f};

    // ��������ƶ���С��Ҳ�����ƶ�:
    // �����¼�ϴλ���ʱ��λ��,����Ҫ���ȵ��������
    PointF m_lastChangePos{0.0f, 0.0f};

    // �仯�ľ���(����仯�������룬����,����ʾ��һ��ͼƬ)
    const float m_distanceChange{23.0f};

    // ��¼��ǰ����ɫ[0, 6)
    int m_iColorIndex{ 0 };

    //���ӵ�ͼƬ
    Image *m_img0;
    Image *m_img1;
    Image *m_img2;
    Image *m_img3;
    Image *m_img4;
    // �滭
    Image *m_img5;
};

