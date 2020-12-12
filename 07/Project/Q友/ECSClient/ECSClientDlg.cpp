// ECSClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "ECSClientDlg.h"
#include "Message.h"
#include "SendDlg.h"
#include "ScrollParent.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define SEX_MAN 10000
#define SEX_WOMAN 10001

extern CScrollParent *SkinScrollBar(CWnd *pWnd);

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSClientDlg dialog

CECSClientDlg::CECSClientDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CECSClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CECSClientDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICON);
    m_nLoginUser = 12345;
    m_nTestNum  = 0;
    m_bLogined = FALSE;
    HRESULT hr;
    hr = m_pXMLDoc.CreateInstance(__uuidof(DOMDocument30));
}

void CECSClientDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CECSClientDlg)
    DDX_Control(pDX, IDC_ACLIST, m_ACList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CECSClientDlg, CDialog)
    //{{AFX_MSG_MAP(CECSClientDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TVN_SELCHANGED, IDC_ACLIST, OnSelchangedAclist)
    ON_NOTIFY(NM_DBLCLK, IDC_ACLIST, OnDblclkAclist)
    ON_WM_CLOSE()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSClientDlg message handlers

BOOL CECSClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if(pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if(!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // 创建图标列表，为账号列表显示图标用
    m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bmp;
    // 部门图标
    bmp.LoadBitmap(IDB_DEPART);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));
    bmp.Detach();

    // 图标：男
    bmp.LoadBitmap(IDB_MAN);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));
    bmp.Detach();

    // 图标：女
    bmp.LoadBitmap(IDB_WOMAN);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));
    bmp.Detach();

    // 图标：人
    bmp.LoadBitmap(IDB_PERSON);
    m_ImageList.Add(&bmp, RGB(255, 255, 255));

    // 与树控件关联
    m_ACList.SetImageList(&m_ImageList, TVSIL_NORMAL);

    // 设置滚动条
    SkinScrollBar(&m_ACList);

    // 启动定时器:检测在线状态
    SetTimer(1, 2000, NULL);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CECSClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CECSClientDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CECSClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CECSClientDlg::OnOK()
{
//  CSendDlg dlg;
//  dlg.DoModal();
}



// 从xml文件中加载信息
void CECSClientDlg::LoadOrgFromFile(BSTR csFileName)
{
    m_pXMLDoc->load(csFileName);
    MSXML2::IXMLDOMElementPtr childNode ;

    // 删除所有树节点
    m_ACList.DeleteAllItems();
    //查找根节点
    childNode = m_pXMLDoc->selectSingleNode("// ITEM[@部门ID = 0]");
    // 遍历节点
    while(childNode != NULL) {
        // 递归调用
        FindSubNode(childNode, TVI_ROOT);
        childNode = childNode->GetnextSibling();
    }
    // 获得根节点
    HTREEITEM hRoot = m_ACList.GetRootItem();
    // 展开
    m_ACList.Expand(hRoot, TVE_EXPAND);
    // 设置用户状态
    SetUserState(m_nLoginUser, 1);
}

