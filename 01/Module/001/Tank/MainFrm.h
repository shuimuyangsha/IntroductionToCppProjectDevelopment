
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

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

};


