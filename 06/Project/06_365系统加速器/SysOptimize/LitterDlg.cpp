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
    size = ::GetLogicalDriveStrings(0, NULL);               // ����������̷�
    if(size != 0) {
        HANDLE heap = ::GetProcessHeap();
        LPSTR lp = (LPSTR)HeapAlloc(heap, HEAP_ZERO_MEMORY, size * sizeof(TCHAR));
        ::GetLogicalDriveStrings(size * sizeof(TCHAR), lp); // �����һ���������̷�
        while(*lp != 0) {
            UINT res = ::GetDriveType(lp);                  // �������������
            if(res = DRIVE_FIXED) {                         // �ǹ̶�Ӳ��
                m_Combo.AddString(lp);                      // ��¼�������̷�
            }
            lp = _tcschr(lp, 0) + 1;
        }
    }
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(FALSE);           // ֹͣ��ť������
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
    m_Combo.GetWindowText(path);        // ��ȡ����
    FileDelete(path);
    ::TerminateThread(m_hThread, 0);    // ��ֹ�߳�
    GetDlgItem(IDC_BUTBEGIN)->EnableWindow(TRUE); // ��ʼ��ť����
    GetDlgItem(IDC_BUTSTOP)->EnableWindow(FALSE); // ֹͣ��ť������
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
        // ���ѡ�������ļ�
        GetPrivateProfileString("�����ļ�����", num, "", ischeck,
                                2, "./litterfile.ini");
        str = ischeck;
        if(str == "1") {
            num.Format("%d", i + 31);
            char text[8];
            // ��������ļ�����
            GetPrivateProfileString("�����ļ�����", num, "", text,
                                    8, "./litterfile.ini");
            FileName = text;
            Name = FilePath + FileName;
            bf = file.FindFile(Name); // �����ļ�
            while(bf) {
                bf = file.FindNextFile();
                if(!file.IsDots() && !file.IsDirectory()) { // ����������ļ�
                    DeleteFile(file.GetFilePath());// ɾ�������ļ�
                    m_List.InsertString(m_List.GetCount(), FilePath);
                }
            }
        }
    }
    FilePath += "*.*";
    bf = file.FindFile(FilePath); // �������ݰ���Ŀ¼
    while(bf) {
        bf = file.FindNextFile(); // ������һ���ļ�
        if(file.IsDots()) {
            continue;
        }
        else if(file.IsDirectory()) { // �����Ŀ¼
            FilePath = file.GetFilePath();
            // ��Ŀ¼ʱ,�����ݹ���ú���ɾ����Ŀ¼�µ��ļ�
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
        // ����һ��λͼ��ˢ
        CBrush m_Brush(&m_BKGround);
        CRect rect;
        GetClientRect(rect);
        // ѡ�л�ˢ
        pDC->SelectObject(&m_Brush);
        // ���ͻ�����
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
