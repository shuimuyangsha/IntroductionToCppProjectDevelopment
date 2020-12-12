// DialogDataImpExp.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogDataImpExp.h"
#include "afxdialogex.h"

#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

using namespace std;

// CDialogDataImpExp 对话框

IMPLEMENT_DYNAMIC(CDialogDataImpExp, CDialogEx)

CDialogDataImpExp::CDialogDataImpExp(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_DATA_IMPEXP, pParent)
    , m_strImportFilePath(_T(""))
    , m_strExportFilePath(_T(""))
    , m_strCode(_T(""))
{
}

CDialogDataImpExp::~CDialogDataImpExp()
{
}

void CDialogDataImpExp::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_strImportFilePath);
    DDX_Text(pDX, IDC_EDIT2, m_strExportFilePath);
    DDX_Control(pDX, IDC_LIST_DATA, m_lst);
    DDX_Text(pDX, IDC_EDIT9, m_strCode);
}


BEGIN_MESSAGE_MAP(CDialogDataImpExp, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogDataImpExp::OnBnClickedButtonLookupImport)
    ON_BN_CLICKED(IDC_BUTTON3, &CDialogDataImpExp::OnBnClickedButtonLookupExport)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogDataImpExp::OnBnClickedButtonQuery)
    ON_BN_CLICKED(IDC_BUTTON8, &CDialogDataImpExp::OnBnClickedButtonImport)
    ON_BN_CLICKED(IDC_BUTTON9, &CDialogDataImpExp::OnBnClickedButtonExport)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_IMPORT_TDX, &CDialogDataImpExp::OnBnClickedButtonImportTdx)
END_MESSAGE_MAP()


// CDialogDataImpExp 消息处理程序


void CDialogDataImpExp::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogDataImpExp::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


