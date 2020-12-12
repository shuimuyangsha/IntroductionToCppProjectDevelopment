// LitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysOptimize.h"
#include "LitterDlg.h"
#include "SelectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
DWORD WINAPI ThreadsProc(LPVOID pParam) ;
/////////////////////////////////////////////////////////////////////////////
// CLitterDlg dialog


CLitterDlg::CLitterDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CLitterDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CLitterDlg)
    //}}AFX_DATA_INIT
}


void CLitterDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLitterDlg)
    DDX_Control(pDX, IDC_LIST1, m_List);
    DDX_Control(pDX, IDC_COMBO1, m_Combo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLitterDlg, CDialog)
    //{{AFX_MSG_MAP(CLitterDlg)
    ON_BN_CLICKED(IDC_BUTSELECT, OnButselect)
    ON_BN_CLICKED(IDC_BUTBEGIN, OnButbegin)
    ON_BN_CLICKED(IDC_BUTSTOP, OnButstop)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLitterDlg message handlers

BOOL CLitterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    DWORD size;
    size = ::GetLogicalDriveStrings(0, NULL);               // 获得驱动器盘符
    if(size != 0) {
        HANDLE heap = ::GetProcessHeap();
        LPSTR lp = (LPSTR)HeapAlloc(heap, HEAP_ZERO_MEMORY, size * sizeof(TCHAR));
        ::GetLogicalDriveStrings(size * sizeof(TCHAR), lp); // 获得下一个驱动器盘符
        while(*lp != 0) {
            UINT res = ::GetDriveType(lp);                  // 获得驱动器类型
            if(res = DRIVE_FIXED) {                         // 是固定硬盘
                m_Combo.AddString(lp);                      // 记录驱动器盘符
            }
            lp = _tcschr(lp, 0) + 1;
        }
    }
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(FALSE);           // 停止按钮不可用
    return TRUE;
}

void CLitterDlg::OnButselect()
{
    CSelectDlg dlg;
    dlg.DoModal();
}

DWORD WINAPI ThreadsProc(LPVOID pParam)
{
    CLitterDlg *pDlg = (CLitterDlg *)pParam;
    pDlg->DeleteLitterFile();
    return 0;
}

void CLitterDlg::OnButbegin()
{
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTBEGIN)->EnableWindow(FALSE);
    ResetEvent(m_hThread);
    DWORD threadID;
    m_hThread = ::CreateThread(NULL, 0, &ThreadsProc, (LPVOID)this, 0, &threadID);
}

void CLitterDlg::OnButstop()
{
    GetDlgItem(IDC_BUTBEGIN)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(FALSE);
    BOOL ret = SetEvent(m_hThread);
    ::TerminateThread(m_hThread, 0);
}

void CLitterDlg::DeleteLitterFile()
{
    CString path;
    m_Combo.GetWindowText(path);        // 获取磁盘
    FileDelete(path);
    ::TerminateThread(m_hThread, 0);    // 终止线程
    GetDlgItem(IDC_BUTBEGIN)->EnableWindow(TRUE); // 开始按钮可用
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(FALSE); // 停止按钮不可用
}

void CLitterDlg::FileDelete(CString FilePath)
{
    CString num, str, Name, FileName;
    CFileFind file;
    if(FilePath.Right(1) != "\\") {
        FilePath += "\\";
    }
    BOOL bf;
    for(int i = 0; i < 25; i++) {
        num.Format("%d", i + 1);
        char ischeck[2];
        // 获得选中垃圾文件
        GetPrivateProfileString("垃圾文件类型", num, "", ischeck,
                                2, "./litterfile.ini");
        str = ischeck;
        if(str == "1") {
            num.Format("%d", i + 31);
            char text[8];
            // 获得垃圾文件类型
            GetPrivateProfileString("垃圾文件类型", num, "", text,
                                    8, "./litterfile.ini");
            FileName = text;
            Name = FilePath + FileName;
            bf = file.FindFile(Name); // 查找文件
            while(bf) {
                bf = file.FindNextFile();
                if(!file.IsDots() && !file.IsDirectory()) { // 如果是垃圾文件
                    DeleteFile(file.GetFilePath());// 删除垃圾文件
                    m_List.InsertString(m_List.GetCount(), FilePath);
                }
            }
        }
    }
    FilePath += "*.*";
    bf = file.FindFile(FilePath); // 查找内容包括目录
    while(bf) {
        bf = file.FindNextFile(); // 查找下一个文件
        if(file.IsDots()) {
            continue;
        }
        else if(file.IsDirectory()) { // 如果是目录
            FilePath = file.GetFilePath();
            // 是目录时,继续递归调用函数删除该目录下的文件
            FileDelete(FilePath);
        }
    }
}

HBRUSH CLitterDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
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
