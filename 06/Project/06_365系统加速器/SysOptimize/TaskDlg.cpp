// TaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "TaskDlg.h"
#include "tlhelp32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg dialog


CTaskDlg::CTaskDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CTaskDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTaskDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CTaskDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTaskDlg)
    DDX_Control(pDX, IDC_LIST1, m_Grid);
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialog)
    //{{AFX_MSG_MAP(CTaskDlg)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
    ON_WM_MEASUREITEM()
    ON_WM_DRAWITEM()
    ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
    ON_COMMAND(ID_MENUREF, OnMenuref)
    ON_COMMAND(ID_MENUSTOP, OnMenustop)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg message handlers

BOOL CTaskDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Menu.LoadMenu(IDR_MENU1);         // 加载菜单资源
    m_Menu.ChangeMenuItem(&m_Menu);
    m_Tab.InsertItem(0, "进程");        // 设置标签页
    m_Tab.InsertItem(1, "窗口");

    m_Grid.SetExtendedStyle(
        LVS_EX_FLATSB                   // 扁平风格滚动条
        | LVS_EX_FULLROWSELECT          // 允许整行选中
        | LVS_EX_HEADERDRAGDROP         // 允许标题拖拽
        | LVS_EX_ONECLICKACTIVATE       // 高亮显示
        | LVS_EX_GRIDLINES              // 画出网格线
    );
    ShowList(0);
    return TRUE;
}

void CTaskDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
    ShowList(m_Tab.GetCurSel());       // 设置列表显示内容
    *pResult = 0;
}
typedef BOOL (__stdcall *funIsHungAppWindow)(HWND hWnd);
void CTaskDlg::ShowList(int num)
{
    // 显示进程列表
    if(num == 0) {
        m_Grid.DeleteAllItems();
        for(int i = 0; i < 4; i++) {
            m_Grid.DeleteColumn(0); // 删除列
        }
        // 设置列
        m_Grid.InsertColumn(0, "映像名称", LVCFMT_LEFT, 100, 0);
        m_Grid.InsertColumn(1, "进程ID", LVCFMT_LEFT, 100, 1);
        m_Grid.InsertColumn(2, "线程数量", LVCFMT_LEFT, 100, 2);
        m_Grid.InsertColumn(3, "优先级别", LVCFMT_LEFT, 100, 3);
        // 生成快照
        HANDLE toolhelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(toolhelp == NULL) {
            return ;
        }
        PROCESSENTRY32 processinfo;
        int i = 0;
        CString str;
        BOOL start = Process32First(toolhelp, &processinfo);    // 获得第一个进程
        while(start) {
            m_Grid.InsertItem(i, "");                           // 插入行
            m_Grid.SetItemText(i, 0, processinfo.szExeFile);    // 获得映像名称
            str.Format("%d", processinfo.th32ProcessID);        // 获得进程ID
            m_Grid.SetItemText(i, 1, str);
            str.Format("%d", processinfo.cntThreads);           // 获得线程数量
            m_Grid.SetItemText(i, 2, str);
            str.Format("%d", processinfo.pcPriClassBase);       // 获得优先级别
            m_Grid.SetItemText(i, 3, str);
            start = Process32Next(toolhelp, &processinfo);      // 获得下一个进程
            i++;
        }
    }
    // 显示窗口列表
    else {
        m_Grid.DeleteAllItems();
        for(int i = 0; i < 6; i++) {
            m_Grid.DeleteColumn(0);                             // 删除列
        }
        m_Grid.InsertColumn(0, "窗口", LVCFMT_LEFT, 200);       // 设置列
        m_Grid.InsertColumn(1, "状态", LVCFMT_LEFT, 100);
        m_Grid.InsertColumn(2, "进程ID", LVCFMT_LEFT, 100);
        HINSTANCE hInstance = LoadLibrary("user32.dll");        // 加载动态库
        auto IsHungAppWindow = (funIsHungAppWindow)
                               GetProcAddress(hInstance, "IsHungAppWindow");
        CWnd *pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);  // 获得窗口句柄
        int i = 0;
        CString cstrCap;
        // 遍历窗口
        while(pWnd) {                                           // 遍历窗口
            // 窗口可见,并且是顶层窗口
            if(pWnd->IsWindowVisible() && !pWnd->GetOwner()) {
                pWnd->GetWindowText(cstrCap);
                if(! cstrCap.IsEmpty()) {
                    m_Grid.InsertItem(i, cstrCap);
                    if(IsHungAppWindow(pWnd->m_hWnd)) {         // 判断程序是否响应
                        m_Grid.SetItemText(i, 1, "不响应");
                    }
                    else {
                        m_Grid.SetItemText(i, 1, "正在运行");
                    }
                    //
                    DWORD dwProcessId;
                    GetWindowThreadProcessId(pWnd->GetSafeHwnd(), &dwProcessId);
                    CString str;
                    str.Format(_T("%d"), dwProcessId);
                    m_Grid.SetItemText(i, 2, str.GetString());
                    i++;
                }
            }
            pWnd = pWnd->GetWindow(GW_HWNDNEXT);                //搜索下一个窗口
        }
    }
}

void CTaskDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // TODO: Add your message handler code here and/or call default
    m_Menu.MeasureItem(lpMeasureItemStruct);
    CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CTaskDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your message handler code here and/or call default
    m_Menu.DrawItem(lpDrawItemStruct);
    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CTaskDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    int pos = m_Grid.GetSelectionMark();
    CPoint point;
    GetCursorPos(&point);
    CMenu *pPopup = m_Menu.GetSubMenu(0);
    CRect rc;
    rc.top = point.x;
    rc.left = point.y;
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
                           rc.top, rc.left, this, &rc);
    *pResult = 0;
}

void CTaskDlg::OnMenuref()
{
    // TODO: Add your command handler code here
    ShowList(m_Tab.GetCurSel());
}

void CTaskDlg::OnMenustop()
{
    //获得当前列表项索引
    int pos = m_Grid.GetSelectionMark();
    CString str = m_Grid.GetItemText(pos, 2);                   //获得进程ID
    DWORD data = atoi(str.GetString());
    HANDLE hProcess;
    // 打开进程
    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, data);     // 打开进程 
    if(hProcess) {
        if(!TerminateProcess(hProcess, 0)) {                    // 终止进程
            CString strError;
            strError.Format("错误号:%d", GetLastError());
            AfxMessageBox(strError, MB_OK | MB_ICONINFORMATION, NULL);
        }
    }
    else {
        CString strError;
        strError.Format("错误号:%d", GetLastError());
        if(GetLastError() == ERROR_ACCESS_DENIED) {
            strError = _T("拒绝访问!") + strError;
        }
        AfxMessageBox(strError, MB_OK | MB_ICONINFORMATION, NULL);
    }
    Sleep(300);                                                 // 设置延时
    OnMenuref();                                                // 刷新列表
}

HBRUSH CTaskDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    CBitmap m_BKGround;
    m_BKGround.LoadBitmap(IDB_BITBLANK);
    if(nCtlColor == CTLCOLOR_DLG) {
        // 定义一个位图画刷
        CBrush m_Brush(&m_BKGround);
        CRect rect;
        GetClientRect(rect);
        // 选中画刷
        pDC->SelectObject(&m_Brush);
        // 填充客户区域
        pDC->FillRect(rect, &m_Brush);
        return m_Brush;
    }
    else {
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
    }
    return hbr;
}
