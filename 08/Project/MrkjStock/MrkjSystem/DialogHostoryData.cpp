// DialogHostoryData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHostoryData.h"
#include "afxdialogex.h"


// CDialogHostoryData �Ի���

IMPLEMENT_DYNAMIC(CDialogHostoryData, CDialogEx)

CDialogHostoryData::CDialogHostoryData(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_HOSTORY_DATA, pParent)
#ifdef DEBUG
    , m_strCode(_T("000001"))
    , m_odtDateStart(COleDateTime(2011, 10, 10, 0, 0, 0))
    , m_odtDateEnd(COleDateTime(2012, 10, 10, 0, 0, 0))
#else
    , m_strCode(_T(""))
    , m_odtDateStart(COleDateTime::GetCurrentTime() - COleDateTimeSpan(365, 0, 0, 0))
    , m_odtDateEnd(COleDateTime::GetCurrentTime())
#endif
{
}

CDialogHostoryData::~CDialogHostoryData()
{
    SAFE_DELETE(m_pDlg01);
    SAFE_DELETE(m_pDlg02);
}

void CDialogHostoryData::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB1, m_tab);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_odtDateStart);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_odtDateEnd);
    DDX_CBString(pDX, IDC_COMBO2, m_strCode);
    DDX_Control(pDX, IDC_COMBO2, m_cbbStockCode);
}


BEGIN_MESSAGE_MAP(CDialogHostoryData, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogHostoryData::OnBnClickedButtonQuery)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDialogHostoryData::OnTcnSelchangeTab1)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogHostoryData ��Ϣ�������


void CDialogHostoryData::OnBnClickedButtonQuery()
{
    UpdateData(TRUE);
    CWnd* pBtn = GetDlgItem(IDC_BUTTON1);
    assert(pBtn);
    // ʹ��ť ������
    pBtn->EnableWindow(FALSE);
    //�Ȼ��ѡ�е�����
    GetSelStockCode(m_strCode);
    //����code��ѯ
    m_pDlg01->UpdateStockData(m_strCode, m_odtDateStart.Format(_T("%Y/%m/%d")), m_odtDateEnd.Format(_T("%Y/%m/%d")));
    m_pDlg02->UpdateKLine(m_strCode, m_odtDateStart.Format(_T("%Y/%m/%d")), m_odtDateEnd.Format(_T("%Y/%m/%d")));
    // ʹ��ť����
    pBtn->EnableWindow(TRUE);
    UpdateData(FALSE);
}

// ���ѡ�еĹ�Ʊ����
bool CDialogHostoryData::GetSelStockCode(CString& strCode)
{
    int index = m_cbbStockCode.GetCurSel();
    if(index < 0) {
        return false;
    }
    auto *p = (PINFO)m_cbbStockCode.GetItemData(index);
    if(!p) {
        return false;
    }
    strCode = get<0>(*p).c_str();
    return true;
}

// ����ѡ����
void CDialogHostoryData::SetSelByStockCode(const CString& strCode)
{
    for(int i = 0; i < m_cbbStockCode.GetCount(); ++i) {
        auto *p = (PINFO)m_cbbStockCode.GetItemData(i);
        if(!p) {
            continue;
        }
        CString str = get<0>(*p).c_str();
        if(strCode == str) {
            m_cbbStockCode.SetCurSel(i);
            break;
        }
    }
}


