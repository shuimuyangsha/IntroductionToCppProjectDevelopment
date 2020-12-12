// DialogFilterKind.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogFilterKind.h"
#include "afxdialogex.h"


// CDialogFilterKind 对话框

IMPLEMENT_DYNAMIC(CDialogFilterKind, CDialogEx)

CDialogFilterKind::CDialogFilterKind(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_STOCK_ROUND, pParent)
{

}

CDialogFilterKind::~CDialogFilterKind()
{
}

void CDialogFilterKind::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_STOCK_KIND, m_lst);
}


BEGIN_MESSAGE_MAP(CDialogFilterKind, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_OK, &CDialogFilterKind::OnBnClickedButtonOk)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL2, &CDialogFilterKind::OnBnClickedButtonCancel2)
END_MESSAGE_MAP()


// CDialogFilterKind 消息处理程序


void CDialogFilterKind::OnBnClickedButtonOk()
{
    using namespace std;
    m_vKinds.clear();
    // 保存选择结果
    for(int i = 0; i < m_lst.GetItemCount(); ++i) {
        if(m_lst.GetCheck(i)) {
            int id = (int)m_lst.GetItemData(i);
            wstring name = m_lst.GetItemText(i, 1).GetString();
            m_vKinds.push_back(make_tuple(id, name));
        }
    }
    // 退出
    OnOK();
}

BOOL CDialogFilterKind::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化

    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
        dwStyle |= LVS_EX_GRIDLINES; //网络线
        dwStyle |= LVS_EX_CHECKBOXES; //CheckBox
        m_lst.SetExtendedStyle(dwStyle);

        // 设置列,并设置大小
        {
            CRect rc;
            m_lst.GetClientRect(rc);

            m_lst.InsertColumn(0, _T("序列"), LVCFMT_LEFT, 40);
            m_lst.InsertColumn(1, _T("代码"), LVCFMT_LEFT, rc.Width() - 40);
        }
    }

    // 显示数据
    {
        using namespace std;
        vector<tuple<int, std::wstring>> v;
        DB.QueryKind(v);

        for(auto tp : v) {
            auto id = get<0>(tp);
            auto name = get<1>(tp);

            CString str;
            int index = m_lst.GetItemCount();
            str.Format(_T("%02d"), index);
            m_lst.InsertItem(index, str.GetString());

            m_lst.SetItemText(index, 1, name.c_str());

            m_lst.SetItemData(index, id);
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CDialogFilterKind::OnBnClickedButtonCancel2()
{
    OnCancel();
}
