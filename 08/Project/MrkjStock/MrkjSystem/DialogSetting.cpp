// DialogSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogSetting.h"
#include "afxdialogex.h"


// CDialogSetting 对话框

IMPLEMENT_DYNAMIC(CDialogSetting, CDialogEx)

CDialogSetting::CDialogSetting(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_SETTING, pParent)
    , m_strID(_T(""))
    , m_strUsername(_T(""))
    , m_strPassword(_T(""))
    , m_b001(FALSE)
    , m_b002(FALSE)
    , m_b003(FALSE)
    , m_b004(FALSE)
    , m_b005(FALSE)
    , m_b006(FALSE)
    , m_b007(FALSE)
    , m_b008(FALSE)
    , m_b009(FALSE)
    , m_b010(FALSE)
{
}

CDialogSetting::~CDialogSetting()
{
}

void CDialogSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_DATA, m_lst);
    DDX_Text(pDX, IDC_EDIT1, m_strID);
    DDX_Text(pDX, IDC_EDIT2, m_strUsername);
    DDX_Text(pDX, IDC_EDIT10, m_strPassword);
    DDX_Control(pDX, IDC_CHECK1, m_btn001);
    DDX_Check(pDX, IDC_CHECK1, m_b001);
    DDX_Control(pDX, IDC_CHECK5, m_btn002);
    DDX_Check(pDX, IDC_CHECK5, m_b002);
    DDX_Control(pDX, IDC_CHECK6, m_btn003);
    DDX_Check(pDX, IDC_CHECK6, m_b003);
    DDX_Control(pDX, IDC_CHECK7, m_btn004);
    DDX_Check(pDX, IDC_CHECK7, m_b004);
    DDX_Control(pDX, IDC_CHECK2, m_btn005);
    DDX_Check(pDX, IDC_CHECK2, m_b005);
    DDX_Control(pDX, IDC_CHECK3, m_btn006);
    DDX_Check(pDX, IDC_CHECK3, m_b006);
    DDX_Control(pDX, IDC_CHECK4, m_btn007);
    DDX_Check(pDX, IDC_CHECK4, m_b007);
    DDX_Control(pDX, IDC_CHECK8, m_btn008);
    DDX_Check(pDX, IDC_CHECK8, m_b008);
    DDX_Control(pDX, IDC_CHECK9, m_btn009);
    DDX_Check(pDX, IDC_CHECK9, m_b009);
    DDX_Control(pDX, IDC_CHECK10, m_btn010);
    DDX_Check(pDX, IDC_CHECK10, m_b010);
}


BEGIN_MESSAGE_MAP(CDialogSetting, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogSetting::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON3, &CDialogSetting::OnBnClickedButtonUpdate)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogSetting::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON8, &CDialogSetting::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_CHECK1, &CDialogSetting::OnBnClickedCheckSystem)
    ON_BN_CLICKED(IDC_CHECK4, &CDialogSetting::OnBnClickedCheckTools)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATA, &CDialogSetting::OnLvnItemchangedListData)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogSetting 消息处理程序


