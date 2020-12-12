#pragma once
#include "Shooter.h"

/*
    ����/���
*/


class CShooter2 :
    public CShooter
{
public:
    CShooter2();
    virtual ~CShooter2();
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

    // ��ǰ����
    enum class EStatus {EStatusHammer, EStatusSaw} ;
    EStatus m_status{EStatus::EStatusHammer};

    // ����״̬
    enum class EStatusHammer {EStatusHammerDownFirst,/*��������0*/
                              EStatusHammerDown,/*��������*/
                              EStatusHammerUp, /*����̧��*/
                             };
    EStatusHammer m_eStatusHammer{ EStatusHammer::EStatusHammerDown };

    // ���״̬
    enum class EStatusSaw {EStatusSawStart, EStatusMoving, EStatusSawStop};
    EStatusSaw m_eStatusSaw{EStatusSaw::EStatusSawStop};
    // ���������ٶ�
    float m_sawSpeed{10.0f};
    // ���з���
    float m_sawDegree{0.0};
    // ��ǰλ��
    PointF m_sawPos{0.0f, 0.0f};

    // �ϴα任�Ƕ�����λ��
    PointF m_mousePosLast{0.0f, 0.0f};

    // ��ǰ������ڵ�λ��
    PointF m_mousePos{0.0f, 0.0f};

    // ���ߵ�ͼƬ
    Image *m_img0;
    Image *m_img1;
    Image *m_img2;

    // ������� ���壬��ˢ
    Gdiplus::Font m_font{L"����", 20.0f};
    SolidBrush m_brush{Color::Red};

};

