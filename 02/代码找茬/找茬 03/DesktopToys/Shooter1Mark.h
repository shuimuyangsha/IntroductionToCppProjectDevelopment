#pragma once
#include "IMark.h"

/*
    �����ˣ������ڴ��������µĺۼ�
*/

class CShooter1Mark :
    public IMark
{
public:
    CShooter1Mark(float x, float y, int index=0);
    virtual ~CShooter1Mark();
    // ���Լ�
    virtual void Draw(Gdiplus::Graphics &gh);
    // ״̬�Ƿ�仯��
    virtual bool IsChanging() const;


private:
    // ͼƬ
    std::vector<Image*> m_img;
    //�ڼ���ͼƬ
    size_t m_index{0}; 
};

