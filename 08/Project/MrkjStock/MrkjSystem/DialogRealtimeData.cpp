// DialogRealtimeData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogRealtimeData.h"
#include "afxdialogex.h"
#include "HttpDataSource.h"
#include "HttpDataEntry.h"
#include "DialogKeybdQuery.h"
#include "DrawBuySellPanel.h"
#include "DrawKLineImagePanel.h"


// �߳�֮�䴫������ : ����Ҫ��ѯ�Ĺ�Ʊ����
static CCriticalSection cs;
static CString g_strStockCode = (_T(""));
static void GlobalSetStockCode(CString code)
{
    cs.Lock();
    g_strStockCode = code;
    cs.Unlock();
}
static CString GlobalGetStockCode()
{
    CString code;
    cs.Lock();
    code = g_strStockCode;
    cs.Unlock();
    return code;
}

// http ���ݲ�ѯ�߳�
namespace http_thread
{
    unsigned __stdcall ThreadRoutine(PVOID pArgs);

    bool bAlive = false;
    unsigned threadID = 0;
    HANDLE hThread = INVALID_HANDLE_VALUE;
    HANDLE hEvent = INVALID_HANDLE_VALUE;

    // ��ʼ��ѯ�߳�
    void Start(CDialogRealtimeData * pThis)
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

    // �����¼�,����һ�β�ѯ
    void FireEvent()
    {
        SetEvent(hEvent);
    }

    // ��ѯ�߳�:
    // ÿ3���ѯһ��(����� FireEvent), ��ѯ�ɹ��ܺ� ������Ϣ
    unsigned __stdcall ThreadRoutine(PVOID pArgs)
    {
        // ��ò���
        auto * pThis = (CDialogRealtimeData*)pArgs;
        // ��ѯ��������
        data_http::CHttpDataSource http;
        // �鵽������
        data_http::CHttpDataEntry httpData;
        while(bAlive) {
            // ÿ3��ȡһ������
            DWORD dwWait = ::WaitForSingleObject(hEvent, 3 * 1000);
            // �����ʱ,�������ź�, ����в�ѯ
            if(WAIT_TIMEOUT == dwWait || WAIT_OBJECT_0 == dwWait) {
                // ƴ�Ӳ�ѯʱ�õ���URL
                CString strStockCode;
                {
                    CString code = GlobalGetStockCode();
                    if(code.IsEmpty()) {
                        continue;
                    }
                    // �� '6'��ͷ��Ϊ��֤
                    if(code[0] == L'6') {
                        strStockCode = _T("sh");
                    }
                    // ����Ϊ ��֤
                    else {
                        strStockCode = _T("sz");
                    }
                    strStockCode += code;
                }
                // ƴ�Ӳ�ѯ�õ���URL
                CString strGIFPath;
                {
                    CString strTempPath;
                    GetTempPath(MAX_PATH, strTempPath.GetBuffer(MAX_PATH));
                    strTempPath.ReleaseBuffer();
                    strTempPath.AppendFormat(_T("%08d_%s_minutes.gif"), GetTickCount(), strStockCode.GetString());
                    strGIFPath = strTempPath.GetString();
                }
                //��������
                http.QueryStockKLineMin(StringHelper::UnicodeToANSI(strStockCode.GetString()), StringHelper::UnicodeToANSI(strGIFPath.GetString()));
                http.QueryStockRealTimeData(StringHelper::UnicodeToANSI(strStockCode.GetString()), httpData);
                // �������ڵ�����,�ڴ��ڽ�����Ϣʱ�ͷ�
                auto p = new data_http::CHttpDataEntry(httpData);
                auto pFile = new CString(strGIFPath);
                pThis->PostMessage(CDialogRealtimeData::sc_MSG_HTTP_QUERY_END, (WPARAM)p, (LPARAM)pFile);
            }
            else {
                continue; // ����
            }
        }
        //__end:
        _endthreadex(0);
        return 0;
    }
}

// CDialogRealtimeData �Ի���

IMPLEMENT_DYNAMIC(CDialogRealtimeData, CDialogEx)

CDialogRealtimeData::CDialogRealtimeData(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_REALTIME_DATA, pParent)
    , m_strCode(_T(""))
{
}

CDialogRealtimeData::~CDialogRealtimeData()
{
}

void CDialogRealtimeData::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO2, m_cbbStockCode);
    DDX_CBString(pDX, IDC_COMBO2, m_strCode);
}


