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

    // �����ı���ɫ
    m_CapitonColor = RGB(0, 0, 255);
    // �����ı�
    m_Caption = "365ϵͳ������";
    // �����ı�����
    m_CaptionFont.CreateFont(14, 10, 0, 3, 600, 0, 0, 0, ANSI_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, FF_ROMAN, "����");
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

    // ��ʼ��
    m_Num = 0;
    m_pTaskDlg = new CTaskDlg;
    m_pTaskDlg->Create(IDD_TASK_DIALOG, this);       // ����ϵͳ�������ģ��
    m_Select.ShowWindow(SW_HIDE);                    // ����ѡ��Ч��ͼƬ
    SetWindowText("365ϵͳ������");                     // �������������
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MINBT);                    // ������С����ťλͼ
    BITMAPINFO bInfo;
    bitmap.GetObject(sizeof(bInfo), &bInfo);          // ���ͼƬ��Ϣ
    m_ButtonWidth = bInfo.bmiHeader.biWidth;        // λͼ���
    m_ButtonHeight = bInfo.bmiHeader.biHeight;      // λͼ�߶�
    bitmap.DeleteObject();

    // ����ͼƬ
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
        DrawDialog(fAll);   // ���Ʊ�����
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
    m_Frame.GetClientRect(&fRect);               // ���ͼƬ�ؼ��Ŀͻ�����
    m_Frame.MapWindowPoints(this, fRect);        // ����ģ�����ʾλ��
    switch(num) {                                // �ж���ʾ��ģ��
        case 1:                                 // ����������ģ��
            m_pContralDlg = new CContralDlg;    // �����Ի���
            m_pContralDlg->Create(IDD_CONTRAL_DIALOG, this);
            m_pContralDlg->MoveWindow(fRect);    // �ƶ�λ��
            m_pContralDlg->ShowWindow(SW_SHOW);  // ��ʾ�Ի���
            break;
        case 2:                                 // ���̿ռ�����ģ��
            m_pDiskDlg = new CDiskDlg;          // �����Ի���
            m_pDiskDlg->Create(IDD_DISK_DIALOG, this);
            m_pDiskDlg->MoveWindow(fRect);       // �ƶ�λ��
            m_pDiskDlg->ShowWindow(SW_SHOW);     // ��ʾ�Ի���
            break;
        case 3:                                 // �����ļ�����ģ��
            m_pLitterDlg = new CLitterDlg;      // �����Ի���
            m_pLitterDlg->Create(IDD_LITTER_DIALOG, this);
            m_pLitterDlg->MoveWindow(fRect);     // �ƶ�λ��
            m_pLitterDlg->ShowWindow(SW_SHOW);   // ��ʾ�Ի���
            break;
        case 4:                                 // ϵͳ�������ģ��
            m_pTaskDlg->MoveWindow(fRect);       // �ƶ�λ��
            m_pTaskDlg->ShowWindow(SW_SHOW);     // ��ʾ�Ի���
            break;
    }
    m_Num = num;
}

void CSysOptimizeDlg::OnClose()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);    // ���ٵ�ǰ��ʾ��ģ��
    }
    m_pTaskDlg->DestroyWindow();    // ����ϵͳ�������ģ��
    CDialog::OnClose();             // �˳�����
}

void CSysOptimizeDlg::DestroyWindowBox(int num)
{
    switch(num) {                           // �ж����ٵ�ģ��
        case 1:                             // ����������ģ��
            m_pContralDlg->DestroyWindow(); // ���ٶԻ���
            break;
        case 2:                             // ���̿ռ�����ģ��
            m_pDiskDlg->DestroyWindow();    // ���ٶԻ���
            break;
        case 3:                             // �����ļ�����ģ��
            m_pLitterDlg->DestroyWindow();  // ���ٶԻ���
            break;
        case 4:                             // ϵͳ�������ģ��
            m_pTaskDlg->ShowWindow(FALSE);  // ���ضԻ���
            break;
    }
}

// ����������
void CSysOptimizeDlg::OnStacontral()
{
    if(m_Num != 0) {
        DestroyWindowBox(m_Num);                        // ���ٵ�ǰ�򿪵�ģ��
    }
    CreateDialogBox(1);                                 // ��ʾ����������ģ��
    CRect rect, rc;
    GetDlgItem(IDC_STACONTRAL)->GetClientRect(&rect);   // ��ÿؼ��Ŀͻ�����
    GetDlgItem(IDC_STACONTRAL)->MapWindowPoints(this,   // ���ô����е�λ��
            rect);
    m_Select.GetClientRect(&rc);
    m_Select.MoveWindow(rect.left - 20,                 // �ƶ�ѡ��Ч��ͼƬ�ؼ�
                        rect.top - 6,
                        rc.Width(),
                        rc.Height(), TRUE);
    m_Select.ShowWindow(SW_SHOW);                       // ��ʾѡ��Ч��ͼƬ�ؼ�
    Invalidate();                                       // ʹ������Ч(�ػ洰��)
}

