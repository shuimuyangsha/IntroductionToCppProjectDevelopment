// DialogHistorySub02.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHistorySub02.h"
#include "afxdialogex.h"

// CDialogHistorySub02 对话框

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


// CDialogHistorySub02 消息处理程序


BOOL CDialogHistorySub02::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
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
    // 取数据
    CMRKJDatabase::TupleStockInfo info;
    // 从数据库中取股票信息,如名字等.
    if(!DB.QueryStockInfoByCode(info, strCode)) {
        CTools::MessageBoxFormat(_T("没有股票信息[%s]"), strCode.GetString());
        return;
    }

    // 查询日线数据
    VDayDataEntry v;
    if(!DB.QueryDayData(v, strCode, strDateStart, strDateEnd)) {
        CTools::MessageBoxFormat(_T("股票没有数据[%s]"), strCode.GetString());
        return;
    }
    // 如果没有数据,进行提示
    if(v.empty()) {
        CTools::MessageBoxFormat(_T("股票没有数据[%s]"), strCode.GetString());
        return;
    }
    // 将数据传给报表控件
    m_stockChart.ReSetData(info, v);
    // 并更新
    InvalidateRect(NULL);
}


BOOL CDialogHistorySub02::PreTranslateMessage(MSG *pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
