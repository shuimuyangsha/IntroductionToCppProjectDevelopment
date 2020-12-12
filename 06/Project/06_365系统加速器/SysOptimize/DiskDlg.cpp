// DiskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "DiskDlg.h"
#include "wininet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib,"wininet.lib")
/////////////////////////////////////////////////////////////////////////////
// CDiskDlg dialog


CDiskDlg::CDiskDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CDiskDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDiskDlg)
    //}}AFX_DATA_INIT
}


void CDiskDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDiskDlg)
    DDX_Control(pDX, IDC_STATEXT2, m_Text2);
    DDX_Control(pDX, IDC_STATEXT1, m_Text1);
    DDX_Control(pDX, IDC_STATEXT, m_Text);
    DDX_Control(pDX, IDC_CHECKLL, m_CheckAll);
    DDX_Control(pDX, IDC_LIST1, m_Grid);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiskDlg, CDialog)
    //{{AFX_MSG_MAP(CDiskDlg)
    ON_BN_CLICKED(IDC_CHECKLL, OnCheckll)
    ON_BN_CLICKED(IDC_BUTCLEAR, OnButclear)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiskDlg message handlers

BOOL CDiskDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Grid.SetExtendedStyle(
        LVS_EX_FLATSB             // 扁平风格滚动条
        | LVS_EX_FULLROWSELECT    // 允许整行选中
        | LVS_EX_HEADERDRAGDROP   // 允许标题拖拽
        | LVS_EX_ONECLICKACTIVATE // 高亮显示
        | LVS_EX_GRIDLINES        // 画出网格线
    );
    m_Grid.InsertColumn(0, "清理项目", LVCFMT_LEFT, 190, 0);
    m_Grid.InsertColumn(1, "项目说明", LVCFMT_LEFT, 332, 1);
    m_Grid.InsertItem(0, "清空回收站");
    m_Grid.SetItemText(0, 1, "将系统回收站中的内容彻底删除");
    m_Grid.InsertItem(1, "清空Internet临时文件");
    m_Grid.SetItemText(1, 1, "删除使用IE浏览器浏览信息时产生的临时文件");
    m_Grid.InsertItem(2, "清空Windows临时文件夹");
    m_Grid.SetItemText(2, 1, "删除在Windows中保存的临时文件 Cookies文件");
    m_Grid.InsertItem(3, "清空文件打开记录");
    m_Grid.SetItemText(3, 1, "清空【开始】/【最近打开的文档】中的记录");
    m_Grid.InsertItem(4, "清空IE地址栏中的记录");
    m_Grid.SetItemText(4, 1, "清除IE地址栏中访问过的地址");
    m_Grid.InsertItem(5, "清空运行记录");
    m_Grid.SetItemText(5, 1, "清空【开始】/【运行】中的历史记录");
    CFont font;
    font.CreatePointFont(120, "宋体"); // 创建字体
    m_Grid.SetFont(&font);             // 设置列表字体
    m_Text.SetFont(&font);             // 设置静态文本控件字体
    m_Text1.SetFont(&font);            // 设置静态文本控件字体
    m_Text2.SetFont(&font);            // 设置静态文本控件字体
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiskDlg::OnCheckll()
{
    int allcheck = m_CheckAll.GetCheck();                    // 获得全选复选框的状态
    for(int i = 0; i < m_Grid.GetItemCount(); i++) {         // 根据列表数据个数循环
        auto *check = (CButton *)GetDlgItem(IDC_CHECK2 + i); // 获得复选框指针
        check->SetCheck(allcheck);                           // 设置复选框状态
    }
}

void CDiskDlg::OnButclear()
{
    for(int i = 0; i < m_Grid.GetItemCount(); i++) {         // 根据列表数据个数循环
        auto *check = (CButton *)GetDlgItem(IDC_CHECK2 + i); // 获得复选框指针
        if(check->GetCheck() == 1) {                         // 如果选中
            ClearDisk(i);                                    // 清除对应项目
        }
    }
    MessageBox("完成");
}

void CDiskDlg::ClearDisk(int num)
{
    LPINTERNET_CACHE_ENTRY_INFO pEntry = NULL;
    HANDLE hDir = NULL;
    HANDLE hTemp = NULL;
    unsigned long size = 4096;
    int i = 0;
    BOOL isEnd = FALSE;                     // 记录是否结束
    BOOL ret = TRUE;                        // 记录是否成功
    HKEY sub;
    DWORD val = 0x00000001;                 // 注册表键值
    CString skey;
    char buffer[128];                       // 保存系统目录路径
    CString syspath;                        // 保存临时文件夹路径
    switch(num) {                           // 判断清除的项目
        case 0:                             // 清空回收站
            GetWindowLong(m_hWnd, 0);
            SHEmptyRecycleBin(m_hWnd, NULL, SHERB_NOCONFIRMATION
                              || SHERB_NOPROGRESSUI
                              || SHERB_NOSOUND);
            break;
        case 1:                             // 清空Internet临时文件
            do {
                pEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[4096];
                pEntry->dwStructSize = 4096;
                if(hDir == NULL) {
                    hDir =  FindFirstUrlCacheEntry(NULL, pEntry, &size);
                    if(hDir) {
                        DeleteUrlCacheEntry(pEntry->lpszSourceUrlName);
                    }
                }
                else {
                    ret = FindNextUrlCacheEntry(hDir, pEntry, &size);
                    if(ret) {
                        DeleteUrlCacheEntry(pEntry->lpszSourceUrlName);
                    }
                }
                if(ret) {
                    while(ret) {
                        ret = FindNextUrlCacheEntry(hDir, pEntry, &size);
                        if(ret) {
                            DeleteUrlCacheEntry(pEntry->lpszSourceUrlName);
                        }
                    }
                }
                else {
                    isEnd = TRUE;
                }
                delete []pEntry;
            }
            while(!isEnd);
            FindCloseUrlCache(hDir);
            break;
        case 2:                             // 清空Windows临时文件夹
            ::GetSystemDirectory(buffer, 128);
            syspath = buffer;
            syspath.Replace("system32", "temp");
            DelFolder(syspath);
            RemoveDirectory(syspath);       // 目录为空时删除目录
            break;
        case 3:                             // 清空文件打开记录
        case 5:                             // 清空运行记录
            skey = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
            ::RegCreateKey(HKEY_CURRENT_USER, skey, &sub);
            RegSetValueEx(sub, "ClearRecentDocsonExit", NULL, REG_BINARY, (BYTE *)&val, 4);
            ::RegCloseKey(sub);
            break;
        case 4:                             // 清空IE地址栏中的记录
            skey = "Software\\Microsoft\\Internet Explorer\\TypedURLs";
            ::RegDeleteKey(HKEY_CURRENT_USER, skey);
            break;
    }
}

HBRUSH CDiskDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
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

void CDiskDlg::DelFolder(CString path)
{
    CFileFind file;
    if(path.Right(1) != "\\") {
        path += "\\*.*";
    }
    BOOL bf;
    bf = file.FindFile(path);                       // 查找文件
    while(bf) {
        bf = file.FindNextFile();                   // 查找下一个文件
        if(!file.IsDots() && !file.IsDirectory()) { // 是文件时直接删除
            DeleteFile(file.GetFilePath());         // 删除文件
        }
        else if(file.IsDots()) {
            continue;
        }
        else if(file.IsDirectory()) {
            path = file.GetFilePath();              // 获得目录路径
            // 是目录时,继续递归调用函数删除该目录下的文件
            DelFolder(path);
            RemoveDirectory(path);                  // 目录为空后删除目录
        }
    }
}
