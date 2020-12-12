#pragma once
#include "IMark.h"

/*
    �����ڴ��������µĺۼ�
*/

class CShooter2Mark :
    public IMark
{
public:
    CShooter2Mark(float x, float y);
    virtual ~CShooter2Mark();

    // ���Լ�
    virtual void Draw(Gdiplus::Graphics &gh);

    // ״̬�Ƿ��ڱ仯
    virtual bool IsChanging() const;

private:
    // �̶�ͼƬ
    Image *m_img;
    // ��̬����Ķ���
    typedef struct {
        Image *m_img2;
        // λ��
        PointF m_pos;
        // �ٶ�(��������)
        PointF m_speed{ 10.0f, 15.0f };
        // ��ǰ�Ƕ�(����������ת)
        float m_dir{ 0.0f };
        // �Ƿ���ʾ
        bool m_bShow{ true };
    } SDynamic;

    // ��̬����Ķ�������
    std::vector<SDynamic> m_vD;
};

