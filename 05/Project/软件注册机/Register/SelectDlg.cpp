// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Register.h"
#include "SelectDlg.h"
#include "RegisterNumDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL Flag;
/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog


CSelectDlg::CSelectDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CSelectDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelectDlg)
    m_Radio = -1;
    //}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectDlg)
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
    DDX_Radio(pDX, IDC_RADIO1, m_Radio);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
    //{{AFX_MSG_MAP(CSelectDlg)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers

BOOL CSelectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 读取注册表
    HKEY key;
    char data[4];
    DWORD size = 4;
    DWORD type = REG_SZ;
    CString skey = "Software\\mingrisoft";
    LSTATUS iret = RegOpenKeyEx(HKEY_CURRENT_USER, skey,
                                REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &key);
    if(iret == 0) {
        CString value;
        // 读取试用次数
        iret = RegQueryValueEx(key, "tryout", 0, &type, (BYTE *)data, &size);
        if(iret == 0) {
            if(data != 0) {
                CString strTime;
                // 在界面上显示试用次数
                strTime.Format("你还可以使用%s次", data);
                GetDlgItem(IDC_STATICTIME)->SetWindowText(strTime);
            }
            else {
                // 界面上的控件设置为不可用
                GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
                // 提示不可以试用软件
                GetDlgItem(IDC_STATICTIME)->SetWindowText("你已经不可以再试用本软件了！");
            }
        }
        else {
            // 设置试用次数为30
            RegSetValueEx(key, "tryout", 0, REG_SZ, (BYTE *)"30", 2);
            OnCancel();
        }
    }
    // 设置"ok"按钮的图片
    m_OK.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(IDB_BITMAPOK)));
    // 设置"cancel"按钮的图片
    m_Cancel.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
                                  MAKEINTRESOURCE(IDB_BACKOFF)));
    m_Radio = 0;
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectDlg::OnOK()
{
    // TODO: Add extra validation here
    UpdateData(TRUE);
    // 选择了"注册"
    if(m_Radio == 0) {
        CDialog::OnOK();
        CRegisterNumDlg dlg;
        dlg.DoModal();
    }
    // 选择了"试用"
    else if(m_Radio == 1) {
        // 打开注册表中 关于试用次数的键值,准备写入
        HKEY key;
        CString skey = "Software\\mingrisoft";
        long iret = RegOpenKeyEx(HKEY_CURRENT_USER,
                                 skey,
                                 REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS,
                                 &key);
        if(iret == 0) {
            // 从界面上获得试用次数相关文字
            CString str;
            GetDlgItem(IDC_STATICTIME)->GetWindowText(str);
            CString num;
            // 次试用次数转换为整型数字
            int run = atoi(str.Mid(12, str.GetLength() - 14));
            num.Format("%d", run - 1);
            // 写入注册表
            RegSetValueEx(key, "tryout", 0, REG_SZ, (BYTE *)num.GetBuffer(0)
                          , num.GetLength());
            // 设置全局标志位
            Flag = TRUE;
            CDialog::OnOK();
        }
    }
}

HBRUSH CSelectDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
    }
    // TODO: Return a different brush if the default is not desired
    return hbr;
}
