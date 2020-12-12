#include "stdafx.h"
#include "SysOptimize.h"
#include "SysOptimizeDlg.h"
#include "ContralDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);      // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysOptimizeDlg dialog

CSysOptimizeDlg::CSysOptimizeDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CSysOptimizeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSysOptimizeDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // 标题文本颜色
    m_CapitonColor = RGB(0, 0, 255);
    // 标题文本
    m_Caption = "365系统加速器";
    // 标题文本字体
    m_CaptionFont.CreateFont(14, 10, 0, 3, 600, 0, 0, 0, ANSI_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, FF_ROMAN, "宋体");
}

void CSysOptimizeDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSysOptimizeDlg)
    DDX_Control(pDX, IDC_STATICSELECT, m_Select);
    DDX_Control(pDX, IDC_FRAME, m_Frame);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSysOptimizeDlg, CDialog)
    //{{AFX_MSG_MAP(CSysOptimizeDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_STACONTRAL, OnStacontral)
    ON_BN_CLICKED(IDC_STADISK, OnStadisk)
    ON_BN_CLICKED(IDC_STALITTER, OnStalitter)
    ON_BN_CLICKED(IDC_STATASK, OnStatask)
    ON_WM_CTLCOLOR()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCACTIVATE()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
    ON_WM_NCPAINT()
    ON_WM_MOVE()
END_MESSAGE_MAP()


BOOL CSysOptimizeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if(pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if(!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // 初始化
    m_Num = 0;
    m_pTaskDlg = new CTaskDlg;
    m_pTaskDlg->Create(IDD_TASK_DIALOG, this);       // 创建系统任务管理模块
    m_Select.ShowWindow(SW_HIDE);                    // 隐藏选中效果图片
    SetWindowText("365系统加速器");                     // 设置主窗体标题
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MINBT);                    // 加载最小化按钮位图
    BITMAPINFO bInfo;
    bitmap.GetObject(sizeof(bInfo), &bInfo);          // 获得图片信息
    m_ButtonWidth = bInfo.bmiHeader.biWidth;        // 位图宽度
    m_ButtonHeight = bInfo.bmiHeader.biHeight;      // 位图高度
    bitmap.DeleteObject();

    // 背景图片
    m_BKGround.LoadBitmap(IDB_WINDOWBK);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSysOptimizeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSysOptimizeDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this);   // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialog::OnPaint();
        DrawDialog(fAll);   // 绘制标题栏
        m_IsDrawForm = TRUE;
    }

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSysOptimizeDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

void CSysOptimizeDlg::CreateDialogBox(int num)
{
    CRect fRect;
    m_Frame.GetClientRect(&fRect);               // 获得图片控件的客户区域
    m_Frame.MapWindowPoints(this, fRect);        // 设置模块的显示位置
    switch(num) {                                // 判断显示的模块
        case 1:                                 // 控制面板操作模块
            m_pContralDlg = new CContralDlg;    // 创建对话框
            m_pContralDlg->Create(IDD_CONTRAL_DIALOG, this);
            m_pContralDlg->MoveWindow(fRect);    // 移动位置
            m_pContralDlg->ShowWindow(SW_SHOW);  // 显示对话框
            break;
        case 2:                                 // 磁盘空间整理模块
            m_pDiskDlg = new CDiskDlg;          // 创建对话框
            m_pDiskDlg->Create(IDD_DISK_DIALOG, this);
            m_pDiskDlg->MoveWindow(fRect);       // 移动位置
            m_pDiskDlg->ShowWindow(SW_SHOW);     // 显示对话框
            break;
        case 3:                                 // 垃圾文件整理模块
            m_pLitterDlg = new CLitterDlg;      // 创建对话框
            m_pLitterDlg->Create(IDD_LITTER_DIALOG, this);
            m_pLitterDlg->MoveWindow(fRect);     // 移动位置
            m_pLitterDlg->ShowWindow(SW_SHOW);   // 显示对话框
            break;
        case 4:                                 // 系统任务管理模块
            m_pTaskDlg->MoveWindow(fRect);       // 移动位置
            m_pTaskDlg->ShowWindow(SW_SHOW);     // 显示对话框
            break;
    }
    m_Num = num;
}

void CSysOptimizeDlg::OnClose()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);    // 销毁当前显示的模块
    }
    m_pTaskDlg->DestroyWindow();    // 销毁系统任务管理模块
    CDialog::OnClose();             // 退出程序
}

