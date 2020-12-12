
// DialogMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopToys.h"
#include "DialogMain.h"
#include "afxdialogex.h"
#include "ScreenTools.h"

#include "Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialogMain �Ի���



CDialogMain::CDialogMain(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DESKTOPTOYS_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialogMain::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogMain, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CDialogMain ��Ϣ�������

BOOL CDialogMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    // �����Ļ�ֱ���
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // ����ȫ��,������͸��,�ޱ������
    {
        // ���ñ����ڴ�С, �ڵ�������Ļ
        MoveWindow(0, 0, w, y);
        //����һ���Ӵ��ڣ������������Ķ���
        m_dlg = make_shared<CDesktopToysDlg>(this);
        m_dlg->Create(IDD_DESKTOPTOYS_DIALOG, this);
        m_dlg->MoveWindow(0, 0, w, y);
        m_dlg->ShowWindow(SW_SHOW);
    }

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDialogMain::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDialogMain::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


BOOL CDialogMain::PreTranslateMessage(MSG *pMsg)
{
    //// ���� 'ESC'��ʱ
    //if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
    //    if (m_pGame->OnESC()) {
    //        return TRUE;
    //    }
    //    else {
    //        // ���������ڴ���,�˳�,��Ϸ����
    //    }
    //}
    return CDialogEx::PreTranslateMessage(pMsg);
}

#if 0

void CDialogMain::OnTimer(UINT_PTR nIDEvent)
{
    {
        static DWORD dwLastTime = 0;
        DWORD dwTime = GetTickCount();
        m_pGame->EnterFrame(dwTime);
        dwLastTime = dwTime;
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CDialogMain::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDialogMain::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonUp(nFlags, point);
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CDialogMain::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    m_pGame->OnLButtonDblClk(nFlags, point);
    CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CDialogMain::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonDown(nFlags, point);
    CDialogEx::OnRButtonDown(nFlags, point);
}

void CDialogMain::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonUp(nFlags, point);
    CDialogEx::OnRButtonUp(nFlags, point);
}

void CDialogMain::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    m_pGame->OnRButtonDblClk(nFlags, point);
    CDialogEx::OnRButtonDblClk(nFlags, point);
}

void CDialogMain::OnMouseMove(UINT nFlags, CPoint point)
{
    m_pGame->OnMouseMove(nFlags, point);
    CDialogEx::OnMouseMove(nFlags, point);
}

#endif
