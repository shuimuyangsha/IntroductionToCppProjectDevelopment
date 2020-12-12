#pragma once

#include "MenuItem.h"
#include "IMouseAction.h"
#include "TipStringFall.h"

/*
    ѡ�񹤾ߵĲ˵�����
*/

class CDMenu
    : public IDrawable
    , public IMouseAction
{
public:
    CDMenu(HWND hWnd);
    virtual ~CDMenu();

    // ���³�ʼ�����˵������λ����Ϣ
    void InitAnimateInfo0();

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

private:
    HWND m_hWnd;
    // �˵�����
    // ����
    std::shared_ptr<CMenuItem> m_box;
    // ����1:��
    std::shared_ptr<CMenuItem> m_cover;
    // ����2:�ر�
    std::shared_ptr<CMenuItem> m_cover2;
    // ѡ��
    std::shared_ptr<CMenuItem> m_item0;
    std::shared_ptr<CMenuItem> m_item1;
    std::shared_ptr<CMenuItem> m_item2;
    std::shared_ptr<CMenuItem> m_item3;

    // ������������
    void EndAnimate();

    typedef enum {EStatusOverCover1,
                  EStatusItem0, // ���Ŷ���
                  EStatusItem1, // ���Ŷ���
                  EStatusItem2,	// ���Ŷ���
                  EStatusItem3,	// ���Ŷ���
                  EStatusOverCover2, // ���Źظ��ӵĶ���
                  EStatusAll
                 } EStatus;
    EStatus m_eStatus{ EStatusOverCover1 };

    // ��ʾ���� : ��ѡ��һ������
    std::shared_ptr<CTipStringFall> m_pTips;
};