void CSysOptimizeDlg::DestroyWindowBox(int num)
{
    switch(num) {                           // 判断销毁的模块
        case 1:                             // 控制面板操作模块
            m_pContralDlg->DestroyWindow(); // 销毁对话框
            break;
        case 2:                             // 磁盘空间整理模块
            m_pDiskDlg->DestroyWindow();    // 销毁对话框
            break;
        case 3:                             // 垃圾文件整理模块
            m_pLitterDlg->DestroyWindow();  // 销毁对话框
            break;
        case 4:                             // 系统任务管理模块
            m_pTaskDlg->ShowWindow(FALSE);  // 隐藏对话框
            break;
    }
}

// 控制面板操作
void CSysOptimizeDlg::OnStacontral()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);                        // 销毁当前打开的模块
    }
    CreateDialogBox(1);                                 // 显示控制面板操作模块
    CRect rect, rc;
    GetDlgItem(IDC_STACONTRAL)->GetClientRect(&rect);   // 获得控件的客户区域
    GetDlgItem(IDC_STACONTRAL)->MapWindowPoints(this,   // 设置窗体中的位置
            rect);
    m_Select.GetClientRect(&rc);
    m_Select.MoveWindow(rect.left - 20,                 // 移动选中效果图片控件
                        rect.top - 6,
                        rc.Width(),
                        rc.Height(), TRUE);
    m_Select.ShowWindow(SW_SHOW);                       // 显示选中效果图片控件
    Invalidate();                                       // 使窗体无效(重绘窗体)
}

// 磁盘空间整理
void CSysOptimizeDlg::OnStadisk()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);
    }
    CreateDialogBox(2);
    CRect rect, rc;
    GetDlgItem(IDC_STADISK)->GetClientRect(&rect);
    GetDlgItem(IDC_STADISK)->MapWindowPoints(this, rect);
    m_Select.GetClientRect(&rc);
    m_Select.MoveWindow(rect.left - 20, rect.top - 6, rc.Width(), rc.Height(), TRUE);
    m_Select.ShowWindow(SW_SHOW);
    Invalidate();
}

// 垃圾文件清理
void CSysOptimizeDlg::OnStalitter()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);
    }
    CreateDialogBox(3);
    CRect rect, rc;
    GetDlgItem(IDC_STALITTER)->GetClientRect(&rect);
    GetDlgItem(IDC_STALITTER)->MapWindowPoints(this, rect);
    m_Select.GetClientRect(&rc);
    m_Select.MoveWindow(rect.left - 20, rect.top - 6, rc.Width(), rc.Height(), TRUE);
    m_Select.ShowWindow(SW_SHOW);
    Invalidate();
}

// 系统任务户理
void CSysOptimizeDlg::OnStatask()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);
    }
    CreateDialogBox(4);
    CRect rect, rc;
    GetDlgItem(IDC_STATASK)->GetClientRect(&rect);
    GetDlgItem(IDC_STATASK)->MapWindowPoints(this, rect);
    m_Select.GetClientRect(&rc);
    m_Select.MoveWindow(rect.left - 20, rect.top - 6, rc.Width(), rc.Height(), TRUE);
    m_Select.ShowWindow(SW_SHOW);
    Invalidate();
}

HBRUSH CSysOptimizeDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if(nCtlColor == CTLCOLOR_DLG) {                 // 如果是对话框
        CBrush m_Brush(&m_BKGround);                // 定义一个位图画刷
        CRect rect;
        GetClientRect(rect);                        // 获取客户区大小
        pDC->SelectObject(&m_Brush);                // 选中画刷
        pDC->FillRect(rect, &m_Brush);              // 填充客户区域
        return m_Brush;                             // 返回新的画刷
    }
    else {
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
    if(nCtlColor == CTLCOLOR_STATIC) {              // 如果是静态文本控件
        pDC->SetBkMode(TRANSPARENT);                // 设置背景透明
    }
    return hbr;
}

