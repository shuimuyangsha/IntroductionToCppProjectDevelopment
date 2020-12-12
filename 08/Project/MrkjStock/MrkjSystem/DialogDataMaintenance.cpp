// DialogDataMaintenance.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogDataMaintenance.h"
#include "afxdialogex.h"
#include <regex>

using namespace std;

// CDialogDataMaintenance 对话框

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


// CDialogDataMaintenance 消息处理程序

BOOL CDialogDataMaintenance::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 初始化界面上的UI控件
    {
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
            }
        }
        // 2. 按钮初始状态
        {
            //m_btnUpdate.EnableWindow(FALSE);
            //m_btnSave.EnableWindow(FALSE);
        }
        // 3. 种类
        UpdateKindList();
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

// 查询股票日线数据,并显示在列表中
void CDialogDataMaintenance::OnBnClickedButtonQuery()
{
    // 根据股票代码,日期 进行查询
    UpdateData(TRUE);
    using namespace std;
    // 检查股票 代码, 是否合
    if(m_strCode.IsEmpty()) {
        AfxMessageBox(_T("请输入股票代码"));
        m_edtCode.SetFocus();
        return;
    }
    // 查询股票的 代码,名称, 种类, 等数据
    CMRKJDatabase::TupleStockInfo info;
    if(!DB.QueryStockInfoByCode(info, m_strCode)) {
        AfxMessageBox(_T("查询数据失败"));
        return;
    }
    // 设置到界面上面,并更新到变量中
    UpdateStockInfo(info);
    // 查询 日线数据
    VDayDataEntry  v;
    bool b = DB.QueryDayData(v, m_strCode
                             , m_odtDateStart.Format(_T("%Y/%m/%d"))
                             , m_odtDateEnd.Format(_T("%Y/%m/%d")));
    if(!b) {
        AfxMessageBox(_T("查询数据失败"));
        goto __Cleanup;
    }
    // 更新数据列表
    UpdateListData(v, info);
__Cleanup:
    UpdateData(FALSE);
}

// 添加 日 数据
void CDialogDataMaintenance::OnBnClickedButtonAdd()
{
    // 只添加数据
    UpdateData(TRUE);
    //验证
    if(!IsInputValid()) {
        UpdateData(FALSE);
        return;
    }
    // 增加股票数据
    // 2.确保数据没有重复
    if(DB.IsExistDayData(m_strCode.GetString(), m_odtDateStart.Format(_T("%Y/%m/%d")))) {
        AfxMessageBox(_T("数据己经存在"));
        return;
    }
    // 3.增加数据
    {
        CDayDataEntry obj;
        obj.code = m_strCode;
        obj.date = m_odtDateStart.Format(_T("%Y/%m/%d")); //日期
        obj.open = m_strOpen; // 开盘
        obj.max = m_strHigh; //最高
        obj.min = m_strLow; //最低
        obj.close = m_strClose; //收盘
        obj.turnover = m_strTurnover; //成交量
        obj.tradingVolume = m_strTradingvolume; //成交额
        if(!DB.AddDayData(obj)) {
            AfxMessageBox(_T("增加失败"));
            goto __Cleanup;
        }
    }
    // 重新进行查询
    OnBnClickedButtonQuery();
__Cleanup:
    UpdateData(FALSE);
}

// 修改
void CDialogDataMaintenance::OnBnClickedButtonUpdate()
{
    int iItem = -1;
    //获得选中行
    {
        POSITION pos = m_lst.GetFirstSelectedItemPosition();
        if(pos != NULL) {
            iItem = m_lst.GetNextSelectedItem(pos);
        }
        if(iItem < 0 || iItem >= m_lst.GetItemCount()) {
            goto __Cleanup;
        }
    }
    //获得第 iItem行的数据,赋给控件变量
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
        //保存修改之前的日期
        m_strOldDate = data.date.c_str();
        UpdateData(FALSE);
    }
__Cleanup:
    return;
}