void CECSClientDlg::FindSubNode(MSXML2::IXMLDOMElementPtr pParentNode ,
                                HTREEITEM hParentItem)
{
    if(pParentNode != NULL) {
        VARIANT varVal;
        CString strValue = "";
        CString csText = "";
        // 读取text属性

        varVal = pParentNode->getAttribute("text");
        if(varVal.vt != VT_EMPTY) {
            csText = (char *)(_bstr_t)varVal;
        }

        // 读取部门ID
        strValue = "";
        varVal = pParentNode->getAttribute("部门ID");
        int DepartID = 0;
        if(varVal.vt != VT_EMPTY) {
            strValue = (char *)(_bstr_t)varVal;
            DepartID = atoi(strValue);
        }

        // 读取上级部门ID
        strValue = "";
        varVal = pParentNode->getAttribute("上级ID");
        int ParentID = 0;
        if(varVal.vt != VT_EMPTY) {
            strValue = (char *)(_bstr_t)varVal;
            ParentID = atoi(strValue);
        }

        // 读取用户ID
        strValue = "";
        varVal = pParentNode->getAttribute("用户ID");
        int UserID = 0;
        if(varVal.vt != VT_EMPTY) {
            strValue = (char *)(_bstr_t)varVal;
            UserID = atoi(strValue);
        }

        // 读取用户名称
        strValue = "";
        varVal = pParentNode->getAttribute("用户名称");
        if(varVal.vt != VT_EMPTY) {
            strValue = (char *)(_bstr_t)varVal;
        }

        // 读取性别
        CString csSex = "";
        varVal = pParentNode->getAttribute("性别");
        if(varVal.vt != VT_EMPTY) {
            csSex = (char *)(_bstr_t)varVal;
        }

        // 插入节点，设置节点数据
        HTREEITEM hRoot;
        DWORD dwData = 0;
        if(UserID == 0) {
            hRoot = m_ACList.InsertItem(csText, 0, 0, hParentItem);
            dwData = MAKELPARAM(DepartID, ParentID); // 设置部门节点数据

        }
        else {
            hRoot = m_ACList.InsertItem(csText, 3, 3, hParentItem); // 掉线状态

            // 设置账户数据，记录性别信息
            if(csSex == "男") {
                dwData = MAKELPARAM(SEX_MAN, UserID); // 设置账户数据
            }
            else if(csSex == "女") {
                dwData = MAKELPARAM(SEX_WOMAN, UserID); // 设置账户数据
            }

        }

        // 设置关联数据
        m_ACList.SetItemData(hRoot, dwData);

        MSXML2::IXMLDOMElementPtr pNode = pParentNode->GetfirstChild();
        while(pNode != NULL) {
            // 递归调用

            FindSubNode(pNode, hRoot);
            pNode = pNode->GetnextSibling();
        }
    }
}

void CECSClientDlg::OnSelchangedAclist(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
}

