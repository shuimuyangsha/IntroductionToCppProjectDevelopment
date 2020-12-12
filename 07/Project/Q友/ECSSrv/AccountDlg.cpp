// AccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "AccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog
/*****************************************************************
在树结构中，部门名称节点数据低字节表示部门ID，高字节表示上级部门ID
账户节点数据低字节为0，高字节表示账户ID
*****************************************************************/

CAccountDlg::CAccountDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CAccountDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAccountDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CAccountDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAccountDlg)
    DDX_Control(pDX, IDC_USERID, m_UserID);
    DDX_Control(pDX, IDC_CURUSER, m_User);
    DDX_Control(pDX, IDC_SEX, m_Sex);
    DDX_Control(pDX, IDC_HANDERSHIP, m_HanderShip);
    DDX_Control(pDX, IDC_DPLIST, m_DPList);
    DDX_Control(pDX, IDC_DEPARTID, m_DepartID);
    DDX_Control(pDX, IDC_DEPART, m_Depart);
    DDX_Control(pDX, IDC_ACPASSWORD, m_Password);
    DDX_Control(pDX, IDC_ACNAME, m_AccountName);
    DDX_Control(pDX, IDC_ACCOUNTID, m_AccountID);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountDlg, CDialog)
    //{{AFX_MSG_MAP(CAccountDlg)
    ON_NOTIFY(TVN_SELCHANGED, IDC_DPLIST, OnSelchangedDplist)
    ON_BN_CLICKED(IDC_BTADD, OnBtadd)
    ON_BN_CLICKED(IDC_BT_DELETE, OnBtDelete)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg message handlers

void CAccountDlg::OnOK()
{

}

BOOL CAccountDlg::PreTranslateMessage(MSG *pMsg)
{
    // 防止按ESC键关闭对话框
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAccountDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 数据库初始化
    m_InitSucess = m_DBMng.InitDatabase();
    // 图片列表设置:用于给树控件增加图片
    m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bmp;
    // 部门图片
    bmp.LoadBitmap(IDB_DEPART);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));
    bmp.Detach();
    // 人员图片:男
    bmp.LoadBitmap(IDB_MAN);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));
    bmp.Detach();
    // 人员图片:女
    bmp.LoadBitmap(IDB_WOMAN);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));

    // 把图片列表和树控件关联
    m_DPList.SetImageList(&m_ImageList, TVSIL_NORMAL);

    // 创建xml指针
    HRESULT hr;
    hr = pXMLDoc.CreateInstance(__uuidof(DOMDocument30));

    // 载入部门信息
    LoadDepartInfo();
    // 载入账户信息
    LoadAccountInfo();
    return TRUE;
}

