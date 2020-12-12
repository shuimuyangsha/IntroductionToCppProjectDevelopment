// DialogFilterKind.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogFilterKind.h"
#include "afxdialogex.h"


// CDialogFilterKind �Ի���

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


// CDialogFilterKind ��Ϣ�������


void CDialogFilterKind::OnBnClickedButtonOk()
{
    using namespace std;
    m_vKinds.clear();
    // ����ѡ����
    for(int i = 0; i < m_lst.GetItemCount(); ++i) {
        if(m_lst.GetCheck(i)) {
            int id = (int)m_lst.GetItemData(i);
            wstring name = m_lst.GetItemText(i, 1).GetString();
            m_vKinds.push_back(make_tuple(id, name));
        }
    }
    // �˳�
    OnOK();
}

BOOL CDialogFilterKind::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //ѡ���� ��������
        dwStyle |= LVS_EX_GRIDLINES; //������
        dwStyle |= LVS_EX_CHECKBOXES; //CheckBox
        m_lst.SetExtendedStyle(dwStyle);

        // ������,�����ô�С
        {
            CRect rc;
            m_lst.GetClientRect(rc);

            m_lst.InsertColumn(0, _T("����"), LVCFMT_LEFT, 40);
            m_lst.InsertColumn(1, _T("����"), LVCFMT_LEFT, rc.Width() - 40);
        }
    }

    // ��ʾ����
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
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogFilterKind::OnBnClickedButtonCancel2()
{
    OnCancel();
}