// ���̿ռ�����
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

// �����ļ�����
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

// ϵͳ������
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

    if(nCtlColor == CTLCOLOR_DLG) {                 // ����ǶԻ���
        CBrush m_Brush(&m_BKGround);                // ����һ��λͼ��ˢ
        CRect rect;
        GetClientRect(rect);                        // ��ȡ�ͻ�����С
        pDC->SelectObject(&m_Brush);                // ѡ�л�ˢ
        pDC->FillRect(rect, &m_Brush);              // ���ͻ�����
        return m_Brush;                             // �����µĻ�ˢ
    }
    else {
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
    if(nCtlColor == CTLCOLOR_STATIC) {              // ����Ǿ�̬�ı��ؼ�
        pDC->SetBkMode(TRANSPARENT);                // ���ñ���͸��
    }
    return hbr;
}

// ���Ʊ���������ť
void CSysOptimizeDlg::DrawDialog(UINT Flags)
{
    CRect WinRC, FactRC;
    // ��ô�������
    GetWindowRect(WinRC);
    // ��������
    FactRC.CopyRect(CRect(0, 0, WinRC.Width(), WinRC.Height()));
    // ��ñ߿�ĸ�
    m_BorderHeight = GetSystemMetrics(SM_CYBORDER);
    // ��ñ߿�Ŀ�
    m_BorderWidth = GetSystemMetrics(SM_CXBORDER);
    // ��ñ������ĸ߶�
    m_CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
    // ��ȡ�����豸������
    CWindowDC WindowDC(this);
    // �����ڴ����DC
    CDC memDC;
    memDC.CreateCompatibleDC(&WindowDC);

    // ���Ʊ���
    if(Flags & fTitle) {
        CBitmap bmpTitle, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bmpTitle);
        // �������������
        bmpTitle.LoadBitmap(IDB_TITLE);

        // ��ȡλͼ��С
        bmpTitle.GetObject(sizeof(bitmapInfo), &bitmapInfo);

        // ѡ�и�λͼ
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

    // ��С����ť�Ĵ�С
    m_MinRect.CopyRect(CRect(m_TitleRc.right - 70, (m_TitleDrawHeight + 2
                             * m_BorderHeight - m_ButtonHeight) / 2,
                             m_ButtonWidth, m_ButtonHeight));
    // �رհ�ť�Ĵ�С
    m_CloseRect.CopyRect(CRect(m_TitleRc.right - 40, (m_TitleDrawHeight + 2
                               * m_BorderHeight - m_ButtonHeight) / 2,
                               m_ButtonWidth, m_ButtonHeight));
    // ������С����ť
    if(Flags & fMinButton) {
        CBitmap bitmapMinBtn, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bitmapMinBtn);
        bitmapMinBtn.LoadBitmap(IDB_MINBT);
        // ��ȡλͼ��С
        bitmapMinBtn.GetObject(sizeof(bitmapInfo), &bitmapInfo);
        OldObj = memDC.SelectObject(&bitmapMinBtn);
        int width = bitmapInfo.bmiHeader.biWidth;
        int height = bitmapInfo.bmiHeader.biHeight;
        WindowDC.StretchBlt(m_MinRect.left, m_MinRect.top, m_MinRect.right,
                            m_MinRect.bottom, &memDC, 0, 0, width, height, SRCCOPY);
        memDC.SelectObject(OldObj);
        bitmapMinBtn.Detach();
    }

    // ���ƹرհ�ť
    if(Flags & fCloseButton) {
        CBitmap bitmapCloseBtn, *OldObj;
        BITMAPINFO bitmapInfo;
        DeleteObject(bitmapCloseBtn);
        bitmapCloseBtn.LoadBitmap(IDB_CLOSEBT);
        // ��ȡλͼ��С
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
    CWindowDC WindowDC(this);   // ��ô����豸������
    CDC memDC;
    memDC.CreateCompatibleDC(&WindowDC);    // �����ڴ���ݵ��豸������
    BITMAPINFO bInfo;
    CBitmap LeftLine;
    int x, y;
    GetWindowRect(ClientRect);   // ��ÿͻ�����
    // ������С����ť����
    tempMin.CopyRect(CRect(m_MinRect.left + ClientRect.left,
                           ClientRect.top + m_MinRect.top,
                           m_MinRect.right + m_MinRect.left + ClientRect.left,
                           m_MinRect.bottom + m_MinRect.top + ClientRect.top));
    // ���ùرհ�ť����
    tempClose.CopyRect(CRect(m_CloseRect.left + ClientRect.left,
                             ClientRect.top + m_CloseRect.top,
                             m_CloseRect.right + m_CloseRect.left +
                             ClientRect.left,
                             m_CloseRect.bottom + m_CloseRect.top +
                             ClientRect.top));
    // �������С����ť���ƶ�ʱ,���İ�ť��ʾ��λͼ
    if(tempMin.PtInRect(point)) {
        // ��ť״̬������С��
        if(m_ButtonState != bsMin) {
            // ������С���ȵ�λͼ
            LeftLine.LoadBitmap(IDB_MINHOTBT);
            LeftLine.GetObject(sizeof(bInfo), &bInfo);

            x = bInfo.bmiHeader.biWidth; // λͼ���
            y = bInfo.bmiHeader.biHeight;// λͼ�߶�
            memDC.SelectObject(&LeftLine);
            // ������С����ť�ȵ�Ч��
            WindowDC.StretchBlt(m_MinRect.left, m_MinRect.top, m_MinRect.right,
                                m_MinRect.bottom, &memDC, 0, 0, x, y, SRCCOPY);
            m_IsDrawForm = FALSE;
            // ���ð�ť״̬
            m_ButtonState = bsMin;
            LeftLine.Detach();
        }
    }
    // ����ڹرհ�ť���ƶ�ʱ,���İ�ť��ʾ��λͼ
    else if(tempClose.PtInRect(point)) {
        if(m_ButtonState != bsClose) {
            // ���عرհ�ť�ȵ�λͼ
            LeftLine.LoadBitmap(IDB_CLOSEHOTBT);
            LeftLine.GetObject(sizeof(bInfo), &bInfo);
            x = bInfo.bmiHeader.biWidth; // λͼ���
            y = bInfo.bmiHeader.biHeight;// λͼ�߶�
            memDC.SelectObject(&LeftLine);
            // ���ƹرհ�ť�ȵ�Ч��
            WindowDC.StretchBlt(m_CloseRect.left, m_CloseRect.top, m_CloseRect.right
                                , m_CloseRect.bottom, &memDC, 0, 0, x, y, SRCCOPY);
            m_IsDrawForm = FALSE;
            m_ButtonState = bsClose;
            LeftLine.Detach();
        }
    }
    // ��겻�ڱ�������ť��
    else {
        // ��ť�����ȵ�Ч��
        if(m_IsDrawForm == FALSE) {
            // ����С����ť
            if(m_ButtonState == bsMin) {
                DrawDialog(fMinButton);   // �ػ���С����ť
            }
            // �ǹرհ�ť
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
    switch(m_ButtonState) {                   // �жϰ�ť״̬
        case bsClose: {                     // �رհ�ť
            OnCancel();                       // �رմ���
        }
        break;
        case bsMin: {                       // ��С����ť
            ShowWindow(SW_SHOWMINIMIZED);     // ��С������
        }
        break;
    }
    CDialog::OnNcLButtonDown(nHitTest, point);
}

BOOL CSysOptimizeDlg::OnNcActivate(BOOL bActive)
{
    auto b = CDialog::OnNcActivate(bActive);
    DrawDialog(fAll);   // ���Ʊ�����
    return b;
}

// ���ƴ�������ı�
void CSysOptimizeDlg::DrawCaption()
{
    // �����ı���Ϊ��
    if(!m_Caption.IsEmpty()) {
        CDC *pDC = GetWindowDC();                 // ��ô����豸������
        pDC->SetBkMode(TRANSPARENT);              // ���ñ���͸��
        pDC->SetTextColor(m_CapitonColor);        // �����ı���ɫ
        pDC->SetTextAlign(TA_CENTER);             // ������ʾ
        CRect rect;
        GetClientRect(rect);                      // ��ô��ڿͻ�����
        pDC->SelectObject(&m_CaptionFont);        // ��������
        pDC->TextOut(rect.Width() / 2,              // �����ı�
                     m_CaptionHeight / 3 + 2,
                     m_Caption);
        ReleaseDC(pDC);                           // �ͷ��豸������
    }
}

void CSysOptimizeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_IsDrawForm == FALSE) {               // ��ť�����ȵ�Ч��
        if(m_ButtonState == bsMin) {          // ����С����ť
            DrawDialog(fMinButton);           // �ػ���С����ť
        }
        else if(m_ButtonState == bsClose) {   // �ǹرհ�ť
            DrawDialog(fCloseButton);         // �ػ�رհ�ť
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
    DrawDialog(fAll);   // ���Ʊ�����
    CDialog::OnMove(x, y);
}
