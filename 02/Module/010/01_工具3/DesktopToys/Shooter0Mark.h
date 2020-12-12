#pragma once
#include "IMark.h"

/*
    �����ˣ������ڴ��������µĺۼ�
*/

class CShooter0Mark :
    public IMark
{
public:
    CShooter0Mark(float x, float y);
    virtual ~CShooter0Mark();

    // ���Լ�
    virtual void Draw(Gdiplus::Graphics &gh);

    // ״̬�Ƿ��ڸı���
    virtual bool IsChanging() const;

private:
    // ͼƬ
    std::vector<Image *> m_img;
    //�ڼ���ͼƬ
    size_t m_index{ 0 };
    //��ת�ĽǶ�
    float m_dir;
};

