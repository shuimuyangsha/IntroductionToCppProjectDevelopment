// DialogHistorySub01.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHistorySub01.h"
#include "afxdialogex.h"

using namespace std;


// CDialogHistorySub01 对话框

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


// CDialogHistorySub01 消息处理程序

BOOL CDialogHistorySub01::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
        dwStyle |= LVS_EX_GRIDLINES; //网络线
        m_lst.SetExtendedStyle(dwStyle);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogHistorySub01::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}

void CDialogHistorySub01::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}

// 更新m_lst中的数据
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

// 获得某行数据
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

//提供给父类进行调用的窗口
void CDialogHistorySub01::UpdateStockData(const CString& stockCode, const CString& strDateStart, const CString strDateEnd)
{
    // 查询数据
    CMRKJDatabase::TupleStockInfo info;
    VDayDataEntry v;
    if(!DB.QueryStockInfoByCode(info, stockCode)) {
        return;
    }
    if(!DB.QueryDayData(v, stockCode, strDateStart, strDateEnd)) {
        return;
    }
    // 显示
    VLineData vv;
    for(auto data : v) {
        vv.push_back(
            make_tuple(
                get<0>(info).c_str(), // 代码
                get<1>(info).c_str(), // 名称
                get<3>(info).c_str(), // 种类
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
        // 设置列,并设置大小
        {
            CRect rc;
            m_lst.GetClientRect(rc);
            m_lst.InsertColumn(EListIndexCode, _T("代码"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexName, _T("名称"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexUpPercent, _T("类别"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexPrice, _T("日期"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexOpen, _T("开盘"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexClose, _T("收盘"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexMax, _T("最高"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexMin, _T("最低"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexTurnover, _T("成交量(股)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            m_lst.InsertColumn(EListIndexTradingVolume, _T("成交额(元)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            //m_lst.SetBkColor(RGB(0, 0, 0));
        }
    }
}


BOOL CDialogHistorySub01::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
