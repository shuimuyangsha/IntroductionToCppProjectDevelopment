// DialogHuShen.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHuShen.h"
#include "afxdialogex.h"
#include "HttpDataSource.h"
#include "HttpDataEntry.h"

#include "DialogStockFilter.h"

// CDialogHuShen 对话框

IMPLEMENT_DYNAMIC(CDialogHuShen, CDialogEx)

CDialogHuShen::CDialogHuShen(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_HUSHEN, pParent)
{

}

CDialogHuShen::~CDialogHuShen()
{
}

void CDialogHuShen::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP(CDialogHuShen, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_FILTER, &CDialogHuShen::OnBnClickedButtonFilter)
    ON_BN_CLICKED(IDC_BUTTON_ALL, &CDialogHuShen::OnBnClickedButtonAll)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_FILTER, &CDialogHuShen::OnBnClickedButtonShowFilter)
END_MESSAGE_MAP()


// CDialogHuShen 消息处理程序
BOOL CDialogHuShen::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
        //dwStyle |= LVS_EX_GRIDLINES; //网络线
        m_lst.SetExtendedStyle(dwStyle);

        CRect rcCli;
        GetClientRect(rcCli);
        const int x_off = 1;
        m_lst.MoveWindow(0 + x_off, +24, rcCli.Width() - 2 * x_off, rcCli.Height());
        // 设置列,并设置大小
        {
            CRect rc;
            m_lst.GetClientRect(rc);

            m_lst.InsertColumn(EListIndexNo, _T("序列"), LVCFMT_LEFT, 40);
            m_lst.InsertColumn(EListIndexCode, _T("代码"), LVCFMT_LEFT, 60);
            m_lst.InsertColumn(EListIndexName, _T("名称"), LVCFMT_LEFT, 70);
            m_lst.InsertColumn(EListIndexZhangFu, _T("涨幅%"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexXianJian, _T("现价"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZhangDie,  _T("涨跌"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexBuy, _T("买价"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexSell, _T("卖价"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZongLiang,   _T("总量"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexOpen, _T("今开"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexHigh, _T("最高"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexLow, _T("最低"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZuoShou, _T("昨收"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZongJinE, _T("总金额"), LVCFMT_RIGHT, 180);

            // 设置背景色
            m_lst.SetBkColor(RGB(0, 0, 0));

            m_lst.SetOnePageCount(MaxCountOnePage);
            m_lst.SetPage(1);
            LoadData(1);

            // 启动线程，更新实时数据
            StartUpdate();
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogHuShen::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}

void CDialogHuShen::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}

void CDialogHuShen::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
}

BOOL CDialogHuShen::PreTranslateMessage(MSG *pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYDOWN) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    if(pMsg->message == WM_MOUSEWHEEL) {
        int zDelta = GET_WHEEL_DELTA_WPARAM(pMsg->wParam);
        zDelta /= 120;
        zDelta *= -1;
        int iPage = m_lst.GetPage();
        LoadData(iPage + zDelta);
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

// 载入数据
void CDialogHuShen::LoadData(int iPage)
{
    switch(m_status) {
        case CDialogHuShen::EAll: {
            // 如果页码小于0，始终显示第一页
            if(iPage <= 0) {
                iPage = 1;
            }
            // 查询本页的内容
            std::vector<CMRKJDatabase::TupleStockInfo> v;
            if(DB.QueryStockInfo(v, iPage, MaxCountOnePage) && !v.empty()) {
                // 记录当前页
                m_lst.SetPage(iPage);

                // 防止闪烁
                m_lst.SetRedraw(FALSE);

                // 删除原来的内容
                m_lst.DeleteAllItems();

                // 插入新内容
                for(unsigned i = 0; i < v.size(); ++i) {
                    int iNo = 1 + i + (iPage - 1) * MaxCountOnePage;
                    CString str;
                    str.Format(_T("%d"), iNo);
                    int index = m_lst.InsertItem(m_lst.GetItemCount(), str, RGB(0xFF, 0xFF, 0xFF));

                    auto obj = v[i];
                    m_lst.SetItemText(index, EListIndexCode, std::get<0>(obj).c_str(), RGB(0x88, 0xF8, 0xF0));
                    m_lst.SetItemText(index, EListIndexName, std::get<1>(obj).c_str(), RGB(0xF0, 0xF8, 0x88));
                }
                m_lst.SetRedraw();

            }
            else {
                // 显示第一页
                m_lst.SetPage(1);
                LoadData(1);
            }
            break;
        }
        case CDialogHuShen::EFilter: {
            // 如果页码小于0，始终显示第一页
            if(iPage <= 0) {
                iPage = 1;
            }

            // 获取本页的代码
            using namespace std;
            vector<wstring> vCodes;
            {
                for(int i = (iPage - 1) * MaxCountOnePage;
                    i < m_vFilterStocks.size() && i < iPage * MaxCountOnePage;
                    ++i) {
                    vCodes.push_back(m_vFilterStocks[i]);
                }
                if(iPage == 1) {
                    vCodes.push_back(L"000001");
                    vCodes.push_back(L"000002");
                    vCodes.push_back(L"000003");
                    vCodes.push_back(L"000008");
                    vCodes.push_back(L"000010");
                }
            }

            // 查询本页的内容
            std::vector<CMRKJDatabase::TupleStockInfo> v;
            if(!vCodes.empty() && DB.QueryStockInfo(v, vCodes) && !v.empty()) {
                // 记录当前页
                m_lst.SetPage(iPage);

                // 防止闪烁
                m_lst.SetRedraw(FALSE);

                // 删除原来的内容
                m_lst.DeleteAllItems();

                // 插入新内容
                for(unsigned i = 0; i < v.size(); ++i) {
                    int iNo = 1 + i + (iPage - 1) * MaxCountOnePage;
                    CString str;
                    str.Format(_T("%d"), iNo);
                    int index = m_lst.InsertItem(m_lst.GetItemCount(), str, RGB(0xFF, 0xFF, 0xFF));

                    auto obj = v[i];
                    m_lst.SetItemText(index, EListIndexCode, std::get<0>(obj).c_str(), RGB(0x88, 0xF8, 0xF0));
                    m_lst.SetItemText(index, EListIndexName, std::get<1>(obj).c_str(), RGB(0xF0, 0xF8, 0x88));
                }
                m_lst.SetRedraw();
            }
            else {
                // 显示第一页
                m_lst.SetPage(1);
                LoadData(1);
            }
            break;
        }
        default:
            break;
    }
}

// 启动更新实时数据的线程
void CDialogHuShen::StartUpdate()
{
    if(!m_hThread) {
        m_hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadRoutine, this, 0, &m_threadID);
    }
}

// 停止更新实时数据的线程
void CDialogHuShen::StopUpdate()
{
    if(m_hThread) {
        ::TerminateThread(m_hThread, 0);
        m_hThread = NULL;
    }
}


// 线程函数
unsigned __stdcall CDialogHuShen::ThreadRoutine(PVOID pArgs)
{
    CDialogHuShen *pThis = (CDialogHuShen *)pArgs;
    // 查询辅助工具
    data_http::CHttpDataSource http;
    // 查到的数据
    data_http::CHttpDataEntry httpData;

    // 字符串转成 double
    auto GetDouble = [](std::string const & value)->float {
        std::stringstream ss(value);
        double d;
        ss >> d;
        return (float)d;
    };

    // 判断颜色值
    auto GetColor = [](std::string const & value, std::string const & sClose) {
        // 涨，跌，持平的颜色
        COLORREF clrUp = RGB(0xFF, 0, 0);
        COLORREF clrDown = RGB(0, 0xFF, 0);
        COLORREF clrKeep = RGB(0xFF, 0xFF, 0xFF);
        // 现价
        double d, close;
        {
            stringstream ss(value);
            ss >> d;
        }

        {
            stringstream ss(sClose);
            ss >> close;
        }

        // 判断颜色
        if(value == sClose) {
            return clrKeep;
        }
        else if(d > close) {
            return clrUp;
        }
        else {
            return clrDown;
        }
    };

    // 获取查询数据需要的 CODE(以sh或sz开头)
    auto GetQueryCode = [](CString const & code)->std::string{
        // 以 '6'开头的为上证
        if(code[0] == L'6')
        {
            return StringHelper::UnicodeToANSI(_T("sh") + code);
        }
        // 否则为 深证
        else {
            return StringHelper::UnicodeToANSI(_T("sz") + code);
        }
    };

    while(1) {
        // 每秒钟更新一次
        Sleep(1);
        try {
            int count = pThis->m_lst.GetItemCount();
            for(int index = 0; index < count; ++index) {
                CString code = pThis->m_lst.GetItemText(index, EListIndexCode);

                // 拼接查询时用到的URL
                std::string queryCode = GetQueryCode(code);

                //请求数据
                if(!http.QueryStockRealTimeData(queryCode, httpData)) {
                    continue;
                }

                // 设置窗口数据
                {
                    // 计算昨日收盘价，方便比较涨跌
                    double close = GetDouble(httpData.close);

                    {
                        // 涨跌值
                        double d1 = (GetDouble(httpData.price) - close);
                        // 涨幅:(现价-昨收) / 昨收
                        double d2 = d1 / close * 100;

                        CString str;
                        str.Format(_T("%.2f"), d2);
                        COLORREF clr = GetColor(httpData.price, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexZhangFu, str, clr);
                        // 涨跌值
                        str.Format(_T("%.2f"),  d1);
                        pThis->m_lst.SetItemText(index, EListIndexZhangDie, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.price));
                        COLORREF clr = GetColor(httpData.price, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexXianJian, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.buyPrice01));
                        COLORREF clr = GetColor(httpData.buyPrice01, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexBuy, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.sellPrice01));
                        COLORREF clr = GetColor(httpData.sellPrice01, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexSell, str, clr);
                    }

                    {
                        pThis->m_lst.SetItemText(index, EListIndexZongLiang,
                                                 StringHelper::ANSIToUnicode(httpData.dealCount).c_str(),
                                                 RGB(0xF0, 0xF8, 0x88));
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.open));
                        COLORREF clr = GetColor(httpData.open, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexOpen, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.maxPrice));
                        COLORREF clr = GetColor(httpData.maxPrice, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexHigh, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.minPrice));
                        COLORREF clr = GetColor(httpData.minPrice, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexLow, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.close));
                        COLORREF clr = RGB(0xFF, 0xFF, 0xFF);
                        pThis->m_lst.SetItemText(index, EListIndexZuoShou, str, clr);
                    }

                    {
                        CString str;
                        str.Format(_T("%.2f"), GetDouble(httpData.dealMoney));
                        COLORREF clr = RGB(0xFF, 0xFF, 0xFF);
                        pThis->m_lst.SetItemText(index, EListIndexZongJinE, str, clr);
                    }
                }
            }
        }
        catch(...) {
            TRACE("更新数据异常");
        }
    }
    _endthreadex(0);
    return 0;
}

void CDialogHuShen::OnBnClickedButtonFilter()
{
    CDialogStockFilter dlg;
    if(IDOK != dlg.DoModal()) {
        return;
    }
    using namespace std;
    // 把结果拿过来
    m_vFilterStocks.clear();
    copy(dlg.m_vFilterStocks.begin(), dlg.m_vFilterStocks.end(), 
         back_inserter<vector<wstring>>(m_vFilterStocks));
    m_status = EFilter;
    LoadData(1);
}


void CDialogHuShen::OnBnClickedButtonAll()
{
    m_status = EAll;
    m_lst.SetPage(1);
    LoadData(1);
}


void CDialogHuShen::OnBnClickedButtonShowFilter()
{
    m_status = EFilter;
    m_lst.SetPage(1);
    LoadData(1);
}
