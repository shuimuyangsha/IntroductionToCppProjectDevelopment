// DialogKindSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogKindSelect.h"
#include "afxdialogex.h"

using namespace std;

// CDialogKindSelect 对话框

CString CDialogKindSelect::m_strSelectCode = _T("");



IMPLEMENT_DYNAMIC(CDialogKindSelect, CDialogEx)

CDialogKindSelect::CDialogKindSelect(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_KIND_SELECT, pParent)
{
}

CDialogKindSelect::~CDialogKindSelect()
{
}

void CDialogKindSelect::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_KIND, m_treeKind);
    DDX_Control(pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP(CDialogKindSelect, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogKindSelect::OnBnClickedButtonOk)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogKindSelect::OnBnClickedButtonCancel)
    ON_NOTIFY(NM_CLICK, IDC_TREE_KIND, &CDialogKindSelect::OnNMClickTreeKind)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogKindSelect 消息处理程序

CString CDialogKindSelect::GetSelectStockCode() const
{
    POSITION pos = m_lst.GetFirstSelectedItemPosition();
    if(pos) {
        int index = m_lst.GetNextSelectedItem(pos);
        CString* pData = (CString*)m_lst.GetItemData(index);
        if(pData) {
            return *pData;
        }
    }
    return _T("");
}

//确定
void CDialogKindSelect::OnBnClickedButtonOk()
{
    m_strSelectCode = GetSelectStockCode();
    OnOK();
}


// 取消
void CDialogKindSelect::OnBnClickedButtonCancel()
{
    CDialogKindSelect::OnCancel();
}


void CDialogKindSelect::OnOK()
{
    ClearList();
    CDialogEx::OnOK();
}


void CDialogKindSelect::OnCancel()
{
    ClearList();
    CDialogEx::OnCancel();
}


BOOL CDialogKindSelect::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        // tree
        {
            m_bmpTree.LoadBitmap(IDB_BITMAP_KIND);
            m_imgListTree.Create(16, 16, ILC_COLOR32, 1, 1);
            m_imgListTree.Add(&m_bmpTree, RGB(0, 0, 0));
            m_treeKind.SetImageList(&m_imgListTree, TVSIL_NORMAL);
        }
        // clistctrl
        {
            m_imgListList.Create(16, 16, ILC_COLOR32, 1, 1);
            m_bmpList.LoadBitmap(IDB_BITMAP_STOCK);
            m_imgListList.Add(&m_bmpList, RGB(0, 0, 0));
            m_lst.SetImageList(&m_imgListList, LVSIL_SMALL);
        }
    }
    UpdateTree();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogKindSelect::UpdateTree()
{
    m_treeKind.DeleteAllItems();
    // 数据库中查询数据
    typedef std::vector<std::tuple<int, std::wstring>> VKInd;
    VKInd v;
    if(!DB.QueryKind(v)) {
        return;
    }
    // 设置 结点名,并关联数据(种类ID, int 型)
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
    }
}

// 清除列表中的数据
void CDialogKindSelect::ClearList()
{
    // 删除掉数据
    for(int i = 0; i < m_lst.GetItemCount(); ++i) {
        auto pData = (CString*)m_lst.GetItemData(i);
        SAFE_DELETE(pData);
    }
    m_lst.DeleteAllItems();
}

// 更新
void CDialogKindSelect::UpdateList(const vector<CMRKJDatabase::TupleStockInfo>& v)
{
    // 清除旧数据
    ClearList();
    // 增加新数据
    for(size_t index = 0; index != v.size(); ++index) {
        int idx = m_lst.InsertItem(index, get<1>(v[index]).c_str());
        m_lst.SetItemData(idx, (DWORD_PTR)new CString(get<0>(v[index]).c_str()));
    }
}

void CDialogKindSelect::OnNMClickTreeKind(NMHDR *pNMHDR, LRESULT *pResult)
{
    // 获取选中项
    HTREEITEM hItem = CTools::GetTreeItemClicked(pNMHDR);
    if(hItem) {
        // 获得种类ID
        int id = (int)m_treeKind.GetItemData(hItem);
        // 根据id查询下面的种类,并刷新右面的CListCtrl
        vector<CMRKJDatabase::TupleStockInfo> v;
        DB.QueryStockInfo(v);
        //去除不是当前ID的数据
        v.erase(remove_if(v.begin(), v.end(),
        [id](const CMRKJDatabase::TupleStockInfo & info)->bool {
            return id != std::get<2>(info);
        }),
        v.end());
        // 更新
        UpdateList(v);
    }
    *pResult = 0;
}

void CDialogKindSelect::OnDestroy()
{
    // 销毁前清理数据
    ClearList();
    CDialogEx::OnDestroy();
}


BOOL CDialogKindSelect::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
