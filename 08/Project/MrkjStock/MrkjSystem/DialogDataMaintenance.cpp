// DialogDataMaintenance.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogDataMaintenance.h"
#include "afxdialogex.h"
#include <regex>

using namespace std;

// CDialogDataMaintenance �Ի���

IMPLEMENT_DYNAMIC(CDialogDataMaintenance, CDialogEx)

CDialogDataMaintenance::CDialogDataMaintenance(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_DATA_MAINTENANCE, pParent)
    , m_strCode(_T(""))
    , m_strName(_T(""))
    , m_odtDateStart(COleDateTime::GetCurrentTime() - COleDateTimeSpan(10 * 365, 0, 0, 0))
    , m_odtDateEnd(COleDateTime::GetCurrentTime())
    , m_strKind(_T(""))
    , m_strOpen(_T(""))
    , m_strClose(_T(""))
    , m_strHigh(_T(""))
    , m_strLow(_T(""))
    , m_strTurnover(_T(""))
    , m_strTradingvolume(_T(""))
{
}

CDialogDataMaintenance::~CDialogDataMaintenance()
{
}

void CDialogDataMaintenance::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_CODE, m_edtCode);
    DDX_Text(pDX, IDC_EDIT_CODE, m_strCode);
    DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Control(pDX, IDC_DATETIMEPICKER_DATESTART, m_dtcDateStart);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATEEND, m_odtDateEnd);
    DDX_Control(pDX, IDC_DATETIMEPICKER_DATEEND, m_dtcDateEnd);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATESTART, m_odtDateStart);
    DDX_Control(pDX, IDC_COMBO_KIND, m_cbbKind);
    DDX_CBString(pDX, IDC_COMBO_KIND, m_strKind);
    DDX_Control(pDX, IDC_EDIT_OPEN, m_edtOpen);
    DDX_Text(pDX, IDC_EDIT_OPEN, m_strOpen);
    DDX_Control(pDX, IDC_EDIT_CLOSE, m_edtClose);
    DDX_Text(pDX, IDC_EDIT_CLOSE, m_strClose);
    DDX_Control(pDX, IDC_EDIT_HIGH, m_edtHigh);
    DDX_Text(pDX, IDC_EDIT_HIGH, m_strHigh);
    DDX_Control(pDX, IDC_EDIT_LOW, m_edtLow);
    DDX_Text(pDX, IDC_EDIT_LOW, m_strLow);
    DDX_Control(pDX, IDC_EDIT_TURNOVER, m_edtTurnover);
    DDX_Text(pDX, IDC_EDIT_TURNOVER, m_strTurnover);
    DDX_Control(pDX, IDC_EDIT_TRADEVOLUME, m_edtTradingvolume);
    DDX_Text(pDX, IDC_EDIT_TRADEVOLUME, m_strTradingvolume);
    DDX_Control(pDX, IDC_LIST_DATA, m_lst);
    DDX_Control(pDX, IDC_BUTTON_UPDATE, m_btnUpdate);
    DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
}

BEGIN_MESSAGE_MAP(CDialogDataMaintenance, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDialogDataMaintenance::OnBnClickedButtonQuery)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CDialogDataMaintenance::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDialogDataMaintenance::OnBnClickedButtonUpdate)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogDataMaintenance::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_DEL, &CDialogDataMaintenance::OnBnClickedButtonDel)
    ON_NOTIFY(NM_CLICK, IDC_LIST_DATA, &CDialogDataMaintenance::OnNMClickListData)
    ON_BN_CLICKED(IDC_BUTTON_QUERY2, &CDialogDataMaintenance::OnBnClickedButtonQueryStockInfo)
    ON_BN_CLICKED(IDC_BUTTON_ADD2, &CDialogDataMaintenance::OnBnClickedButtonAddStockInfo)
    ON_BN_CLICKED(IDC_BUTTON_UPDATE2, &CDialogDataMaintenance::OnBnClickedButtonUpdateStockInfo)
    ON_BN_CLICKED(IDC_BUTTON_DEL2, &CDialogDataMaintenance::OnBnClickedButtonDelteStockInfo)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogDataMaintenance ��Ϣ�������

