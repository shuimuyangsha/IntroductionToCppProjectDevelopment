#include "stdafx.h"
#include "Shooter2.h"
#include "Shooter2Mark.h"
#include "Shooter2MarkSaw.h"


CShooter2::CShooter2()
{
    //  ����ͼƬ
    m_img0 = Image::FromFile(_T("res/The Toolbox01.png"));
    m_img1 = Image::FromFile(_T("res/The Toolbox02.png"));
    m_img2 = Image::FromFile(_T("res/The Toolbox03.png"));
}

CShooter2::~CShooter2()
{
}

// ˳ʱ����ת��ͼ
void DrawImageRotate(PointF mousePt, Graphics &gh, Image *img, float degree)
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

// ���� �Լ���ǰ�����Ի��Լ�
void CShooter2::Draw(Gdiplus::Graphics &gh)
{
    switch(m_status) {
        // ����
        case EStatus::EStatusHammer: {
            switch(m_eStatusHammer) {
                // ��������һ�ε�� :  �����ӵ�,ͬʱ����ͼƬ�仯
                case EStatusHammer::EStatusHammerDownFirst: {
                    // ����һ���ӵ�
                    g_game->Append(std::make_shared<CShooter2Mark>(m_mousePos.X, m_mousePos.Y));
                    m_eStatusHammer = EStatusHammer::EStatusHammerDown;
                    /*break; // ע������û��break;�������ִ������һ��case����*/
                }
                // ���������
                case EStatusHammer::EStatusHammerDown: {
                    auto img = m_img1;
                    RectF rc;
                    rc.X = m_mousePos.X;
                    rc.Y = m_mousePos.Y;
                    rc.Width = (float)img->GetWidth();
                    rc.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rc);
                    break;
                }
                // �������̧��
                case EStatusHammer::EStatusHammerUp: {
                    auto img = m_img2;
                    RectF rc;
                    rc.X = m_mousePos.X;
                    rc.Y = m_mousePos.Y;
                    rc.Width = (float)img->GetWidth();
                    rc.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rc);
                    break;
                }
                default: {
                    // �����ߵ�����
                    assert(false);
                    break;
                }
            }
            break;
        }
        //���
        case EStatus::EStatusSaw: {
            // ���ͼƬ
            auto img = m_img0;
            // �������λ��,�ʹ�С
            RectF rc;
            rc.X = m_sawPos.X;
            rc.Y = m_sawPos.Y;
            rc.Width = (float)img->GetWidth();
            rc.Height = (float)img->GetHeight();
            // ���Ƶ��ͼƬ
            DrawImageRotate(m_sawPos, gh, img, m_sawDegree);

            switch(m_eStatusSaw) {

                // ����ǿ�ʼ״̬,�����и�Ч��
                case CShooter2::EStatusSaw::EStatusSawStart: {
                    // ʲôҲ������Ҳ��������ֱ��������һ��case ����
                }
                // �����ƶ�״̬, ����ƶ�
                case CShooter2::EStatusSaw::EStatusMoving: {
                    float radian = Degree2Radian(m_sawDegree);

                    // ����һ���ӵ�
                    g_game->Append(std::make_shared<CShooter2MarkSaw>(m_mousePos.X, m_mousePos.Y));

                    // �����⵱ǰ��λ��
                    const float speed = 5.0f;
                    auto newX = m_sawPos.X + speed * cos(radian);
                    auto newY = m_sawPos.Y + speed * sin(radian);

                    if(newX >= 0.0f && newX <= g_game->GetWidth() && newY >= 0.0f && newY <= g_game->GetHeigth()) {
                        m_sawPos.X = newX;
                        m_sawPos.Y = newY;
                        // ʹ���λ�ø���仯
                        SetCursorPos(static_cast<int>(m_sawPos.X), static_cast<int>(m_sawPos.Y));
                    }
                    break;
                }
                // ֹͣ
                case CShooter2::EStatusSaw::EStatusSawStop: {
                    break;
                }
                default:
                    break;
            }
            if(m_eStatusSaw == EStatusSaw::EStatusSawStart) {
                // ��ֹ״̬��ʲôҲ����
            }
            break;
        }
        default : {
            break;
        }
    }
}

/// ////////////////////////////////////////////////////////////////////////
/// ���������Ϣ : ��Ҫ�������д���
/// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
// ����������
bool CShooter2::OnLButtonDown(UINT nFlags, CPoint point)
{
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // ��������
            m_eStatusHammer = EStatusHammer::EStatusHammerDownFirst;
            break;
        }
        case EStatus::EStatusSaw: {
            // ��� ��ʼ�����
            m_eStatusSaw = EStatusSaw::EStatusSawStart;
            // ��ǰλ��
            m_sawPos = m_mousePos;
            break;
        }
        default : {
            break;
        }
    }

    return false;
}

// ������̧��
bool CShooter2::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // ����̧��
            m_eStatusHammer = EStatusHammer::EStatusHammerUp;
            break;
        }
        case EStatus::EStatusSaw: {
            // ���ֹͣ
            m_eStatusSaw = EStatusSaw::EStatusSawStop;
            break;
        }
        default : {
            break;
        }
    }

    return false;
}

// ����Ҽ�����
bool CShooter2::OnRButtonDown(UINT nFlags, CPoint point)
{
    // ������: �����ǰ�Ǵ���,�л�Ϊ���
    if(m_status == EStatus::EStatusHammer) {
        m_status = EStatus::EStatusSaw;
    }
    else {
        // ������: �����ǰ�ǵ��,�л�Ϊ����
        m_status = EStatus::EStatusHammer;
    }
    return true;
}

// ������������
float distance(PointF pt1, PointF pt2)
{
    return std::sqrt(std::pow(pt1.X - pt2.X, 2) + std::pow(pt1.Y - pt2.Y, 2));
}

// ����ƶ�
bool CShooter2::OnMouseMove(UINT nFlags, CPoint point)
{
    // ��¼���λ��
    m_mousePos.X = (float)point.x;
    m_mousePos.Y = (float)point.y;

    // ���ݲ�ͬ��������д���
    switch(m_status) {
        case EStatus::EStatusHammer: {
            // ���ӣ�ά��ԭ״̬
            break;
        }
        case EStatus::EStatusSaw: {
            // ���û�оⶫ�����任�Ƕ�
            if(m_eStatusSaw == EStatusSaw::EStatusSawStop) {
                // ��¼ ���ӵ�λ��
                m_sawPos = m_mousePos;
                // �任�Ƕ�
                if(distance(m_mousePosLast, m_mousePos) > 10) {
                    // ����Ƕ�
                    float theta = (atan2(m_mousePos.Y - m_mousePosLast.Y, m_mousePos.X - m_mousePosLast.X));
                    if(theta < 0.0f) {
                        theta = PI(2.0f) + theta;
                    }
                    // �Ƕ�:��λ(��)
                    m_sawDegree = Radian2Degree(theta);
                    // ��ס����λ��
                    m_mousePosLast = m_mousePos;
                }
            }
            break;
        }
        default : {
            break;
        }
    }

    return true;
}





