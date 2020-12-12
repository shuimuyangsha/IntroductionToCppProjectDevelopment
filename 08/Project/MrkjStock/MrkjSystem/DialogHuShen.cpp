// DialogHuShen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogHuShen.h"
#include "afxdialogex.h"
#include "HttpDataSource.h"
#include "HttpDataEntry.h"

#include "DialogStockFilter.h"

// CDialogHuShen �Ի���

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


// CDialogHuShen ��Ϣ�������
BOOL CDialogHuShen::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
        lStyle &= ~LVS_TYPEMASK;
        lStyle |= LVS_REPORT;
        SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
        DWORD dwStyle = m_lst.GetExtendedStyle();
        dwStyle |= LVS_EX_FULLROWSELECT; //ѡ���� ��������
        //dwStyle |= LVS_EX_GRIDLINES; //������
        m_lst.SetExtendedStyle(dwStyle);

        CRect rcCli;
        GetClientRect(rcCli);
        const int x_off = 1;
        m_lst.MoveWindow(0 + x_off, +24, rcCli.Width() - 2 * x_off, rcCli.Height());
        // ������,�����ô�С
        {
            CRect rc;
            m_lst.GetClientRect(rc);

            m_lst.InsertColumn(EListIndexNo, _T("����"), LVCFMT_LEFT, 40);
            m_lst.InsertColumn(EListIndexCode, _T("����"), LVCFMT_LEFT, 60);
            m_lst.InsertColumn(EListIndexName, _T("����"), LVCFMT_LEFT, 70);
            m_lst.InsertColumn(EListIndexZhangFu, _T("�Ƿ�%"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexXianJian, _T("�ּ�"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZhangDie,  _T("�ǵ�"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexBuy, _T("���"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexSell, _T("����"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZongLiang,   _T("����"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexOpen, _T("��"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexHigh, _T("���"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexLow, _T("���"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZuoShou, _T("����"), LVCFMT_RIGHT, 70);
            m_lst.InsertColumn(EListIndexZongJinE, _T("�ܽ��"), LVCFMT_RIGHT, 180);

            // ���ñ���ɫ
            m_lst.SetBkColor(RGB(0, 0, 0));

            m_lst.SetOnePageCount(MaxCountOnePage);
            m_lst.SetPage(1);
            LoadData(1);

            // �����̣߳�����ʵʱ����
            StartUpdate();
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogHuShen::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}

void CDialogHuShen::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}

void CDialogHuShen::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
}

BOOL CDialogHuShen::PreTranslateMessage(MSG *pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYDOWN) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
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

// ��������
void CDialogHuShen::LoadData(int iPage)
{
    switch(m_status) {
        case CDialogHuShen::EAll: {
            // ���ҳ��С��0��ʼ����ʾ��һҳ
            if(iPage <= 0) {
                iPage = 1;
            }
            // ��ѯ��ҳ������
            std::vector<CMRKJDatabase::TupleStockInfo> v;
            if(DB.QueryStockInfo(v, iPage, MaxCountOnePage) && !v.empty()) {
                // ��¼��ǰҳ
                m_lst.SetPage(iPage);

                // ��ֹ��˸
                m_lst.SetRedraw(FALSE);

                // ɾ��ԭ��������
                m_lst.DeleteAllItems();

                // ����������
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
                // ��ʾ��һҳ
                m_lst.SetPage(1);
                LoadData(1);
            }
            break;
        }
        case CDialogHuShen::EFilter: {
            // ���ҳ��С��0��ʼ����ʾ��һҳ
            if(iPage <= 0) {
                iPage = 1;
            }

            // ��ȡ��ҳ�Ĵ���
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

            // ��ѯ��ҳ������
            std::vector<CMRKJDatabase::TupleStockInfo> v;
            if(!vCodes.empty() && DB.QueryStockInfo(v, vCodes) && !v.empty()) {
                // ��¼��ǰҳ
                m_lst.SetPage(iPage);

                // ��ֹ��˸
                m_lst.SetRedraw(FALSE);

                // ɾ��ԭ��������
                m_lst.DeleteAllItems();

                // ����������
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
                // ��ʾ��һҳ
                m_lst.SetPage(1);
                LoadData(1);
            }
            break;
        }
        default:
            break;
    }
}

// ��������ʵʱ���ݵ��߳�
void CDialogHuShen::StartUpdate()
{
    if(!m_hThread) {
        m_hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadRoutine, this, 0, &m_threadID);
    }
}

// ֹͣ����ʵʱ���ݵ��߳�
void CDialogHuShen::StopUpdate()
{
    if(m_hThread) {
        ::TerminateThread(m_hThread, 0);
        m_hThread = NULL;
    }
}


// �̺߳���
unsigned __stdcall CDialogHuShen::ThreadRoutine(PVOID pArgs)
{
    CDialogHuShen *pThis = (CDialogHuShen *)pArgs;
    // ��ѯ��������
    data_http::CHttpDataSource http;
    // �鵽������
    data_http::CHttpDataEntry httpData;

    // �ַ���ת�� double
    auto GetDouble = [](std::string const & value)->float {
        std::stringstream ss(value);
        double d;
        ss >> d;
        return (float)d;
    };

    // �ж���ɫֵ
    auto GetColor = [](std::string const & value, std::string const & sClose) {
        // �ǣ�������ƽ����ɫ
        COLORREF clrUp = RGB(0xFF, 0, 0);
        COLORREF clrDown = RGB(0, 0xFF, 0);
        COLORREF clrKeep = RGB(0xFF, 0xFF, 0xFF);
        // �ּ�
        double d, close;
        {
            stringstream ss(value);
            ss >> d;
        }

        {
            stringstream ss(sClose);
            ss >> close;
        }

        // �ж���ɫ
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

    // ��ȡ��ѯ������Ҫ�� CODE(��sh��sz��ͷ)
    auto GetQueryCode = [](CString const & code)->std::string{
        // �� '6'��ͷ��Ϊ��֤
        if(code[0] == L'6')
        {
            return StringHelper::UnicodeToANSI(_T("sh") + code);
        }
        // ����Ϊ ��֤
        else {
            return StringHelper::UnicodeToANSI(_T("sz") + code);
        }
    };

    while(1) {
        // ÿ���Ӹ���һ��
        Sleep(1);
        try {
            int count = pThis->m_lst.GetItemCount();
            for(int index = 0; index < count; ++index) {
                CString code = pThis->m_lst.GetItemText(index, EListIndexCode);

                // ƴ�Ӳ�ѯʱ�õ���URL
                std::string queryCode = GetQueryCode(code);

                //��������
                if(!http.QueryStockRealTimeData(queryCode, httpData)) {
                    continue;
                }

                // ���ô�������
                {
                    // �����������̼ۣ�����Ƚ��ǵ�
                    double close = GetDouble(httpData.close);

                    {
                        // �ǵ�ֵ
                        double d1 = (GetDouble(httpData.price) - close);
                        // �Ƿ�:(�ּ�-����) / ����
                        double d2 = d1 / close * 100;

                        CString str;
                        str.Format(_T("%.2f"), d2);
                        COLORREF clr = GetColor(httpData.price, httpData.close);
                        pThis->m_lst.SetItemText(index, EListIndexZhangFu, str, clr);
                        // �ǵ�ֵ
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
            TRACE("���������쳣");
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
    // �ѽ���ù���
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
