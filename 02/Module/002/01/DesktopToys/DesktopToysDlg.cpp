
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
            // ��ͼ������Ļ,�����浽�ļ���
            RECT r{0, 0, 100, 300};
            HBITMAP hBmp = CScreenTools::CopyScreenToBitmap(&r);
            // ���汳��ͼ
            CScreenTools::SaveBitmapToFile(hBmp, _T("C:\\�����ƻ�����ͼ����.bmp"));
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
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDesktopToysDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

