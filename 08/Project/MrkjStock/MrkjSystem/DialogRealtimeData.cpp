// DialogRealtimeData.cpp : 实现文件
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


// 线程之间传递数据 : 传递要查询的股票代码
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

// http 数据查询线程
namespace http_thread
{
    unsigned __stdcall ThreadRoutine(PVOID pArgs);

    bool bAlive = false;
    unsigned threadID = 0;
    HANDLE hThread = INVALID_HANDLE_VALUE;
    HANDLE hEvent = INVALID_HANDLE_VALUE;

    // 开始查询线程
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

    // 触发事件,进行一次查询
    void FireEvent()
    {
        SetEvent(hEvent);
    }

    // 查询线程:
    // 每3秒查询一次(或调用 FireEvent), 查询成功能后 发送消息
    unsigned __stdcall ThreadRoutine(PVOID pArgs)
    {
        // 获得参数
        auto * pThis = (CDialogRealtimeData*)pArgs;
        // 查询辅助工具
        data_http::CHttpDataSource http;
        // 查到的数据
        data_http::CHttpDataEntry httpData;
        while(bAlive) {
            // 每3钟取一次数据
            DWORD dwWait = ::WaitForSingleObject(hEvent, 3 * 1000);
            // 如果超时,或者有信号, 则进行查询
            if(WAIT_TIMEOUT == dwWait || WAIT_OBJECT_0 == dwWait) {
                // 拼接查询时用到的URL
                CString strStockCode;
                {
                    CString code = GlobalGetStockCode();
                    if(code.IsEmpty()) {
                        continue;
                    }
                    // 以 '6'开头的为上证
                    if(code[0] == L'6') {
                        strStockCode = _T("sh");
                    }
                    // 否则为 深证
                    else {
                        strStockCode = _T("sz");
                    }
                    strStockCode += code;
                }
                // 拼接查询用到的URL
                CString strGIFPath;
                {
                    CString strTempPath;
                    GetTempPath(MAX_PATH, strTempPath.GetBuffer(MAX_PATH));
                    strTempPath.ReleaseBuffer();
                    strTempPath.AppendFormat(_T("%08d_%s_minutes.gif"), GetTickCount(), strStockCode.GetString());
                    strGIFPath = strTempPath.GetString();
                }
                //请求数据
                http.QueryStockKLineMin(StringHelper::UnicodeToANSI(strStockCode.GetString()), StringHelper::UnicodeToANSI(strGIFPath.GetString()));
                http.QueryStockRealTimeData(StringHelper::UnicodeToANSI(strStockCode.GetString()), httpData);
                // 传给窗口的数据,在窗口接收消息时释放
                auto p = new data_http::CHttpDataEntry(httpData);
                auto pFile = new CString(strGIFPath);
                pThis->PostMessage(CDialogRealtimeData::sc_MSG_HTTP_QUERY_END, (WPARAM)p, (LPARAM)pFile);
            }
            else {
                continue; // 继续
            }
        }
        //__end:
        _endthreadex(0);
        return 0;
    }
}

// CDialogRealtimeData 对话框

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


// CDialogRealtimeData 消息处理程序


BOOL CDialogRealtimeData::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    {
        UpdateStockInfoComboBox();
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogRealtimeData::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}

void CDialogRealtimeData::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}

void CDialogRealtimeData::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    // TODO: 在此处添加消息处理程序代码
}

void CDialogRealtimeData::OnTimer(UINT_PTR nIDEvent)
{
    CDialogEx::OnTimer(nIDEvent);
}

void CDialogRealtimeData::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // 放一个图片控件是为了便于调整位置
    CRect rc;
    GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    // 起始位置
    const int x = rc.left;
    const int y = rc.top;
    // 如果有数据
    if(!m_httpData.buyCount01.empty()) {
        //创建一个bmp
        Bitmap bmp(rc.Width(), rc.Height());
        //创建bmp的 gh
        Graphics bmpGraphics(&bmp);
        bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
        // 在些bmp上面画图
        // 画 K线图
        static CDrawKLineImagePanel kLine;
        kLine.Update(bmpGraphics, x, y, m_strFilePath.c_str());
        // 画买卖报价
        static CDrawBuySellPanel dp;
        dp.Update(bmpGraphics, 600, y, m_httpData);
        // 创建dc的 gdi对象
        Graphics graphics(dc.GetSafeHdc());
        CachedBitmap cachedBmp(&bmp, &graphics);
        // 将 bmp 画到真正的设备上面
        graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
    }
}

BOOL CDialogRealtimeData::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

afx_msg LRESULT CDialogRealtimeData::OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam)
{
    // 存储 实时数据
    auto httpData = (data_http::CHttpDataEntry *)wParam;
    // 下载的K线图路径
    auto strFilePath = (CString *)lParam;
    // 保存数据,并删除指针
    if(httpData) {
        m_httpData = *httpData;
        SAFE_DELETE(httpData);
    }
    if(strFilePath) {
        m_strFilePath = *strFilePath;
        SAFE_DELETE(strFilePath);
    }
    // 更新图片显示
    CRect r;
    GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(r);
    ScreenToClient(&r);
    InvalidateRect(&r, TRUE);
    return 0;
}

void CDialogRealtimeData::UpdateStockInfoComboBox()
{
    UpdateData(TRUE);
    //保存当前选项
    CString strOldCode;
    if(m_cbbStockCode.GetCurSel() >= 0) {
        auto p = (PINFO)m_cbbStockCode.GetItemData(m_cbbStockCode.GetCurSel());

        if(p) {
            strOldCode = get<0>(*p).c_str();
        }
    }
    // 清理数据
    ClearList();
    // 查询新项
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
    //恢复当前选项
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
    // 清理数据
    for(int index = 0; index < m_cbbStockCode.GetCount(); ++index) {
        auto ptr = (PINFO)m_cbbStockCode.GetItemData(index);
        SAFE_DELETE(ptr);
    }
    // 清理选项
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
    // 给查询线程传递数据
    GlobalSetStockCode(str);
    // 开始查询线程(如果未开启,会开启线程)
    http_thread::Start(this);
    // 触发查询事件,使线程立即开始一次新查询
    http_thread::FireEvent();
}

void CDialogRealtimeData::OnDestroy()
{
    http_thread::Stop();
    // 销毁窗口前 清理数据
    ClearList();
    CDialogEx::OnDestroy();
}