BEGIN_MESSAGE_MAP(CDialogRealtimeData, CDialogEx)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_MESSAGE(sc_MSG_HTTP_QUERY_END, &CDialogRealtimeData::OnMsgHttpQueryEnd)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogRealtimeData::OnBnClickedButtonQuery)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogRealtimeData ��Ϣ�������


BOOL CDialogRealtimeData::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        UpdateStockInfoComboBox();
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogRealtimeData::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}

void CDialogRealtimeData::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}

void CDialogRealtimeData::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    // TODO: �ڴ˴������Ϣ����������
}

void CDialogRealtimeData::OnTimer(UINT_PTR nIDEvent)
{
    CDialogEx::OnTimer(nIDEvent);
}

void CDialogRealtimeData::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // ��һ��ͼƬ�ؼ���Ϊ�˱��ڵ���λ��
    CRect rc;
    GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    // ��ʼλ��
    const int x = rc.left;
    const int y = rc.top;
    // ���������
    if(!m_httpData.buyCount01.empty()) {
        //����һ��bmp
        Bitmap bmp(rc.Width(), rc.Height());
        //����bmp�� gh
        Graphics bmpGraphics(&bmp);
        bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
        // ��Щbmp���滭ͼ
        // �� K��ͼ
        static CDrawKLineImagePanel kLine;
        kLine.Update(bmpGraphics, x, y, m_strFilePath.c_str());
        // ����������
        static CDrawBuySellPanel dp;
        dp.Update(bmpGraphics, 600, y, m_httpData);
        // ����dc�� gdi����
        Graphics graphics(dc.GetSafeHdc());
        CachedBitmap cachedBmp(&bmp, &graphics);
        // �� bmp �����������豸����
        graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
    }
}

BOOL CDialogRealtimeData::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

afx_msg LRESULT CDialogRealtimeData::OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam)
{
    // �洢 ʵʱ����
    auto httpData = (data_http::CHttpDataEntry *)wParam;
    // ���ص�K��ͼ·��
    auto strFilePath = (CString *)lParam;
    // ��������,��ɾ��ָ��
    if(httpData) {
        m_httpData = *httpData;
        SAFE_DELETE(httpData);
    }
    if(strFilePath) {
        m_strFilePath = *strFilePath;
        SAFE_DELETE(strFilePath);
    }
    // ����ͼƬ��ʾ
    CRect r;
    GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(r);
    ScreenToClient(&r);
    InvalidateRect(&r, TRUE);
    return 0;
}

void CDialogRealtimeData::UpdateStockInfoComboBox()
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
    // ��������
    ClearList();
    // ��ѯ����
    vector<INFO> v;
    if(DB.QueryStockInfo(v)) {
        for(auto info : v) {
            CString str;
            str.Format(_T("%s - %-10s - %-10s"), get<0>(info).c_str()
                       , get<1>(info).c_str(), get<3>(info).c_str());
            int index = m_cbbStockCode.AddString(str);
            m_cbbStockCode.SetItemData(index, (DWORD_PTR)(new INFO(info)));

        }
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

void CDialogRealtimeData::ClearList()
{
    // ��������
    for(int index = 0; index < m_cbbStockCode.GetCount(); ++index) {
        auto ptr = (PINFO)m_cbbStockCode.GetItemData(index);
        SAFE_DELETE(ptr);
    }
    // ����ѡ��
    m_cbbStockCode.ResetContent();
}

bool CDialogRealtimeData::GetSelStockCode(CString& strCode)
{
    int index = m_cbbStockCode.GetCurSel();
    if(index < 0) {
        return false;
    }
    auto p = (PINFO)m_cbbStockCode.GetItemData(index);
    if(!p) {
        return false;
    }
    strCode = get<0>(*p).c_str();
    return true;
}

void CDialogRealtimeData::OnBnClickedButtonQuery()
{
    CString str;
    if(!GetSelStockCode(str)) {
        return;
    }
    // ����ѯ�̴߳�������
    GlobalSetStockCode(str);
    // ��ʼ��ѯ�߳�(���δ����,�Ὺ���߳�)
    http_thread::Start(this);
    // ������ѯ�¼�,ʹ�߳�������ʼһ���²�ѯ
    http_thread::FireEvent();
}

void CDialogRealtimeData::OnDestroy()
{
    http_thread::Stop();
    // ���ٴ���ǰ ��������
    ClearList();
    CDialogEx::OnDestroy();
}
