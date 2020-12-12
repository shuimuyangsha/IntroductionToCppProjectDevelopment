
// DesktopToysDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopToys.h"
#include "DesktopToysDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesktopToysDlg 对话框



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


// CDesktopToysDlg 消息处理程序

BOOL CDesktopToysDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // TODO: 在此添加额外的初始化代码

    // 获得屏幕分辨率
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // 布满全屏,并窗口透明,无标题标栏
    {
        // 设置本窗口大小, 遮挡整个屏幕
        MoveWindow(0, 0, w, y);
    }

    /****下面代码演示 屏幕截图功能的用法****/
    {
        {
            // 截图部分屏幕
            RECT r{0, 0, 100, 300}; // 截图的大小
            HBITMAP hBmp = CScreenTools::CopyScreenToBitmap(&r);
            // 转化为 Bitmap* 类型,因为下面行输出要用到
            bmp = Bitmap::FromHBITMAP(hBmp, NULL);
        }
    }
    /****演示完毕****/

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDesktopToysDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        {
            // 获得本窗口DC
            HDC hdc = ::GetDC(GetSafeHwnd());
            // 创建一个 Grahpics对象:绘图,写字全靠它
            Gdiplus::Graphics gh(hdc);
            // 绘制图形
            gh.DrawImage(bmp, Rect(0, 0, 100, 300));
            // 释放DC
            ::ReleaseDC(GetSafeHwnd(), hdc);
        }
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDesktopToysDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CDesktopToysDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnLButtonDown(nFlags, point);
}


void CDesktopToysDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    AfxMessageBox(_T("鼠标左键抬起消息"));

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CDesktopToysDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnRButtonDown(nFlags, point);
}


void CDesktopToysDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    AfxMessageBox(_T("鼠标右键抬起消息"));

    CDialogEx::OnRButtonUp(nFlags, point);
}


void CDesktopToysDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CDialogEx::OnMouseMove(nFlags, point);
}


void CDesktopToysDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnTimer(nIDEvent);
}