// 双击树
void CECSClientDlg::OnDblclkAclist(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    HTREEITEM hItem = m_ACList.GetSelectedItem();
    HTREEITEM hRoot = m_ACList.GetRootItem();
    if(hItem != NULL) {
        // 对于账户节点来说，低字节为0，高字节表示账户ID
        DWORD dwData = m_ACList.GetItemData(hItem);
        int ID = HIWORD(dwData);
        int lowID = LOWORD(dwData);
        // 对于当前登录的账户，不在发送数据的范围内，即不能够向自己发送数据
        if((lowID == SEX_MAN || lowID == SEX_WOMAN) &&
           ID != 0 && ID != m_nLoginUser) {
            CSendDlg *pDlg = NULL;
            POSITION pos = NULL;
            if(SearchSendDlg(ID, pos) == FALSE) {
                pDlg = new CSendDlg();
                // 关联对话框的目标ID
                pDlg->m_UserID = ID;
                pDlg->Create(IDD_SENDDLG_DIALOG);
                m_pSdDlgList.AddTail(pDlg);
            }
            else if(pDlg == NULL && pos != NULL) {
                pDlg = (CSendDlg *)m_pSdDlgList.GetAt(pos);
            }
            if(pDlg != NULL) {
                // 添加对方账户ID

                CString csID;
                csID.Format("%d", ID);
                pDlg->m_DesID.ResetContent();   // 清空数据

                pDlg->m_DesID.AddString(csID);
                pDlg->m_DesID.SetCurSel(0);
                CString csName = m_ACList.GetItemText(hItem);
                // 添加对方账户名
                pDlg->m_DesName.ResetContent(); // 清空数据
                pDlg->m_DesName.AddString(csName);
                pDlg->m_DesName.SetCurSel(0);

                // 添加本地账户ID和名称
                CString csLocalID;
                csLocalID.Format("%d", m_nLoginUser);
                pDlg->m_LocalID.SetWindowText(csLocalID);
                CString csLocalName = "";
                BOOL bRet = FindUserName(m_nLoginUser, csLocalName);
                if(bRet) {
                    pDlg->m_LocalName.SetWindowText(csLocalName);
                }
                // 显示窗口
                pDlg->ShowWindow(SW_SHOW);

                // 在其他发送窗口之上显示当前发送窗口
                pDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
        }
    }
    *pResult = 0;
}

/********************************************************

功能描述：根据对方的用户ID查找当前是否有发送对话框与其关联
          如果没有，可能需要建立对话框，有，则直接显示该对
          话框

*********************************************************/
BOOL CECSClientDlg::SearchSendDlg(int nID, POSITION &ptPos)
{
    BOOL bRet = FALSE;
    int nCount = m_pSdDlgList.GetCount();
    if(nCount != 0)     // 遍历对话框列表，搜索目标对话框

    {
        POSITION pos = m_pSdDlgList.GetHeadPosition();
        while(pos != NULL) {
            CSendDlg *pDlg = NULL;
            ptPos = pos;
            pDlg = (CSendDlg *)m_pSdDlgList.GetNext(pos);
            if(pDlg != NULL) {
                if(pDlg->m_UserID == nID) {
                    bRet = TRUE;
                    break;
                }
            }
            else {
                ptPos = NULL;
            }
        }
    }
    return bRet;
}
// 根据用户ID,从树控件中查找用户名称

BOOL CECSClientDlg::FindUserName(int nUserID, CString &csName)
{
    HTREEITEM hRoot = m_ACList.GetRootItem();
    if(hRoot != NULL) {
        HTREEITEM hSubItem;
        return  FindNode(hRoot, nUserID, csName, hSubItem);
    }
    return FALSE;
}

BOOL CECSClientDlg::FindNode(HTREEITEM hParent, int nUserID,
                             CString &csName, HTREEITEM &hRetItem)
{
    // 先判断是否为账户节点

    // 对于账户节点来说，低字节表示性别，高字节表示账户ID

    DWORD dwData = m_ACList.GetItemData(hParent);
    int ID = HIWORD(dwData);
    int lowID = LOWORD(dwData);
    CString csNodeName =  m_ACList.GetItemText(hParent);
    if((lowID == SEX_MAN || lowID == SEX_WOMAN) && ID == nUserID) // 找到了节点

    {
        csName = m_ACList.GetItemText(hParent);
        hRetItem = hParent;
        return TRUE;
    }
    else    // 如果有子节点，遍历所有子节点

    {
        if(m_ACList.ItemHasChildren(hParent)) {
            HTREEITEM hSubItem = m_ACList.GetChildItem(hParent);
            while(hSubItem != NULL) {
                BOOL bRet = FindNode(hSubItem, nUserID, csName, hRetItem);
                if(bRet) {
                    return bRet;
                }
                hSubItem = m_ACList.GetNextItem(hSubItem, TVGN_NEXT);
            }
        }
        // 如果有兄弟节点，遍历兄弟节点

        HTREEITEM hNextNode = m_ACList.GetNextItem(hParent, TVGN_NEXT);
        while(hNextNode != NULL) {
            return FindNode(hNextNode, nUserID, csName, hRetItem);
            hNextNode = m_ACList.GetNextItem(hNextNode, TVGN_NEXT);
        }
    }
    return FALSE;
}

void CECSClientDlg::OnClose()
{
    CDialog::OnClose();
}

void CECSClientDlg::OnCancel()
{
    if(m_nLoginUser != 0) {
        // 向服务器发送掉线通知

        CMessage Msg;
        Msg.m_nMsgType = MT_DISCONNECT;
        Msg.m_nAccountID[0] = m_nLoginUser;
        m_ClientSock.Send(&Msg, sizeof(CMessage), MSG_OOB);
    }
    CDialog::OnCancel();
}

// 根据用户ID设置用于状态
void CECSClientDlg::SetUserState(int nUserID, int nState)
{
    // 根据用户ID获取项目句柄

    HTREEITEM hRoot = m_ACList.GetRootItem();
    if(hRoot != NULL) {
        HTREEITEM hSubItem;
        CString csName;
        BOOL bRet = FindNode(hRoot, nUserID, csName, hSubItem);
        if(bRet) {
            // 获取用户性别信息

            DWORD dwData = m_ACList.GetItemData(hSubItem);
            WORD nSex = LOWORD(dwData);
            int nImage, nSelImage;
            m_ACList.GetItemImage(hSubItem, nImage, nSelImage);
            if(nState == 0) {
                if(nImage != 3) {
                    m_ACList.SetItemImage(hSubItem, 3, 3);
                }
            }
            else {
                if(nSex == SEX_MAN) {
                    if(nImage != 1) {
                        m_ACList.SetItemImage(hSubItem, 1, 1);
                    }
                }
                else if(nSex == SEX_WOMAN) {
                    if(nImage != 2) {
                        m_ACList.SetItemImage(hSubItem, 2, 2);
                    }
                }
            }
        }
    }
}

void CECSClientDlg::OnTimer(UINT nIDEvent)
{
    if(nIDEvent == 1) {
        if(m_nTestNum > 4) { // 4*2秒种没有收到服务器发来的信息，表示已经断线
            HTREEITEM hRoot = m_ACList.GetRootItem();
            SetAllUserState(hRoot, 0);
            m_nTestNum = 0;
            SetUserState(m_nLoginUser, 0);

            // 重新连接服务器
            // 主机名
            char chHostName[MAX_PATH] = {0};
            gethostname(chHostName, MAX_PATH);
            hostent *phost = gethostbyname(chHostName);
            // 获得 本机IP
            char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);
            m_ClientSock.ShutDown();
            m_ClientSock.Close();
            m_ClientSock.Create(3000 + rand() % 2000, SOCK_STREAM, chIP);

            BOOL bEnable = TRUE;
            m_ClientSock.SetSockOpt(SO_OOBINLINE, &bEnable , sizeof(BOOL));

            BOOL bRet = m_ClientSock.Connect(m_ServerIP, m_nPort);
            // 如果连接成功
            if(bRet) {
                // 发送首次登录消息
                CMessage cmLoginInfo;
                CString csID;
                csID.Format("%d", m_nLoginUser);

                strncpy(cmLoginInfo.m_chAccount, csID, csID.GetLength());
                strncpy(cmLoginInfo.m_chPassword, m_Password, m_Password.GetLength());
                cmLoginInfo.m_nMsgType = MT_FIRSTLOG;
                m_ClientSock.Send(&cmLoginInfo, sizeof(CMessage));
            }
        }
        else {
            m_nTestNum++;
        }
    }
    CDialog::OnTimer(nIDEvent);
}

