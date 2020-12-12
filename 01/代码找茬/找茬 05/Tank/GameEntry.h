#pragma once
#include <gdiplus.h>
#include <math.h>
#include <cmath>

class CGameEntry
{
public:
    CGameEntry()
    {
    }

    virtual ~CGameEntry()
    {
    }
public:
    virtual void Draw(Gdiplus::Graphics &gh) const = 0;
};

// ���ƶ�������
class CGameEntryMoveable : public CGameEntry
{
public:
    CGameEntryMoveable()
    {
    }

    virtual ~CGameEntryMoveable()
    {
    }

    // �������캯��
    CGameEntryMoveable(const CGameEntryMoveable &rhs)
    {
        if(this != &rhs) {
            //�Ƿ���Ч
            this->m_bActive = rhs.m_bActive;
            // ռ�õ�����
            this->m_rect = rhs.m_rect;
            // �ƶ��ٶ�
            this->m_fSpeed = rhs.m_fSpeed;
            // ��ǰ�ĽǶ�
            this->m_direction = rhs.m_direction;
            // ÿ����ת�ĽǶ�
            this->m_directionTurn = rhs.m_directionTurn;
        }
    }

    // ���ظ�ֵ����
    CGameEntryMoveable &operator=(const CGameEntryMoveable &rhs)
    {
        if(this != &rhs) {
            //�Ƿ���Ч
            this->m_bActive = rhs.m_bActive;
            // ռ�õ�����
            this->m_rect = rhs.m_rect;
            // �ƶ��ٶ�
            this->m_fSpeed = rhs.m_fSpeed;
            // ��ǰ�ĽǶ�
            this->m_direction = rhs.m_direction;
            // ÿ����ת�ĽǶ�
            this->m_directionTurn = rhs.m_directionTurn;
        }
        return *this;
    }
public:
    virtual void Draw(Gdiplus::Graphics &gh) const = 0;

    /********************���ƶ������ ��ͬ����********************/
    // ��ת
    virtual void RotateRight()
    {

        m_direction += m_directionTurn;
        if(m_direction >= 360) {
            m_direction -= 360;
        }
    }

    // ��ת
    virtual void RotateLeft()
    {
        m_direction -= m_directionTurn;
        if(m_direction < 0) {
            m_direction = static_cast<float>(360 - m_directionTurn);
        }
    }

    // ǰ�ƶ�
    virtual void Forward()
    {
        m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
        m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
    }

    // ���ƶ�
    virtual void Backward()
    {
        m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
        m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
    }

    // ���ƽ������ٶȷ���
    virtual float GetXSpeed() const
    {
        return static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
    }

    // ��ü�ֱ������ٶȷ���
    virtual float GetYSpeed() const
    {
        return -static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
    }

    // ��һ��(ǰ����)λ��
    virtual RectF ForwardNextRect() const
    {
        RectF rc = m_rect;
        rc.X += static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
        rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
        return rc;
    }

    // ��һ��(���˵�)λ��
    virtual RectF BackwardNextRect() const
    {
        RectF rc = m_rect;
        rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
        rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
        return rc;
    }

    // ��ȡͷ��λ�� : ͼƬΪ���ͣ�ͷ��λ�� ���ǽǶ�Ϊ0ʱ���������ĵ�Ϊ�뾶���Խ���/2)���ȣ���ֵ���Լ��� X���λ��
    virtual PointF GetHeadPos() const
    {
        PointF ptCenter = GetCenterPoint();
        PointF ptHead = ptCenter;
        float fRadius = std::sqrt(std::pow(m_rect.Width / 2, 2) + std::pow(m_rect.Height / 2, 2));

        ptHead.X += fRadius * sin(GetDirectionArc());
        ptHead.Y -= fRadius * cos(GetDirectionArc());
        return ptHead;
    }

    // ��ȡ���ĵ�
    virtual PointF GetCenterPoint()const
    {
        PointF center = PointF(m_rect.X + m_rect.Width / 2,
                               m_rect.Y + m_rect.Height / 2);
        return center;
    }

    // �������ĵ�
    virtual void SetCenterPoint(const PointF &ptCenter)
    {
        PointF cen = GetCenterPoint();
        cen.X = ptCenter.X - cen.X;
        cen.Y = ptCenter.Y - cen.Y;
        m_rect.Offset(cen);
    }

    // ���Դ�ȡ��
#ifndef PI
#define PI (3.1415926f)
#endif
    // ���ýǶ� : ��λΪ a*PI
    virtual void SetDirectionArc(float dir)
    {
        m_direction = dir * 180.0f / PI;
    };

    // ���ýǶ�(��λ�� ��)
    virtual void SetDirection(float dir)
    {
        m_direction = dir;
    }

    // ��õ�ǰ�ĽǶ�(��λ�� a*PI)
    virtual float GetDirectionArc() const
    {
        return PI * m_direction / 180.0f;
    }

    // ��õ�ǰ�ĽǶ�(��λ�� ��)
    virtual float GetDirection() const
    {
        return m_direction;
    }

    // ����ÿ����ת�ĽǶ�
    virtual void SetDirectionTurnArc(float dir)
    {
        m_directionTurn = PI * dir / 180.0f;
    };

    // ����ÿ����ת�ĽǶ�
    virtual void SetDirectionTurn(float dir)
    {
        m_directionTurn = dir;
    }

    // ��õ�ǰ�ĽǶ�(��λ�� PI)
    virtual float GetDirectionTurnArc() const
    {
        return PI * m_directionTurn / 180.0f;
    }

    // ��õ�ǰ�Ļ���(��λ�� ��)
    virtual float GetDirectionTurn() const
    {
        return m_direction;
    }

    // �Ƿ�����Ч��
    virtual  bool IsActive() const
    {
        return m_bActive;
    };

    // �Ƿ�����Ч��
    virtual void SetActive(bool bActive)
    {
        m_bActive = bActive;
    }

    // ռ�÷�Χ
    virtual void SetRect(const RectF rect)
    {
        m_rect = rect;
    }

    // ռ�÷�Χ
    virtual RectF GetRect() const
    {
        return m_rect;
    }

    // �ƶ��ٶ�
    virtual void SetSpeed(float speed)
    {
        m_fSpeed = speed;
    }

    // �ƶ��ٶ�
    virtual float GetSpeed() const
    {
        return m_fSpeed;
    }

private:
    //�Ƿ���Ч
    bool m_bActive{false};
    // ռ�õ�����
    RectF m_rect{0, 0, 100, 100};
    // �ƶ��ٶ�
    float m_fSpeed{10};
    // ��ǰ�ĽǶ�
    float m_direction{0};
    // ÿ����ת�ĽǶ�
    float m_directionTurn{5};
};



