// DialogStockFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogStockFilter.h"
#include "afxdialogex.h"
#include "DialogFilterKind.h"


#define WM_USER_OTHER_THREAD (WM_USER + 1)

// CDialogStockFilter �Ի���

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


// CDialogStockFilter ��Ϣ�������


BOOL CDialogStockFilter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    {
        // ���еò������λ��
        CRect rc;
        {
            CStatic *ps = (CStatic *)GetDlgItem(IDC_STATIC_PARA);
            ps->GetWindowRect(rc);
            ScreenToClient(rc);
        }

        {
            m_comboFilter.AddString(_T("UPN                 - ��������"));

            CRect r = rc;
            r.left += 10;
            r.top += 20;
            r.right = r.left + 30;
            r.bottom = r.top + 20;

            m_stt_upn_days0.Create(_T("����"), WS_CHILD | WS_VISIBLE | SS_CENTER, r, this, IDC_EDIT_UPN_DAYS);
            m_stt_upn_days0.SetFont(this->GetFont());

            r.left = r.right + 10;
            r.right = r.left + 80;
            m_edt_upn_days.Create(ES_RIGHT | ES_NUMBER, r, this, IDC_STATIC_UPN_DAYS0);
            m_edt_upn_days.SetWindowText(_T("3"));

            r.left = r.right + 10;
            r.right = r.left + 20;
            m_stt_upn_days1.Create(_T("��"), SS_LEFT, r, this, IDC_STATIC_UPN_DAYS1);
            m_stt_upn_days1.SetFont(this->GetFont());

            UPN(TRUE);
        }

        {
            m_comboFilter.AddString(_T("DOWNN             - ��������"));

            CRect r = rc;
            r.left += 10;
            r.top += 20;
            r.right = r.left + 30;
            r.bottom = r.top + 20;
            m_stt_downn_days0.Create(_T("����"), SS_LEFT, r, this, IDC_EDIT_DOWNN_DAYS);
            m_stt_downn_days0.SetFont(this->GetFont());

            r.left = r.right + 10;
            r.right = r.left + 80;
            m_edt_downn_days.Create(ES_RIGHT | ES_NUMBER, r, this, IDC_STATIC_DOWNN_DAYS0);
            m_edt_downn_days.SetWindowText(_T("3"));

            r.left = r.right + 10;
            r.right = r.left + 20;
            m_stt_downn_days1.Create(_T("��"), SS_LEFT, r, this, IDC_STATIC_DOWNN_DAYS1);
            m_stt_downn_days1.SetFont(this->GetFont());
        }

        // ѡ�е�һ��
        m_comboFilter.SetCurSel(0);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDialogStockFilter::PreTranslateMessage(MSG *pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���

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
        BOOL bAndOr; // TRUE:ȫ����,FALSE:ȫ����
        std::vector<SFunc> vFuncs; // ��������
        CString strDateStart, strDateEnd; // ����������������
        std::vector<int> vKinds; // ѡ�ɷ�Χ
    } query_condition;

    unsigned __stdcall ThreadRoutine(PVOID pArgs);

    bool bAlive = false;
    unsigned threadID = 0;
    HANDLE hThread = INVALID_HANDLE_VALUE;
    HANDLE hEvent = INVALID_HANDLE_VALUE;

    // ��ʼ��ѯ�߳�
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
                             , FALSE//�Զ�����
                             , FALSE // ��ʼ�� ���ź�״̬
                             , NULL);
        hThread = (HANDLE)(::_beginthreadex(nullptr, 0, ThreadRoutine, pThis, 0, &threadID));
    }

    // ֹͣ��ѯ�߳�
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

    // �������� : �жϹ�Ʊ�����Ƿ��������
    BOOL CheckStock(VDayDataEntry const &vdd, BOOL bAndOr, std::vector<SFunc> vFuncs)
    {
        // �Ƿ���������N������
        auto CheckUPN = [](VDayDataEntry const & vdd, int N)->BOOL {
            if(N <= 1)
            {
                return FALSE;
            }
            // ����Ҫ��1+N�����ݣ������ж��Ƿ�����N��
            if(vdd.size() <= N)
            {
                return FALSE;
            }
            // �����һ����ǰ�ж�����������N��
            int cnt = 0;
            for(unsigned i1 = vdd.size() - 1; i1 >= 0; --i1)
            {
                unsigned i0 = i1 - 1;
                // ת�����ڵ������Ƚϴ�С
                wstringstream ss0(vdd[i0].close), ss1(vdd[i1].close);
                float d0, d1;
                ss0 >> d0, ss1 >> d1;
                // ���ǰһ�� >= �ڶ��죬˵��û���ǣ��򲻷�����
                if(d0 >= d1) {
                    // �����������ټ����ж���
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
        // �Ƿ���������N������
        auto CheckDOWNN = [](VDayDataEntry const & vdd, int N)->BOOL {
            if(N <= 1)
            {
                return FALSE;
            }
            // ����Ҫ��1+N�����ݣ������ж��Ƿ�����N��
            if(vdd.size() <= N)
            {
                return FALSE;
            }
            // �����һ����ǰ�ж�����������N��
            int cnt = 0;
            for(unsigned i1 = vdd.size() - 1; i1 >= 0; --i1)
            {
                unsigned i0 = i1 - 1;
                // ת�����ڵ������Ƚϴ�С
                wstringstream ss0(vdd[i0].close), ss1(vdd[i1].close);
                float d0, d1;
                ss0 >> d0, ss1 >> d1;
                // ���ǰһ�� < �ڶ��죬˵�����ˣ��򲻷�����
                if(d0 < d1) {
                    // �����������ټ����ж���
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

        // ���ȫ�������� : �����Ƿ񲻷������������������,����ǰ����
        if(query_condition.bAndOr) {
            // �Ե�ǰ��Ʊ����ִ�й�����Ϣ
            for(SFunc &fun : query_condition.vFuncs) {
                if(fun.strFunc == _T("UPN")) {
                    //ȡ�ò���,ת��Ϊ�죬�ú�����ʾ���� N ��
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("����%d��\r\n", N);
                    if(!CheckUPN(vdd, N)) {
                        return FALSE;
                    }
                }
                else if(fun.strFunc == _T("DOWNN")) {
                    //ȡ�ò���,ת��Ϊ�죬�ú�����ʾ���� N ��
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("����%d��\r\n", N);
                    if(!CheckDOWNN(vdd, N)) {
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }
        // ȫ������or : �ж��Ƿ�����������������,����ǰ����
        else {
            // �Ե�ǰ��Ʊ����ִ�й�����Ϣ
            for(SFunc &fun : query_condition.vFuncs) {
                if(fun.strFunc == _T("UPN")) {
                    //ȡ�ò���,ת��Ϊ�죬�ú�����ʾ���� N ��
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("����%d��\r\n", N);
                    if(CheckUPN(vdd, N)) {
                        return TRUE;
                    }
                }
                else if(fun.strFunc == _T("DOWNN")) {
                    //ȡ�ò���,ת��Ϊ�죬�ú�����ʾ���� N ��
                    wstringstream ss(fun.vParams[0].GetString());
                    int N = 0;
                    ss >> N;
                    TRACE("����%d��\r\n", N);
                    if(!CheckDOWNN(vdd, N)) {
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }

    }

    // ��ѯ�߳�:
    // ÿ3���ѯһ��(����� FireEvent), ��ѯ�ɹ��ܺ� ������Ϣ
    unsigned __stdcall ThreadRoutine(PVOID pArgs)
    {
        // ��ò���
        auto *pThis = (CDialogStockFilter *)pArgs;

        // �������еĹ�Ʊ��������Ӧ�õ���Ʊ�������ɸѡ
        std::vector<CMRKJDatabase::TupleStockInfo> v;
        DB.QueryStockInfo(v, query_condition.vKinds);
        ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 1, v.size());
        for(int i = 0; i < v.size() && pThis->m_bFilter && bAlive; ++i) {
            // ��ѯ��Ʊ��������
            VDayDataEntry vdd;
            if(!DB.QueryDayData(vdd, std::get<0>(v[i]).c_str(), query_condition.strDateStart.GetString(), query_condition.strDateEnd.GetString())) {
                continue;
            }
            //���˳ɹ��ļ��뵽�б���
            if(CheckStock(vdd, query_condition.bAndOr, query_condition.vFuncs)) {
                // ���� + ���ݹ��˳ɹ�������
                auto strCode = _wcsdup(std::get<0>(v[i]).c_str());
                ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 2, (LPARAM)strCode);
            }
            else {
                // ����
                ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 2, 0);
            }
        }
        // �ָ���ť����
        ::PostMessage(pThis->GetSafeHwnd(), WM_USER_OTHER_THREAD, 3, 0);
        // �ѽ������
        _endthreadex(0);
        return 0;
    }
}


// ����������Ҫ�̵���Ϣ
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
            GetDlgItem(IDC_BUTTON_DO)->SetWindowText(_T("ִ��ѡ��"));
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
        // ����ϴ�ѡ�ɵĽ��
        m_vFilterStocks.clear();

        // �������ŵ���ʼλ��
        m_process.SetPos(0);

        // ����һ�������ռ�,����ʹ��
        namespace t = filter_tread;

        // ȡ��������ȫ���룿 or ȫ����
        t::query_condition.bAndOr = (((CButton *)GetDlgItem(IDC_RADIO_ADN))->GetCheck() == BST_CHECKED);

        // ȡ������������ʽ������������
        {
            auto &tmp = t::query_condition;
            tmp.vFuncs.clear();
            for(int index = 0; index < m_listFilters.GetCount(); ++index) {
                CString str;
                m_listFilters.GetText(index, str);
                // ����ַ���
                int iStart = 0;
                // ������
                CString s = str.Tokenize(_T(" ()"), iStart);
                assert(!s.IsEmpty());
                t::SFunc fun;
                fun.strFunc = s;

                // ����
                s = str.Tokenize(_T(" ()"), iStart);
                for(; !s.IsEmpty(); s = str.Tokenize(_T(" ()"), iStart)) {
                    fun.vParams.push_back(s);
                }
                // ������
                t::query_condition.vFuncs.push_back(fun);
            }

            if(tmp.vFuncs.empty()) {
                AfxMessageBox(_T("������ѡ��һ��ѡ�ɹ�ʽ"));
                return FALSE;
            }
        }

        // �����������
        {
            COleDateTime dtStart, dtEnd;
            m_dtStart.GetTime(dtStart);
            m_dtEnd.GetTime(dtEnd);
            // ��ȡ�����ַ���
            t::query_condition.strDateStart = dtStart.Format(_T("%Y/%m/%d"));
            t::query_condition.strDateEnd = dtEnd.Format(_T("%Y/%m/%d"));
        }

        // ��ȡѡ�ɷ�Χ
        {
            t::query_condition.vKinds.clear();
            GetKinds(t::query_condition.vKinds);
            if(t::query_condition.vKinds.empty()) {
                AfxMessageBox(_T("��ѡ��ѡ�ɷ�Χ"));
                return FALSE;
            }
        }

        // ��־����ѡ��
        m_bFilter = bFilter;
        // �����߳�
        filter_tread::Start(this);
        return TRUE;
    }
    else {
        // ֹͣѡ���߳�
        filter_tread::Stop();
    }

    return m_bFilter;
}

void CDialogStockFilter::OnBnClickedButtonDo()
{
    CString str;
    auto pBtn = (CButton *)GetDlgItem(IDC_BUTTON_DO);
    pBtn->GetWindowText(str);
    if(str == _T("ִ��ѡ��")) {
        if(DoFilter(TRUE)) {
            pBtn->SetWindowText(_T("ֹͣ"));
        }
    }
    else {
        DoFilter(FALSE);
        pBtn->SetWindowText(_T("ִ��ѡ��"));
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
    // ���ԭ����
    while(m_lstBoxKinds.GetCount() > 0) {
        m_lstBoxKinds.DeleteString(0);
    }

    // �����µ�
    for(auto p : dlg.m_vKinds) {
        int id = get<0>(p);
        auto name = get<1>(p);

        int index = m_lstBoxKinds.GetCount();
        index = m_lstBoxKinds.InsertString(index, name.c_str());
        m_lstBoxKinds.SetItemData(index, id);
    }
}

// ��ȡ��Ʊ��Χ
void CDialogStockFilter::GetKinds(std::vector<int> &v)
{
    for(int i = 0; i < m_lstBoxKinds.GetCount(); ++i) {
        v.push_back(m_lstBoxKinds.GetItemData(i));
    }
}

