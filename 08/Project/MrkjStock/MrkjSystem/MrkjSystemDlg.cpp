
// MrkjSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "MrkjSystemDlg.h"
#include "afxdialogex.h"

#include "DialogAbout.h"
#include "DialogCalc.h"
#include "DialogDataImpExp.h"
#include "DialogDataKind.h"
#include "DialogDataMaintenance.h"
#include "DialogHostoryData.h"
#include "DialogKindSelect.h"
#include "DialogRealtimeData.h"
#include "DialogSetting.h"
#include "DialogScreenShot.h"
#include "ScreenTools.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMrkjSystemDlg �Ի���



CMrkjSystemDlg::CMrkjSystemDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_MRKJSYSTEM_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMrkjSystemDlg::~CMrkjSystemDlg()
{
    // ɾ�������ָ���Ա
    SAFE_DELETE(m_pDlgHuShen);
    SAFE_DELETE(m_pDlgCalc);
    SAFE_DELETE(m_pDlgDataImpExp);
    SAFE_DELETE(m_pDlgDataKind);
    SAFE_DELETE(m_pDlgDataMaintenance);
    SAFE_DELETE(m_pDlgHostoryData);
    SAFE_DELETE(m_pDlgKindSelect);
    SAFE_DELETE(m_pDlgRealtimeData);
    SAFE_DELETE(m_pDlgSetting);
}

void CMrkjSystemDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMrkjSystemDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_MENU_DATA_MAINTENANCE, &CMrkjSystemDlg::OnMenuDataMaintenance)
    ON_COMMAND(ID_MENU_DATA_IMPEXP, &CMrkjSystemDlg::OnMenuDataImpexp)
    ON_COMMAND(ID_MENU_DATA_KIND, &CMrkjSystemDlg::OnMenuDataKind)
    ON_COMMAND(ID_MENU_PRINT, &CMrkjSystemDlg::OnMenuPrint)
    ON_COMMAND(ID_MENU_EXIT_SYS, &CMrkjSystemDlg::OnMenuExitSys)
    ON_COMMAND(ID_MENU_KIND_SELECT, &CMrkjSystemDlg::OnMenuKindSelect)
    ON_COMMAND(ID_MENU_HOSTORY_DATA, &CMrkjSystemDlg::OnMenuHostoryData)
    ON_COMMAND(ID_MENU_REALTIME_DATA, &CMrkjSystemDlg::OnMenuRealtimeData)
    ON_COMMAND(ID_MENU_CALC, &CMrkjSystemDlg::OnMenuCalc)
    ON_COMMAND(ID_MENU_SCREEN_SHOT, &CMrkjSystemDlg::OnMenuScreenShot)
    ON_COMMAND(ID_MENU_SETTING, &CMrkjSystemDlg::OnMenuSetting)
    ON_COMMAND(ID_MENU_HELP, &CMrkjSystemDlg::OnMenuHelp)
    ON_COMMAND(ID_MENU_ABOUT, &CMrkjSystemDlg::OnMenuAbout)
    ON_WM_CTLCOLOR()
    ON_COMMAND(ID_MENU_REAL_HUSHEN, &CMrkjSystemDlg::OnMenuRealHushen)
END_MESSAGE_MAP()


// CMrkjSystemDlg ��Ϣ�������

