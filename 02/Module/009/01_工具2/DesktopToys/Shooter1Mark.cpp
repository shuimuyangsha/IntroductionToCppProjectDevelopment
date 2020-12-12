#include "stdafx.h"
#include "Shooter1Mark.h"


CShooter1Mark::CShooter1Mark(float x, float y, int index)
{
    // ����ͼƬ
    // 16*16
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 0.png")));
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 1.png")));
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 2.png")));
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 3.png")));
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 4.png")));
    m_img.push_back(Image::FromFile(_T("res/Pablo The Painter Mark 5.png")));

    // ��¼�ڼ���ͼƬ
    m_index = index;

    // ���ô�С
    RectF rc;
    rc.X = 0;
    rc.Y = 0;
    rc.Width = (float)m_img[0]->GetWidth();
    rc.Height = (float)m_img[0]->GetHeight();
    SetRect(rc);

    // �������ĵ�
    SetCenterPos(x, y);
}


CShooter1Mark::~CShooter1Mark()
{
}

void CShooter1Mark::Draw(Gdiplus::Graphics &gh)
{
    // ����ǰ��ͼƬ
    gh.DrawImage(m_img[m_index], GetRect());
}

bool CShooter1Mark::IsChanging() const
{
    // ״̬һֱ���仯
    return false;
}