// 保存
void CDialogDataMaintenance::OnBnClickedButtonSave()
{
    /*
        把结果保存到数据库中,
        只保存日 数据
    */
    UpdateData(TRUE);
    //验证
    if(!IsInputValid()) {
        UpdateData(FALSE);
        return;
    }
    // 2.确保数据没有重复,除了和自己未保存的外
    {
        CString newDate = m_odtDateStart.Format(_T("%Y/%m/%d"));
        if(newDate == m_strOldDate) {
            //不必检查
        }
        else if(DB.IsExistDayData(m_strCode.GetString(), newDate)) {
            AfxMessageBox(_T("数据己经存在"));
            return;
        }
    }
    // 3.增加数据
    {
        CDayDataEntry obj;
        obj.code = m_strCode;
        obj.date = m_odtDateStart.Format(_T("%Y/%m/%d")); //日期
        obj.open = m_strOpen; // 开盘
        obj.max = m_strHigh; //最高
        obj.min = m_strLow; //最低
        obj.close = m_strClose; //收盘
        obj.turnover = m_strTurnover; //成交量
        obj.tradingVolume = m_strTradingvolume; //成交额
        if(!DB.UpdateDayData(obj, m_strOldDate)) {
            AfxMessageBox(_T("增加失败"));
            goto __Cleanup;
        }
    }
    // 重新进行查询
    OnBnClickedButtonQuery();
__Cleanup:
    UpdateData(FALSE);
}

// 删除
void CDialogDataMaintenance::OnBnClickedButtonDel()
{
    UpdateData(TRUE);
    // 获取所有选中行
    std::stack<int> sIndex;
    {
        POSITION pos = m_lst.GetFirstSelectedItemPosition();
        while(pos) {
            sIndex.push(m_lst.GetNextSelectedItem(pos));
        }
    }
    // 逐个删除数据
    while(!sIndex.empty()) {
        // 取出,移除数据(从栈中)
        int index = sIndex.top();
        sIndex.pop();
        CString date = m_lst.GetItemText(index, EListIndexPrice);
        if(!DB.DelDayData(m_strCode.GetString(), date.GetString())) {
            CTools::MessageBoxFormat(_T("删除[代码:%s, 日期:%s]失败"), m_strCode.GetString(), date.GetString());
            break;
        }
    }
    // 重新查询数据,更新界面表格
    OnBnClickedButtonQuery();
    UpdateData(FALSE);
}

