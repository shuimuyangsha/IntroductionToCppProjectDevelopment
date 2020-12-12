
// MrkjSystemDlg.cpp : 实现文件
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


// CMrkjSystemDlg 对话框



CMrkjSystemDlg::CMrkjSystemDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_MRKJSYSTEM_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMrkjSystemDlg::~CMrkjSystemDlg()
{
    // 删除本类的指针成员
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


// CMrkjSystemDlg 消息处理程序

BOOL CMrkjSystemDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标
    // 背景画刷
    {
        CBitmap bmp;
        if(bmp.LoadBitmap(IDB_BITMAP_MAINDLG_BACKGROUND)) {
            m_bkBrush.CreatePatternBrush(&bmp);
        }
    }
    // 标题
    {
        CString str;
        str.LoadStringW(IDS_STRING_MAINDLG_TITLE);
        this->SetWindowTextW(str);
    }
    // 菜单
    {
        m_menu.LoadMenu(IDR_MENU_MAIN);
        SetMenu(&m_menu);
#if CHECK_LOGIN
        const CMRKJDatabase::DBEntryUser &user = g_loginUser;
        // 设置主菜单 可用状态
        m_menu.EnableMenuItem(0, (user.p001 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(1, (user.p005 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(2, (user.p006 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        m_menu.EnableMenuItem(3, (user.p007 == L"1" ? MF_ENABLED : MF_GRAYED) | MF_BYPOSITION);
        // 设置子菜单 可用状态
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
    // 状态栏
    {
        // 加载的字符串资源的ID
        UINT array[] = { IDS_STRING_STATUS_PANETEXT_LOGINUSER
                         , IDS_STRING_STATUS_PANETEXT_LOGINTIME
                         , IDS_STRING_STATUS_PANETEXT_COPYRIGHT
                       };
        m_statusBar.Create(this);
        m_statusBar.SetIndicators(array, _countof(array));
        CRect rcCli;
        GetClientRect(rcCli);
        int index = 0;
        // 设置 登陆用户名
        {
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            CString str = m_statusBar.GetPaneText(index) + g_loginUser.username.c_str();
            m_statusBar.SetPaneText(index, str);
            ++index;
        }
        // 设置 登陆时间
        {
            CTime t = CTime::GetCurrentTime();
            CString str = m_statusBar.GetPaneText(index);
            str += t.Format(_T("%Y-%m-%d %H:%d:%S"));
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            m_statusBar.SetPaneText(index, str);
            ++index;
        }
        // 设置 版权信息
        {
            CString str;
            str.LoadString(IDS_STRING_STATUS_PANETEXT_COPYRIGHT);
            m_statusBar.SetPaneInfo(index, array[index], 0, rcCli.Width() * 1 / _countof(array));
            ++index;
        }
        // 设置背景色
        m_statusBar.GetStatusBarCtrl().SetBkColor(RGB(178, 212, 255));
        // 移动到指定的位置
        RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_STRING_STATUS_PANETEXT_COPYRIGHT);
    }
    // 子对话框
    {
        // 存入数组,便于使用
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
        //初始时全部不显示
        for each(auto pdlg in m_vDlgPtr) {
            pdlg->MoveWindow(rc);
            pdlg->ShowWindow(SW_HIDE);
        }
    }

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//只显示自己的窗口,其余的隐藏
void CMrkjSystemDlg::ShowChange(CDialogEx *pDlg)
{
    // 查找当前己经显示的窗口
    for(auto p : m_vDlgPtr) {
        if(p->IsWindowVisible()) {
            m_pDlgLastShow = p;
            break;
        }
    }
    // 更新窗口数据
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
    // 显示新窗口
    if(pDlg && pDlg->GetSafeHwnd()) {
        pDlg->ShowWindow(SW_SHOW);
    }
    //隐藏其它窗口
    for(auto p : m_vDlgPtr) {
        if(p != pDlg) {
            p->ShowWindow(SW_HIDE);
        }
    }
    m_statusBar.ShowWindow(SW_SHOW);
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMrkjSystemDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
    if(IDOK == AfxMessageBox(_T("退出?"), MB_OKCANCEL)) {
        CDialogEx::OnOK();
    }
}

void CMrkjSystemDlg::OnCancel()
{
    if(IDOK == AfxMessageBox(_T("退出?"), MB_OKCANCEL)) {
        CDialogEx::OnCancel();
    }
}

// 基础数据维护
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
    // TODO: 在此添加命令处理程序代码
}

void CMrkjSystemDlg::OnMenuExitSys()
{
    if(IDOK == AfxMessageBox(_T("退出系统?"), MB_OKCANCEL)) {
        CDialogEx::OnOK();
    }
}

void CMrkjSystemDlg::OnMenuKindSelect()
{
    CDialogKindSelect dlg;
    if(IDOK == dlg.DoModal()) {
        // 设置
        // 显示
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
        // 取后面的30条数据进行默认的显示
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
    //隐藏自己
    ShowWindow(SW_HIDE);
    // 显示模态对话框
    CDialogScreenShot dlg;
    HBITMAP hBmp = NULL;
    if(IDOK == dlg.DoModal()) {
        // 获取鼠标选中的区域
        CRect rc = dlg.GetSelectRect();
        // 先获得选中区域的图片(以防止被其它弹出窗口遮挡)
        hBmp = CScreenTools::CopyScreenToBitmap(&rc);
        // 弹出一个对话框, 获取保存图片的位置
        static CString strFilename = (CTools::GetDir() + _T("\\截图.bmp")).c_str();
        {
            static TCHAR szFilter[] = _T("bmp(*.bmp)|*.bmp|所有文件(*.*)|*.*||");
            CFileDialog dlg(FALSE, // 创建文件打开对话框:FALSE保存对话框　
                            _T(".txt"),
                            strFilename.GetString(),
                            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                            szFilter
                           );
            if(IDOK != dlg.DoModal()) {
                goto __End;
            }
            // 显示选择的文件内容
            strFilename = dlg.GetPathName();
        }
        if(!strFilename.IsEmpty()) {
            //把指定区域保存成图片
            CScreenTools::SaveBitmapToFile(hBmp, strFilename);
        }
    }
__End:
    // 删除位图句柄
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
        // 生成背景画刷
        return (HBRUSH)m_bkBrush.GetSafeHandle();
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
