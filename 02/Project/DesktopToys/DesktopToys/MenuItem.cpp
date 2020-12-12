#include "stdafx.h"
#include "MenuItem.h"


CMenuItem::CMenuItem()
{
}

CMenuItem::CMenuItem(float x, float y, PCTSTR szImg, PCTSTR szTips)
{
    m_img = Image::FromFile(szImg);

    int width = m_img->GetWidth();
    int height = m_img->GetHeight();

    // ���� ̹�� ǰռ�õ�����
    RectF r(static_cast<float>(x)
            , static_cast<float>(y)
            , static_cast<float>(width)
            , static_cast<float>(height));
    SetRect(r);
    m_sizeInit.Width = (float)width;
    m_sizeInit.Height = (float)height;

    m_pTips = make_shared<CTipStringChangeColor>(szTips);
}


CMenuItem::~CMenuItem()
{
}

// ���� �Լ���ǰ�����Ի��Լ�
void CMenuItem::Draw(Gdiplus::Graphics &gh)
{
    if(m_indexAnimate >= m_vAnimateInfo.size()) {
        // �����������ƶ�λ�õȲ���
        m_indexAnimate = m_vAnimateInfo.size();
        if(m_indexAnimate == 0) {
            return;
        }
        m_indexAnimate--;
    }

    {
        auto const &info = m_vAnimateInfo[m_indexAnimate++];
        // �ƶ�λ��
        SetCenterPos(info.pos);
        // ����
        SetSize(info.size.Width, info.size.Height);
    }
    gh.DrawImage(m_img, GetRect());

    // ����ʾ����
    m_pTips->Draw(gh);
}

// ����������
bool CMenuItem::OnLButtonDown(UINT nFlags, CPoint point)
{
    // �鿴�������ĸ��˵�
    if(this->GetRect().Contains((float)point.x, (float)point.y)) {
        return true;
    }
    else {
        return false;
    }
}

// ����Ҽ�����
bool CMenuItem::OnRButtonDown(UINT nFlags, CPoint point)
{
    // ʲôҲ����
    return false;
}

// ����ƶ�
bool CMenuItem::OnMouseMove(UINT nFlags, CPoint point)
{
    if(this->GetRect().Contains((float)point.x, (float)point.y)) {
        m_pTips->SetPos(point);
        return true;
    }
    else {
        m_pTips->SetEnd();
    }
    return false;
}

// ��ʼ���Ŷ���;
void CMenuItem::StartAnimate()
{

}

// ���ö�����Ϣ:�ɳ���ɢ��������
void CMenuItem::InitAnimateInfo0(float x, float y, float dir)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // ��ʼ
    m_indexAnimate = 0;
    // �˶�·����Ϣ���
    m_vAnimateInfo.clear();
    // ��ʼ��С
    float w = m_sizeInit.Width * 0.01f, h = m_sizeInit.Height * 0.01f;
    // ԭʼ��С
    float ww = m_sizeInit.Width, hh = m_sizeInit.Height;

    // ���
    float www = m_sizeInit.Width * 2.0f, hhh = m_sizeInit.Height * 2.0f;

    SAnimateInfo info;

    // ��һ��λ��
    {
        info.size.Width = w;
        info.size.Height = h;
        info.pos.X = x;
        info.pos.Y = y;
        m_vAnimateInfo.push_back(info);
    }
    float distance = 1.0f;
    // �ﵽ���ֵ
    while(true) {
        // ��Сÿ������ 10%
        w *= (1 + 0.1f);
        h *= (1 + 0.1f);
        if(w > www && h > hhh) {
            break;
        }
        // λ��ÿ���ƶ� Distance ����
        x += distance * cos(PI(2) - dir);
        y += distance * sin(PI(2) - dir);

        {

            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }

    distance = 4.0f;
    // �ָ���ԭʼֵ
    while(true) {
        // ��Сÿ������ 10%
        w *= (1.0f - 0.03f);
        h *= (1.0f - 0.03f);
        if(w <= ww && h <= hh) {
            break;
        }
        // λ��ÿ���ƶ� Distance ����
        x += distance * cos(PI(2) - dir);
        y += distance * sin(PI(2) - dir);
        {
            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }
    // �����ʼ��С

    info.size = m_sizeInit;
    info.pos.X = x;
    info.pos.Y = y;
    m_vAnimateInfo.push_back(info);
}

// ���ö�����Ϣ:�����ĵ�����ת
void CMenuItem::InitAnimateInfo1(float x, float y, float dir)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // ��ʼ
    m_indexAnimate = 0;
    // �˶�·����Ϣ���
    m_vAnimateInfo.clear();
    // ���ݳ�ʼλ�ã��ͷ��������µ�·��
    // �����
    float w = m_sizeInit.Width, h = m_sizeInit.Height;
    SAnimateInfo info;

    // ��һ��λ��
    {
        info.size.Width = w;
        info.size.Height = h;
        info.pos.X = x;
        info.pos.Y = y;
        m_vAnimateInfo.push_back(info);
    }

    for(int i = 0; i < 25; ++i) {
        // �߶�ÿ������ 10%
        h *= (1 - 0.05f);
        {

            info.size.Width = w;
            info.size.Height = h;
            info.pos.X = x;
            info.pos.Y = y;
            m_vAnimateInfo.push_back(info);
        }
    }
}

// ���ö�����Ϣ:�����ĵ�����ת,���򷭴�
void CMenuItem::InitAnimateInfoReverse()
{
    if(m_bAnimate2) {
        return;
    }
    else {
        m_bAnimate2 = true;
    }
    // ��ʼ
    m_indexAnimate = 0;
    // �Ѳ��ŵ� ��Ϣ������������һ��
    std::reverse(m_vAnimateInfo.begin(), m_vAnimateInfo.end());
}

// ���ö�����Ϣ:ʲôҲ����
void CMenuItem::InitAnimateInfo3(float x, float y)
{
    if(m_bAnimate) {
        return;
    }
    else {
        m_bAnimate = true;
    }
    // ��ʼ
    m_indexAnimate = 0;
    // �˶�·����Ϣ���
    m_vAnimateInfo.clear();
    // ���ݳ�ʼλ�ã��ͷ��������µ�·��
    // �����
    float w = m_sizeInit.Width * 0.01f, h = m_sizeInit.Height * 0.01f;
    SAnimateInfo info;
    // �����ʼ��С
    info.size = m_sizeInit;
    info.pos.X = x;
    info.pos.Y = y;
    m_vAnimateInfo.push_back(info);
}


