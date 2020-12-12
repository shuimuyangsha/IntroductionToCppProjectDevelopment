#include "stdafx.h"
#include "Shooter3.h"
#include "Shooter3Mark.h"
#include "GameTimer.h"


CShooter3::CShooter3()
{
    // ����ͼƬ
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 0.png")));
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 1.png")));
    m_vImgUp.push_back(Image::FromFile(_T("res/Eraser Ed 2.png")));
    m_vImgDown.push_back(Image::FromFile(_T("res/Eraser Ed 00.png")));
    m_vImgDown.push_back(Image::FromFile(_T("res/Eraser Ed 01.png")));
}


CShooter3::~CShooter3()
{
}

// ���� �Լ���ǰ�����Ի��Լ�
void CShooter3::Draw(Gdiplus::Graphics &gh)
{
    switch(m_status) {
        // �Ҽ����£�գ��
        case CShooter3::EStatus::EStatusLeftUp: {
            if(!m_bRightDown) {
                // ������ʾ����ͼƬ��գ�۾�
                static size_t index = 0;
                if(index >= 3) {
                    index = 0;
                }
                // ����ǰͼƬ
                {
                    auto img = m_vImgUp[index];
                    RectF rect;
                    rect.X = m_mousePos.X;
                    rect.Y = m_mousePos.Y;
                    rect.Width = (float)img->GetWidth();
                    rect.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rect);
                }
                //����ʱ����������
                static CGameTimeval val(200);
                if(val.IsTimeval()) {
                    index++;
                }
                // �ж��Ƿ���Ҫգ��
                if(index == 2) {
                    if((0 == rand() % 4)) {
                        // ���Ķ�index�� ��ʱ�����
                    }
                    else {
                        index = 0;
                    }
                }
            }
            else {
                // ֻ��ʾ���۵�ͼƬ
                size_t index = 2;
                {
                    auto img = m_vImgUp[index];
                    RectF rect;
                    rect.X = m_mousePos.X;
                    rect.Y = m_mousePos.Y;
                    rect.Width = (float)img->GetWidth();
                    rect.Height = (float)img->GetHeight();
                    gh.DrawImage(img, rect);
                }
            }
            break;
        }
        // ������£� ����Ļ
        case CShooter3::EStatus::EStatusLeftDown: {
            // ������ʾ����ͼƬ
            static size_t index = 0;
            if(index > 1) {
                index = 0;
            }
            auto img = m_vImgDown[index];
            RectF rect;
            rect.X = m_mousePos.X;
            rect.Y = m_mousePos.Y;
            rect.Width = (float)img->GetWidth();
            rect.Height = (float)img->GetHeight();
            gh.DrawImage(img, rect);
            if(index == 1) {
                // �ͷ���м
                g_game->Append(std::make_shared<CShooter3Mark>(m_mousePos.X, m_mousePos.Y));
            }
            // 30���뻭һ��
            static CGameTimeval val(30);
            if(val.IsTimeval()) {
                index++;
            }
            break;
        }
        default:
            break;
    }
}


/// ////////////////////////////////////////////////////////////////////////
/// ���������Ϣ : ��Ҫ�������д���
/// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
// ����������
bool CShooter3::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_status = EStatus::EStatusLeftDown;
    return true;
}

// ������̧��
bool CShooter3::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_status = EStatus::EStatusLeftUp;
    return true;
}

// ����Ҽ�����
bool CShooter3::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_bRightDown = true;
    return true;
}

// ����Ҽ�̧��
bool CShooter3::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_bRightDown = false;
    return true;
}

// ����ƶ�
bool CShooter3::OnMouseMove(UINT nFlags, CPoint point)
{
    // ��¼���λ��
    m_mousePos.X = (float)point.x;
    m_mousePos.Y = (float)point.y;
    return true;
}





