// DialogKeybdQuery.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogKeybdQuery.h"
#include "afxdialogex.h"
#include "MRKJDatabase.h"

// CDialogKeybdQuery 对话框

IMPLEMENT_DYNAMIC(CDialogKeybdQuery, CDialogEx)

CDialogKeybdQuery::CDialogKeybdQuery(const CString& strInput, int x, int y, CWnd* pParent/*=NULL*/)
    : CDialogEx(IDD_DIALOG_KEYBD_QUERY, pParent)
    , m_strQueryInput(strInput)
    , m_x(x)
    , m_y(y)
{
}

CDialogKeybdQuery::~CDialogKeybdQuery()
{
}

void CDialogKeybdQuery::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_QUERY_INPUT, m_edtQueryInput);
    DDX_Control(pDX, IDC_LIST_QUERY_RESULT, m_lstBoxQueryResult);
}


BEGIN_MESSAGE_MAP(CDialogKeybdQuery, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_QUERY_INPUT, &CDialogKeybdQuery::OnEnChangeEditQueryInput)
    ON_LBN_DBLCLK(IDC_LIST_QUERY_RESULT, &CDialogKeybdQuery::OnLbnDblclkListQueryResult)
    ON_EN_SETFOCUS(IDC_EDIT_QUERY_INPUT, &CDialogKeybdQuery::OnEnSetfocusEditQueryInput)
END_MESSAGE_MAP()


// CDialogKeybdQuery 消息处理程序


BOOL CDialogKeybdQuery::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 设置第一个字母
    SetDlgItemText(IDC_EDIT_QUERY_INPUT, m_strQueryInput.GetString());
    // 设置显示位置
    {
        RECT rc;
        GetWindowRect(&rc);
        LONG w = rc.right - rc.left;
        LONG h = rc.bottom - rc.top;
        rc.left = m_x - w;
        rc.top = m_y - h;
        rc.right = m_x;
        rc.bottom = m_y;
        MoveWindow(&rc);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogKeybdQuery::OnEnChangeEditQueryInput()
{
    // 获最前的输入
    CString strInput;
    m_edtQueryInput.GetWindowTextW(strInput);
    if(strInput.GetLength() <= 0) {
        return;
    }
    // 查询结果
    typedef vector<CMRKJDatabase::TupleStockInfo> VStockInfo;
    VStockInfo v;
    DB.QueryStockInfoFuzzy(v, strInput);
    //把查询结果更新到下面的列表框
    CListBox& res = m_lstBoxQueryResult;
    //res.LockWindowUpdate();
    res.SetRedraw(FALSE);
    res.ResetContent();
    for each(auto s in v) {
        auto ss = get<0>(s) + L"  " + get<1>(s);
        res.AddString(ss.c_str());
    }
    //选中列表框中第一框
    if(res.GetCount() > 0) {
        res.SetCurSel(0);
    }
    res.SetRedraw(TRUE);
    //res.UnlockWindowUpdate();
}

// 双击 listbox事件
void CDialogKeybdQuery::OnLbnDblclkListQueryResult()
{
    int index = m_lstBoxQueryResult.GetCurSel();
    if(index >= 0) {
        m_lstBoxQueryResult.GetText(index, m_strQueryResultSel);
    }
    this->OnOK();
}

// 当 输入框 获得焦点时,取消选择
void CDialogKeybdQuery::OnEnSetfocusEditQueryInput()
{
    m_edtQueryInput.SetSel(-1, 0, FALSE);
}


BOOL CDialogKeybdQuery::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        // 回车键时,相当于选择了一行结果,并按了OK
        {
            if(pMsg->wParam == VK_RETURN) {
                OnLbnDblclkListQueryResult();
            }
        }
        // 下面的处理,使本控件在输入时, 可以按上下键进行选择
        {
            if(pMsg->wParam == VK_DOWN) {
                int index = m_lstBoxQueryResult.GetCurSel();
                int count = m_lstBoxQueryResult.GetCount();
                index++;
                if(index >= count) {
                    index = count - 1;
                }
                if(index >= 0) {
                    m_lstBoxQueryResult.SetCurSel(index);
                }
                return TRUE;
            }
            else if(pMsg->wParam == VK_UP) {
                int index = m_lstBoxQueryResult.GetCurSel();
                index--;
                if(index < 0) {
                    index = 0;
                }
                m_lstBoxQueryResult.SetCurSel(index);
                return TRUE;
            }
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogKeybdQuery::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CDialogKeybdQuery::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
