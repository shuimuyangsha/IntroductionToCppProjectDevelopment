// DepartmentMng.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "DepartmentMng.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepartmentMng dialog


CDepartmentMng::CDepartmentMng(CWnd *pParent /*=NULL*/)
    : CDialog(CDepartmentMng::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDepartmentMng)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDepartmentMng::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDepartmentMng)
    DDX_Control(pDX, IDC_CURRENTNODE, m_CurrentNode);
    DDX_Control(pDX, IDC_DPNAME, m_DPName);
    DDX_Control(pDX, IDC_DPLIST, m_DPList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDepartmentMng, CDialog)
    //{{AFX_MSG_MAP(CDepartmentMng)
    ON_NOTIFY(NM_CLICK, IDC_DPLIST, OnClickDplist)
    ON_BN_CLICKED(IDC_BTADD, OnBtadd)
    ON_NOTIFY(TVN_SELCHANGED, IDC_DPLIST, OnSelchangedDplist)
    ON_BN_CLICKED(IDC_BTDELETE, OnBtdelete)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDepartmentMng message handlers

void CDepartmentMng::OnOK()
{

}

BOOL CDepartmentMng::PreTranslateMessage(MSG *pMsg)
{
    // 防止按ESC键关闭对话框
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDepartmentMng::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_InitSucess = m_DBMng.InitDatabase();
    m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bmp;
    bmp.LoadBitmap(IDB_DEPART);
    m_ImageList.Add(&bmp, RGB(0, 0, 0));
    m_DPList.SetImageList(&m_ImageList, TVSIL_NORMAL);
    m_CurrentID = -2;
    LoadDepartInfo();
    return TRUE;
}

// 重新加载部门组织结构
void CDepartmentMng::LoadDepartInfo()
{
    if(m_InitSucess) {
        m_DPList.DeleteAllItems();  // 删除所有节点
        CString sql = "select * from tb_Department where ParentID = 0";
        CString dpName;
        HTREEITEM hRoot, hParent;
        m_DBMng.m_pRecord = m_DBMng.m_pConnect->Execute(
                                "select * from tb_Department where ParentID = 0",
                                0, adCmdText);
        hRoot = m_DPList.InsertItem("部门组织", 0, 0);
        while(! m_DBMng.m_pRecord->ADOEOF) {
            dpName = (TCHAR *)(_bstr_t)m_DBMng.m_pRecord->GetFields()
                     ->GetItem("DepartName")->Value;
            hParent = m_DPList.InsertItem(dpName, 0, 0, hRoot);
            int id = m_DBMng.m_pRecord->GetFields()->GetItem("DepartID")->Value.iVal;
            // 将每个节点关联一个数据，数据低字节表示当前部门的ID，高字节表示上一级部门的ID
            DWORD dwData =  MAKELPARAM(id, 0);
            m_DPList.SetItemData(hParent, dwData);

            // 将所有子节点添加到父节点下
            FindNode(hParent, id);
            m_DBMng.m_pRecord->MoveNext();
        }
        m_DPList.Expand(hRoot, TVE_EXPAND);
    }
}

void CDepartmentMng::FindNode(HTREEITEM hParent, int ParentID)
{
    // 声明指针
    _ConnectionPtr pCon;
    _RecordsetPtr pRecord;

    // 创建实例
    pCon.CreateInstance("ADODB.connection");
    pRecord.CreateInstance("ADODB.recordset");

    // 设置数据库连接串
    pCon->ConnectionString = m_DBMng.m_ConnectStr.AllocSysString();
    pCon->Open("", "", "", -1);

    // SQL语句:查询指定ID的部门
    CString sql;
    sql.Format("select * from tb_Department where ParentID = %d", ParentID);
    pRecord = pCon->Execute((_bstr_t)sql, 0, adCmdText);
    CString dpName;
    HTREEITEM hTemp;
    DWORD dwData;
    // 遍历数据
    while(! pRecord->ADOEOF) {
        dpName = (TCHAR *)(_bstr_t) pRecord->GetFields()->GetItem("DepartName")->Value;
        int id = pRecord->GetFields()->GetItem("DepartID")->Value.iVal;
        hTemp = m_DPList.InsertItem(dpName, 0, 0, hParent);
        dwData = MAKELPARAM(id, ParentID);
        m_DPList.SetItemData(hTemp, dwData);
        // 递规调用
        FindNode(hTemp, id);
        // 移动指针
        pRecord->MoveNext();
    }
}

void CDepartmentMng::OnClickDplist(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
}

// 添加节点
void CDepartmentMng::OnBtadd()
{
    CString csName;
    m_DPName.GetWindowText(csName);

    if(!csName.IsEmpty()) {
        HTREEITEM hItem = m_DPList.GetSelectedItem();
        if(hItem != NULL) {
            m_DPList.InsertItem(csName, 0, 1, hItem);
            DWORD dwData = m_DPList.GetItemData(hItem);
            int ParentID = 0;

            if(dwData != 0) {
                ParentID = LOWORD(dwData);
            }
            // 保存数据到数据表
            CString sql;
            sql.Format("select * from GetNewDepartID");

            _RecordsetPtr pRecord = m_DBMng.m_pConnect->Execute("select * from GetNewDepartID", 0, adCmdText);
            int ID = 1;
            if(pRecord != NULL) {
                if(!pRecord->ADOEOF) {
                    ID = pRecord->GetFields()->GetItem("NewID")->Value.iVal;
                }
            }
            if(ID >= 10000) {
                MessageBox("部门数量不能大于10000!", "提示");
                return;
            }
            sql.Format("Insert into tb_Department values (%d,'%s',%d)", ID, csName, ParentID);
            try {
                m_DBMng.m_pConnect->Execute((_bstr_t)sql, 0, adCmdText);
                LoadDepartInfo();
                MessageBox("操作成功!", "提示");

            }
            catch(...) {
                MessageBox("操作失败!", "提示");
            }
        }
    }
}

void CDepartmentMng::OnSelchangedDplist(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    // TODO: Add your control notification handler code here
    HTREEITEM hItem = m_DPList.GetSelectedItem();
    if(hItem != NULL) {
        m_CurrentNode.SetWindowText(m_DPList.GetItemText(hItem));
    }
    else {
        m_CurrentNode.SetWindowText("");
    }
    *pResult = 0;
}

// 删除节点
void CDepartmentMng::OnBtdelete()
{
    HTREEITEM hItem = m_DPList.GetSelectedItem();
    if(hItem != NULL) {
        if(hItem == m_DPList.GetRootItem()) {
            MessageBox("根节点不能删除!", "提示");
            return ;
        }
        HTREEITEM hChild = m_DPList.GetChildItem(hItem);
        if(hChild == NULL) { // 执行删除操作
            // 获取节点ID，根据节点ID执行删除操作
            DWORD dwData = m_DPList.GetItemData(hItem);
            int ID = LOWORD(dwData);
            if(MessageBox("确实要进行删除操作吗?", "提示", MB_YESNO) == IDYES) {
                CString sql;
                sql.Format("delete from tb_Department where DepartID= %d", ID);
                try {
                    m_DBMng.m_pConnect->Execute((_bstr_t)sql, 0, adCmdText);
                    m_DPList.DeleteItem(hItem);
                    MessageBox("删除成功!", "提示");
                }
                catch(...) {
                    MessageBox("操作失败!", "提示");
                }
            }
        }
        else {
            MessageBox("当前节点含有子节点，不能执行删除操作，请先删除所有子节点!", "提示");
        }
    }
}