void CECSClientDlg::SetAllUserState(HTREEITEM hParent, int nState)
{
    // 先判断是否为账户节点
    // 对于账户节点来说，低字节表示性别，高字节表示账户ID
    DWORD dwData = m_ACList.GetItemData(hParent);
    int ID = HIWORD(dwData);
    int lowID = LOWORD(dwData);
    if(lowID == SEX_MAN || lowID == SEX_WOMAN) { // 找到了节点
        if(nState == 0) {
            m_ACList.SetItemImage(hParent, 3, 3);
        }
        else {
            if(lowID == SEX_MAN) {
                m_ACList.SetItemImage(hParent, 1, 1);
            }
            else if(lowID == SEX_WOMAN) {
                m_ACList.SetItemImage(hParent, 2, 2);
            }
        }
    }
    else {  // 如果有子节点，遍历所有子节点
        if(m_ACList.ItemHasChildren(hParent)) {
            HTREEITEM hSubItem = m_ACList.GetChildItem(hParent);
            while(hSubItem != NULL) {
                SetAllUserState(hSubItem, nState);
                hSubItem = m_ACList.GetNextItem(hSubItem, TVGN_NEXT);
            }
        }
        // 如果有兄弟节点，遍历兄弟节点

        HTREEITEM hNextNode = m_ACList.GetNextItem(hParent, TVGN_NEXT);
        while(hNextNode != NULL) {
            SetAllUserState(hNextNode, nState);
            hNextNode = m_ACList.GetNextItem(hNextNode, TVGN_NEXT);
        }
    }
}
