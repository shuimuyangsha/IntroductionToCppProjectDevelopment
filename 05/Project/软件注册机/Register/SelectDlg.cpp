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

    // ��ȡע���
    HKEY key;
    char data[4];
    DWORD size = 4;
    DWORD type = REG_SZ;
    CString skey = "Software\\mingrisoft";
    LSTATUS iret = RegOpenKeyEx(HKEY_CURRENT_USER, skey,
                                REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &key);
    if(iret == 0) {
        CString value;
        // ��ȡ���ô���
        iret = RegQueryValueEx(key, "tryout", 0, &type, (BYTE *)data, &size);
        if(iret == 0) {
            if(data != 0) {
                CString strTime;
                // �ڽ�������ʾ���ô���
                strTime.Format("�㻹����ʹ��%s��", data);
                GetDlgItem(IDC_STATICTIME)->SetWindowText(strTime);
            }
            else {
                // �����ϵĿؼ�����Ϊ������
                GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
                // ��ʾ�������������
                GetDlgItem(IDC_STATICTIME)->SetWindowText("���Ѿ������������ñ�����ˣ�");
            }
        }
        else {
            // �������ô���Ϊ30
            RegSetValueEx(key, "tryout", 0, REG_SZ, (BYTE *)"30", 2);
            OnCancel();
        }
    }
    // ����"ok"��ť��ͼƬ
    m_OK.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(IDB_BITMAPOK)));
    // ����"cancel"��ť��ͼƬ
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
    // ѡ����"ע��"
    if(m_Radio == 0) {
        CDialog::OnOK();
        CRegisterNumDlg dlg;
        dlg.DoModal();
    }
    // ѡ����"����"
    else if(m_Radio == 1) {
        // ��ע����� �������ô����ļ�ֵ,׼��д��
        HKEY key;
        CString skey = "Software\\mingrisoft";
        long iret = RegOpenKeyEx(HKEY_CURRENT_USER,
                                 skey,
                                 REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS,
                                 &key);
        if(iret == 0) {
            // �ӽ����ϻ�����ô����������
            CString str;
            GetDlgItem(IDC_STATICTIME)->GetWindowText(str);
            CString num;
            // �����ô���ת��Ϊ��������
            int run = atoi(str.Mid(12, str.GetLength() - 14));
            num.Format("%d", run - 1);
            // д��ע���
            RegSetValueEx(key, "tryout", 0, REG_SZ, (BYTE *)num.GetBuffer(0)
                          , num.GetLength());
            // ����ȫ�ֱ�־λ
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