// 加载部门信息
void CAccountDlg::LoadDepartInfo()
{
    if(m_InitSucess) {
        m_DPList.DeleteAllItems();   // 删除所有节点
        CString sql = "select * from tb_Department where ParentID = 0";
        CString dpName;
        HTREEITEM hRoot, hParent;
        m_DBMng.m_pRecord = m_DBMng.m_pConnect->Execute("select * from tb_Department where ParentID = 0", 0, adCmdText);
        hRoot = m_DPList.InsertItem("部门组织", 0, 0);

        // 通过智能指针创建一个解析器的实例
        CECSSrvApp *pApp = (CECSSrvApp *)AfxGetApp();
        char chFileName[MAX_PATH] = { 0 };
        strcpy(chFileName, pApp->m_chFullPath);
        strcat(chFileName, "\\File\\");
        // 创建目录
        CreateDirectory(chFileName, NULL);
        strcat(chFileName, "org.xml");
        //  BSTR csFile = chName;

        pXMLDoc->createComment((_bstr_t)chFileName);
        BSTR csName = L"ITEM";
        VARIANT vrType;
        VariantInit(&vrType);
        vrType.vt = VT_INT;
        vrType.iVal = MSXML2::NODE_ELEMENT;
        MSXML2::IXMLDOMNodePtr pInsert = pXMLDoc->createNode(_T("element"), csName, "");

        if(pInsert != NULL) {
            BSTR csAttr = L"text";      // text属性
            MSXML2::IXMLDOMAttributePtr pAttr = pXMLDoc->createAttribute(csAttr);
            VARIANT vrValue = (_variant_t)(_bstr_t)(L"部门组织");
            pAttr->put_value(vrValue);
            pInsert->attributes->setNamedItem(pAttr);

            csAttr = L"部门ID";           // 部门ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"0");
            pAttr->put_value(vrValue);
            pInsert->attributes->setNamedItem(pAttr);

            csAttr = L"上级ID";           // 上级ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"0");
            pAttr->put_value(vrValue);
            pInsert->attributes->setNamedItem(pAttr);

            csAttr = L"用户ID";           // 用户ID
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"0");
            pAttr->put_value(vrValue);
            pInsert->attributes->setNamedItem(pAttr);

            csAttr = L"用户名称";           // 用户名称
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"");
            pInsert->attributes->setNamedItem(pAttr);

            csAttr = L"性别";             // 性别
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"");
            pInsert->attributes->setNamedItem(pAttr);

        }
        pXMLDoc->appendChild(pInsert);   // 向XML中保存根节点

        while(!m_DBMng.m_pRecord->ADOEOF) {
            dpName = (TCHAR *)(_bstr_t)m_DBMng.m_pRecord->GetFields()->GetItem("DepartName")->Value;
            hParent = m_DPList.InsertItem(dpName, 0, 0, hRoot);
            int id = m_DBMng.m_pRecord->GetFields()->GetItem("DepartID")->Value.iVal;
            // 将每个节点关联一个数据，数据低字节表示当前部门的ID，高字节表示上一级部门的ID
            DWORD dwData = MAKELPARAM(id, 0);
            m_DPList.SetItemData(hParent, dwData);

            // 向xml中插入子节点
            MSXML2::IXMLDOMNodePtr pTmp = pXMLDoc->createNode(_T("element"), csName, "");
            if(pTmp != NULL) {
                CString csTmp;
                BSTR csAttr = L"text";      // text属性
                MSXML2::IXMLDOMAttributePtr pAttr = pXMLDoc->createAttribute(csAttr);
                VARIANT vrValue = (_variant_t)(_bstr_t)(dpName);
                pAttr->put_value(vrValue);
                pTmp->attributes->setNamedItem(pAttr);

                csAttr = L"部门ID";           // 部门ID属性
                pAttr = pXMLDoc->createAttribute(csAttr);
                csTmp.Format("%d", id);
                vrValue = (_variant_t)(_bstr_t)(csTmp);
                pAttr->put_value(vrValue);
                pTmp->attributes->setNamedItem(pAttr);

                csAttr = L"上级ID";           // 上级ID属性
                pAttr = pXMLDoc->createAttribute(csAttr);
                vrValue = (_variant_t)(_bstr_t)(L"0");
                pAttr->put_value(vrValue);
                pTmp->attributes->setNamedItem(pAttr);

                csAttr = L"用户ID";           // 上级ID属性
                pAttr = pXMLDoc->createAttribute(csAttr);
                vrValue = (_variant_t)(_bstr_t)(L"0");
                pAttr->put_value(vrValue);
                pTmp->attributes->setNamedItem(pAttr);

                csAttr = L"用户名称";       // 上级ID属性
                pAttr = pXMLDoc->createAttribute(csAttr);
                vrValue = (_variant_t)(_bstr_t)(L"");
                pTmp->attributes->setNamedItem(pAttr);

                csAttr = L"性别";         // 上级ID属性
                pAttr = pXMLDoc->createAttribute(csAttr);
                vrValue = (_variant_t)(_bstr_t)(L"");
                pTmp->attributes->setNamedItem(pAttr);

                pInsert->appendChild(pTmp);

            }
            // 将所有子节点添加到父节点下
            FindNode(hParent, id, pTmp);
            m_DBMng.m_pRecord->MoveNext();
        }
        pXMLDoc->save((_bstr_t)chFileName);
        m_DPList.Expand(hRoot, TVE_EXPAND);
    }
}

