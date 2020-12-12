// DialogKeybdQuery.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogKeybdQuery.h"
#include "afxdialogex.h"
#include "MRKJDatabase.h"

// CDialogKeybdQuery �Ի���

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


// CDialogKeybdQuery ��Ϣ�������


BOOL CDialogKeybdQuery::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ���õ�һ����ĸ
    SetDlgItemText(IDC_EDIT_QUERY_INPUT, m_strQueryInput.GetString());
    // ������ʾλ��
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
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogKeybdQuery::OnEnChangeEditQueryInput()
{
    // ����ǰ������
    CString strInput;
    m_edtQueryInput.GetWindowTextW(strInput);
    if(strInput.GetLength() <= 0) {
        return;
    }
    // ��ѯ���
    typedef vector<CMRKJDatabase::TupleStockInfo> VStockInfo;
    VStockInfo v;
    DB.QueryStockInfoFuzzy(v, strInput);
    //�Ѳ�ѯ������µ�������б��
    CListBox& res = m_lstBoxQueryResult;
    //res.LockWindowUpdate();
    res.SetRedraw(FALSE);
    res.ResetContent();
    for each(auto s in v) {
        auto ss = get<0>(s) + L"  " + get<1>(s);
        res.AddString(ss.c_str());
    }
    //ѡ���б���е�һ��
    if(res.GetCount() > 0) {
        res.SetCurSel(0);
    }
    res.SetRedraw(TRUE);
    //res.UnlockWindowUpdate();
}

// ˫�� listbox�¼�
void CDialogKeybdQuery::OnLbnDblclkListQueryResult()
{
    int index = m_lstBoxQueryResult.GetCurSel();
    if(index >= 0) {
        m_lstBoxQueryResult.GetText(index, m_strQueryResultSel);
    }
    this->OnOK();
}

// �� ����� ��ý���ʱ,ȡ��ѡ��
void CDialogKeybdQuery::OnEnSetfocusEditQueryInput()
{
    m_edtQueryInput.SetSel(-1, 0, FALSE);
}


BOOL CDialogKeybdQuery::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        // �س���ʱ,�൱��ѡ����һ�н��,������OK
        {
            if(pMsg->wParam == VK_RETURN) {
                OnLbnDblclkListQueryResult();
            }
        }
        // ����Ĵ���,ʹ���ؼ�������ʱ, ���԰����¼�����ѡ��
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
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}


void CDialogKeybdQuery::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}