BOOL CDialogHostoryData::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        // �ͻ�����С
        CRect rcCli;
        GetClientRect(rcCli);
        CRect rcHeader;
        GetDlgItem(IDC_STATIC_HEADER)->GetWindowRect(rcHeader);
        // ��Ʊ����
        {
            UpdateStockInfoComboBox();
        }
        // TAB
        {
            m_tab.InsertItem(0, _T("���ݷ���"));
            m_tab.InsertItem(1, _T("K �� ͼ"));
            CRect rcTab;
            m_tab.GetClientRect(rcTab);
            m_pDlg01->Create(IDD_DIALOG_HISTORY_SUB_01, &m_tab);
            m_pDlg02->Create(IDD_DIALOG_HISTORY_SUB_02, &m_tab);
            const int iTabHeaderHi = 22;
            m_pDlg01->MoveWindow(0, 0 + iTabHeaderHi, rcTab.Width(), rcTab.Height() - iTabHeaderHi);
            m_pDlg02->MoveWindow(0, 0 + iTabHeaderHi, rcTab.Width(), rcTab.Height() - iTabHeaderHi);
            m_pDlg01->ShowWindow(SW_SHOW);
            m_pDlg02->ShowWindow(SW_HIDE);
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogHostoryData::OnOK()
{
    ClearComboBox();
    CDialogEx::OnOK();
}


void CDialogHostoryData::OnCancel()
{
    ClearComboBox();
    CDialogEx::OnCancel();
}

// ��������
void CDialogHostoryData::ClearComboBox()
{
    // ��������
    for(int index = 0; index < m_cbbStockCode.GetCount(); ++index) {
        auto ptr = (PINFO)m_cbbStockCode.GetItemData(index);
        SAFE_DELETE(ptr);
    }
    // ����ѡ��
    m_cbbStockCode.ResetContent();
}


void CDialogHostoryData::UpdateStockInfoComboBox()
{
    UpdateData(TRUE);
    //���浱ǰѡ��
    CString strOldCode;
    if(m_cbbStockCode.GetCurSel() >= 0) {
        auto p = (PINFO)m_cbbStockCode.GetItemData(m_cbbStockCode.GetCurSel());
        if(p) {
            strOldCode = get<0>(*p).c_str();
        }
    }
    {
		m_cbbStockCode.SetRedraw(FALSE);
        // ��������
        ClearComboBox();
        // ��ѯ����
        vector<INFO> v;
        if(DB.QueryStockInfo(v)) {
            for(auto info : v) {
                CString str;
                str.Format(_T("%s - %-10s - %-10s")
                           , get<0>(info).c_str()
                           , get<1>(info).c_str()
                           , get<3>(info).c_str());
                int index = m_cbbStockCode.AddString(str);
                m_cbbStockCode.SetItemData(index, (DWORD_PTR)(new INFO(info)));
            }
        }
		m_cbbStockCode.SetRedraw(TRUE);
    }
    //�ָ���ǰѡ��
    if(!strOldCode.IsEmpty()) {
        for(int i = 0; i < m_cbbStockCode.GetCount(); ++i) {
            auto p = (PINFO)m_cbbStockCode.GetItemData(i);
            if(p && strOldCode == get<0>(*p).c_str()) {
                m_cbbStockCode.SetCurSel(i);
                break;
            }
        }
    }
    UpdateData(FALSE);
}

void CDialogHostoryData::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
    int idx = m_tab.GetCurSel();
    if(idx == 0) {
        m_pDlg01->ShowWindow(TRUE);
        m_pDlg02->ShowWindow(FALSE);
    }
    else if(idx == 1) {
        m_pDlg01->ShowWindow(FALSE);
        m_pDlg02->ShowWindow(TRUE);
    }
    else {
        TRACE("Ϊʲô????");
        assert(false);
    }
    *pResult = 0;
}

void CDialogHostoryData::SetDataAndUpdate(const CString& stockCode, const COleDateTime& odtDateStart, const COleDateTime& odtDateEnd)
{
    UpdateData(TRUE);
    m_strCode = stockCode;
    SetSelByStockCode(stockCode);
    m_odtDateStart = odtDateStart;
    m_odtDateEnd = odtDateEnd;
    UpdateData(FALSE);
    //�����ѯ��ť,�������� Tabҳ������
    OnBnClickedButtonQuery();
}

void CDialogHostoryData::OnDestroy()
{
    ClearComboBox();
    CDialogEx::OnDestroy();
}


BOOL CDialogHostoryData::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
