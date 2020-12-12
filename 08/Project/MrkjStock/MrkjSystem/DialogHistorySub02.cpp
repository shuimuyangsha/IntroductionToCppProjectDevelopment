// DialogHistorySub02.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHistorySub02.h"
#include "afxdialogex.h"

// CDialogHistorySub02 �Ի���

IMPLEMENT_DYNAMIC(CDialogHistorySub02, CDialogEx)

CDialogHistorySub02::CDialogHistorySub02(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_HISTORY_SUB_02, pParent)
{
}

CDialogHistorySub02::~CDialogHistorySub02()
{
}

void CDialogHistorySub02::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHART, m_stockChart);
}


BEGIN_MESSAGE_MAP(CDialogHistorySub02, CDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogHistorySub02 ��Ϣ�������


BOOL CDialogHistorySub02::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogHistorySub02::OnOK()
{
    CDialogEx::OnOK();
}

void CDialogHistorySub02::OnCancel()
{
    CDialogEx::OnCancel();
}

void CDialogHistorySub02::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if(m_stockChart.GetSafeHwnd()) {
        CRect rcCli;
        GetClientRect(rcCli);
        m_stockChart.MoveWindow(rcCli);
    }
}

void CDialogHistorySub02::UpdateKLine(const CString &strCode,
                                      const CString &strDateStart,
                                      const CString &strDateEnd)
{
    using namespace std;
    // ȡ����
    CMRKJDatabase::TupleStockInfo info;
    // �����ݿ���ȡ��Ʊ��Ϣ,�����ֵ�.
    if(!DB.QueryStockInfoByCode(info, strCode)) {
        CTools::MessageBoxFormat(_T("û�й�Ʊ��Ϣ[%s]"), strCode.GetString());
        return;
    }

    // ��ѯ��������
    VDayDataEntry v;
    if(!DB.QueryDayData(v, strCode, strDateStart, strDateEnd)) {
        CTools::MessageBoxFormat(_T("��Ʊû������[%s]"), strCode.GetString());
        return;
    }
    // ���û������,������ʾ
    if(v.empty()) {
        CTools::MessageBoxFormat(_T("��Ʊû������[%s]"), strCode.GetString());
        return;
    }
    // �����ݴ�������ؼ�
    m_stockChart.ReSetData(info, v);
    // ������
    InvalidateRect(NULL);
}


BOOL CDialogHistorySub02::PreTranslateMessage(MSG *pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