BOOL CMrkjSystemDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��
    // ������ˢ
    {
        CBitmap bmp;
        if(bmp.LoadBitmap(IDB_BITMAP_MAINDLG_BACKGROUND)) {
            m_bkBrush.CreatePatternBrush(&bmp);
        }
    }
    // ����
    {
        CString str;
        str.LoadStringW(IDS_STRING_MAINDLG_TITLE);
        this->SetWindowTextW(str);
    }
    // �˵�
    {
        m_menu.LoadMenu(IDR_MENU_MAIN);
        SetMenu(&m_menu);
#if CHECK_LOGIN
        const CMRKJDatabase::DBEntryUser &user = g_loginUser;
        // �������˵� ����״̬
        m_menu.EnableMenuItem(0, (user.p001 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(1, (user.p005 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(2, (user.p006 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(3, (user.p007 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        // �����Ӳ˵� ����״̬
        m_menu.EnableMenuItem(ID_MENU_DATA_MAINTENANCE, user.p002 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_DATA_IMPEXP, user.p003 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_DATA_KIND, user.p004 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_KIND_SELECT, user.p005 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_HOSTORY_DATA, user.p005 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_REALTIME_DATA, user.p006 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_CALC, user.p008 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_SCREEN_SHOT, user.p009 == L"1" ? MF_ENABLED : MF_GRAYED);
        m_menu.EnableMenuItem(ID_MENU_SETTING, user.p010 == L"1" ? MF_ENABLED : MF_GRAYED);
#endif
    }
    // ״̬��
    {
        // ���ص��ַ�����Դ��ID
        UINT array[] = { IDS_STRING_STATUS_PANETEXT_LOGINUSER
                         , IDS_STRING_STATUS_PANETEXT_LOGINTIME
                         , IDS_STRING_STATUS_PANETEXT_COPYRIGHT
                       };
        m_statusBar.Create(this);
        m_statusBar.SetIndicators(array, _countof(array));
        CRect rcCli;
        GetClientRect(rcCli);
        int index = 0;
        // ���� ��½�û���
        {
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            CString str = m_statusBar.GetPaneText(index) + g_loginUser.username.c_str();
            m_statusBar.SetPaneText(index, str);
            ++index;
        }
        // ���� ��½ʱ��
        {
            CTime t = CTime::GetCurrentTime();
            CString str = m_statusBar.GetPaneText(index);
            str += t.Format(_T("%Y-%m-%d %H:%d:%S"));
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            m_statusBar.SetPaneText(index, str);
            ++index;
        }
        // ���� ��Ȩ��Ϣ
        {
            CString str;
            str.LoadString(IDS_STRING_STATUS_PANETEXT_COPYRIGHT);
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            ++index;
        }
        // ���ñ���ɫ
        m_statusBar.GetStatusBarCtrl().SetBkColor(RGB(178, 212, 255));
        // �ƶ���ָ����λ��
        RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_STRING_STATUS_PANETEXT_COPYRIGHT);
    }
    // �ӶԻ���
    {
        // ��������,����ʹ��
        m_vDlgPtr.push_back(m_pDlgHuShen);
        m_vDlgPtr.push_back(m_pDlgCalc);
        m_vDlgPtr.push_back(m_pDlgDataImpExp);
        m_vDlgPtr.push_back(m_pDlgDataKind);
        m_vDlgPtr.push_back(m_pDlgDataMaintenance);
        m_vDlgPtr.push_back(m_pDlgHostoryData);
        m_vDlgPtr.push_back(m_pDlgKindSelect);
        m_vDlgPtr.push_back(m_pDlgRealtimeData);
        m_vDlgPtr.push_back(m_pDlgSetting);

        CRect rc;
        {
            GetClientRect(rc);
            CRect rcs;
            m_statusBar.GetWindowRect(rcs);
            rc.bottom -=  1 + rcs.Height();
        }
        m_pDlgHuShen->Create(IDD_DIALOG_HUSHEN, this);
        m_pDlgCalc->Create(IDD_DIALOG_CALC, this);
        m_pDlgDataImpExp->Create(IDD_DIALOG_DATA_IMPEXP, this);
        m_pDlgDataKind->Create(IDD_DIALOG_DATA_KIND, this);
        m_pDlgDataMaintenance->Create(IDD_DIALOG_DATA_MAINTENANCE, this);
        m_pDlgHostoryData->Create(IDD_DIALOG_HOSTORY_DATA, this);
        m_pDlgKindSelect->Create(IDD_DIALOG_KIND_SELECT, this);
        m_pDlgRealtimeData->Create(IDD_DIALOG_REALTIME_DATA, this);
        m_pDlgSetting->Create(IDD_DIALOG_SETTING, this);
        //��ʼʱȫ������ʾ
        for each(auto pdlg in m_vDlgPtr) {
            pdlg->MoveWindow(rc);
            pdlg->ShowWindow(SW_HIDE);
        }
    }

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//ֻ��ʾ�Լ��Ĵ���,���������
void CMrkjSystemDlg::ShowChange(CDialogEx *pDlg)
{
    // ���ҵ�ǰ������ʾ�Ĵ���
    for(auto p : m_vDlgPtr) {
        if(p->IsWindowVisible()) {
            m_pDlgLastShow = p;
            break;
        }
    }
    // ���´�������
    {
        if(pDlg == m_pDlgDataMaintenance) {
            m_pDlgDataMaintenance->UpdateKindList();
        }
        if(pDlg == m_pDlgHostoryData) {
            m_pDlgHostoryData->UpdateStockInfoComboBox();
        }
        if(pDlg == m_pDlgRealtimeData) {
            m_pDlgRealtimeData->UpdateStockInfoComboBox();
        }
    }
    // ��ʾ�´���
    if(pDlg && pDlg->GetSafeHwnd()) {
        pDlg->ShowWindow(SW_SHOW);
    }
    //������������
    for(auto p : m_vDlgPtr) {
        if(p != pDlg) {
            p->ShowWindow(SW_HIDE);
        }
    }
    m_statusBar.ShowWindow(SW_SHOW);
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMrkjSystemDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMrkjSystemDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMrkjSystemDlg::PreTranslateMessage(MSG *pMsg)
{
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CMrkjSystemDlg::OnOK()
{
    if(IDOK == AfxMessageBox(_T("�˳�?"), MB_OKCANCEL)) {
        CDialogEx::OnOK();
    }
}

void CMrkjSystemDlg::OnCancel()
{
    if(IDOK == AfxMessageBox(_T("�˳�?"), MB_OKCANCEL)) {
        CDialogEx::OnCancel();
    }
}

// ��������ά��
void CMrkjSystemDlg::OnMenuDataMaintenance()
{
    ShowChange(m_pDlgDataMaintenance);
}

void CMrkjSystemDlg::OnMenuDataImpexp()
{
    ShowChange(m_pDlgDataImpExp);
}

void CMrkjSystemDlg::OnMenuDataKind()
{
    ShowChange(m_pDlgDataKind);
}

void CMrkjSystemDlg::OnMenuPrint()
{
    // TODO: �ڴ���������������
}

void CMrkjSystemDlg::OnMenuExitSys()
{
    if(IDOK == AfxMessageBox(_T("�˳�ϵͳ?"), MB_OKCANCEL)) {
        CDialogEx::OnOK();
    }
}

void CMrkjSystemDlg::OnMenuKindSelect()
{
    CDialogKindSelect dlg;
    if(IDOK == dlg.DoModal()) {
        // ����
        // ��ʾ
        ShowChange(m_pDlgHostoryData);
        CString strCode = m_pDlgKindSelect->GetTargetStockCode();
        if(strCode.IsEmpty()) {
            return;
        }
        const unsigned days = 21;
        VDayDataEntry v;
        COleDateTime dts = COleDateTime::GetCurrentTime() - COleDateTimeSpan(days, 0, 0, 0);
        COleDateTime dte = COleDateTime::GetCurrentTime();
        DB.QueryDayData(v, strCode);
        // ȡ�����30�����ݽ���Ĭ�ϵ���ʾ
        if(!v.empty()) {
            dts = v.size() >= days ? v[v.size() - days].GetDate() : v[0].GetDate() ;
            dte = v[v.size() - 1].GetDate();
        }
        m_pDlgHostoryData->SetDataAndUpdate(strCode, dts, dte);
    }
}

void CMrkjSystemDlg::OnMenuHostoryData()
{
    ShowChange(m_pDlgHostoryData);
}

void CMrkjSystemDlg::OnMenuRealHushen()
{
    ShowChange(m_pDlgHuShen);
}

void CMrkjSystemDlg::OnMenuRealtimeData()
{
    ShowChange(m_pDlgRealtimeData);
}

void CMrkjSystemDlg::OnMenuCalc()
{
    ShowChange(m_pDlgCalc);
}

void CMrkjSystemDlg::OnMenuScreenShot()
{
    //�����Լ�
    ShowWindow(SW_HIDE);
    // ��ʾģ̬�Ի���
    CDialogScreenShot dlg;
    HBITMAP hBmp = NULL;
    if(IDOK == dlg.DoModal()) {
        // ��ȡ���ѡ�е�����
        CRect rc = dlg.GetSelectRect();
        // �Ȼ��ѡ�������ͼƬ(�Է�ֹ���������������ڵ�)
        hBmp = CScreenTools::CopyScreenToBitmap(&rc);
        // ����һ���Ի���, ��ȡ����ͼƬ��λ��
        static CString strFilename = (CTools::GetDir() + _T("\\��ͼ.bmp")).c_str();
        {
            static TCHAR szFilter[] = _T("bmp(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||");
            CFileDialog dlg(FALSE, // �����ļ��򿪶Ի���:FALSE����Ի���
                            _T(".txt"),
                            strFilename.GetString(),
                            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                            szFilter
                           );
            if(IDOK != dlg.DoModal()) {
                goto __End;
            }
            // ��ʾѡ����ļ�����
            strFilename = dlg.GetPathName();
        }
        if(!strFilename.IsEmpty()) {
            //��ָ�����򱣴��ͼƬ
            CScreenTools::SaveBitmapToFile(hBmp, strFilename);
        }
    }
__End:
    // ɾ��λͼ���
    if(hBmp) {
        DeleteObject(hBmp);
    }
    ShowWindow(SW_SHOW);
}

void CMrkjSystemDlg::OnMenuSetting()
{
    ShowChange(m_pDlgSetting);
}

void CMrkjSystemDlg::OnMenuHelp()
{
    std::wstring strDir = CTools::GetDir();
    CString path(strDir.c_str());
    PathAppend(path.GetBuffer(1024), _T("help.html"));
    path.ReleaseBuffer();
    ShellExecute(NULL, _T("open"), path.GetString(), NULL, NULL, SW_SHOW);
}

void CMrkjSystemDlg::OnMenuAbout()
{
    CDialogAbout dlg;
    dlg.DoModal();
}

HBRUSH CMrkjSystemDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if(pWnd == this) {
        // ���ɱ�����ˢ
        return (HBRUSH)m_bkBrush.GetSafeHandle();
    }
    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}