BOOL CDialogDataMaintenance::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ��ʼ�������ϵ�UI�ؼ�
    {
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
                m_lst.InsertColumn(EListIndexCode, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexName, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexUpPercent, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexPrice, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexOpen, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexClose, _T("����"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMax, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexMin, _T("���"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTurnover, _T("�ɽ���(��)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
                m_lst.InsertColumn(EListIndexTradingVolume, _T("�ɽ���(Ԫ)"), LVCFMT_LEFT, rc.Width() / EListIndexMaxLimit);
            }
        }
        // 2. ��ť��ʼ״̬
        {
            //m_btnUpdate.EnableWindow(FALSE);
            //m_btnSave.EnableWindow(FALSE);
        }
        // 3. ����
        UpdateKindList();
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

// ��ѯ��Ʊ��������,����ʾ���б���
void CDialogDataMaintenance::OnBnClickedButtonQuery()
{
    // ���ݹ�Ʊ����,���� ���в�ѯ
    UpdateData(TRUE);
    using namespace std;
    // ����Ʊ ����, �Ƿ��
    if(m_strCode.IsEmpty()) {
        AfxMessageBox(_T("�������Ʊ����"));
        m_edtCode.SetFocus();
        return;
    }
    // ��ѯ��Ʊ�� ����,����, ����, ������
    CMRKJDatabase::TupleStockInfo info;
    if(!DB.QueryStockInfoByCode(info, m_strCode)) {
        AfxMessageBox(_T("��ѯ����ʧ��"));
        return;
    }
    // ���õ���������,�����µ�������
    UpdateStockInfo(info);
    // ��ѯ ��������
    VDayDataEntry  v;
    bool b = DB.QueryDayData(v, m_strCode
                             , m_odtDateStart.Format(_T("%Y/%m/%d"))
                             , m_odtDateEnd.Format(_T("%Y/%m/%d")));
    if(!b) {
        AfxMessageBox(_T("��ѯ����ʧ��"));
        goto __Cleanup;
    }
    // ���������б�
    UpdateListData(v, info);
__Cleanup:
    UpdateData(FALSE);
}

// ��� �� ����
void CDialogDataMaintenance::OnBnClickedButtonAdd()
{
    // ֻ�������
    UpdateData(TRUE);
    //��֤
    if(!IsInputValid()) {
        UpdateData(FALSE);
        return;
    }
    // ���ӹ�Ʊ����
    // 2.ȷ������û���ظ�
    if(DB.IsExistDayData(m_strCode.GetString(), m_odtDateStart.Format(_T("%Y/%m/%d")))) {
        AfxMessageBox(_T("���ݼ�������"));
        return;
    }
    // 3.��������
    {
        CDayDataEntry obj;
        obj.code = m_strCode;
        obj.date = m_odtDateStart.Format(_T("%Y/%m/%d")); //����
        obj.open = m_strOpen; // ����
        obj.max = m_strHigh; //���
        obj.min = m_strLow; //���
        obj.close = m_strClose; //����
        obj.turnover = m_strTurnover; //�ɽ���
        obj.tradingVolume = m_strTradingvolume; //�ɽ���
        if(!DB.AddDayData(obj)) {
            AfxMessageBox(_T("����ʧ��"));
            goto __Cleanup;
        }
    }
    // ���½��в�ѯ
    OnBnClickedButtonQuery();
__Cleanup:
    UpdateData(FALSE);
}

// �޸�
void CDialogDataMaintenance::OnBnClickedButtonUpdate()
{
    int iItem = -1;
    //���ѡ����
    {
        POSITION pos = m_lst.GetFirstSelectedItemPosition();
        if(pos != NULL) {
            iItem = m_lst.GetNextSelectedItem(pos);
        }
        if(iItem < 0 || iItem >= m_lst.GetItemCount()) {
            goto __Cleanup;
        }
    }
    //��õ� iItem�е�����,�����ؼ�����
    {
        CDayDataEntry data;
        GetListData(data, iItem);
        m_strCode = data.code.c_str();
        m_odtDateStart.ParseDateTime(data.date.c_str());
        m_odtDateEnd = m_odtDateStart;
        m_strOpen = data.open.c_str();
        m_strClose = data.close.c_str();
        m_strHigh = data.max.c_str();
        m_strLow = data.min.c_str();
        m_strTurnover = data.turnover.c_str();
        m_strTradingvolume = data.tradingVolume.c_str();
        //�����޸�֮ǰ������
        m_strOldDate = data.date.c_str();
        UpdateData(FALSE);
    }
__Cleanup:
    return;
}

// ����
void CDialogDataMaintenance::OnBnClickedButtonSave()
{
    /*
        �ѽ�����浽���ݿ���,
        ֻ������ ����
    */
    UpdateData(TRUE);
    //��֤
    if(!IsInputValid()) {
        UpdateData(FALSE);
        return;
    }
    // 2.ȷ������û���ظ�,���˺��Լ�δ�������
    {
        CString newDate = m_odtDateStart.Format(_T("%Y/%m/%d"));
        if(newDate == m_strOldDate) {
            //���ؼ��
        }
        else if(DB.IsExistDayData(m_strCode.GetString(), newDate)) {
            AfxMessageBox(_T("���ݼ�������"));
            return;
        }
    }
    // 3.��������
    {
        CDayDataEntry obj;
        obj.code = m_strCode;
        obj.date = m_odtDateStart.Format(_T("%Y/%m/%d")); //����
        obj.open = m_strOpen; // ����
        obj.max = m_strHigh; //���
        obj.min = m_strLow; //���
        obj.close = m_strClose; //����
        obj.turnover = m_strTurnover; //�ɽ���
        obj.tradingVolume = m_strTradingvolume; //�ɽ���
        if(!DB.UpdateDayData(obj, m_strOldDate)) {
            AfxMessageBox(_T("����ʧ��"));
            goto __Cleanup;
        }
    }
    // ���½��в�ѯ
    OnBnClickedButtonQuery();
__Cleanup:
    UpdateData(FALSE);
}

// ɾ��
void CDialogDataMaintenance::OnBnClickedButtonDel()
{
    UpdateData(TRUE);
    // ��ȡ����ѡ����
    std::stack<int> sIndex;
    {
        POSITION pos = m_lst.GetFirstSelectedItemPosition();
        while(pos) {
            sIndex.push(m_lst.GetNextSelectedItem(pos));
        }
    }
    // ���ɾ������
    while(!sIndex.empty()) {
        // ȡ��,�Ƴ�����(��ջ��)
        int index = sIndex.top();
        sIndex.pop();
        CString date = m_lst.GetItemText(index, EListIndexPrice);
        if(!DB.DelDayData(m_strCode.GetString(), date.GetString())) {
            CTools::MessageBoxFormat(_T("ɾ��[����:%s, ����:%s]ʧ��"), m_strCode.GetString(), date.GetString());
            break;
        }
    }
    // ���²�ѯ����,���½�����
    OnBnClickedButtonQuery();
    UpdateData(FALSE);
}

// ����������Ϣ
void CDialogDataMaintenance::UpdateKindList()
{
    // ���ѡ����
    CString str;
    if(m_cbbKind.GetCurSel() >= 0) {
        m_cbbKind.GetLBText(m_cbbKind.GetCurSel(), str);
    }
    // ɾ��������
    m_cbbKind.ResetContent();
    // �����µ�����
    {
        typedef vector<tuple<int, wstring>> Tv;
        Tv v;
        if(DB.QueryKind(v)) {
            for(size_t i = 0; i != v.size(); ++i) {
                int index = m_cbbKind.AddString(get<1>(v[i]).c_str());
                m_cbbKind.SetItemData(index, (DWORD_PTR)get<0>(v[i]));
            }
        }
    }
    // ����ѡ����
    if(m_cbbKind.GetCount() > 0) {
        m_cbbKind.SetCurSel(0);
    }
    for(int i = 0; i < m_cbbKind.GetCount(); ++i) {
        CString s;
        m_cbbKind.GetLBText(i, s);
        if(s == str) {
            m_cbbKind.SetCurSel(i);
            break;
        }
    }
}

// ����m_lst�е�����
void CDialogDataMaintenance::UpdateListData(const VDayDataEntry& v, const CMRKJDatabase::TupleStockInfo& info)
{
    m_lst.DeleteAllItems();
    for each(auto data in v) {
        int index = m_lst.GetItemCount();
        m_lst.InsertItem(index, get<0>(info).c_str());
        //m_lst.SetItemText(index, EListIndexCode, code.c_str());
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
void CDialogDataMaintenance::GetListData(CDayDataEntry& data, int index)
{
    assert(index >= 0 && index < m_lst.GetItemCount());
    UpdateData(TRUE);
    data.code = m_strCode;
    data.date = m_lst.GetItemText(index, EListIndexPrice);
    data.date = m_lst.GetItemText(index, EListIndexPrice);
    data.open = m_lst.GetItemText(index, EListIndexOpen);
    data.close = m_lst.GetItemText(index, EListIndexClose);
    data.max = m_lst.GetItemText(index, EListIndexMax);
    data.min = m_lst.GetItemText(index, EListIndexMin);
    data.turnover = m_lst.GetItemText(index, EListIndexTurnover);
    data.tradingVolume = m_lst.GetItemText(index, EListIndexTradingVolume);
}

// ���¹�Ʊ��Ϣ
void CDialogDataMaintenance::UpdateStockInfo(const CMRKJDatabase::TupleStockInfo& info)
{
    UpdateData(TRUE);
    m_strCode = get<0>(info).c_str();
    m_strName = get<1>(info).c_str();
    // ����,����ֵ
    for(int index = 0; index < m_cbbKind.GetCount(); ++index) {
        int id = m_cbbKind.GetItemData(index);
        if(id == get<2>(info)) {
            m_strKind = get<3>(info).c_str();
            m_cbbKind.SetCurSel(index);
            break;
        }
    }
    UpdateData(FALSE);
}

// ��õ�ǰѡ��� ����id
bool CDialogDataMaintenance::GetKindIdSel(int& id)
{
    int iCur = m_cbbKind.GetCurSel();
    if(iCur >= 0) {
        id = static_cast<int>(m_cbbKind.GetItemData(iCur));
        return true;
    }
    return false;
}

// ���ؼ������ĺϷ���
bool CDialogDataMaintenance::IsInputValid()
{
    try {
        // ��ʼ����
        COleDateTime::DateTimeStatus s = m_odtDateStart.GetStatus();
        if(s == COleDateTime::invalid) {
            return false;
        }
        // ��������
        COleDateTime::DateTimeStatus send = m_odtDateEnd.GetStatus();
        if(send == COleDateTime::invalid) {
            return false;
        }
        // ��ƱƷ��
        wregex reg(L"^[+-]?(\\d*.\\d{2}|\\d*)$");
        // ���̼�
        if(!regex_match(m_strOpen.GetString(), reg)) {
            return false;
        }
        // ���̼�
        if(!regex_match(m_strClose.GetString(), reg)) {
            return false;
        }
        // ��߼�
        if(!regex_match(m_strHigh.GetString(), reg)) {
            return false;
        }
        // ��ͼ�
        if(!regex_match(m_strLow.GetString(), reg)) {
            return false;
        }
        // �ɽ���
        if(!regex_match(m_strTurnover.GetString(), reg)) {
            return false;
        }
        // �ɽ���
        if(!regex_match(m_strTradingvolume.GetString(), reg)) {
            return false;
        }
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

void CDialogDataMaintenance::OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int iItem = -1;
    //���ѡ����
    {
        POSITION pos = m_lst.GetFirstSelectedItemPosition();
        if(pos != NULL) {
            iItem = m_lst.GetNextSelectedItem(pos);
        }
        if(iItem < 0 || iItem >= m_lst.GetItemCount()) {
            goto __Cleanup;
        }
    }
__Cleanup:
    *pResult = 0;
}

//��Ʊ��ѯ
void CDialogDataMaintenance::OnBnClickedButtonQueryStockInfo()
{
    UpdateData();
    // ����code��ѯ
    CMRKJDatabase::TupleStockInfo info;
    if(DB.QueryStockInfoByCode(info, m_strCode)) {
        UpdateStockInfo(info);
    }
    // �������ֲ�ѯ
    else if(DB.QueryStockInfoByName(info, m_strName)) {
        UpdateStockInfo(info);
    }
    // û���ҵ�
    else {
        CTools::MessageBoxFormat(_T("û�д˹�Ʊ�����Ϣ[code:%s, name:%s] ")
                                 , m_strCode.GetString()
                                 , m_strName.GetString());
    }
    UpdateData(FALSE);
}

//��Ʊ���
void CDialogDataMaintenance::OnBnClickedButtonAddStockInfo()
{
    //���ӹ�Ʊ,��������
    UpdateData(TRUE);
    // ���Ϸ���
    if(m_strCode.IsEmpty()) {
        m_edtCode.SetFocus();
        return;
    }
    int id = 0;
    if(!GetKindIdSel(id)) {
        m_cbbKind.ShowDropDown(TRUE);
        return;
    }
    CMRKJDatabase::TupleStockInfo info = make_tuple(m_strCode.GetString(), m_strName.GetString(), id, m_strKind.GetString());
    // ����,�������,�͸���,�����ھ����Ӳ����� �������ݱ�
    if(!DB.UpdStockInfoOrAddTableByStockCode(info)) {
        CTools::MessageBoxFormat(_T("�������ݱ��'%s'ʧ��"), m_strCode.GetString());
        return;
    }
    // ȷ���������,�����¹�Ʊ����
    if(!DB.MakeSureExitsTable(m_strCode.GetString(), m_strName.GetString())) {
        return;
    }
    UpdateData(FALSE);
}

// ��Ʊ�޸�
void CDialogDataMaintenance::OnBnClickedButtonUpdateStockInfo()
{
    UpdateData(TRUE);
    // ��ȡѡ�е� ����id
    int id = 0;
    if(!GetKindIdSel(id)) {
        return;
    }
    CMRKJDatabase::TupleStockInfo info = make_tuple(m_strCode.GetString(), m_strName.GetString(), id, m_strKind.GetString());
    if(!DB.UpdateStockInfoByStockCode(info)) {
        CTools::MessageBoxFormat(_T("�޸�ʧ��:%s"), m_strCode.GetString());
        return;
    }
    else {
        CTools::MessageBoxFormat(_T("�޸ĳɹ�:%s"), m_strCode.GetString());
    }
    UpdateData(FALSE);
}

// ��Ʊɾ��
void CDialogDataMaintenance::OnBnClickedButtonDelteStockInfo()
{
    UpdateData();
    // ����code��ѯ
    CMRKJDatabase::TupleStockInfo info;
    bool b = DB.QueryStockInfoByCode(info, m_strCode) || DB.QueryStockInfoByName(info, m_strName);
    // û���ҵ�
    if(!b) {
        CTools::MessageBoxFormat(_T("û�д˹�Ʊ�����Ϣ[code:%s, name:%s] ")
                                 , m_strCode.GetString()
                                 , m_strName.GetString());
        return;
    }
    if(b) {
        CString str;
        str.Format(_T("�Ƿ�ɾ����Ʊ��Ϣ[%s]?"), get<1>(info).c_str());
        if(!(IDOK == AfxMessageBox(str, MB_OKCANCEL))) {
            return;
        }
        //ɾ��
        if(!DB.DelStockInfoByCode(get<0>(info).c_str())) {
            CTools::MessageBoxFormat(_T("ɾ��ʧ��[code:%s, name:%s] ")
                                     , get<0>(info).c_str()
                                     , get<1>(info).c_str());
        }
        //��Ϣ����
        UpdateStockInfo(make_tuple(L"", L"", -1, L""));
        m_lst.DeleteAllItems();
    }
    UpdateData(FALSE);
}

void CDialogDataMaintenance::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if(m_lst.GetSafeHwnd()) {
        // �����ڿͻ����Ĵ�С
        CRect rcCli;
        GetClientRect(rcCli);
        m_lst.MoveWindow(0, 200, rcCli.Width(), rcCli.Height() - 200);
    }
}


void CDialogDataMaintenance::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogDataMaintenance::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


BOOL CDialogDataMaintenance::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
