
// DesktopToysDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopToys.h"
#include "DesktopToysDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesktopToysDlg �Ի���



CDesktopToysDlg::CDesktopToysDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DESKTOPTOYS_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDesktopToysDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDesktopToysDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
//    ON_WM_TIMER()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CDesktopToysDlg ��Ϣ�������

BOOL CDesktopToysDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    // �����Ļ�ֱ���
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // ����ȫ��,������͸��,�ޱ������
    {
        // ���ñ����ڴ�С, �ڵ�������Ļ
        MoveWindow(0, 0, w, y);
    }

    /****���������ʾ ��Ļ��ͼ���ܵ��÷�****/
    {
        {
            // ��ͼ������Ļ
            RECT r{0, 0, 100, 300}; // ��ͼ�Ĵ�С
            HBITMAP hBmp = CScreenTools::CopyScreenToBitmap(&r);
            // ת��Ϊ Bitmap* ����,��Ϊ���������Ҫ�õ�
            bmp = Bitmap::FromHBITMAP(hBmp, NULL);
        }
    }
    /****��ʾ���****/

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDesktopToysDlg::OnPaint()
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
        {
            // ��ñ�����DC
            HDC hdc = ::GetDC(GetSafeHwnd());
            // ����һ�� Grahpics����:��ͼ,д��ȫ����
            Gdiplus::Graphics gh(hdc);
            // ����ͼ��
            gh.DrawImage(bmp, Rect(0, 0, 100, 300));
            // �ͷ�DC
            ::ReleaseDC(GetSafeHwnd(), hdc);
        }
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDesktopToysDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CDesktopToysDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnLButtonDown(nFlags, point);
}


void CDesktopToysDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    AfxMessageBox(_T("������̧����Ϣ"));

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CDesktopToysDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnRButtonDown(nFlags, point);
}


void CDesktopToysDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    AfxMessageBox(_T("����Ҽ�̧����Ϣ"));

    CDialogEx::OnRButtonUp(nFlags, point);
}


void CDesktopToysDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CDialogEx::OnMouseMove(nFlags, point);
}


void CDesktopToysDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnTimer(nIDEvent);
}
