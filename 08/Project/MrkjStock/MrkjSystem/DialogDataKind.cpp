// DialogDataKind.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogDataKind.h"
#include "afxdialogex.h"


// CDialogDataKind 对话框

IMPLEMENT_DYNAMIC(CDialogDataKind, CDialogEx)

CDialogDataKind::CDialogDataKind(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_DATA_KIND, pParent)
    , m_strKindName(_T(""))
{
}

CDialogDataKind::~CDialogDataKind()
{
}

void CDialogDataKind::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_KIND, m_treeKind);
    DDX_Text(pDX, IDC_EDIT1, m_strKindName);
}


BEGIN_MESSAGE_MAP(CDialogDataKind, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogDataKind::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogDataKind::OnBnClickedButtonUpdate)
    ON_BN_CLICKED(IDC_BUTTON3, &CDialogDataKind::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogDataKind::OnBnClickedButtonDelete)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_KIND, &CDialogDataKind::OnTvnSelchangedTreeKind)
END_MESSAGE_MAP()


// CDialogDataKind 消息处理程序


void CDialogDataKind::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogDataKind::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


BOOL CDialogDataKind::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        static CBitmap bmp;
        bmp.LoadBitmap(IDB_BITMAP_KIND);
        static CImageList imgList;
        imgList.Create(16, 16, ILC_COLOR32, 1, 1);
        imgList.Add(&bmp, RGB(0, 0, 0));
        m_treeKind.SetImageList(&imgList, TVSIL_NORMAL);
        UpdateTree();
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogDataKind::UpdateTree()
{
    m_treeKind.DeleteAllItems();
    typedef std::vector<std::tuple<int, std::wstring>> VKInd;
    VKInd v;
    if(!DB.QueryKind(v)) {
        return;
    }
    HTREEITEM hFirstItem = NULL;
    for(auto i = 0; i != v.size(); ++i) {
        HTREEITEM hItem = m_treeKind.InsertItem(std::get<1>(v[i]).c_str());
        m_treeKind.SetItemData(hItem, std::get<0>(v[i]));
        if(!hFirstItem) {
            hFirstItem = hItem;
        }
    }
    //选中第一个
    if(hFirstItem) {
        m_treeKind.Select(hFirstItem, TVGN_CARET);
        m_strKindName = m_treeKind.GetItemText(hFirstItem);
        UpdateData(FALSE);
    }
}

// 增加
void CDialogDataKind::OnBnClickedButtonAdd()
{
    UpdateData(TRUE);
    if(DB.IsExistsKindName(m_strKindName)) {
        CTools::MessageBoxFormat(_T("同名种类己经存在:%s"), m_strKindName.GetString());
        return;
    }
    if(!DB.AddKind(m_strKindName)) {
        AfxMessageBox(_T("增加失败"));
    }
    else {
        UpdateTree();
    }
    UpdateData(FALSE);
}


// 修改
void CDialogDataKind::OnBnClickedButtonUpdate()
{
    UpdateData(TRUE);
    HTREEITEM hItem = m_treeKind.GetSelectedItem();
    if(!hItem) {
        CTools::MessageBoxFormat(_T("未选择修改项"));
        return;
    }
    if(DB.IsExistsKindName(m_strKindName)) {
        CTools::MessageBoxFormat(_T("己经存在:%s"), m_strKindName.GetString());
        return;
    }
    int id = (int)m_treeKind.GetItemData(hItem);
    if(!DB.UpdateKind(id, m_strKindName)) {
        AfxMessageBox(_T("修改失败"));
    }
    else {
        UpdateTree();
    }
    UpdateData(FALSE);
}

// 关闭
void CDialogDataKind::OnBnClickedButtonClose()
{
    this->ShowWindow(FALSE);
}

// 删除
void CDialogDataKind::OnBnClickedButtonDelete()
{
    UpdateData(TRUE);
    if(!DB.DelKind(m_strKindName)) {
        AfxMessageBox(_T("删除失败"));
    }
    else {
        UpdateTree();
    }
    UpdateData(FALSE);
}


void CDialogDataKind::OnTvnSelchangedTreeKind(NMHDR *pNMHDR, LRESULT *pResult)
{
    DWORD dwPos = GetMessagePos();
    TVHITTESTINFO ht = {0};
    ht.pt.x = GET_X_LPARAM(dwPos);
    ht.pt.y = GET_Y_LPARAM(dwPos);
    ::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);
    TreeView_HitTest(pNMHDR->hwndFrom, &ht);
    HTREEITEM hItem = ht.hItem;
    if(hItem) {
        m_strKindName =  m_treeKind.GetItemText(hItem);
        UpdateData(FALSE);
    }
    *pResult = 0;
}


BOOL CDialogDataKind::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
