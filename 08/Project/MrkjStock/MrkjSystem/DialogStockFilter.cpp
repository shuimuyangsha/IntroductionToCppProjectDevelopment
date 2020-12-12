// DialogStockFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogStockFilter.h"
#include "afxdialogex.h"
#include "DialogFilterKind.h"


#define WM_USER_OTHER_THREAD (WM_USER + 1)

// CDialogStockFilter 对话框

IMPLEMENT_DYNAMIC(CDialogStockFilter, CDialogEx)

CDialogStockFilter::CDialogStockFilter(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_STOCK_FILTER, pParent)
{

}

CDialogStockFilter::~CDialogStockFilter()
{
    DoFilter(FALSE);
}

void CDialogStockFilter::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_FILTER, m_comboFilter);
    DDX_Control(pDX, IDC_LIST_FILTER_CURR, m_listFilters);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtStart);
    DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtEnd);
    DDX_Control(pDX, IDC_PROGRESS1, m_process);
    DDX_Control(pDX, IDC_LIST_FILTER_KIND, m_lstBoxKinds);
}


BEGIN_MESSAGE_MAP(CDialogStockFilter, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_FILTER, &CDialogStockFilter::OnCbnSelchangeComboFilter)
    ON_BN_CLICKED(IDC_BUTTON_FILTER_ADD, &CDialogStockFilter::OnBnClickedButtonFilterAdd)
    ON_BN_CLICKED(IDC_BUTTON_FILTER_DEL, &CDialogStockFilter::OnBnClickedButtonFilterDel)
    ON_BN_CLICKED(IDC_BUTTON_DO, &CDialogStockFilter::OnBnClickedButtonDo)
    ON_MESSAGE(WM_USER_OTHER_THREAD, &CDialogStockFilter::OnOtherThreadMessage)
    ON_BN_CLICKED(IDC_BUTTON_OK, &CDialogStockFilter::OnBnClickedButtonOk)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDialogStockFilter::OnBnClickedButtonCancel)
    ON_BN_CLICKED(IDC_BUTTON_FILTER_ROUND, &CDialogStockFilter::OnBnClickedButtonFilterRound)
END_MESSAGE_MAP()


// CDialogStockFilter 消息处理程序


