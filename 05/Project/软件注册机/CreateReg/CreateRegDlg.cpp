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
        MessageBox("用户名不能为空！");
        return;
    }
    //定义一个密钥数组
    CString code[16] = {"ah", "tm", "ib", "nw", "rt", "vx", "zc", "gf",
                        "pn", "xq", "fc", "oj", "wm", "eq", "np", "qw"
                       };
    CString reg, stred;
    int num;
    stred = GetCPUNum() + GetDiskNum() + GetMacAddress();
    stred.MakeLower();
    //根据十六进制数字从密钥数组中选择字符串
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

    // 结果转化成大写
    reg.MakeUpper();

    // 设置界面上编辑框的内容
    GetDlgItem(IDC_EDIT2)->SetWindowText(reg.Mid(0, 5));
    GetDlgItem(IDC_EDIT3)->SetWindowText(reg.Mid(5, 5));
    GetDlgItem(IDC_EDIT4)->SetWindowText(reg.Mid(10, 5));
    GetDlgItem(IDC_EDIT5)->SetWindowText(reg.Mid(15, 5));

    // 把结果写入注册表
    HKEY key;
    CString skey = "Software\\mingrisoft"; //如果没有子项就新建
    RegOpenKey(HKEY_CURRENT_USER, skey, &key);
    CString value = name + "-" + reg;
    int iret = RegSetValueEx(key, "regnum", 0, REG_SZ, (BYTE *)value.GetBuffer(0),
                             value.GetLength());

    //只能写入REG_SZ型数据
    if(iret == 0) {
        MessageBox("创建成功", "提示", MB_OK);
    }
    RegSetValueEx(key, "isreg", 0, REG_SZ, (BYTE *)"0", 1);

    // 把注册号,写入"sn.txt"文件
    CFile file;
    char path[256];
    ::GetCurrentDirectory(256, path);
    CString filename = path;
    filename += "\\sn.txt";
    file.Open(filename, CFile::modeCreate | CFile::modeWrite); //
    CString text = name + "\r\n" + reg.Mid(0, 5) + "-" + reg.Mid(5, 5) +
                   "-" + reg.Mid(10, 5) + "-" + reg.Mid(15, 5);
    // 写入
    file.Write(text, text.GetLength());
    // 关闭文件
    file.Close();
}

CString CCreateRegDlg::GetCPUNum()
{
    //获取CPU序列号
    unsigned long s1, s2;

    CString CPUID1, CPUID2;
    __asm{
        mov eax, 01h // 传入功能号
        xor edx, edx // 清空结果,执行cpuid指令之后.结果会放在edx,eax中
        cpuid        // 执行指令,获取结果
        mov s1, edx  // 读取结果
        mov s2, eax  // 读取结果
    }

    CPUID1.Format("%08X%08X", s1, s2);
    __asm{
        mov eax, 03h // 传入功能号
        xor ecx, ecx // 清空结果,执行cpuid指令之后.结果会放在edx,eax中
        xor edx, edx // 同上
        cpuid        // 执行指令
        mov s1, edx  // 读取结果
        mov s2, ecx  // 读取结果
    }
    CPUID2.Format("%08X%08X", s1, s2);

    // 拼接结果
    CString CpuID = CPUID1 + CPUID2;
    return CpuID.Mid(5, 3); // 从拼接的结果中取出一段,作为返回结果
}

CString CCreateRegDlg::GetDiskNum()
{
    DWORD ser;
    char namebuf[128];
    char filebuf[128];
    //获取C盘的序列号
    ::GetVolumeInformation("c:\\",   // 欲获取信息的磁盘的根路径
                           namebuf,  // 用于装载磁盘名的缓冲区
                           128,      // 上面缓冲区的大小
                           &ser,     // 用于装载磁盘序列号的变量
                           0,        // 用于装载文件名每一部分的长度
                           0,        // 标志位
                           filebuf,  // 用于装载文件系统的名称
                           128       // 上面缓冲区的大小
                          );
    CString DiskID;
    // 格式化成字符串
    DiskID.Format("%08X", ser);
    // 返回第3个开始的3个字符
    return DiskID.Mid(3, 3);
}

CString CCreateRegDlg::GetMacAddress()
{
    NCB nInfo;
    // 内容清零
    memset(&nInfo, 0, sizeof(NCB));
    // 设置命令
    nInfo.ncb_command  = NCBRESET;
    nInfo.ncb_lana_num = 0;
    // 执行
    Netbios(&nInfo);

    ADAPTER_INFO AdaINfo;
    //初始化NetBIOS
    memset(&nInfo, 0, sizeof(NCB));
    nInfo.ncb_command  = NCBASTAT;
    nInfo.ncb_lana_num = 0;
    nInfo.ncb_buffer   = (unsigned char *)&AdaINfo;
    nInfo.ncb_length   = sizeof(ADAPTER_INFO);
    strncpy((char *)nInfo.ncb_callname, "*", NCBNAMSZ);
    Netbios(&nInfo);

    // 格式化成字符串
    CString MacAddr;
    MacAddr.Format("%02X%02X%02X%02X%02X%02X",
                   AdaINfo.nStatus.adapter_address[0],
                   AdaINfo.nStatus.adapter_address[1],
                   AdaINfo.nStatus.adapter_address[2],
                   AdaINfo.nStatus.adapter_address[3],
                   AdaINfo.nStatus.adapter_address[4],
                   AdaINfo.nStatus.adapter_address[5]
                  );
    // 返回一段字符串
    return MacAddr.Mid(4, 4);
}
