#pragma once

#include "IDrawable.h"
#include "IMouseAction.h"

/*
    ѡ�񹤾ߵĲ˵�����Ĳ˵���
*/

class CMenuItem
    : public IDrawable
    , public IMouseAction
{
public:
    CMenuItem();

    CMenuItem(float x, float y, PCTSTR szImg, PCTSTR szTips);

    virtual ~CMenuItem();

    // ���� �Լ���ǰ�����Ի��Լ�
    virtual void Draw(Gdiplus::Graphics &gh);

    /// ////////////////////////////////////////////////////////////////////////
    /// ���������Ϣ
    /// ��� �������򷵻�false,������Ծߴ��ж��Ƿ�������д���
    // ����������
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    // ����Ҽ�����
    virtual bool OnRButtonDown(UINT nFlags, CPoint point);
    // ����ƶ�
    virtual bool OnMouseMove(UINT nFlags, CPoint point);


    // ��ʼ���Ŷ���;
    void StartAnimate();
    // ���ö�����Ϣ:�ɳ���ɢ��������
    void InitAnimateInfo0(float x, float y, float dir);
    // ���ö�����Ϣ:�����ĵ�����ת
    void InitAnimateInfo1(float x, float y, float dir);
    // ���ö�����Ϣ:�����ĵ�����ת,���򷭴�
    void InitAnimateInfoReverse();
    // ���ö�����Ϣ:ʲôҲ����
    void InitAnimateInfo3(float x, float y);

    // ��ֹ���Ŷ�����ֱ���������(��ֹ����)
    void EndAnimate()
    {
        m_indexAnimate = m_vAnimateInfo.size();
    }
    // �����Ƿ񲥷����
    bool IsAnimateEnd() const
    {
        return m_indexAnimate != 0 && m_indexAnimate >= m_vAnimateInfo.size();
    }
private:
    // ͼƬ
    Image *m_img{ nullptr };

    // ����Ĵ�С
    SizeF m_sizeInit;

    // ���������Ϣ
    typedef struct {
        SizeF size;
        PointF pos;
    } SAnimateInfo;
    std::vector<SAnimateInfo> m_vAnimateInfo;
    // ��ǰ�ڵڼ�֡��λ��
    size_t m_indexAnimate;
    // ��ǵ�ǰ�Ƿɳ�����
    bool m_bAnimate{ false };
    // ��ǵ�ǰ�Ƿ�ת����
    bool m_bAnimate2{ false };
};

