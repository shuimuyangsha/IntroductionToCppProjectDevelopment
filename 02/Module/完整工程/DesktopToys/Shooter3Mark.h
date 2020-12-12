#pragma once
#include "IMark.h"

/*
�����ˣ������ڴ��������µĺۼ�
*/

class CShooter3Mark :
    public IMark
{
public:
    CShooter3Mark(float x, float y);
    virtual ~CShooter3Mark();
    // ���Լ�
    virtual void Draw(Gdiplus::Graphics &gh);
    // ״̬�Ƿ��ڱ仯
    virtual bool IsChanging() const;

private:
    // �̶�ͼƬ
    Image *m_img;
    // ͼƬ�ĽǶ�
    float m_degree;
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
    std::vector<SDynamic> m_vD;
};