void CAccountDlg::FindNode(HTREEITEM hParent, int ParentID, MSXML2::IXMLDOMNodePtr pParentNode)
{
    _ConnectionPtr pCon;
    _RecordsetPtr pRecord;

    pCon.CreateInstance("ADODB.connection");
    pRecord.CreateInstance("ADODB.recordset");

    pCon->ConnectionString = m_DBMng.m_ConnectStr.AllocSysString();
    pCon->Open("", "", "", -1);

    CString sql;
    sql.Format("select * from tb_Department where ParentID = %d", ParentID);
    pRecord = pCon->Execute((_bstr_t)sql, 0, adCmdText);
    CString dpName;
    HTREEITEM hTemp;
    DWORD dwData;
    while(!pRecord->ADOEOF) {
        dpName = (TCHAR *)(_bstr_t)pRecord->GetFields()->GetItem("DepartName")->Value;
        int id = pRecord->GetFields()->GetItem("DepartID")->Value.iVal;
        hTemp = m_DPList.InsertItem(dpName, 0, 0, hParent);
        dwData = MAKELPARAM(id, ParentID);
        m_DPList.SetItemData(hTemp, dwData);

        // 保存部门树结构到XML中
        BSTR csName = L"ITEM";
        MSXML2::IXMLDOMNodePtr pTmp = pXMLDoc->createNode(_T("element"), csName, "");
        if(pTmp != NULL) {
            CString csTmp;
            BSTR csAttr = L"text";      // text属性
            MSXML2::IXMLDOMAttributePtr pAttr = pXMLDoc->createAttribute(csAttr);
            VARIANT vrValue = (_variant_t)(_bstr_t)(dpName);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"部门ID";           // 部门ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            csTmp.Format("%d", id);
            vrValue = (_variant_t)(_bstr_t)(csTmp);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"上级ID";           // 上级ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            csTmp.Format("%d", ParentID);
            vrValue = (_variant_t)(_bstr_t)(csTmp);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"用户ID";           // 用户ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"0");
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"用户名称";       // 用户名称
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"");
            pTmp->attributes->setNamedItem(pAttr);


            csAttr = L"性别";     // 用户名称
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(L"");
            pTmp->attributes->setNamedItem(pAttr);

            pParentNode->appendChild(pTmp);
        }

        FindNode(hTemp, id, pTmp);
        pRecord->MoveNext();
    }
}

void CAccountDlg::OnSelchangedDplist(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    HTREEITEM hItem = m_DPList.GetSelectedItem();
    HTREEITEM hRoot = m_DPList.GetRootItem();

    if(hItem != NULL) {  // 不允许在根节点下添加
        DWORD dwData = m_DPList.GetItemData(hItem);
        int ID = LOWORD(dwData);
        if(ID != 0) {
            m_Depart.SetWindowText(m_DPList.GetItemText(hItem));
            DWORD dwData = m_DPList.GetItemData(hItem);
            int ID = LOWORD(dwData);
            CString csID;
            csID.Format("%d", ID);
            m_DepartID.SetWindowText(csID);
            m_User.SetWindowText("");
            m_UserID.SetWindowText("");
        }
        else {
            m_Depart.SetWindowText("");
            m_DepartID.SetWindowText("");
            if(hItem != hRoot) {
                CString csUsr = m_DPList.GetItemText(hItem);
                m_User.SetWindowText(csUsr);
                CString csUsrID;
                DWORD dwData = m_DPList.GetItemData(hItem);
                int ID = HIWORD(dwData);
                csUsrID.Format("%d", ID);
                m_UserID.SetWindowText(csUsrID);
            }
        }
    }
    else {
        m_Depart.SetWindowText("");
        m_DepartID.SetWindowText("");
        m_User.SetWindowText("");
        m_UserID.SetWindowText("");

    }
    *pResult = 0;
}

