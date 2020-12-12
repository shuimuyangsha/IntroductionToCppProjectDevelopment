#pragma once
#include "Shooter.h"
#include "GameTimer.h"

/*
    ��������
*/


class CShooter0 :
    public CShooter
{
public:
    CShooter0();
    virtual ~CShooter0();
    // ���� �Լ���ǰ�����Ի��Լ�
    virtual void Draw(Gdiplus::Graphics &gh);
    /// ////////////////////////////////////////////////////////////////////////
    /// ���������Ϣ : ��Ҫ�������д���
    /// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
    // ����������
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);

    // ������̧��
    virtual bool OnLButtonUp(UINT nFlags, CPoint point)
    {
        return false;
    }

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
    virtual bool OnMouseMove(UINT nFlags, CPoint point)
    {
        return false;
    }

private:
    std::vector<Image *> m_vImage;
    // ��ǰ���ڵĽ׶�
    typedef enum {EStatus0 = 0,

                  EStatusMove0, // �ƶ� ����
                  EStatusMove1, // �ƶ� ������
                  EStatusMove2, // �ƶ� ����
                  EStatusMove3, // �ƶ� ������
                  EStatusMove4, // �ƶ� ����
                  EStatusMove5, // �ƶ� ������
                  EStatusMove6, // �ƶ� ����
                  EStatusMove7, // �ƶ� ������

                  EStatusFireLeft, // ���� ����
                  EStatusFireRight, // ���� ����
                 } EStatus;
    EStatus m_eStatus{EStatus0};

    // ��ǰ��ʾ��ͼƬ
    size_t m_index{0};

    // �޶���ʱ�����»ζ�
    CGameTimeval m_timer0{500};

    // ��¼��ǰ�����ϣ���������
    bool m_bUp{ false };


    /* ��ĳ�������ƶ�
        ��ʼ�ƶ�
        �����ָ���ֹ״̬
    */
    void DrawMove(Gdiplus::Graphics &gh);

    // ����Ҽ������λ��
    PointF m_moveTo;

    // �ƶ��ķ���
    float m_moveDir;


    /* ���𶯻����� : ������ 4 ֡����
        // 0 ԭ��ת��
        // 1 �ۼ�����
        // 2 �ۼ����� 
        // 3 �����ʼ
        // 4 �����
        // 5 ������
    */

    // ��֡ͼƬ֮���ʱ����
    CGameTimeval m_timerFire{100};

    // ��ǰ��ʾ�ڼ���ͼƬ
    size_t m_index_fire{0}; 

    // ���ƿ��𶯻�
    void DrawFire(Gdiplus::Graphics &gh);

    // ����Ҽ������λ��
    PointF m_fireTo;
};

