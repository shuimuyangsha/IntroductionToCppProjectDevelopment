// RegisterNumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Register.h"
#include "RegisterNumDlg.h"
#include "SelectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL Flag;
#define HOTKEY_PASTE        11111
/////////////////////////////////////////////////////////////////////////////
// CRegisterNumDlg dialog

CRegisterNumDlg::CRegisterNumDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CRegisterNumDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CRegisterNumDlg)
    m_Name = _T("");
    m_strRegisterCode = _T("");
    //}}AFX_DATA_INIT
}


void CRegisterNumDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRegisterNumDlg)
    DDX_Control(pDX, IDC_BACKOFF, m_Backoff);
    DDX_Control(pDX, IDC_ADVANCE, m_Advance);
    DDX_Control(pDX, IDC_EDIT1, m_edtName);
    DDX_Control(pDX, IDC_EDIT2, m_edtRegisterCode);
    DDX_Text(pDX, IDC_EDIT1, m_Name);
    DDX_Text(pDX, IDC_EDIT2, m_strRegisterCode);
    DDV_MaxChars(pDX, m_strRegisterCode, 23);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterNumDlg, CDialog)
    //{{AFX_MSG_MAP(CRegisterNumDlg)
    ON_BN_CLICKED(IDC_BACKOFF, OnBackoff)
    ON_BN_CLICKED(IDC_ADVANCE, OnAdvance)
    ON_WM_CHANGECBCHAIN()
    ON_WM_DRAWCLIPBOARD()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_EDIT1, &CRegisterNumDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterNumDlg message handlers

void CRegisterNumDlg::OnBackoff()
{
    // 调用父类的OnOk()方法
    CDialog::OnOK();
    // 显示模态对话框:选择试用? or 注册
    CSelectDlg dlg;
    dlg.DoModal();
}

void CRegisterNumDlg::OnAdvance()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    // 判断用户名和注册破是否输入了
    if(m_Name.IsEmpty() || m_strRegisterCode.IsEmpty()) {
        MessageBox("用户名或注册码错误！");
        return;
    }

    // 打开注册表相关键值
    HKEY key;
    char data[32];
    DWORD size = 32;
    DWORD type = REG_SZ;
    CString skey = "Software\\mingrisoft";
    long iret = RegOpenKeyEx(HKEY_CURRENT_USER, skey,
                             REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &key);
    // 打开成功
    if(iret == 0) {
        CString value;
        // 查询
        iret = RegQueryValueEx(key, "regnum", 0, &type, (BYTE *)data, &size);
        CString text = data; // AAA-OJQWQWEQGFOJEQWMNWWM
        // 查找其中的 '-'
        int index = text.Find("-");
        CString strCode = m_strRegisterCode;
        // 删除其中的'-'
        strCode.Replace(_T("-"), _T(""));
        if(iret == 0) {
            // 判断是否相等
            if(text.Mid(0, index) == m_Name && text.Mid(1 + index) == strCode) {
                Flag = TRUE;
                RegSetValueEx(key, "isreg", 0, REG_SZ, (BYTE *)"1", 1);
            }
            else {
                MessageBox("用户名或注册码错误！");
                return;
            }
        }
        else {
            RegSetValueEx(key, "regnum", 0, REG_SZ, (BYTE *)"0", 1);
        }
    }
    CDialog::OnOK();
}

BOOL CRegisterNumDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 隐藏注册码编辑框
    GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
    // 注册按钮显示图片
    m_Advance.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCE(IDB_ADVANCE)));
    // 后退按钮显示图片
    m_Backoff.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCE(IDB_BACKOFF)));
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRegisterNumDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
    }
    // TODO: Return a different brush if the default is not desired
    return hbr;
}

void CRegisterNumDlg::OnEnChangeEdit1()
{
    m_edtRegisterCode.ShowWindow(SW_SHOW);
}