BOOL CDialogStockFilter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    {
        // 获中得参数框的位置
        CRect rc;
        {
            CStatic *ps = (CStatic *)GetDlgItem(IDC_STATIC_PARA);
            ps->GetWindowRect(rc);
            ScreenToClient(rc);
        }

        {
            m_comboFilter.AddString(_T("UPN                 - 连涨数天"));

            CRect r = rc;
            r.left += 10;
            r.top += 20;
            r.right = r.left + 30;
            r.bottom = r.top + 20;

            m_stt_upn_days0.Create(_T("连涨"), WS_CHILD | WS_VISIBLE | SS_CENTER, r, this, IDC_EDIT_UPN_DAYS);
            m_stt_upn_days0.SetFont(this->GetFont());

            r.left = r.right + 10;
            r.right = r.left + 80;
            m_edt_upn_days.Create(ES_RIGHT | ES_NUMBER, r, this, IDC_STATIC_UPN_DAYS0);
            m_edt_upn_days.SetWindowText(_T("3"));

            r.left = r.right + 10;
            r.right = r.left + 20;
            m_stt_upn_days1.Create(_T("天"), SS_LEFT, r, this, IDC_STATIC_UPN_DAYS1);
            m_stt_upn_days1.SetFont(this->GetFont());

            UPN(TRUE);
        }

        {
            m_comboFilter.AddString(_T("DOWNN             - 连跌数天"));

            CRect r = rc;
            r.left += 10;
            r.top += 20;
            r.right = r.left + 30;
            r.bottom = r.top + 20;
            m_stt_downn_days0.Create(_T("连跌"), SS_LEFT, r, this, IDC_EDIT_DOWNN_DAYS);
            m_stt_downn_days0.SetFont(this->GetFont());

            r.left = r.right + 10;
            r.right = r.left + 80;
            m_edt_downn_days.Create(ES_RIGHT | ES_NUMBER, r, this, IDC_STATIC_DOWNN_DAYS0);
            m_edt_downn_days.SetWindowText(_T("3"));

            r.left = r.right + 10;
            r.right = r.left + 20;
            m_stt_downn_days1.Create(_T("天"), SS_LEFT, r, this, IDC_STATIC_DOWNN_DAYS1);
            m_stt_downn_days1.SetFont(this->GetFont());
        }

        // 选中第一项
        m_comboFilter.SetCurSel(0);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


BOOL CDialogStockFilter::PreTranslateMessage(MSG *pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogStockFilter::UPN(BOOL bShow/* = TRUE*/)
{
    if(bShow) {
        m_edt_upn_days.ShowWindow(SW_SHOW);
        m_stt_upn_days0.ShowWindow(SW_SHOW);
        m_stt_upn_days1.ShowWindow(SW_SHOW);
    }
    else {
        m_edt_upn_days.ShowWindow(SW_HIDE);
        m_stt_upn_days0.ShowWindow(SW_HIDE);
        m_stt_upn_days1.ShowWindow(SW_HIDE);
    }
}

void CDialogStockFilter::DOWNN(BOOL bShow/* = TRUE*/)
{
    if(bShow) {
        m_edt_downn_days.ShowWindow(SW_SHOW);
        m_stt_downn_days0.ShowWindow(SW_SHOW);
        m_stt_downn_days1.ShowWindow(SW_SHOW);
    }
    else {
        m_edt_downn_days.ShowWindow(SW_HIDE);
        m_stt_downn_days0.ShowWindow(SW_HIDE);
        m_stt_downn_days1.ShowWindow(SW_HIDE);
    }
}

void CDialogStockFilter::OnCbnSelchangeComboFilter()
{
    int index = m_comboFilter.GetCurSel();
    switch(index) {
        case 0: {
            UPN(TRUE);
            DOWNN(FALSE);
            break;
        }
        case 1: {
            DOWNN(TRUE);
            UPN(FALSE);
            break;
        }
        default:
            break;
    }
}


void CDialogStockFilter::OnBnClickedButtonFilterAdd()
{
    int index = m_comboFilter.GetCurSel();
    switch(index) {
        case 0: {
            CString strFun, strPara;

            m_comboFilter.GetLBText(index, strFun);
            strFun = strFun.Mid(0, strFun.Find(_T('-')));
            strFun.Trim(_T(" "));

            m_edt_upn_days.GetWindowText(strPara);

            CString strFilter;
            strFilter.Format(_T("%s (%s)"), strFun.GetString(), strPara.GetString());
            int index = m_listFilters.AddString(strFilter);
            m_listFilters.SetCurSel(index);
            break;
        }
        case 1: {
            CString strFun, strPara;

            m_comboFilter.GetLBText(index, strFun);
            strFun = strFun.Mid(0, strFun.Find(_T('-')));
            strFun.Trim(_T(" "));

            m_edt_downn_days.GetWindowText(strPara);

            CString strFilter;
            strFilter.Format(_T("%s (%s)"), strFun.GetString(), strPara.GetString());
            int index = m_listFilters.AddString(strFilter);
            m_listFilters.SetCurSel(index);
            break;
        }
        default:
            break;
    }
}


void CDialogStockFilter::OnBnClickedButtonFilterDel()
{
    int index = m_listFilters.GetCurSel();
    if(index >= 0) {
        m_listFilters.DeleteString(index);
        if(m_listFilters.GetCount() > 0) {
            return;
        }
        while(index >= 0) {
            if(index < m_listFilters.GetCount()) {
                m_listFilters.SetCurSel(index);
                break;
            }
            else {
                index--;
            }
        }
    }
}


namespace filter_tread
{
    struct SFunc {
        CString strFunc;
        std::vector<CString> vParams;
    };

    struct {
        BOOL bAndOr; // TRUE:全部与,FALSE:全部或
        std::vector<SFunc> vFuncs; // 条件函数
        CString strDateStart, strDateEnd; // 日线数据日期区间
        std::vector<int> vKinds; // 选股范围
    } query_condition;

    unsigned __stdcall ThreadRoutine(PVOID pArgs);

    bool bAlive = false;
    unsigned threadID = 0;
    HANDLE hThread = INVALID_HANDLE_VALUE;
    HANDLE hEvent = INVALID_HANDLE_VALUE;

    // 开始查询线程
    void Start(CDialogStockFilter *pThis)
    {
        if(bAlive) {
            return;
        }
        if(INVALID_HANDLE_VALUE != hEvent) {
            return;
        }
        if(INVALID_HANDLE_VALUE != hThread) {
            return;
        }
        bAlive = true;
        hEvent = CreateEvent(NULL
                             , FALSE//自动重置
                             , FALSE // 初始是 无信号状态
                             , NULL);
        hThread = (HANDLE)(::_beginthreadex(nullptr, 0, ThreadRoutine, pThis, 0, &threadID));
    }

    // 停止查询线程
    void Stop(DWORD dwTimeout = 1000 * 3 + 200)
    {
        if(bAlive) {
            bAlive = false;
        }
        if(INVALID_HANDLE_VALUE != hEvent) {
            CloseHandle(hEvent);
            hEvent = INVALID_HANDLE_VALUE;
        }
        if(INVALID_HANDLE_VALUE != hThread) {
            WaitForSingleObject(hThread, dwTimeout);
            TerminateThread(hThread, 0);
            CloseHandle(hThread);
            hThread = INVALID_HANDLE_VALUE;
        }
    }

    // 符助函数 : 判断股票数据是否符合条件
    BOOL CheckStock(VDayDataEntry const &vdd, BOOL bAndOr, std::vector<SFunc> vFuncs)
    {
        // 是否满足连涨N天条件
        auto CheckUPN = [](VDayDataEntry const & vdd, int N)->BOOL {
            if(N <= 1)
            {
                return FALSE;
            }
            // 至少要有1+N天数据，才能判断是否连涨N天
            if(vdd.size() <= N)
            {
                return FALSE;
            }
            // 从最后一天向前判断是面连跌了N天
            int cnt = 0;
            for(unsigned i1 = vdd.size() - 1; i1 >= 0; --i1)
            {
                unsigned i0 = i1 - 1;
                // 转换成孚点数，比较大小
                wstringstream ss0(vdd[i0].close), ss1(vdd[i1].close);
                float d0, d1;
                ss0 >> d0, ss1 >> d1;
                // 如果前一天 >= 第二天，说明没有涨，则不符条件
                if(d0 >= d1) {
                    // 跳出，不必再继续判断了
                    break;
                }
                else {
                    cnt++;
                }
                if(cnt >= N) {
                    return TRUE;
                }
            }
            return cnt >= N;
        };
        // 是否满足连跌N天条件
        auto CheckDOWNN = [](VDayDataEntry const & vdd, int N)->BOOL {
            if(N <= 1)
            {
                return FALSE;
            }
            // 至少要有1+N天数据，才能判断是否连涨N天
            if(vdd.size() <= N)
            {
                return FALSE;
            }
            // 从最后一天向前判断是面连跌了N天
            int cnt = 0;
            for(unsigned i1 = vdd.size() - 1; i1 >= 0; --i1)
            {
                unsigned i0 = i1 - 1;
                // 转换成孚点数，比较大小
                wstringstream ss0(vdd[i0].close), ss1(vdd[i1].close);
                float d0, d1;
                ss0 >> d0, ss1 >> d1;
                // 如果前一天 < 第二天，说明涨了，则不符条件
                if(d0 < d1) {
                    // 跳出，不必再继续判断了
                    break;
                }
                else {
                    cnt++;
                }
                if(cnt >= N) {
                    return TRUE;
                }
            }
            return cnt >= N;
        };

        // 如果全部条件与 : 判数是否不符合条件，如果不符合,就提前跳出
        if(query_condition.bAndOr) {
            // 对当前股票数据执行过滤消息
            for(SFunc &fun : query_condition.vFuncs) {
                if(fun.strFunc == _T("UPN")) {
                    //取得参数,转化为天，该函数表示连涨 N 天
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("连涨%d天\r\n", N);
                    if(!CheckUPN(vdd, N)) {
                        return FALSE;
                    }
                }
                else if(fun.strFunc == _T("DOWNN")) {
                    //取得参数,转化为天，该函数表示连跌 N 天
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("连跌%d天\r\n", N);
                    if(!CheckDOWNN(vdd, N)) {
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }
        // 全部条件or : 判断是否符合条件，如果符合,就提前跳出
        else {
            // 对当前股票数据执行过滤消息
            for(SFunc &fun : query_condition.vFuncs) {
                if(fun.strFunc == _T("UPN")) {
                    //取得参数,转化为天，该函数表示连涨 N 天
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("连涨%d天\r\n", N);
                    if(CheckUPN(vdd, N)) {
                        return TRUE;
                    }
                }
                else if(fun.strFunc == _T("DOWNN")) {
                    //取得参数,转化为天，该函数表示连跌 N 天
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("连跌%d天\r\n", N);
                    if(!CheckDOWNN(vdd, N)) {
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }

    }

    // 查询线程:
    // 每3秒查询一次(或调用 FireEvent), 查询成功能后 发送消息
    unsigned __stdcall ThreadRoutine(PVOID pArgs)
    {
        // 获得参数
        auto *pThis = (CDialogStockFilter *)pArgs;

        // 遍历所有的股票，把条件应用到股票上面进行筛选
        std::vector<CMRKJDatabase::TupleStockInfo> v;
        DB.QueryStockInfo(v, query_condition.vKinds);
        ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 1, v.size());
        for(int i = 0; i < v.size() && pThis->m_bFilter && bAlive; ++i) {
            // 查询股票日线数据
            VDayDataEntry vdd;
            if(!DB.QueryDayData(vdd, std::get<0>(v[i]).c_str(), query_condition.strDateStart.GetString(), query_condition.strDateEnd.GetString())) {
                continue;
            }
            //过滤成功的加入到列表中
            if(CheckStock(vdd, query_condition.bAndOr, query_condition.vFuncs)) {
                // 步进 + 传递过滤成功的数据
                auto strCode = _wcsdup(std::get<0>(v[i]).c_str());
                ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 2, (LPARAM)strCode);
            }
            else {
                // 步进
                ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 2, 0);
            }
        }
        // 恢复按钮文字
        ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 3, 0);
        // 把结果传回
        _endthreadex(0);
        return 0;
    }
}


// 处理其它线要程的消息
LRESULT CDialogStockFilter::OnOtherThreadMessage(WPARAM wParam, LPARAM lParam)
{
    switch(wParam) {
        case 1: {
            m_process.SetRange(0, (int)lParam);
            m_process.SetStep(1);
            break;
        }
        case 2: {
            m_process.StepIt();
            auto strCode = (wchar_t *)lParam;
            if(strCode) {
                m_vFilterStocks.push_back(strCode);
                free(strCode);
                strCode = nullptr;
            }
            break;
        }
        case 3: {
            GetDlgItem(IDC_BUTTON_DO)->SetWindowText(_T("执行选股"));
            break;
        }
        default:
            break;
    }
    return 0;
}

BOOL CDialogStockFilter::DoFilter(BOOL bFilter)
{
    if(bFilter) {
        // 清空上次选股的结果
        m_vFilterStocks.clear();

        // 进度条放到初始位置
        m_process.SetPos(0);

        // 缩短一下命名空间,方便使用
        namespace t = filter_tread;

        // 取得条件：全部与？ or 全部或
        t::query_condition.bAndOr = (((CButton *)GetDlgItem(IDC_RADIO_ADN))->GetCheck() == BST_CHECKED);

        // 取得所有条件公式，并解析出来
        {
            auto &tmp = t::query_condition;
            tmp.vFuncs.clear();
            for(int index = 0; index < m_listFilters.GetCount(); ++index) {
                CString str;
                m_listFilters.GetText(index, str);
                // 拆分字符串
                int iStart = 0;
                // 函数名
                CString s = str.Tokenize(_T(" ()"), iStart);
                assert(!s.IsEmpty());
                t::SFunc fun;
                fun.strFunc = s;

                // 参数
                s = str.Tokenize(_T(" ()"), iStart);
                for(; !s.IsEmpty(); s = str.Tokenize(_T(" ()"), iStart)) {
                    fun.vParams.push_back(s);
                }
                // 存起来
                t::query_condition.vFuncs.push_back(fun);
            }

            if(tmp.vFuncs.empty()) {
                AfxMessageBox(_T("请至少选择一个选股公式"));
                return FALSE;
            }
        }

        // 获得日期区间
        {
            COleDateTime dtStart, dtEnd;
            m_dtStart.GetTime(dtStart);
            m_dtEnd.GetTime(dtEnd);
            // 获取日期字符串
            t::query_condition.strDateStart = dtStart.Format(_T("%Y/%m/%d"));
            t::query_condition.strDateEnd = dtEnd.Format(_T("%Y/%m/%d"));
        }

        // 获取选股范围
        {
            t::query_condition.vKinds.clear();
            GetKinds(t::query_condition.vKinds);
            if(t::query_condition.vKinds.empty()) {
                AfxMessageBox(_T("请选择选股范围"));
                return FALSE;
            }
        }

        // 标志正在选股
        m_bFilter = bFilter;
        // 启动线程
        filter_tread::Start(this);
        return TRUE;
    }
    else {
        // 停止选股线程
        filter_tread::Stop();
    }

    return m_bFilter;
}

void CDialogStockFilter::OnBnClickedButtonDo()
{
    CString str;
    auto pBtn = (CButton *)GetDlgItem(IDC_BUTTON_DO);
    pBtn->GetWindowText(str);
    if(str == _T("执行选股")) {
        if(DoFilter(TRUE)) {
            pBtn->SetWindowText(_T("停止"));
        }
    }
    else {
        DoFilter(FALSE);
        pBtn->SetWindowText(_T("执行选股"));
    }
}


void CDialogStockFilter::OnBnClickedButtonOk()
{
    OnOK();
}


void CDialogStockFilter::OnBnClickedButtonCancel()
{
    OnCancel();
}


void CDialogStockFilter::OnBnClickedButtonFilterRound()
{
    CDialogFilterKind dlg;
    if(IDOK != dlg.DoModal()) {
        return;
    }
    // 清除原来的
    while(m_lstBoxKinds.GetCount() > 0) {
        m_lstBoxKinds.DeleteString(0);
    }

    // 插入新的
    for(auto p : dlg.m_vKinds) {
        int id = get<0>(p);
        auto name = get<1>(p);

        int index = m_lstBoxKinds.GetCount();
        index = m_lstBoxKinds.InsertString(index, name.c_str());
        m_lstBoxKinds.SetItemData(index, id);
    }
}

// 获取股票范围
void CDialogStockFilter::GetKinds(std::vector<int> &v)
{
    for(int i = 0; i < m_lstBoxKinds.GetCount(); ++i) {
        v.push_back(m_lstBoxKinds.GetItemData(i));
    }
}