BOOL CDialogDataImpExp::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 初始化界面上的UI控件
    {
        // 0. 本窗口客户区的大小
        CRect rcCli;
        GetClientRect(rcCli);
        // 1. CListCtrl 控件
        {
            LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
            lStyle &= ~LVS_TYPEMASK;
            lStyle |= LVS_REPORT;
            SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
            DWORD dwStyle = m_lst.GetExtendedStyle();
            dwStyle |= LVS_EX_FULLROWSELECT; //选中行 整征高亮
            dwStyle |= LVS_EX_GRIDLINES; //网络线
            m_lst.SetExtendedStyle(dwStyle);
            // 设置列,并设置大小
            {
                CRect rc;
                m_lst.GetClientRect(rc);
                m_lst.InsertColumn(EListIndexCode, _T("股票代码"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexName, _T("股票名称"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexUpPercent, _T("股票类别"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexPrice, _T("日期"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexOpen, _T("开盘"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexClose, _T("收盘"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMax, _T("最高"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMin, _T("最低"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTurnover, _T("成交量(股)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTradingVolume, _T("成交额(元)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            }
        }
    }
    // 如果调试模式,显示导入通达信数据的按钮
#define IMPORT_TDX 0
#if IMPORT_TDX
    {
        auto pBtn = (CButton*)GetDlgItem(IDC_BUTTON_IMPORT_TDX);
        assert(pBtn);
        pBtn->ShowWindow(SW_SHOW);
        pBtn->EnableWindow(TRUE);
    }
#endif
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


// 更新m_lst中的数据
void CDialogDataImpExp::UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info)
{
    m_lst.DeleteAllItems();
    for each(auto data in v) {
        int index = m_lst.GetItemCount();
        CString strNo;
        strNo.Format(_T("%04d"), index + 1);
        m_lst.InsertItem(index, get<0>(info).c_str()); //代码
        m_lst.SetItemText(index, EListIndexName, get<1>(info).c_str());
        m_lst.SetItemText(index, EListIndexUpPercent, get<3>(info).c_str());
        m_lst.SetItemText(index, EListIndexPrice, data.date.c_str());
        m_lst.SetItemText(index, EListIndexOpen, data.open.c_str());
        m_lst.SetItemText(index, EListIndexClose, data.close.c_str());
        m_lst.SetItemText(index, EListIndexMax, data.max.c_str());
        m_lst.SetItemText(index, EListIndexMin, data.min.c_str());
        m_lst.SetItemText(index, EListIndexTurnover, data.turnover.c_str());
        m_lst.SetItemText(index, EListIndexTradingVolume, data.tradingVolume.c_str());
    }
}

// 获得某行数据
void CDialogDataImpExp::GetListData(std::vector<CString>& data, int index)
{
    assert(index >= 0 && index < m_lst.GetItemCount());
    UpdateData(TRUE);
    data.push_back(m_lst.GetItemText(index, EListIndexCode));
    data.push_back(m_lst.GetItemText(index, EListIndexName));
    data.push_back(m_lst.GetItemText(index, EListIndexUpPercent));
    data.push_back(m_lst.GetItemText(index, EListIndexPrice));
    data.push_back(m_lst.GetItemText(index, EListIndexOpen));
    data.push_back(m_lst.GetItemText(index, EListIndexClose));
    data.push_back(m_lst.GetItemText(index, EListIndexMax));
    data.push_back(m_lst.GetItemText(index, EListIndexMin));
    data.push_back(m_lst.GetItemText(index, EListIndexTurnover));
    data.push_back(m_lst.GetItemText(index, EListIndexTradingVolume));
}

namespace imp_exp_hlp
{
    //获取文件内容
    bool GetFileContent(const std::wstring& filename, std::wstring& code
                        , std::wstring& name, VDayDataEntry& vRes)
    {
        using namespace std;
        using namespace StringHelper;
        string line;
        bool bRet = false;
        ifstream file(UnicodeToANSI(filename), ios::in);
        if(file.bad()) {
            AfxMessageBox(_T("打开文件失败"));
            goto __Cleanup;
        }
        // 读取第一行，获得代码和称
        if(!getline(file, line)) {
            AfxMessageBox(_T("文件中没有数据"));
            goto __Cleanup;
        }
        //拆分获得
        {
            int index0 = 0;
            int index1 = line.find(' ', 0);
            int index2 = line.find("日线");
            if(index1 < 0 || index2 <= 0 || index1 >= index2) {
                AfxMessageBox(_T("获取股票名称和代码失败，请检查文件格式"));
                goto __Cleanup;
            }
            code = ANSIToUnicode(line.substr(0, index1));
            name = ANSIToUnicode(line.substr(1 + index1, index2 - 1 - index1));
            if(code.empty() || name.empty()) {
                AfxMessageBox(_T("获取股票名称和代码失败，请检查文件格式"));
                goto __Cleanup;
            }
        }
        // 越过第二行(标题)
        if(!getline(file, line)) {
            goto __Cleanup;
        }
        {
            // 读取后面的
            std::deque<string> content;
            while(getline(file, line)) {
                content.push_back(line);
            }
            //　删除最后一行（是没用的数据）
            if(!content.empty()) {
                content.pop_back();
            }
            // 如果没有内容,直接返回
            if(content.empty()) {
                goto __Cleanup;
            }
            //遍历，拆分，生数据，插入数据库
            for(auto it = content.begin(); it != content.end(); ++it) {
                vector<string> v;
                StringHelper::SplitString(v, *it, "\t");
                if(v.size() != 7) {
                    TRACE("数据不符合要求");
                    continue;
                }
                CDayDataEntry obj;
                obj.code = code;
                obj.date = UTF8ToUnicode(v[0]);
                obj.open = ANSIToUnicode(v[1]);
                obj.max = ANSIToUnicode(v[2]);
                obj.min = UTF8ToUnicode(v[3]);
                obj.close = UTF8ToUnicode(v[4]);
                obj.turnover = UTF8ToUnicode(v[5]);
                obj.tradingVolume = UTF8ToUnicode(v[6]);
                vRes.push_back(obj);
            }
        }
        bRet = true;
__Cleanup:
        if(!file.bad()) {
            file.close();
        }
        return bRet;
    }

    //获取文件内容 之 code, name
    bool GetFileContentCodeName(const std::wstring& filename, std::wstring& code, std::wstring& name)
    {
        using namespace std;
        using namespace StringHelper;
        string line;
        bool bRet = false;
        ifstream file(UnicodeToANSI(filename), ios::in);
        if(file.bad()) {
            AfxMessageBox(_T("打开文件失败"));
            goto __Cleanup;
        }
        // 读取第一行，获得代码和称
        if(!getline(file, line)) {
            AfxMessageBox(_T("文件中没有数据"));
            goto __Cleanup;
        }
        //拆分获得
        {
            int index0 = 0;
            int index1 = line.find(' ', 0);
            int index2 = line.find("日线");
            if(index1 < 0 || index2 <= 0 || index1 >= index2) {
                AfxMessageBox(_T("获取股票名称和代码失败，请检查文件格式"));
                goto __Cleanup;
            }
            code = ANSIToUnicode(line.substr(0, index1));
            name = ANSIToUnicode(line.substr(1 + index1, index2 - 1 - index1));
            if(code.empty() || name.empty()) {
                AfxMessageBox(_T("获取股票名称和代码失败，请检查文件格式"));
                goto __Cleanup;
            }
        }
        bRet = true;
__Cleanup:
        if(!file.bad()) {
            file.close();
        }
        return bRet;
    }

    // 文件内容导入数据库
    void ToDatabase(const std::wstring& filename, std::wstring& code, std::wstring& name)
    {
        using namespace std;
        //　读取文件内容
        VDayDataEntry v;
        if(!GetFileContent(filename, code, name, v)) {
            return;
        }
        // 确认存在 table 如果不存在创建
        if(!DB.MakeSureExitsTable(code.c_str(), name.c_str())) {
            return;
        }
        DB.AddDayData(v);
    }

    // 文件内容导入数据库: CODE/NAME -> T_CODE_NAME;
    void ToDatabaseCodeName(const std::wstring& filename)
    {
        using namespace std;
        //　读取文件内容
        wstring code, name;
        VDayDataEntry v;
        if(!GetFileContent(filename, code, name, v)) {
            return;
        }
        // 确认存在 table 如果不存在创建
        if(!DB.MakeDayTable(code)) {
            return;
        }
        DB.AddStock(code.c_str(), name.c_str());
    }

    using namespace std;

    CString headers[] = { _T("股票代码"), _T("股票名称"), _T("股票类别"), _T("日期"), _T("开盘")
                          , _T("收盘"), _T("最高"), _T("最低"), _T("成交量"), _T("成交额")
                        };

    // 内容写入Excel
    void ToExcel(const wstring& filename, const vector<vector<CString>>& v)
    {
        CString strFile = filename.c_str();
        COleVariant covTrue((short)TRUE);
        COleVariant covFalse((short)FALSE);
        COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        CApplication app;
        CWorkbook book;
        CWorkbooks books;
        CWorksheet sheet;
        CWorksheets sheets;
        CRange range;
        if(!app.CreateDispatch(_T("Excel.Application"))) {
            CTools::MessageBoxFormat(_T("创建 接口失败"));
            return;
        }
        //向下 一层一层的获得操作对象
        books = app.get_Workbooks();
        book = books.Add(covOptional);
        sheets = book.get_Worksheets();
        sheet = sheets.get_Item(COleVariant((short)1));
        // 显示
        //app.put_Visible(TRUE);
        range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1")));
        // 表示写入的是第几行
        _variant_t varRoleIndex(1);
        //写入表头
        {
            for(int i = 0; i < _countof(headers); ++i) {
                // 第几列
                _variant_t varColumnIndex((long)i + 1);
                range.put_NumberFormat(COleVariant(L"@"));
                range.put_Item(varRoleIndex, varColumnIndex, _variant_t(headers[i].GetString()));
            }
        }
        //写入内容
        {
            for(auto vline : v) {
                varRoleIndex.intVal += 1;
                for(unsigned i = 0; i != vline.size(); ++i) {
                    // 第几列
                    _variant_t varColumnIndex((long)i + 1);
                    range.put_NumberFormat(COleVariant(L"@"));
                    range.put_Item(varRoleIndex, varColumnIndex, _variant_t(vline[i].GetString()));
                }
            }
        }
        // 保存
        book.SaveCopyAs(COleVariant(strFile));
        book.put_Saved(true);
        //释放
        range.ReleaseDispatch();
        book.ReleaseDispatch();
        books.ReleaseDispatch();
        sheet.ReleaseDispatch();
        sheets.ReleaseDispatch();
        app.Quit();
        app.ReleaseDispatch();
    }

    // 从Excel中读取内容
    void FromExcel(const wstring& filename, vector<vector<CString>>& v)
    {
        CString strFile = filename.c_str();
        COleVariant covTrue((short)TRUE);
        COleVariant covFalse((short)FALSE);
        COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        CApplication app;
        CWorkbook book;
        CWorkbooks books;
        CWorksheet sheet;
        CWorksheets sheets;
        CRange range;
        if(!app.CreateDispatch(_T("Excel.Application"))) {
            CTools::MessageBoxFormat(_T("创建 接口失败"));
            return;
        }
        //向下 一层一层的获得操作对象
        books = app.get_Workbooks();
        book = books.Open(strFile, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
        sheets = book.get_Worksheets();
        sheet = sheets.get_Item(COleVariant((short)1));
        // 得到使用的
        range = sheet.get_UsedRange();
        // 行数
        range = range.get_Rows();
        long rows = range.get_Count();
        // 列数
        range = range.get_Columns();
        long cols = range.get_Count();
        if(_countof(headers) != cols) {
            // 数据列数不对
            goto __Cleanup;
        }
        //得到所有的 Cell
        range = sheet.get_Cells();
        //读(注意下标从1开始)
        for(int i = 2/*不要表头那一行*/; i <= rows; ++i) {
            vector<CString> vLine;
            COleVariant varRowIndex(static_cast<long>(i));
            for(int j = 1; j <= cols; ++j) {
                COleVariant varColumnIndex(static_cast<long>(j));
                VARIANT vResult;
                CString strResult;
                vResult = range.get_Item(varRowIndex, varColumnIndex);
                strResult = (wchar_t*)(_bstr_t)vResult;
                vLine.push_back(strResult);
            }
            v.push_back(vLine);
        }
__Cleanup:
        //释放
        range.ReleaseDispatch();
        book.ReleaseDispatch();
        books.ReleaseDispatch();
        sheet.ReleaseDispatch();
        sheets.ReleaseDispatch();
        app.Quit();
        app.ReleaseDispatch();
    }

}
using namespace imp_exp_hlp;

void CDialogDataImpExp::OnBnClickedButtonLookupImport()
{
    UpdateData(TRUE);
    /* 导入文件为 UTF8 无BOM 编码
       格式是:
     */
    using namespace std;
    static CString strFilename = _T("");
    static TCHAR szFilter[] = _T("Excel文件(*.xls)|*.xls|所有文件(*.*)|*.*||");
    CFileDialog dlg(TRUE, // 创建文件打开对话框:FALSE保存对话框　
                    _T(".txt"),
                    strFilename.GetString(),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    szFilter
                   );
    if(IDOK != dlg.DoModal()) {
        return;
    }
    // 显示选择的文件内容
    m_strImportFilePath = dlg.GetPathName();
    UpdateData(FALSE);
}


void CDialogDataImpExp::OnBnClickedButtonLookupExport()
{
    UpdateData(TRUE);
    using namespace std;
    static CString strFilename = _T("");
    static TCHAR szFilter[] = _T("Excel(*.xls)|*.xls|所有文件(*.*)|*.*||");
    CFileDialog dlg(FALSE, // 创建文件打开对话框:FALSE保存对话框　
                    _T(".txt"),
                    strFilename.GetString(),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    szFilter
                   );
    if(IDOK != dlg.DoModal()) {
        return;
    }
    // 显示选择的文件内容
    m_strExportFilePath = dlg.GetPathName();
    UpdateData(FALSE);
}


// 查询
void CDialogDataImpExp::OnBnClickedButtonQuery()
{
    UpdateData();
    // 根据code 查询 名称,类别
    std::wstring name, kind;
    CMRKJDatabase::TupleStockInfo info;
    VDayDataEntry v;
    if(!DB.QueryStockInfoByCode(info, m_strCode)) {
        v.clear();
        // 置空 列表 数据
        UpdateListData(v, info);
        return;
    }
    if(DB.QueryDayData(v, m_strCode)) {
        UpdateListData(v, info);
    }
    UpdateData(FALSE);
}

static CString ToStandardDate(const CString& str)
{
    CString s;
    int iStart = 0;
    CString sYear = str.Tokenize(_T("/"), iStart);
    CString sMon  = str.Tokenize(_T("/"), iStart);
    CString sDay  = str.Tokenize(_T("/"), iStart);
    s.Format(_T("%04s/%02s/%02s"), sYear.GetString(), sMon.GetString(), sDay.GetString());
    return s;
}



void CDialogDataImpExp::OnBnClickedButtonImport()
{
    UpdateData(TRUE);
    CString strFilename = m_strImportFilePath;
    CString msg(_T("导入:"));
    if(IDOK != AfxMessageBox(msg + strFilename, MB_OKCANCEL)) {
        return;
    }
    CWnd* pBtn = GetDlgItem(IDC_BUTTON8);
    pBtn->EnableWindow(FALSE);
    // 读取Excel的内容
    vector<vector<CString>> v;
    FromExcel(strFilename.GetString(), v);
    if(v.empty() || v[0].size() < 3) {
        AfxMessageBox(_T("文档中没有数据"));
        goto __End;
    }
    //写入数据库
    {
        auto it = v.begin();
        CString code = v[0][0];
        CString name = v[0][1];
        CString kindName  = v[0][2];
        // 确认 种类id
        int kind_id = 0;
        if(!DB.QueryKindIdByName(kind_id, kindName)) {
            // 那就插入默认值好了
        }
        // 确认表存在
        CMRKJDatabase::TupleStockInfo info = make_tuple(code.GetString() , name.GetString()
                                             , kind_id , kindName.GetString());
        if(!DB.UpdStockInfoOrAddTableByStockCode(info)) {
            CTools::MessageBoxFormat(_T("插入股票信息库失败:%s,%s,%s"), code.GetString(), name.GetString(), kindName.GetString());
            goto __End;
        }
        // 插入数据
        for(auto vLine : v) {
            CDayDataEntry d;
            d.code = vLine[0]; //股票代码
            d.date = ToStandardDate(vLine[3]); //日期
            d.open = vLine[4]; // 开盘
            d.max = vLine[5]; //最高
            d.min = vLine[6]; //最低
            d.close = vLine[7]; //收盘
            d.turnover = vLine[8]; //成交量
            d.tradingVolume = vLine[9]; //成交额
            DB.AddOrUpdateDayDataByDate(d);
        }
    }
__End:
    pBtn->EnableWindow(TRUE);
    UpdateData(FALSE);
}


void CDialogDataImpExp::OnBnClickedButtonExport()
{
    UpdateData(TRUE);
    CString strFilename = m_strExportFilePath;
    CString msg(_T("导出到:"));
    if(IDOK != AfxMessageBox(msg + strFilename, MB_OKCANCEL)) {
        return;
    }
    CWnd* pBtn = GetDlgItem(IDC_BUTTON9);
    pBtn->EnableWindow(FALSE);
    //导出
    {
        // 获得界面上的所有的内容
        vector<vector<CString>> vv;
        for(int i = 0; i < m_lst.GetItemCount(); ++i) {
            vector<CString> v;
            GetListData(v, i);
            vv.push_back(v);
        }
        //写入Excel
        ToExcel(m_strExportFilePath.GetString(), vv);
    }
//__End:
    pBtn->EnableWindow(TRUE);
    UpdateData(FALSE);
}


void CDialogDataImpExp::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if(m_lst.GetSafeHwnd()) {
        // 本窗口客户区的大小
        CRect rcCli;
        GetClientRect(rcCli);
        const size_t offset = 140;
        m_lst.MoveWindow(0, offset, rcCli.Width(), rcCli.Height() - offset);
    }
}


void CDialogDataImpExp::OnBnClickedButtonImportTdx()
{
    auto pBtn = (CButton*)GetDlgItem(IDC_BUTTON_IMPORT_TDX);
    assert(pBtn);
    pBtn->EnableWindow(FALSE);
    CString root = _T("D:\\new_tdx\\T0002\\export\\");
    WIN32_FIND_DATA wfd;
    HANDLE hFind = ::FindFirstFile(root + _T("*.txt"), &wfd);
    int cnt = 0;
    if(INVALID_HANDLE_VALUE == hFind) {
        goto __Cleanup;
    }
    do {
        cnt++;
        CString sFilename = wfd.cFileName;
        pBtn->SetWindowText(sFilename);
        wstring filename((root + _T("\\") + sFilename).GetString());
        wstring code, name;
        ToDatabase(filename, code, name);
    }
    while(0 != FindNextFile(hFind, &wfd));
    //最后显示数据
    {
        CString s;
        s.Format(_T("数量:%d"), cnt);
        pBtn->SetWindowText(s);
    }
__Cleanup:
    pBtn->EnableWindow(TRUE);
}


BOOL CDialogDataImpExp::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
