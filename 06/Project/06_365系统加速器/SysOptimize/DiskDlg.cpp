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
        LVS_EX_FLATSB             // ��ƽ��������
        | LVS_EX_FULLROWSELECT    // ��������ѡ��
        | LVS_EX_HEADERDRAGDROP   // ���������ק
        | LVS_EX_ONECLICKACTIVATE // ������ʾ
        | LVS_EX_GRIDLINES        // ����������
    );
    m_Grid.InsertColumn(0, "������Ŀ", LVCFMT_LEFT, 190, 0);
    m_Grid.InsertColumn(1, "��Ŀ˵��", LVCFMT_LEFT, 332, 1);
    m_Grid.InsertItem(0, "��ջ���վ");
    m_Grid.SetItemText(0, 1, "��ϵͳ����վ�е����ݳ���ɾ��");
    m_Grid.InsertItem(1, "���Internet��ʱ�ļ�");
    m_Grid.SetItemText(1, 1, "ɾ��ʹ��IE����������Ϣʱ��������ʱ�ļ�");
    m_Grid.InsertItem(2, "���Windows��ʱ�ļ���");
    m_Grid.SetItemText(2, 1, "ɾ����Windows�б������ʱ�ļ� Cookies�ļ�");
    m_Grid.InsertItem(3, "����ļ��򿪼�¼");
    m_Grid.SetItemText(3, 1, "��ա���ʼ��/������򿪵��ĵ����еļ�¼");
    m_Grid.InsertItem(4, "���IE��ַ���еļ�¼");
    m_Grid.SetItemText(4, 1, "���IE��ַ���з��ʹ��ĵ�ַ");
    m_Grid.InsertItem(5, "������м�¼");
    m_Grid.SetItemText(5, 1, "��ա���ʼ��/�����С��е���ʷ��¼");
    CFont font;
    font.CreatePointFont(120, "����"); // ��������
    m_Grid.SetFont(&font);             // �����б�����
    m_Text.SetFont(&font);             // ���þ�̬�ı��ؼ�����
    m_Text1.SetFont(&font);            // ���þ�̬�ı��ؼ�����
    m_Text2.SetFont(&font);            // ���þ�̬�ı��ؼ�����
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiskDlg::OnCheckll()
{
    int allcheck = m_CheckAll.GetCheck();                    // ���ȫѡ��ѡ���״̬
    for(int i = 0; i < m_Grid.GetItemCount(); i++) {         // �����б����ݸ���ѭ��
        auto *check = (CButton *)GetDlgItem(IDC_CHECK2 + i); // ��ø�ѡ��ָ��
        check->SetCheck(allcheck);                           // ���ø�ѡ��״̬
    }
}

void CDiskDlg::OnButclear()
{
    for(int i = 0; i < m_Grid.GetItemCount(); i++) {         // �����б����ݸ���ѭ��
        auto *check = (CButton *)GetDlgItem(IDC_CHECK2 + i); // ��ø�ѡ��ָ��
        if(check->GetCheck() == 1) {                         // ���ѡ��
            ClearDisk(i);                                    // �����Ӧ��Ŀ
        }
    }
    MessageBox("���");
}

void CDiskDlg::ClearDisk(int num)
{
    LPINTERNET_CACHE_ENTRY_INFO pEntry = NULL;
    HANDLE hDir = NULL;
    HANDLE hTemp = NULL;
    unsigned long size = 4096;
    int i = 0;
    BOOL isEnd = FALSE;                     // ��¼�Ƿ����
    BOOL ret = TRUE;                        // ��¼�Ƿ�ɹ�
    HKEY sub;
    DWORD val = 0x00000001;                 // ע����ֵ
    CString skey;
    char buffer[128];                       // ����ϵͳĿ¼·��
    CString syspath;                        // ������ʱ�ļ���·��
    switch(num) {                           // �ж��������Ŀ
        case 0:                             // ��ջ���վ
            GetWindowLong(m_hWnd, 0);
            SHEmptyRecycleBin(m_hWnd, NULL, SHERB_NOCONFIRMATION
                              || SHERB_NOPROGRESSUI
                              || SHERB_NOSOUND);
            break;
        case 1:                             // ���Internet��ʱ�ļ�
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
        case 2:                             // ���Windows��ʱ�ļ���
            ::GetSystemDirectory(buffer, 128);
            syspath = buffer;
            syspath.Replace("system32", "temp");
            DelFolder(syspath);
            RemoveDirectory(syspath);       // Ŀ¼Ϊ��ʱɾ��Ŀ¼
            break;
        case 3:                             // ����ļ��򿪼�¼
        case 5:                             // ������м�¼
            skey = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
            ::RegCreateKey(HKEY_CURRENT_USER, skey, &sub);
            RegSetValueEx(sub, "ClearRecentDocsonExit", NULL, REG_BINARY, (BYTE *)&val, 4);
            ::RegCloseKey(sub);
            break;
        case 4:                             // ���IE��ַ���еļ�¼
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

void CDiskDlg::DelFolder(CString path)
{
    CFileFind file;
    if(path.Right(1) != "\\") {
        path += "\\*.*";
    }
    BOOL bf;
    bf = file.FindFile(path);                       // �����ļ�
    while(bf) {
        bf = file.FindNextFile();                   // ������һ���ļ�
        if(!file.IsDots() && !file.IsDirectory()) { // ���ļ�ʱֱ��ɾ��
            DeleteFile(file.GetFilePath());         // ɾ���ļ�
        }
        else if(file.IsDots()) {
            continue;
        }
        else if(file.IsDirectory()) {
            path = file.GetFilePath();              // ���Ŀ¼·��
            // ��Ŀ¼ʱ,�����ݹ���ú���ɾ����Ŀ¼�µ��ļ�
            DelFolder(path);
            RemoveDirectory(path);                  // Ŀ¼Ϊ�պ�ɾ��Ŀ¼
        }
    }
}
