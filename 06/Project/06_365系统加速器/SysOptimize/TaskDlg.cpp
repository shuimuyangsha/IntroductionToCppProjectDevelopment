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

    m_Menu.LoadMenu(IDR_MENU1);         // ���ز˵���Դ
    m_Menu.ChangeMenuItem(&m_Menu);
    m_Tab.InsertItem(0, "����");        // ���ñ�ǩҳ
    m_Tab.InsertItem(1, "����");

    m_Grid.SetExtendedStyle(
        LVS_EX_FLATSB                   // ��ƽ��������
        | LVS_EX_FULLROWSELECT          // ��������ѡ��
        | LVS_EX_HEADERDRAGDROP         // ���������ק
        | LVS_EX_ONECLICKACTIVATE       // ������ʾ
        | LVS_EX_GRIDLINES              // ����������
    );
    ShowList(0);
    return TRUE;
}

void CTaskDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
    ShowList(m_Tab.GetCurSel());       // �����б���ʾ����
    *pResult = 0;
}
typedef BOOL (__stdcall *funIsHungAppWindow)(HWND hWnd);
void CTaskDlg::ShowList(int num)
{
    // ��ʾ�����б�
    if(num == 0) {
        m_Grid.DeleteAllItems();
        for(int i = 0; i < 4; i++) {
            m_Grid.DeleteColumn(0); // ɾ����
        }
        // ������
        m_Grid.InsertColumn(0, "ӳ������", LVCFMT_LEFT, 100, 0);
        m_Grid.InsertColumn(1, "����ID", LVCFMT_LEFT, 100, 1);
        m_Grid.InsertColumn(2, "�߳�����", LVCFMT_LEFT, 100, 2);
        m_Grid.InsertColumn(3, "���ȼ���", LVCFMT_LEFT, 100, 3);
        // ���ɿ���
        HANDLE toolhelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(toolhelp == NULL) {
            return ;
        }
        PROCESSENTRY32 processinfo;
        int i = 0;
        CString str;
        BOOL start = Process32First(toolhelp, &processinfo);    // ��õ�һ������
        while(start) {
            m_Grid.InsertItem(i, "");                           // ������
            m_Grid.SetItemText(i, 0, processinfo.szExeFile);    // ���ӳ������
            str.Format("%d", processinfo.th32ProcessID);        // ��ý���ID
            m_Grid.SetItemText(i, 1, str);
            str.Format("%d", processinfo.cntThreads);           // ����߳�����
            m_Grid.SetItemText(i, 2, str);
            str.Format("%d", processinfo.pcPriClassBase);       // ������ȼ���
            m_Grid.SetItemText(i, 3, str);
            start = Process32Next(toolhelp, &processinfo);      // �����һ������
            i++;
        }
    }
    // ��ʾ�����б�
    else {
        m_Grid.DeleteAllItems();
        for(int i = 0; i < 6; i++) {
            m_Grid.DeleteColumn(0);                             // ɾ����
        }
        m_Grid.InsertColumn(0, "����", LVCFMT_LEFT, 200);       // ������
        m_Grid.InsertColumn(1, "״̬", LVCFMT_LEFT, 100);
        m_Grid.InsertColumn(2, "����ID", LVCFMT_LEFT, 100);
        HINSTANCE hInstance = LoadLibrary("user32.dll");        // ���ض�̬��
        auto IsHungAppWindow = (funIsHungAppWindow)
                               GetProcAddress(hInstance, "IsHungAppWindow");
        CWnd *pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);  // ��ô��ھ��
        int i = 0;
        CString cstrCap;
        // ��������
        while(pWnd) {                                           // ��������
            // ���ڿɼ�,�����Ƕ��㴰��
            if(pWnd->IsWindowVisible() && !pWnd->GetOwner()) {
                pWnd->GetWindowText(cstrCap);
                if(! cstrCap.IsEmpty()) {
                    m_Grid.InsertItem(i, cstrCap);
                    if(IsHungAppWindow(pWnd->m_hWnd)) {         // �жϳ����Ƿ���Ӧ
                        m_Grid.SetItemText(i, 1, "����Ӧ");
                    }
                    else {
                        m_Grid.SetItemText(i, 1, "��������");
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
            pWnd = pWnd->GetWindow(GW_HWNDNEXT);                //������һ������
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
    //��õ�ǰ�б�������
    int pos = m_Grid.GetSelectionMark();
    CString str = m_Grid.GetItemText(pos, 2);                   //��ý���ID
    DWORD data = atoi(str.GetString());
    HANDLE hProcess;
    // �򿪽���
    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, data);     // �򿪽��� 
    if(hProcess) {
        if(!TerminateProcess(hProcess, 0)) {                    // ��ֹ����
            CString strError;
            strError.Format("�����:%d", GetLastError());
            AfxMessageBox(strError, MB_OK | MB_ICONINFORMATION, NULL);
        }
    }
    else {
        CString strError;
        strError.Format("�����:%d", GetLastError());
        if(GetLastError() == ERROR_ACCESS_DENIED) {
            strError = _T("�ܾ�����!") + strError;
        }
        AfxMessageBox(strError, MB_OK | MB_ICONINFORMATION, NULL);
    }
    Sleep(300);                                                 // ������ʱ
    OnMenuref();                                                // ˢ���б�
}

HBRUSH CTaskDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
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
