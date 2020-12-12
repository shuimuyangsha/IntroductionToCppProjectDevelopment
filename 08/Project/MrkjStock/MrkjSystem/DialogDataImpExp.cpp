// DialogDataImpExp.cpp : ʵ���ļ�
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

// CDialogDataImpExp �Ի���

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


// CDialogDataImpExp ��Ϣ�������


void CDialogDataImpExp::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogDataImpExp::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


BOOL CDialogDataImpExp::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ��ʼ�������ϵ�UI�ؼ�
    {
        // 0. �����ڿͻ����Ĵ�С
        CRect rcCli;
        GetClientRect(rcCli);
        // 1. CListCtrl �ؼ�
        {
            LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
            lStyle &= ~LVS_TYPEMASK;
            lStyle |= LVS_REPORT;
            SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
            DWORD dwStyle = m_lst.GetExtendedStyle();
            dwStyle |= LVS_EX_FULLROWSELECT; //ѡ���� ��������
            dwStyle |= LVS_EX_GRIDLINES; //������
            m_lst.SetExtendedStyle(dwStyle);
            // ������,�����ô�С
            {
                CRect rc;
                m_lst.GetClientRect(rc);
                m_lst.InsertColumn(EListIndexCode, _T("��Ʊ����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexName, _T("��Ʊ����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexUpPercent, _T("��Ʊ���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexPrice, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexOpen, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexClose, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMax, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMin, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTurnover, _T("�ɽ���(��)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTradingVolume, _T("�ɽ���(Ԫ)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            }
        }
    }
    // �������ģʽ,��ʾ����ͨ�������ݵİ�ť
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
    // �쳣: OCX ����ҳӦ���� FALSE
}


// ����m_lst�е�����
void CDialogDataImpExp::UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info)
{
    m_lst.DeleteAllItems();
    for each(auto data in v) {
        int index = m_lst.GetItemCount();
        CString strNo;
        strNo.Format(_T("%04d"), index + 1);
        m_lst.InsertItem(index, get<0>(info).c_str()); //����
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

// ���ĳ������
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
    //��ȡ�ļ�����
    bool GetFileContent(const std::wstring& filename, std::wstring& code
                        , std::wstring& name, VDayDataEntry& vRes)
    {
        using namespace std;
        using namespace StringHelper;
        string line;
        bool bRet = false;
        ifstream file(UnicodeToANSI(filename), ios::in);
        if(file.bad()) {
            AfxMessageBox(_T("���ļ�ʧ��"));
            goto __Cleanup;
        }
        // ��ȡ��һ�У���ô���ͳ�
        if(!getline(file, line)) {
            AfxMessageBox(_T("�ļ���û������"));
            goto __Cleanup;
        }
        //��ֻ��
        {
            int index0 = 0;
            int index1 = line.find(' ', 0);
            int index2 = line.find("����");
            if(index1 < 0 || index2 <= 0 || index1 >= index2) {
                AfxMessageBox(_T("��ȡ��Ʊ���ƺʹ���ʧ�ܣ������ļ���ʽ"));
                goto __Cleanup;
            }
            code = ANSIToUnicode(line.substr(0, index1));
            name = ANSIToUnicode(line.substr(1 + index1, index2 - 1 - index1));
            if(code.empty() || name.empty()) {
                AfxMessageBox(_T("��ȡ��Ʊ���ƺʹ���ʧ�ܣ������ļ���ʽ"));
                goto __Cleanup;
            }
        }
        // Խ���ڶ���(����)
        if(!getline(file, line)) {
            goto __Cleanup;
        }
        {
            // ��ȡ�����
            std::deque<string> content;
            while(getline(file, line)) {
                content.push_back(line);
            }
            //��ɾ�����һ�У���û�õ����ݣ�
            if(!content.empty()) {
                content.pop_back();
            }
            // ���û������,ֱ�ӷ���
            if(content.empty()) {
                goto __Cleanup;
            }
            //��������֣������ݣ��������ݿ�
            for(auto it = content.begin(); it != content.end(); ++it) {
                vector<string> v;
                StringHelper::SplitString(v, *it, "\t");
                if(v.size() != 7) {
                    TRACE("���ݲ�����Ҫ��");
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

    //��ȡ�ļ����� ֮ code, name
    bool GetFileContentCodeName(const std::wstring& filename, std::wstring& code, std::wstring& name)
    {
        using namespace std;
        using namespace StringHelper;
        string line;
        bool bRet = false;
        ifstream file(UnicodeToANSI(filename), ios::in);
        if(file.bad()) {
            AfxMessageBox(_T("���ļ�ʧ��"));
            goto __Cleanup;
        }
        // ��ȡ��һ�У���ô���ͳ�
        if(!getline(file, line)) {
            AfxMessageBox(_T("�ļ���û������"));
            goto __Cleanup;
        }
        //��ֻ��
        {
            int index0 = 0;
            int index1 = line.find(' ', 0);
            int index2 = line.find("����");
            if(index1 < 0 || index2 <= 0 || index1 >= index2) {
                AfxMessageBox(_T("��ȡ��Ʊ���ƺʹ���ʧ�ܣ������ļ���ʽ"));
                goto __Cleanup;
            }
            code = ANSIToUnicode(line.substr(0, index1));
            name = ANSIToUnicode(line.substr(1 + index1, index2 - 1 - index1));
            if(code.empty() || name.empty()) {
                AfxMessageBox(_T("��ȡ��Ʊ���ƺʹ���ʧ�ܣ������ļ���ʽ"));
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

    // �ļ����ݵ������ݿ�
    void ToDatabase(const std::wstring& filename, std::wstring& code, std::wstring& name)
    {
        using namespace std;
        //����ȡ�ļ�����
        VDayDataEntry v;
        if(!GetFileContent(filename, code, name, v)) {
            return;
        }
        // ȷ�ϴ��� table ��������ڴ���
        if(!DB.MakeSureExitsTable(code.c_str(), name.c_str())) {
            return;
        }
        DB.AddDayData(v);
    }

    // �ļ����ݵ������ݿ�: CODE/NAME -> T_CODE_NAME;
    void ToDatabaseCodeName(const std::wstring& filename)
    {
        using namespace std;
        //����ȡ�ļ�����
        wstring code, name;
        VDayDataEntry v;
        if(!GetFileContent(filename, code, name, v)) {
            return;
        }
        // ȷ�ϴ��� table ��������ڴ���
        if(!DB.MakeDayTable(code)) {
            return;
        }
        DB.AddStock(code.c_str(), name.c_str());
    }

    using namespace std;

    CString headers[] = { _T("��Ʊ����"), _T("��Ʊ����"), _T("��Ʊ���"), _T("����"), _T("����")
                          , _T("����"), _T("���"), _T("���"), _T("�ɽ���"), _T("�ɽ���")
                        };

    // ����д��Excel
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
            CTools::MessageBoxFormat(_T("���� �ӿ�ʧ��"));
            return;
        }
        //���� һ��һ��Ļ�ò�������
        books = app.get_Workbooks();
        book = books.Add(covOptional);
        sheets = book.get_Worksheets();
        sheet = sheets.get_Item(COleVariant((short)1));
        // ��ʾ
        //app.put_Visible(TRUE);
        range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1")));
        // ��ʾд����ǵڼ���
        _variant_t varRoleIndex(1);
        //д���ͷ
        {
            for(int i = 0; i < _countof(headers); ++i) {
                // �ڼ���
                _variant_t varColumnIndex((long)i + 1);
                range.put_NumberFormat(COleVariant(L"@"));
                range.put_Item(varRoleIndex, varColumnIndex, _variant_t(headers[i].GetString()));
            }
        }
        //д������
        {
            for(auto vline : v) {
                varRoleIndex.intVal += 1;
                for(unsigned i = 0; i != vline.size(); ++i) {
                    // �ڼ���
                    _variant_t varColumnIndex((long)i + 1);
                    range.put_NumberFormat(COleVariant(L"@"));
                    range.put_Item(varRoleIndex, varColumnIndex, _variant_t(vline[i].GetString()));
                }
            }
        }
        // ����
        book.SaveCopyAs(COleVariant(strFile));
        book.put_Saved(true);
        //�ͷ�
        range.ReleaseDispatch();
        book.ReleaseDispatch();
        books.ReleaseDispatch();
        sheet.ReleaseDispatch();
        sheets.ReleaseDispatch();
        app.Quit();
        app.ReleaseDispatch();
    }

    // ��Excel�ж�ȡ����
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
            CTools::MessageBoxFormat(_T("���� �ӿ�ʧ��"));
            return;
        }
        //���� һ��һ��Ļ�ò�������
        books = app.get_Workbooks();
        book = books.Open(strFile, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
        sheets = book.get_Worksheets();
        sheet = sheets.get_Item(COleVariant((short)1));
        // �õ�ʹ�õ�
        range = sheet.get_UsedRange();
        // ����
        range = range.get_Rows();
        long rows = range.get_Count();
        // ����
        range = range.get_Columns();
        long cols = range.get_Count();
        if(_countof(headers) != cols) {
            // ������������
            goto __Cleanup;
        }
        //�õ����е� Cell
        range = sheet.get_Cells();
        //��(ע���±��1��ʼ)
        for(int i = 2/*��Ҫ��ͷ��һ��*/; i <= rows; ++i) {
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
        //�ͷ�
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
    /* �����ļ�Ϊ UTF8 ��BOM ����
       ��ʽ��:
     */
    using namespace std;
    static CString strFilename = _T("");
    static TCHAR szFilter[] = _T("Excel�ļ�(*.xls)|*.xls|�����ļ�(*.*)|*.*||");
    CFileDialog dlg(TRUE, // �����ļ��򿪶Ի���:FALSE����Ի���
                    _T(".txt"),
                    strFilename.GetString(),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    szFilter
                   );
    if(IDOK != dlg.DoModal()) {
        return;
    }
    // ��ʾѡ����ļ�����
    m_strImportFilePath = dlg.GetPathName();
    UpdateData(FALSE);
}


void CDialogDataImpExp::OnBnClickedButtonLookupExport()
{
    UpdateData(TRUE);
    using namespace std;
    static CString strFilename = _T("");
    static TCHAR szFilter[] = _T("Excel(*.xls)|*.xls|�����ļ�(*.*)|*.*||");
    CFileDialog dlg(FALSE, // �����ļ��򿪶Ի���:FALSE����Ի���
                    _T(".txt"),
                    strFilename.GetString(),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    szFilter
                   );
    if(IDOK != dlg.DoModal()) {
        return;
    }
    // ��ʾѡ����ļ�����
    m_strExportFilePath = dlg.GetPathName();
    UpdateData(FALSE);
}


// ��ѯ
void CDialogDataImpExp::OnBnClickedButtonQuery()
{
    UpdateData();
    // ����code ��ѯ ����,���
    std::wstring name, kind;
    CMRKJDatabase::TupleStockInfo info;
    VDayDataEntry v;
    if(!DB.QueryStockInfoByCode(info, m_strCode)) {
        v.clear();
        // �ÿ� �б� ����
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
    CString msg(_T("����:"));
    if(IDOK != AfxMessageBox(msg + strFilename, MB_OKCANCEL)) {
        return;
    }
    CWnd* pBtn = GetDlgItem(IDC_BUTTON8);
    pBtn->EnableWindow(FALSE);
    // ��ȡExcel������
    vector<vector<CString>> v;
    FromExcel(strFilename.GetString(), v);
    if(v.empty() || v[0].size() < 3) {
        AfxMessageBox(_T("�ĵ���û������"));
        goto __End;
    }
    //д�����ݿ�
    {
        auto it = v.begin();
        CString code = v[0][0];
        CString name = v[0][1];
        CString kindName  = v[0][2];
        // ȷ�� ����id
        int kind_id = 0;
        if(!DB.QueryKindIdByName(kind_id, kindName)) {
            // �ǾͲ���Ĭ��ֵ����
        }
        // ȷ�ϱ����
        CMRKJDatabase::TupleStockInfo info = make_tuple(code.GetString() , name.GetString()
                                             , kind_id , kindName.GetString());
        if(!DB.UpdStockInfoOrAddTableByStockCode(info)) {
            CTools::MessageBoxFormat(_T("�����Ʊ��Ϣ��ʧ��:%s,%s,%s"), code.GetString(), name.GetString(), kindName.GetString());
            goto __End;
        }
        // ��������
        for(auto vLine : v) {
            CDayDataEntry d;
            d.code = vLine[0]; //��Ʊ����
            d.date = ToStandardDate(vLine[3]); //����
            d.open = vLine[4]; // ����
            d.max = vLine[5]; //���
            d.min = vLine[6]; //���
            d.close = vLine[7]; //����
            d.turnover = vLine[8]; //�ɽ���
            d.tradingVolume = vLine[9]; //�ɽ���
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
    CString msg(_T("������:"));
    if(IDOK != AfxMessageBox(msg + strFilename, MB_OKCANCEL)) {
        return;
    }
    CWnd* pBtn = GetDlgItem(IDC_BUTTON9);
    pBtn->EnableWindow(FALSE);
    //����
    {
        // ��ý����ϵ����е�����
        vector<vector<CString>> vv;
        for(int i = 0; i < m_lst.GetItemCount(); ++i) {
            vector<CString> v;
            GetListData(v, i);
            vv.push_back(v);
        }
        //д��Excel
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
        // �����ڿͻ����Ĵ�С
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
    //�����ʾ����
    {
        CString s;
        s.Format(_T("����:%d"), cnt);
        pBtn->SetWindowText(s);
    }
__Cleanup:
    pBtn->EnableWindow(TRUE);
}


BOOL CDialogDataImpExp::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