void CAccountDlg::OnBtadd()
{
    CString csAcID, csAcName, csSex, csDepart, csDepartID, csPassword, csHanderShip;
    m_AccountID.GetWindowText(csAcID);
    m_AccountName.GetWindowText(csAcName);
    m_Depart.GetWindowText(csDepart);
    m_DepartID.GetWindowText(csDepartID);
    m_Password.GetWindowText(csPassword);
    m_HanderShip.GetWindowText(csHanderShip);
    m_Sex.GetWindowText(csSex);
    // 验证信息的完整性
    if(csAcID.IsEmpty() || csAcName.IsEmpty() || csSex.IsEmpty() || csDepart.IsEmpty()
       || csDepartID.IsEmpty() || csPassword.IsEmpty() || csHanderShip.IsEmpty()) {
        MessageBox("账户信息不能为空!", "提示");
        return;
    }
    if(atoi(csAcID) > 300000) {
        MessageBox("账户不能大于30000!", "提示");
        return;
    }
    // 判断帐号是否存在
    CString csSQL;
    csSQL.Format("select AccountID from tb_Account where AccountID = %d",
                 atoi(csAcID));
    _RecordsetPtr pRecord = m_DBMng.m_pConnect->Execute(
                                "select * from GetNewDepartID", 0, adCmdText);
    if(pRecord->ADOEOF != pRecord->BOF) {
        MessageBox("该账户已存在!", "提示");
        return;
    }


    // 保存数据
    csSQL.Format("Insert into tb_Account "
                 "(AccountID,ECSName,[Password],Sex, DepartID,DepartName,HeaderShip)"
                 " values (%d,'%s','%s','%s',%d,'%s','%s')",
                 atoi(csAcID), csAcName, csPassword, csSex,
                 atoi(csDepartID), csDepart, csHanderShip);
    try {
        m_DBMng.m_pConnect->Execute((_bstr_t)csSQL, 0, adCmdText);
        HTREEITEM hItem = m_DPList.GetSelectedItem();
        HTREEITEM hInsert;
        // 根据性别决定图片
        if(csSex == "男") {
            hInsert = m_DPList.InsertItem(csAcName, 1, 1, hItem);
        }
        else {
            hInsert = m_DPList.InsertItem(csAcName, 2, 2, hItem);
        }
        if(hInsert != NULL) {
            // 关联数据
            DWORD dwData = MAKELRESULT(0, atoi(csAcID));
            m_DPList.SetItemData(hInsert, dwData);
        }
        ClearAcInfo();
        MessageBox("操作成功!", "提示");
    }
    catch(_com_error &e) {
        CString str("操作失败!\r\n");
        str.Append(e.ErrorMessage());
        MessageBox(str, "提示");
    }
}

void CAccountDlg::ClearAcInfo()
{
    m_AccountID.SetWindowText("");
    m_AccountName.SetWindowText("");
    m_Depart.SetWindowText("");
    m_DepartID.SetWindowText("");
    m_Password.SetWindowText("");
    m_HanderShip.SetWindowText("");
    m_Sex.SetWindowText("");
}

// 加载账户信息
void CAccountDlg::LoadAccountInfo()
{
    CString csSQL, csName, csDepart, csSex;
    int iDepartID, iAccountID;

    csSQL.Format("select * from tb_Account");
    _RecordsetPtr pRecord = m_DBMng.m_pConnect->Execute((_bstr_t)csSQL, 0, adCmdText);
    HTREEITEM hRoot = m_DPList.GetRootItem();

    // 通过智能指针创建一个解析器的实例
    CECSSrvApp *pApp = (CECSSrvApp *)AfxGetApp();
    char chFileName[MAX_PATH] = { 0 };
    strcpy(chFileName, pApp->m_chFullPath);
    strcat(chFileName, "\\File\\");
    // 创建目录
    CreateDirectory(chFileName, NULL);
    strcat(chFileName, "org.xml");

    pXMLDoc->load((_bstr_t)chFileName);


    while(!pRecord->ADOEOF) {
        csName = (TCHAR *)(_bstr_t)pRecord->GetFields()->GetItem("ECSName")->Value;
        csDepart = (TCHAR *)(_bstr_t)pRecord->GetFields()->GetItem("DepartName")->Value;
        csSex = (TCHAR *)(_bstr_t)pRecord->GetFields()->GetItem("Sex")->Value;
        iDepartID = pRecord->GetFields()->GetItem("DepartID")->Value.iVal;
        iAccountID = pRecord->GetFields()->GetItem("AccountID")->Value.iVal;
        HTREEITEM hItem = GetDepart(iDepartID, hRoot);
        HTREEITEM hInsert = NULL;


        // 从XML中查找节点
        MSXML2::IXMLDOMElementPtr childNode;
        CString csQuery;
        csQuery.Format("// ITEM[@部门ID = %d]", iDepartID);
        childNode = pXMLDoc->selectSingleNode((_bstr_t)csQuery);
        if(childNode != NULL) {
            TRACE("创建 [%s]", csName.GetString());
            MSXML2::IXMLDOMNodePtr pTmp = pXMLDoc->createNode(_T("element"), (_bstr_t)csName, "");
            TRACE("end.\r\n");
            CString csTmp;
            BSTR csAttr = L"text";      // text属性
            MSXML2::IXMLDOMAttributePtr pAttr = pXMLDoc->createAttribute(csAttr);
            VARIANT vrValue = (_variant_t)(_bstr_t)(csName);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"部门ID";           // 部门ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            csTmp.Format("%d", iDepartID);
            vrValue = (_variant_t)(_bstr_t)(csTmp);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"上级ID";           // 上级ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)("");
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"用户ID";           // 用户ID属性
            pAttr = pXMLDoc->createAttribute(csAttr);
            csTmp.Format("%d", iAccountID);
            vrValue = (_variant_t)(_bstr_t)(csTmp);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"用户名称";       // 用户名称
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(csName);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            csAttr = L"性别";     // 性别
            pAttr = pXMLDoc->createAttribute(csAttr);
            vrValue = (_variant_t)(_bstr_t)(csSex);
            pAttr->put_value(vrValue);
            pTmp->attributes->setNamedItem(pAttr);

            childNode->appendChild(pTmp);
        }


        if(hItem != NULL) {
            if(csSex == "男") {
                hInsert = m_DPList.InsertItem(csName, 1, 1, hItem);
            }
            else {
                hInsert = m_DPList.InsertItem(csName, 2, 2, hItem);
            }
        }
        if(hInsert != NULL) {
            DWORD dwData = MAKELRESULT(0, iAccountID);
            m_DPList.SetItemData(hInsert, dwData);
        }
        pXMLDoc->save((_bstr_t)chFileName);
        pRecord->MoveNext();
    }
}

