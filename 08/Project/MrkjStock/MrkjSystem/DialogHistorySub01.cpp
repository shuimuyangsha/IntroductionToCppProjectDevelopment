// DialogHistorySub01.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHistorySub01.h"
#include "afxdialogex.h"

using namespace std;


// CDialogHistorySub01 �Ի���

IMPLEMENT_DYNAMIC(CDialogHistorySub01, CDialogEx)

CDialogHistorySub01::CDialogHistorySub01(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_HISTORY_SUB_01, pParent)
{
}

CDialogHistorySub01::~CDialogHistorySub01()
{
}

void CDialogHistorySub01::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP(CDialogHistorySub01, CDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogHistorySub01 ��Ϣ�������

BOOL CDialogHistorySub01::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //ѡ���� ��������
        dwStyle |= LVS_EX_GRIDLINES; //������
        m_lst.SetExtendedStyle(dwStyle);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogHistorySub01::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}

void CDialogHistorySub01::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}

// ����m_lst�е�����
void CDialogHistorySub01::UpdateListData(const VLineData& v)
{
    m_lst.SetRedraw(FALSE);
    m_lst.DeleteAllItems();
    for each(auto data in v) {
        int index = m_lst.GetItemCount();
        m_lst.InsertItem(index, get<EListIndexCode>(data));
        m_lst.SetItemText(index, EListIndexName, get<EListIndexName>(data));
        m_lst.SetItemText(index, EListIndexUpPercent, get<EListIndexUpPercent>(data));
        m_lst.SetItemText(index, EListIndexPrice, get<EListIndexPrice>(data));
        m_lst.SetItemText(index, EListIndexMax, get<EListIndexMax>(data));
        m_lst.SetItemText(index, EListIndexOpen, get<EListIndexOpen>(data));
        m_lst.SetItemText(index, EListIndexClose, get<EListIndexClose>(data));
        m_lst.SetItemText(index, EListIndexMin, get<EListIndexMin>(data));
        m_lst.SetItemText(index, EListIndexTurnover, get<EListIndexTurnover>(data));
        m_lst.SetItemText(index, EListIndexTradingVolume, get<EListIndexTradingVolume>(data));
    }
    m_lst.SetRedraw(TRUE);
}

// ���ĳ������
void CDialogHistorySub01::GetListData(TupleLineData& data, int index)
{
    assert(index >= 0 && index < m_lst.GetItemCount());
    UpdateData(TRUE);
    data = make_tuple(
               m_lst.GetItemText(index, EListIndexCode),
               m_lst.GetItemText(index, EListIndexName),
               m_lst.GetItemText(index, EListIndexUpPercent),
               m_lst.GetItemText(index, EListIndexPrice),
               m_lst.GetItemText(index, EListIndexMax),
               m_lst.GetItemText(index, EListIndexOpen),
               m_lst.GetItemText(index, EListIndexClose),
               m_lst.GetItemText(index, EListIndexMin),
               m_lst.GetItemText(index, EListIndexTurnover),
               m_lst.GetItemText(index, EListIndexTradingVolume)
           );
}

//�ṩ��������е��õĴ���
void CDialogHistorySub01::UpdateStockData(const CString& stockCode, const CString& strDateStart, const CString strDateEnd)
{
    // ��ѯ����
    CMRKJDatabase::TupleStockInfo info;
    VDayDataEntry v;
    if(!DB.QueryStockInfoByCode(info, stockCode)) {
        return;
    }
    if(!DB.QueryDayData(v, stockCode, strDateStart, strDateEnd)) {
        return;
    }
    // ��ʾ
    VLineData vv;
    for(auto data : v) {
        vv.push_back(
            make_tuple(
                get<0>(info).c_str(), // ����
                get<1>(info).c_str(), // ����
                get<3>(info).c_str(), // ����
                data.date.c_str(),
                data.max.c_str(),
                data.open.c_str(),
                data.close.c_str(),
                data.min.c_str(),
                data.turnover.c_str(),
                data.tradingVolume.c_str()
            )
        );
    }
    UpdateListData(vv);
}

void CDialogHistorySub01::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if(this->GetSafeHwnd() && m_lst.GetSafeHwnd()) {
        static bool bFirst = true;
        if(bFirst) {
            bFirst = false;
        }
        CRect rcCli;
        GetClientRect(rcCli);
        const int x_off = 1;
        m_lst.MoveWindow(0 + x_off, 0, rcCli.Width() - 2 * x_off, rcCli.Height());
        // ������,�����ô�С
        {
            CRect rc;
            m_lst.GetClientRect(rc);
            m_lst.InsertColumn(EListIndexCode, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexName, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexUpPercent, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexPrice, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexOpen, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexClose, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexMax, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexMin, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexTurnover, _T("�ɽ���(��)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexTradingVolume, _T("�ɽ���(Ԫ)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            //m_lst.SetBkColor(RGB(0, 0, 0));
        }
    }
}


BOOL CDialogHistorySub01::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
