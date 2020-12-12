
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "Game.h"

class CMainFrame : public CFrameWnd
{
    
public:
    CMainFrame();
protected: 
    DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:

// ʵ��
public:
    virtual ~CMainFrame();

// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

    // �ͻ����Ĵ�С
    int m_iWidth{ 800 };
    int m_iHeight{ 600 };

    // ��ʱ�� ID
    enum ETimerId { ETimerIdGameLoop = 1 };

    // ��Ϸ���� : ע��,�˴�ʹ����CGame��
    CGame m_game;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


