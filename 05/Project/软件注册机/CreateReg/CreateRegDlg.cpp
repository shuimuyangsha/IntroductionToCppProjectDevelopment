// CreateRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CreateReg.h"
#include "CreateRegDlg.h"

#include "nb30.h"
#pragma comment (lib,"netapi32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
struct ADAPTER_INFO {
    ADAPTER_STATUS nStatus;
    NAME_BUFFER    nBuffer;
};

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
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
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
// CCreateRegDlg dialog

CCreateRegDlg::CCreateRegDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CCreateRegDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCreateRegDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateRegDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCreateRegDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCreateRegDlg, CDialog)
    //{{AFX_MSG_MAP(CCreateRegDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateRegDlg message handlers

BOOL CCreateRegDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
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

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    HKEY key;
    LPCTSTR skey = "Software\\mingrisoft";
    long iret    = RegCreateKey(HKEY_CURRENT_USER, skey, &key);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCreateRegDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateRegDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCreateRegDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CCreateRegDlg::OnOK()
{
    // TODO: Add extra validation here
    CString name;
    GetDlgItem(IDC_EDIT1)->GetWindowText(name);
    if(name.IsEmpty()) {
        MessageBox("�û�������Ϊ�գ�");
        return;
    }
    //����һ����Կ����
    CString code[16] = {"ah", "tm", "ib", "nw", "rt", "vx", "zc", "gf",
                        "pn", "xq", "fc", "oj", "wm", "eq", "np", "qw"
                       };
    CString reg, stred;
    int num;
    stred = GetCPUNum() + GetDiskNum() + GetMacAddress();
    stred.MakeLower();
    //����ʮ���������ִ���Կ������ѡ���ַ���
    for(int i = 0; i < 10; i++) {
        char p = stred.GetAt(i);
        if(p >= 'a' && p <= 'f') {
            num = p - 'a' + 10;
        }
        else {
            num = p - '0';
        }
        CString tmp = code[num];
        reg += tmp;
    }

    // ���ת���ɴ�д
    reg.MakeUpper();

    // ���ý����ϱ༭�������
    GetDlgItem(IDC_EDIT2)->SetWindowText(reg.Mid(0, 5));
    GetDlgItem(IDC_EDIT3)->SetWindowText(reg.Mid(5, 5));
    GetDlgItem(IDC_EDIT4)->SetWindowText(reg.Mid(10, 5));
    GetDlgItem(IDC_EDIT5)->SetWindowText(reg.Mid(15, 5));

    // �ѽ��д��ע���
    HKEY key;
    CString skey = "Software\\mingrisoft"; //���û��������½�
    RegOpenKey(HKEY_CURRENT_USER, skey, &key);
    CString value = name + "-" + reg;
    int iret = RegSetValueEx(key, "regnum", 0, REG_SZ, (BYTE *)value.GetBuffer(0),
                             value.GetLength());

    //ֻ��д��REG_SZ������
    if(iret == 0) {
        MessageBox("�����ɹ�", "��ʾ", MB_OK);
    }
    RegSetValueEx(key, "isreg", 0, REG_SZ, (BYTE *)"0", 1);

    // ��ע���,д��"sn.txt"�ļ�
    CFile file;
    char path[256];
    ::GetCurrentDirectory(256, path);
    CString filename = path;
    filename += "\\sn.txt";
    file.Open(filename, CFile::modeCreate | CFile::modeWrite); //
    CString text = name + "\r\n" + reg.Mid(0, 5) + "-" + reg.Mid(5, 5) +
                   "-" + reg.Mid(10, 5) + "-" + reg.Mid(15, 5);
    // д��
    file.Write(text, text.GetLength());
    // �ر��ļ�
    file.Close();
}

CString CCreateRegDlg::GetCPUNum()
{
    //��ȡCPU���к�
    unsigned long s1, s2;

    CString CPUID1, CPUID2;
    __asm{
        mov eax, 01h // ���빦�ܺ�
        xor edx, edx // ��ս��,ִ��cpuidָ��֮��.��������edx,eax��
        cpuid        // ִ��ָ��,��ȡ���
        mov s1, edx  // ��ȡ���
        mov s2, eax  // ��ȡ���
    }

    CPUID1.Format("%08X%08X", s1, s2);
    __asm{
        mov eax, 03h // ���빦�ܺ�
        xor ecx, ecx // ��ս��,ִ��cpuidָ��֮��.��������edx,eax��
        xor edx, edx // ͬ��
        cpuid        // ִ��ָ��
        mov s1, edx  // ��ȡ���
        mov s2, ecx  // ��ȡ���
    }
    CPUID2.Format("%08X%08X", s1, s2);

    // ƴ�ӽ��
    CString CpuID = CPUID1 + CPUID2;
    return CpuID.Mid(5, 3); // ��ƴ�ӵĽ����ȡ��һ��,��Ϊ���ؽ��
}

CString CCreateRegDlg::GetDiskNum()
{
    DWORD ser;
    char namebuf[128];
    char filebuf[128];
    //��ȡC�̵����к�
    ::GetVolumeInformation("c:\\",   // ����ȡ��Ϣ�Ĵ��̵ĸ�·��
                           namebuf,  // ����װ�ش������Ļ�����
                           128,      // ���滺�����Ĵ�С
                           &ser,     // ����װ�ش������кŵı���
                           0,        // ����װ���ļ���ÿһ���ֵĳ���
                           0,        // ��־λ
                           filebuf,  // ����װ���ļ�ϵͳ������
                           128       // ���滺�����Ĵ�С
                          );
    CString DiskID;
    // ��ʽ�����ַ���
    DiskID.Format("%08X", ser);
    // ���ص�3����ʼ��3���ַ�
    return DiskID.Mid(3, 3);
}

CString CCreateRegDlg::GetMacAddress()
{
    NCB nInfo;
    // ��������
    memset(&nInfo, 0, sizeof(NCB));
    // ��������
    nInfo.ncb_command  = NCBRESET;
    nInfo.ncb_lana_num = 0;
    // ִ��
    Netbios(&nInfo);

    ADAPTER_INFO AdaINfo;
    //��ʼ��NetBIOS
    memset(&nInfo, 0, sizeof(NCB));
    nInfo.ncb_command  = NCBASTAT;
    nInfo.ncb_lana_num = 0;
    nInfo.ncb_buffer   = (unsigned char *)&AdaINfo;
    nInfo.ncb_length   = sizeof(ADAPTER_INFO);
    strncpy((char *)nInfo.ncb_callname, "*", NCBNAMSZ);
    Netbios(&nInfo);

    // ��ʽ�����ַ���
    CString MacAddr;
    MacAddr.Format("%02X%02X%02X%02X%02X%02X",
                   AdaINfo.nStatus.adapter_address[0],
                   AdaINfo.nStatus.adapter_address[1],
                   AdaINfo.nStatus.adapter_address[2],
                   AdaINfo.nStatus.adapter_address[3],
                   AdaINfo.nStatus.adapter_address[4],
                   AdaINfo.nStatus.adapter_address[5]
                  );
    // ����һ���ַ���
    return MacAddr.Mid(4, 4);
}