// 更新种类信息
void CDialogDataMaintenance::UpdateKindList()
{
    // 获得选中项
    CString str;
    if(m_cbbKind.GetCurSel() >= 0) {
        m_cbbKind.GetLBText(m_cbbKind.GetCurSel(), str);
    }
    // 删除旧数据
    m_cbbKind.ResetContent();
    // 插入新的数据
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
    // 设置选中项
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

// 更新m_lst中的数据
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

// 获得某行数据
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

// 更新股票信息
void CDialogDataMaintenance::UpdateStockInfo(const CMRKJDatabase::TupleStockInfo& info)
{
    UpdateData(TRUE);
    m_strCode = get<0>(info).c_str();
    m_strName = get<1>(info).c_str();
    // 查找,并赋值
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

// 获得当前选择的 种类id
bool CDialogDataMaintenance::GetKindIdSel(int& id)
{
    int iCur = m_cbbKind.GetCurSel();
    if(iCur >= 0) {
        id = static_cast<int>(m_cbbKind.GetItemData(iCur));
        return true;
    }
    return false;
}

// 检查控件变量的合法性
bool CDialogDataMaintenance::IsInputValid()
{
    try {
        // 开始日期
        COleDateTime::DateTimeStatus s = m_odtDateStart.GetStatus();
        if(s == COleDateTime::invalid) {
            return false;
        }
        // 结束日期
        COleDateTime::DateTimeStatus send = m_odtDateEnd.GetStatus();
        if(send == COleDateTime::invalid) {
            return false;
        }
        // 股票品种
        wregex reg(L"^[+-]?(\\d*.\\d{2}|\\d*)$");
        // 开盘价
        if(!regex_match(m_strOpen.GetString(), reg)) {
            return false;
        }
        // 收盘价
        if(!regex_match(m_strClose.GetString(), reg)) {
            return false;
        }
        // 最高价
        if(!regex_match(m_strHigh.GetString(), reg)) {
            return false;
        }
        // 最低价
        if(!regex_match(m_strLow.GetString(), reg)) {
            return false;
        }
        // 成交量
        if(!regex_match(m_strTurnover.GetString(), reg)) {
            return false;
        }
        // 成交额
        if(!regex_match(m_strTradingvolume.GetString(), reg)) {
            return false;
        }
    }
    catch(...) {
        TRACE("发生异常\r\n");
        return false;
    }
    return true;
}

void CDialogDataMaintenance::OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int iItem = -1;
    //获得选中行
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

//股票查询
void CDialogDataMaintenance::OnBnClickedButtonQueryStockInfo()
{
    UpdateData();
    // 根据code查询
    CMRKJDatabase::TupleStockInfo info;
    if(DB.QueryStockInfoByCode(info, m_strCode)) {
        UpdateStockInfo(info);
    }
    // 根据名字查询
    else if(DB.QueryStockInfoByName(info, m_strName)) {
        UpdateStockInfo(info);
    }
    // 没有找到
    else {
        CTools::MessageBoxFormat(_T("没有此股票相关信息[code:%s, name:%s] ")
                                 , m_strCode.GetString()
                                 , m_strName.GetString());
    }
    UpdateData(FALSE);
}

//股票添加
void CDialogDataMaintenance::OnBnClickedButtonAddStockInfo()
{
    //增加股票,名字种类
    UpdateData(TRUE);
    // 检查合法性
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
    // 增加,如果存在,就更新,不存在就增加并创建 日线数据表
    if(!DB.UpdStockInfoOrAddTableByStockCode(info)) {
        CTools::MessageBoxFormat(_T("创建数据表格'%s'失败"), m_strCode.GetString());
        return;
    }
    // 确保有这个表,并更新股票名字
    if(!DB.MakeSureExitsTable(m_strCode.GetString(), m_strName.GetString())) {
        return;
    }
    UpdateData(FALSE);
}

// 股票修改
void CDialogDataMaintenance::OnBnClickedButtonUpdateStockInfo()
{
    UpdateData(TRUE);
    // 获取选中的 种类id
    int id = 0;
    if(!GetKindIdSel(id)) {
        return;
    }
    CMRKJDatabase::TupleStockInfo info = make_tuple(m_strCode.GetString(), m_strName.GetString(), id, m_strKind.GetString());
    if(!DB.UpdateStockInfoByStockCode(info)) {
        CTools::MessageBoxFormat(_T("修改失败:%s"), m_strCode.GetString());
        return;
    }
    else {
        CTools::MessageBoxFormat(_T("修改成功:%s"), m_strCode.GetString());
    }
    UpdateData(FALSE);
}

// 股票删除
void CDialogDataMaintenance::OnBnClickedButtonDelteStockInfo()
{
    UpdateData();
    // 根据code查询
    CMRKJDatabase::TupleStockInfo info;
    bool b = DB.QueryStockInfoByCode(info, m_strCode) || DB.QueryStockInfoByName(info, m_strName);
    // 没有找到
    if(!b) {
        CTools::MessageBoxFormat(_T("没有此股票相关信息[code:%s, name:%s] ")
                                 , m_strCode.GetString()
                                 , m_strName.GetString());
        return;
    }
    if(b) {
        CString str;
        str.Format(_T("是否删除股票信息[%s]?"), get<1>(info).c_str());
        if(!(IDOK == AfxMessageBox(str, MB_OKCANCEL))) {
            return;
        }
        //删除
        if(!DB.DelStockInfoByCode(get<0>(info).c_str())) {
            CTools::MessageBoxFormat(_T("删除失败[code:%s, name:%s] ")
                                     , get<0>(info).c_str()
                                     , get<1>(info).c_str());
        }
        //信息更新
        UpdateStockInfo(make_tuple(L"", L"", -1, L""));
        m_lst.DeleteAllItems();
    }
    UpdateData(FALSE);
}

void CDialogDataMaintenance::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if(m_lst.GetSafeHwnd()) {
        // 本窗口客户区的大小
        CRect rcCli;
        GetClientRect(rcCli);
        m_lst.MoveWindow(0, 200, rcCli.Width(), rcCli.Height() - 200);
    }
}


void CDialogDataMaintenance::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnOK();
}


void CDialogDataMaintenance::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CDialogEx::OnCancel();
}


BOOL CDialogDataMaintenance::PreTranslateMessage(MSG* pMsg)
{
    // 防止 ESC/回车键 自动退出
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // 返回非0,代表此消息不继续进行分发
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
