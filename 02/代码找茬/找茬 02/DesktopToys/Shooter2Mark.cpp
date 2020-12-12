#include "stdafx.h"
#include "Shooter2Mark.h"


CShooter2Mark::CShooter2Mark(float x, float y)
{
    SetCenterPos(x, y);

    // ����[1,5]֮��������
    int i = 1 + rand() % 5;
    TCHAR szFilename[MAX_PATH] = { 0 };
    _stprintf_s(szFilename, _T("res/The Toolbox Mark %02d.png"), i);
    m_img = Image::FromFile(szFilename);


    for(int i = 0; i < (1 + rand() % 3); ++i) {
        SDynamic dy;
        // ��������
        dy.m_img2 = Image::FromFile(_T("res/The Toolbox Mark 06.png"));

        // �ٶ�(��������) [-5, 4 / -5, 4]
        dy.m_speed = PointF(float(rand() % 10 - 5), float(rand() % 10 - 5));

        // λ��
        dy.m_pos.X = x + float(rand() % 80 - 40);
        dy.m_pos.Y = y + float(rand() % 80 - 40);
        // ��ǰ�Ƕ�(����������ת)
        dy.m_dir = Degree2Radian(rand() % 360);
        // �Ƿ���ʾ
        dy.m_bShow = true;
        m_vD.push_back(dy);
    }
}


CShooter2Mark::~CShooter2Mark()
{
}


// ˳ʱ����ת��ͼ
static void DrawImageRotate(PointF mousePt, Graphics &gh, Image *img, float degree)
{
    //��ת��ͼƽ��
    PointF center = mousePt;
    center.X += img->GetWidth() / 2.0f;
    center.Y += img->GetHeight() / 2.0f;

    // 1.ƽ�Ʊ任 : �ƶ�����㵽 ̹������
    gh.TranslateTransform(center.X, center.Y);
    // 2.��ת�任 : ʹ����ϵ����̹�˵ĽǶ�һ��
    gh.RotateTransform(degree);
    // 3.�ָ�ԭ��
    gh.TranslateTransform(-center.X, -center.Y);

    // �����˳�ʱ�ָ�
    ON_SCOPE_EXIT([&]() {
        // ��������任 : �����Ƿ��أ�
        // 1.�ƶ������ĵ�
        gh.TranslateTransform(center.X, center.Y);
        // 2.����ת
        gh.RotateTransform(-degree);
        // 3.���ƶ���ȥ
        gh.TranslateTransform(-center.X, -center.Y);
        //ScaleTransform : ����
    });


    // ����ͼ��
    {
        RectF rc;
        rc.X = mousePt.X;
        rc.Y = mousePt.Y;
        rc.Width = (float)img->GetWidth();
        rc.Height = (float)img->GetHeight();
        gh.DrawImage(img, rc);
    }
}


void CShooter2Mark::Draw(Gdiplus::Graphics &gh)
{
    // ��ͼƬ
    gh.DrawImage(m_img, GetCenterPos());

    // ����������
    for(auto &dy : m_vD) {
        if(dy.m_bShow) {
            // �ж��Ƿ����������
            if(!g_game->GetRectF().Contains(dy.m_pos)) {
                continue;
            }

            // û���䵽����
            // ������
            DrawImageRotate(dy.m_pos, gh, dy.m_img2, Radian2Degree(dy.m_dir));
            // 
            {
                // ����λ��
                dy.m_pos.X += dy.m_speed.X;
                dy.m_pos.Y += dy.m_speed.Y;
                // �����ٶ�:y��������������ٶ�
                dy.m_speed.Y += 1.0f;
                //������ǰ�ĽǶ�
                dy.m_dir += PI(0.1f);
                if(dy.m_dir >= PI(2.0f)) {
                    dy.m_dir = 0.0f;
                }
            }
        }
    }

    // �Ƴ������Ϸ������Ķ���
    m_vD.erase(std::remove_if(m_vD.begin(),
                              m_vD.end(),
    [&](auto & lhs)->bool {
        // �������ڴ����е�ȫ���Ƴ�Ҫ
        return (!g_game->GetRectF().Contains(lhs.m_pos));
    }), m_vD.end());
}

bool CShooter2Mark::IsChanging() const
{
    // û�пգ�˵�����ж�̬����
    return !m_vD.empty();
}