BOOL CDialogSetting::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 1. CListCtrl 控件
    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
        dwStyle |= LVS_EX_GRIDLINES; //网络线
        m_lst.SetExtendedStyle(dwStyle);
        // 设置列,并设置大小
        {
            CRect rc;
            m_lst.GetClientRect(rc);
            m_lst.InsertColumn(EListIndexID, _T("编号"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexUsername, _T("用户名"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexPassword, _T("密码"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP001, _T("系统权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP002, _T("基础数据维护权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP003, _T("品种维护权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP004, _T("导入导出权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP005, _T("查询权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP006, _T("即时数据权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP007, _T("工具权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP008, _T("金融计算器权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP009, _T("屏幕截图权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexP010, _T("系统设置权限"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
        }
    }
    UpdateListData();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogSetting::OnOK()
{
    ClearList();
    CDialogEx::OnOK();
}

void CDialogSetting::OnCancel()
{
    ClearList();
    CDialogEx::OnCancel();
}

BOOL CDialogSetting::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogSetting::UpdateListData()
{
    CMRKJDatabase::VDBEntryUser v;
    if(!DB.QueryUser(v)) {
        return;
    }
    UpdateListData(v);
}

// 清理m_lst数据
void CDialogSetting::ClearList()
{
    m_lst.DeleteAllItems();
}


// 更新m_lst中的数据
void CDialogSetting::UpdateListData(const CMRKJDatabase::VDBEntryUser& v)
{
    ClearList();
    for each(auto data in v) {
        int index = m_lst.GetItemCount();
        m_lst.InsertItem(index, data.id.c_str());
        m_lst.SetItemText(index, EListIndexUsername, data.username.c_str());
        m_lst.SetItemText(index, EListIndexPassword, data.password.c_str());
        m_lst.SetItemText(index, EListIndexP001, data.p001.c_str());
        m_lst.SetItemText(index, EListIndexP002, data.p002.c_str());
        m_lst.SetItemText(index, EListIndexP003, data.p003.c_str());
        m_lst.SetItemText(index, EListIndexP004, data.p004.c_str());
        m_lst.SetItemText(index, EListIndexP005, data.p005.c_str());
        m_lst.SetItemText(index, EListIndexP006, data.p006.c_str());
        m_lst.SetItemText(index, EListIndexP007, data.p007.c_str());
        m_lst.SetItemText(index, EListIndexP008, data.p008.c_str());
        m_lst.SetItemText(index, EListIndexP009, data.p009.c_str());
        m_lst.SetItemText(index, EListIndexP010, data.p010.c_str());
    }
}

// 获得某行数据
void CDialogSetting::GetListData(CMRKJDatabase::DBEntryUser& data, int index)
{
    assert(index >= 0 && index < m_lst.GetItemCount());
    UpdateData(TRUE);
    data.id = m_lst.GetItemText(index, EListIndexID).GetString();
    data.username = m_lst.GetItemText(index, EListIndexUsername).GetString();
    data.password = m_lst.GetItemText(index, EListIndexPassword).GetString();
    data.p001 = m_lst.GetItemText(index, EListIndexP001).GetString();
    data.p002 = m_lst.GetItemText(index, EListIndexP002).GetString();
    data.p003 = m_lst.GetItemText(index, EListIndexP003).GetString();
    data.p004 = m_lst.GetItemText(index, EListIndexP004).GetString();
    data.p005 = m_lst.GetItemText(index, EListIndexP005).GetString();
    data.p006 = m_lst.GetItemText(index, EListIndexP006).GetString();
    data.p007 = m_lst.GetItemText(index, EListIndexP007).GetString();
    data.p008 = m_lst.GetItemText(index, EListIndexP008).GetString();
    data.p009 = m_lst.GetItemText(index, EListIndexP009).GetString();
    data.p010 = m_lst.GetItemText(index, EListIndexP010).GetString();
}

void CDialogSetting::OnBnClickedButtonAdd()
{
    UpdateData();
    if(DB.IsExistUsername(m_strUsername)) {
        CTools::MessageBoxFormat(_T("用户名'%s'己经存在"), m_strUsername.GetString());
        return;
    }
    CMRKJDatabase::DBEntryUser user;
    user.id = m_strID.GetString();
    user.username = m_strUsername.GetString();
    user.password = m_strPassword.GetString();
    user.p002 = StringHelper::ToString(m_b002);
    user.p003 = StringHelper::ToString(m_b003);
    user.p004 = StringHelper::ToString(m_b004);
    user.p005 = StringHelper::ToString(m_b005);
    user.p006 = StringHelper::ToString(m_b006);
    user.p008 = StringHelper::ToString(m_b008);
    user.p009 = StringHelper::ToString(m_b009);
    user.p010 = StringHelper::ToString(m_b010);
    if(!DB.AddUser(user)) {
        CTools::MessageBoxFormat(_T("增加用户[%s]失败"), m_strUsername.GetString());
    }
    UpdateData(FALSE);
    UpdateListData();
}


void CDialogSetting::OnBnClickedButtonUpdate()
{
    UpdateData();
    {
        CMRKJDatabase::DBEntryUser user;
        if(!DB.QueryUser(user, m_strID)) {
            AfxMessageBox(_T("没有此用户?"));
            return;
        }
        // 如果 是前户名 != 数据库中的用户名 and 当前用户名在数据库中存在,则不应该修改该用户
        // 因为修改过后的用户 和数据库中的其它用户重名
        if(user.username != m_strUsername.GetString()) {
            if(DB.IsExistUsername(m_strUsername)) {
                CTools::MessageBoxFormat(_T("用户名'%s'己经存在"), m_strUsername.GetString());
                return;
            }
        }
    }
    CMRKJDatabase::DBEntryUser user;
    user.id = m_strID.GetString();
    user.username = m_strUsername.GetString();
    user.password = m_strPassword.GetString();
    user.p001 = StringHelper::ToString(m_b001);
    user.p002 = StringHelper::ToString(m_b002);
    user.p003 = StringHelper::ToString(m_b003);
    user.p004 = StringHelper::ToString(m_b004);
    user.p005 = StringHelper::ToString(m_b005);
    user.p006 = StringHelper::ToString(m_b006);
    user.p007 = StringHelper::ToString(m_b007);
    user.p008 = StringHelper::ToString(m_b008);
    user.p009 = StringHelper::ToString(m_b009);
    user.p010 = StringHelper::ToString(m_b010);
    if(!DB.UpdUser(user)) {
        CTools::MessageBoxFormat(_T("更改用户[%s]失败"), m_strUsername.GetString());
    }
    UpdateData(FALSE);
    UpdateListData();
}


void CDialogSetting::OnBnClickedButtonDelete()
{
    CMRKJDatabase::DBEntryUser d;
    // 获取选中的用户
    int index = GetSelectLineIndex();
    if(index >= 0) {
        GetListData(d, index);
    }
    else {
        return;
    }
    // 如果是当前登陆的用户则不能删除
    if(d.id == g_loginUser.id) {
        AfxMessageBox(_T("不能删除正在登陆的用户"));
        return;
    }
    //
    if(!DB.DelUser(d.id.c_str())) {
        CTools::MessageBoxFormat(_T("删除用户[%s]失败"), m_strUsername.GetString());
    }
    UpdateListData();
}


void CDialogSetting::OnBnClickedButtonClose()
{
    OnOK();
}


void CDialogSetting::OnBnClickedCheckSystem()
{
    UpdateData(TRUE);
    m_b002 = m_b001;
    m_b003 = m_b001;
    m_b004 = m_b001;
    UpdateData(FALSE);
}


void CDialogSetting::OnBnClickedCheckTools()
{
    UpdateData(TRUE);
    m_b008 = m_b007;
    m_b009 = m_b007;
    m_b010 = m_b007;
    UpdateData(FALSE);
}


int CDialogSetting::GetSelectLineIndex()
{
    int index = -1;
    POSITION pos = m_lst.GetFirstSelectedItemPosition();
    if(pos) {
        index = m_lst.GetNextSelectedItem(pos);
    }
    return index;
}


void CDialogSetting::OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem >= 0) {
        CMRKJDatabase::DBEntryUser data;
        GetListData(data, pNMListView->iItem);
        m_strID = data.id.c_str();
        m_strUsername = data.username.c_str();
        m_strPassword = data.password.c_str();
        m_b001 = StringHelper::StringTo<int>(data.p001.c_str());
        m_b002 = StringHelper::StringTo<int>(data.p002.c_str());
        m_b003 = StringHelper::StringTo<int>(data.p003.c_str());
        m_b004 = StringHelper::StringTo<int>(data.p004.c_str());
        m_b005 = StringHelper::StringTo<int>(data.p005.c_str());
        m_b006 = StringHelper::StringTo<int>(data.p006.c_str());
        m_b007 = StringHelper::StringTo<int>(data.p007.c_str());
        m_b008 = StringHelper::StringTo<int>(data.p008.c_str());
        m_b009 = StringHelper::StringTo<int>(data.p009.c_str());
        m_b010 = StringHelper::StringTo<int>(data.p010.c_str());
        UpdateData(FALSE);
    }
    //__End:
    *pResult = 0;
}


void CDialogSetting::OnDestroy()
{
    CDialogEx::OnDestroy();
}
