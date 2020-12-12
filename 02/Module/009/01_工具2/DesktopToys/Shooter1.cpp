#include "stdafx.h"
#include "Shooter1.h"
#include "Shooter1Mark.h"


CShooter1::CShooter1 (int height) : m_nHeight (height)
{
	// ����ͼƬ
	m_img0 = Image::FromFile (_T ("res/Pablo The Painter 00.png"));
	m_img1 = Image::FromFile (_T ("res/Pablo The Painter 01.png"));
	m_img2 = Image::FromFile (_T ("res/Pablo The Painter 02.png"));
	m_img3 = Image::FromFile (_T ("res/Pablo The Painter 03.png"));
	m_img4 = Image::FromFile (_T ("res/Pablo The Painter 04.png"));
	m_img5 = Image::FromFile (_T ("res/Pablo The Painter 05.png"));
}


CShooter1::~CShooter1 ()
{
}

// ���� �Լ���ǰ�����Ի��Լ�
void CShooter1::Draw (Gdiplus::Graphics &gh)
{
	// �Ȼ�����,��ƴ�ӳɵ�

	// ��ͼƬ
	auto drawImg = [&](Image * img) {
		RectF rc;
		rc.X = m_mousePos.X;
		rc.Y = m_mousePos.Y;
		rc.Width = (float)img->GetHeight ();
		rc.Height = (float)img->GetWidth ();
		rc.Offset (-(rc.Width / 3.0f - 10.0f), 0);
		gh.DrawImage (img, rc);
	};

	// �ظ�������ͬ��ͼƬ��ƴ�ӳ�һ������
	{
		// ���ͼƬ��С
		float h = (float)m_img0->GetHeight ();
		float w = (float)m_img0->GetWidth ();

		RectF rc;
		rc.Width = w;
		rc.Height = h;
		rc.X = m_mousePos.X;
		// ѭ�����ƣ�Xλ�ò��䣬yλ��һֱ���ӣ�ֱ����Ļ��Ϊֹ
		for (int y = 0; y < m_nHeight; ++y) {
			rc.Y = y * h;
			gh.DrawImage (m_img0, rc);
		}
	}

	// ���ݵ�ǰ״̬�����Ʋ�ͬ��ͼƬ
	switch (m_status) {
	case 0: {
		break;
	}
	case 1: {
		drawImg (m_img1);
		break;
	}
	case 2: {
		drawImg (m_img2);
		break;
	}
	case 3: {
		drawImg (m_img3);
		break;
	}
	case 4: {
		drawImg (m_img4);
		break;
	}
	case 5: {
		drawImg (m_img5);
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
bool CShooter1::OnLButtonDown (UINT nFlags, CPoint point)
{
	// ��ˢ״̬
	m_status = 5;
	return true;
}

// ������̧��
bool CShooter1::OnLButtonUp (UINT nFlags, CPoint point)
{
	//�ָ�ǰһ��״̬
	m_status = 1;
	return true;
}

// ����Ҽ�����
bool CShooter1::OnRButtonDown (UINT nFlags, CPoint point)
{
	// ����ɫ
	if (1 + m_iColorIndex >= 6) {
		m_iColorIndex = 0;
	}
	else {
		m_iColorIndex++;
	}
	return false;
}

// ����ƶ�
bool CShooter1::OnMouseMove (UINT nFlags, CPoint point)
{
	// ��¼��ǰ��λ��
	m_mousePos.X = (float)point.x;
	m_mousePos.Y = (float)point.y;

	// �л�����ˢ״̬
	if (m_status == 5) {
		m_status = 5;
		//�ж��Ƿ���Ҫ����һ��mark
		RectF rc;
		rc.X = m_mousePosLast.X;
		rc.Y = m_mousePosLast.Y;
		rc.Width = 1;
		rc.Height = 1;
		if (!rc.Contains (m_mousePos)) {
			// ��¼λ��
			m_mousePosLast = m_mousePos;
			// �ڱ���ͼ������һ�����
			g_game->Append (std::make_shared<CShooter1Mark> (
				m_mousePos.X, m_mousePos.Y, m_iColorIndex));
		}
		return true;
	}

	/* ���Ƿ�ˢ״̬ ����������ƶ�����*/
	// �ж��Ƿ���Ҫ����
	// �����ƶ�
	if ((point.y > m_lastChangePos.Y) && 
		(m_distanceChange <= abs (point.y - m_lastChangePos.Y))) {

		// 0:����; 1:��,2:�һ�ͷ; 3:��,4���ͷ;5:��ˢ
		switch (m_status) {
		case 1: {
			m_status = 3;
			break;
		}
		case 2: {
			m_status = 3;
			break;
		}
		case 3: {
			m_status = 1;
			break;
		}
		case 4: {
			m_status = 1;
			break;
		}
		default:
			break;
		}
		// ��¼���һ�ε�λ��
		m_lastChangePos = m_mousePos;
	}
	// �����ƶ�
	else if ((point.y < m_lastChangePos.Y) && 
		     (m_distanceChange <= abs (point.y - m_lastChangePos.Y))) {
		// 0:����; 1:��,2:�һ�ͷ; 3:��,4���ͷ;5:��ˢ
		switch (m_status) {
		case 1: {
			m_status = 3;
			break;
		}
		case 2: {
			m_status = 3;
			break;
		}
		case 3: {
			m_status = 1;
			break;
		}
		case 4: {
			m_status = 1;
			break;
		}
		default:
			break;
		}

		// ��¼���һ�ε�λ��
		m_lastChangePos = m_mousePos;
	}
	else {
		//�ƶ������С�����任״̬
	}
	return true;
}