// 绘制标题栏及按钮
void CSysOptimizeDlg::DrawDialog(UINT Flags)
{
    CRect WinRC, FactRC;
    // 获得窗口区域
    GetWindowRect(WinRC);
    // 拷贝区域
    FactRC.CopyRect(CRect(0, 0, WinRC.Width(), WinRC.Height()));
    // 获得边框的高
    m_BorderHeight = GetSystemMetrics(SM_CYBORDER);
    // 获得边框的宽
    m_BorderWidth = GetSystemMetrics(SM_CXBORDER);
    // 获得标题栏的高度
    m_CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
    // 获取窗口设备上下文
    CWindowDC WindowDC(this);
    // 创建内存兼容DC
    CDC memDC;
    memDC.CreateCompatibleDC(&WindowDC);

    // 绘制标题
    if(Flags & fTitle) {
        CBitmap bmpTitle, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bmpTitle);
        // 载入标题栏文字
        bmpTitle.LoadBitmap(IDB_TITLE);

        // 获取位图大小
        bmpTitle.GetObject(sizeof(bitmapInfo), &bitmapInfo);

        // 选中该位图
        OldObj = memDC.SelectObject(&bmpTitle);

        int width = bitmapInfo.bmiHeader.biWidth;
        int height = bitmapInfo.bmiHeader.biHeight;

        m_TitleDrawHeight = (m_CaptionHeight + 4 > height) ?
                            m_CaptionHeight + 4 :
                            height;
        CRect rr(FactRC.left, 0, FactRC.right, m_TitleDrawHeight);
        m_TitleRc.CopyRect(rr);

        WindowDC.StretchBlt(m_TitleRc.left, m_TitleRc.top,
                            m_TitleRc.Width(), m_TitleRc.Height(),
                            &memDC, 0, 0, width, height, SRCCOPY);
        bmpTitle.Detach();
        memDC.SelectObject(OldObj);
    }

    // 最小化按钮的大小
    m_MinRect.CopyRect(CRect(m_TitleRc.right - 70, (m_TitleDrawHeight + 2
                             * m_BorderHeight - m_ButtonHeight) / 2,
                             m_ButtonWidth, m_ButtonHeight));
    // 关闭按钮的大小
    m_CloseRect.CopyRect(CRect(m_TitleRc.right - 40, (m_TitleDrawHeight + 2
                               * m_BorderHeight - m_ButtonHeight) / 2,
                               m_ButtonWidth, m_ButtonHeight));
    // 绘制最小化按钮
    if(Flags & fMinButton) {
        CBitmap bitmapMinBtn, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bitmapMinBtn);
        bitmapMinBtn.LoadBitmap(IDB_MINBT);
        // 获取位图大小
        bitmapMinBtn.GetObject(sizeof(bitmapInfo), &bitmapInfo);
        OldObj = memDC.SelectObject(&bitmapMinBtn);
        int width = bitmapInfo.bmiHeader.biWidth;
        int height = bitmapInfo.bmiHeader.biHeight;
        WindowDC.StretchBlt(m_MinRect.left, m_MinRect.top, m_MinRect.right,
                            m_MinRect.bottom, &memDC, 0, 0, width, height, SRCCOPY);
        memDC.SelectObject(OldObj);
        bitmapMinBtn.Detach();
    }

    // 绘制关闭按钮
    if(Flags & fCloseButton) {
        CBitmap bitmapCloseBtn, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bitmapCloseBtn);
        bitmapCloseBtn.LoadBitmap(IDB_CLOSEBT);
        // 获取位图大小
        bitmapCloseBtn.GetObject(sizeof(bitmapInfo), &bitmapInfo);
        OldObj = memDC.SelectObject(&bitmapCloseBtn);
        int width = bitmapInfo.bmiHeader.biWidth;
        int height = bitmapInfo.bmiHeader.biHeight;
        WindowDC.StretchBlt(m_CloseRect.left, m_CloseRect.top, m_CloseRect.right,
                            m_CloseRect.bottom, &memDC, 0, 0, width, height, SRCCOPY);
        memDC.SelectObject(OldObj);
        bitmapCloseBtn.Detach();
    }

    DrawCaption();
}

void CSysOptimizeDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    CRect tempMin, tempMax, tempClose, ClientRect;
    CWindowDC WindowDC(this);   // 获得窗口设备上下文
    CDC memDC;
    memDC.CreateCompatibleDC(&WindowDC);    // 创建内存兼容的设备上下文
    BITMAPINFO bInfo;
    CBitmap LeftLine;
    int x, y;
    GetWindowRect(ClientRect);   // 获得客户区域
    // 设置最小化按钮区域
    tempMin.CopyRect(CRect(m_MinRect.left + ClientRect.left,
                           ClientRect.top + m_MinRect.top,
                           m_MinRect.right + m_MinRect.left + ClientRect.left,
                           m_MinRect.bottom + m_MinRect.top + ClientRect.top));
    // 设置关闭按钮区域
    tempClose.CopyRect(CRect(m_CloseRect.left + ClientRect.left,
                             ClientRect.top + m_CloseRect.top,
                             m_CloseRect.right + m_CloseRect.left +
                             ClientRect.left,
                             m_CloseRect.bottom + m_CloseRect.top +
                             ClientRect.top));
    // 鼠标在最小化按钮上移动时,更改按钮显示的位图
    if(tempMin.PtInRect(point)) {
        // 按钮状态不是最小化
        if(m_ButtonState != bsMin) {
            // 加载最小化热点位图
            LeftLine.LoadBitmap(IDB_MINHOTBT);
            LeftLine.GetObject(sizeof(bInfo), &bInfo);

            x = bInfo.bmiHeader.biWidth; // 位图宽度
            y = bInfo.bmiHeader.biHeight;// 位图高度
            memDC.SelectObject(&LeftLine);
            // 绘制最小化按钮热点效果
            WindowDC.StretchBlt(m_MinRect.left, m_MinRect.top, m_MinRect.right,
                                m_MinRect.bottom, &memDC, 0, 0, x, y, SRCCOPY);
            m_IsDrawForm = FALSE;
            // 设置按钮状态
            m_ButtonState = bsMin;
            LeftLine.Detach();
        }
    }
    // 鼠标在关闭按钮上移动时,更改按钮显示的位图
    else if(tempClose.PtInRect(point)) {
        if(m_ButtonState != bsClose) {
            // 加载关闭按钮热点位图
            LeftLine.LoadBitmap(IDB_CLOSEHOTBT);
            LeftLine.GetObject(sizeof(bInfo), &bInfo);
            x = bInfo.bmiHeader.biWidth; // 位图宽度
            y = bInfo.bmiHeader.biHeight;// 位图高度
            memDC.SelectObject(&LeftLine);
            // 绘制关闭按钮热点效果
            WindowDC.StretchBlt(m_CloseRect.left, m_CloseRect.top, m_CloseRect.right
                                , m_CloseRect.bottom, &memDC, 0, 0, x, y, SRCCOPY);
            m_IsDrawForm = FALSE;
            m_ButtonState = bsClose;
            LeftLine.Detach();
        }
    }
    // 鼠标不在标题栏按钮上
    else {
        // 按钮具有热点效果
        if(m_IsDrawForm == FALSE) {
            // 是最小化按钮
            if(m_ButtonState == bsMin) {
                DrawDialog(fMinButton);   // 重绘最小化按钮
            }
            // 是关闭按钮
            else if(m_ButtonState == bsClose) {
                DrawDialog(fCloseButton);
            }
        }
        m_ButtonState = bsNone;
    }
    LeftLine.DeleteObject();
    ReleaseDC(&memDC);
    CDialog::OnNcMouseMove(nHitTest, point);
}

void CSysOptimizeDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    switch(m_ButtonState) {                   // 判断按钮状态
        case bsClose: {                     // 关闭按钮
            OnCancel();                       // 关闭窗口
        }
        break;
        case bsMin: {                       // 最小化按钮
            ShowWindow(SW_SHOWMINIMIZED);     // 最小化窗体
        }
        break;
    }
    CDialog::OnNcLButtonDown(nHitTest, point);
}

BOOL CSysOptimizeDlg::OnNcActivate(BOOL bActive)
{
    auto b = CDialog::OnNcActivate(bActive);
    DrawDialog(fAll);   // 绘制标题栏
    return b;
}

// 绘制窗体标题文本
void CSysOptimizeDlg::DrawCaption()
{
    // 标题文本不为空
    if(!m_Caption.IsEmpty()) {
        CDC *pDC = GetWindowDC();                 // 获得窗口设备上下文
        pDC->SetBkMode(TRANSPARENT);              // 设置背景透明
        pDC->SetTextColor(m_CapitonColor);        // 设置文本颜色
        pDC->SetTextAlign(TA_CENTER);             // 居中显示
        CRect rect;
        GetClientRect(rect);                      // 获得窗口客户区域
        pDC->SelectObject(&m_CaptionFont);        // 设置字体
        pDC->TextOut(rect.Width() / 2,              // 绘制文本
                     m_CaptionHeight / 3 + 2,
                     m_Caption);
        ReleaseDC(pDC);                           // 释放设备上下文
    }
}

void CSysOptimizeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_IsDrawForm == FALSE) {               // 按钮具有热点效果
        if(m_ButtonState == bsMin) {          // 是最小化按钮
            DrawDialog(fMinButton);           // 重绘最小化按钮
        }
        else if(m_ButtonState == bsClose) {   // 是关闭按钮
            DrawDialog(fCloseButton);         // 重绘关闭按钮
        }
    }
    m_ButtonState = bsNone;
    CDialog::OnMouseMove(nFlags, point);
}

void CSysOptimizeDlg::OnNcPaint()
{
    DrawDialog(fAll);
}

void CSysOptimizeDlg::OnMove(int x, int y)
{
    DrawDialog(fAll);   // 绘制标题栏
    CDialog::OnMove(x, y);
}