HTREEITEM CAccountDlg::GetDepart(int DepartID, HTREEITEM hCurrent)
{
    HTREEITEM hRet = NULL;
    if(hCurrent != NULL) {
        DWORD dwData = m_DPList.GetItemData(hCurrent);
        int ID = LOWORD(dwData);
        if(ID != 0 && ID == DepartID) {
            hRet = hCurrent;
            return hRet;
        }
        HTREEITEM hFind;
        BOOL bFind = m_DPList.ItemHasChildren(hCurrent);
        if(bFind) {
            hFind = m_DPList.GetChildItem(hCurrent);
            dwData = m_DPList.GetItemData(hFind);
            ID = LOWORD(dwData);
            if(ID != 0 && ID == DepartID) {
                return hFind;
            }
            if(m_DPList.ItemHasChildren(hFind)) {
                hRet = GetDepart(DepartID, hFind);
            }
            if(hRet != NULL) {
                return hRet;
            }
            while(hFind != NULL) {
                hFind = m_DPList.GetNextItem(hFind, TVGN_NEXT);
                hRet = GetDepart(DepartID, hFind);
                if(hRet != NULL) {
                    return hRet;
                }
            }
        }
    }
    return hRet;
}

// 删除账户信息
void CAccountDlg::OnBtDelete()
{
    CString csUserID;
    m_UserID.GetWindowText(csUserID);
    if(!csUserID.IsEmpty()) {
        if(MessageBox("确实要删除当前账户信息吗?", "提示", MB_YESNO) == IDYES) {
            CString csSQL;
            csSQL.Format("Delete From tb_Account where AccountID = %d", atoi(csUserID));
            try {
                HTREEITEM hItem = m_DPList.GetSelectedItem();
                if(hItem != NULL) {
                    DWORD dwData = m_DPList.GetItemData(hItem);
                    int ID = LOWORD(dwData);
                    if(ID == 0) {    // 再次确认当前节点是账户信息
                        m_DBMng.m_pConnect->Execute((_bstr_t)csSQL, 0, adCmdText);
                        m_DPList.DeleteItem(hItem);
                        MessageBox("操作成功!", "提示");
                    }
                    else {
                        MessageBox("当前节点信息与删除信息不匹配!", "提示");
                    }
                }
            }
            catch(...) {
                MessageBox("操作失败!", "提示");
            }
        }
    }
    else {
        MessageBox("请在列表中选择删除的账户信息!", "提示");
    }
}
