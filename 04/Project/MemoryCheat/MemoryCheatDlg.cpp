
// MemoryCheatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "MemoryCheatDlg.h"
#include "afxdialogex.h"
#include "DialogProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �ص����� ָʾ �Ƿ���� ����
static bool *g_pGoon = nullptr;

// �����б�ķ�Χ(0 ~ rage)
static int range = 100;

// �״�ɨ��ص�����:
bool __stdcall FirstSearchRoutine(void *pArgs, size_t nPageCount, size_t index)
{
    CProgressCtrl *p = (CProgressCtrl *)pArgs;
    if(nPageCount == 0) {
        return *g_pGoon;
    }

    p->SetPos(static_cast<int>(index / (nPageCount / float(range))));
    return *g_pGoon;
}

// �´�ɨ��ص�����
bool __stdcall NextSearchRoutine(void *pArgs, size_t addrCount, size_t index)
{
    CProgressCtrl *p = (CProgressCtrl *)pArgs;
    if(addrCount == 0) {
        return *g_pGoon;
    }

    p->SetPos(static_cast<int>(index / (addrCount / float(range))));
    return *g_pGoon;
}


CMemoryCheatDlg::CMemoryCheatDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_MEMORYCHEAT_DIALOG, pParent)
    , m_strSearchValue(_T(""))
    , m_strValueType(_T(""))
    , m_strLimitBegin(_T("0x00000000"))
    , m_strLimitEnd(_T("0x7FFFffff"))
    , m_strDesEdit(_T(""))
    , m_strValueEdit(_T(""))
    , m_strValueTypeEdit(_T(""))
    , m_strAddressEdit(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    g_pGoon = &m_bGoon;
}

void CMemoryCheatDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ADDRESS_TEMP, m_lstAddressTemp);
    DDX_Control(pDX, IDC_LIST_ADDRESS_TARGET, m_lstAddressTarget);
    DDX_Text(pDX, IDC_EDIT_SEARCH_VALUE, m_strSearchValue);
    DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE, m_strValueType);
    DDX_Control(pDX, IDC_COMBO_VALUE_TYPE, m_cbbValueType);
    DDX_Text(pDX, IDC_EDIT_LIMIT_START, m_strLimitBegin);
    DDX_Text(pDX, IDC_EDIT_LIMIT_END, m_strLimitEnd);
    DDX_Control(pDX, IDC_PROGRESS_SEARCH, m_pProcess);
    DDX_Text(pDX, IDC_EDIT_DES, m_strDesEdit);
    DDX_Text(pDX, IDC_EDIT_VALUE, m_strValueEdit);
    DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE2, m_strValueTypeEdit);
    DDX_Control(pDX, IDC_COMBO_VALUE_TYPE2, m_cbbValueTypeEdit);
    DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddressEdit);
}

BEGIN_MESSAGE_MAP(CMemoryCheatDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_PROGRESS, &CMemoryCheatDlg::OnBnClickedButtonProgress)
    ON_BN_CLICKED(IDC_BUTTON_FIRST, &CMemoryCheatDlg::OnBnClickedButtonFirst)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &CMemoryCheatDlg::OnBnClickedButtonNext)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CMemoryCheatDlg::OnBnClickedButtonStop)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADDRESS_TEMP, &CMemoryCheatDlg::OnNMDblclkListAddressTemp)
    ON_NOTIFY(NM_CLICK, IDC_LIST_ADDRESS_TARGET, &CMemoryCheatDlg::OnNMClickListAddressTarget)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CMemoryCheatDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_DEL, &CMemoryCheatDlg::OnBnClickedButtonDel)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMemoryCheatDlg::OnBnClickedButtonSave)
    ON_CBN_SELCHANGE(IDC_COMBO_VALUE_TYPE2, &CMemoryCheatDlg::OnCbnSelchangeComboValueTypeEdit)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_PLANT_INJECT, &CMemoryCheatDlg::OnBnClickedButtonPlantInject)
END_MESSAGE_MAP()


// CMemoryCheatDlg ��Ϣ�������

BOOL CMemoryCheatDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    // �����ʼ��
    {
        // ��ʱ���� �б�
        {
            CListCtrl &m_lst = m_lstAddressTemp;
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
                m_lst.InsertColumn(EListTempIndexAddress, _T("��ַ"), LVCFMT_LEFT, 80);
            }
        }

        // ���� �����б�
        {
            CListCtrl &m_lst = m_lstAddressTarget;
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
                m_lst.InsertColumn(EListTargetIndexDescription, _T("˵��"), LVCFMT_LEFT, 80);
                m_lst.InsertColumn(EListTargetIndexAddress, _T("��ַ"), LVCFMT_LEFT, 80);
                m_lst.InsertColumn(EListTargetIndexType, _T("����"), LVCFMT_LEFT, 80);
                m_lst.InsertColumn(EListTargetIndexValue, _T("ֵ"), LVCFMT_LEFT, 80);
            }
        }

        // ֵ����
        {
            auto &ss = m_indexValueTypeArray;
            ss[0].index = 0, ss[0].strValueType.LoadString(IDS_STRING_1BYTE);
            ss[1].index = 1, ss[1].strValueType.LoadString(IDS_STRING_2BYTE);
            ss[2].index = 2, ss[2].strValueType.LoadString(IDS_STRING_4BYTE);
            ss[3].index = 3, ss[3].strValueType.LoadString(IDS_STRING_FLOAT);
            ss[4].index = 4, ss[4].strValueType.LoadString(IDS_STRING_DOUBLE);
        }
        // ���� ֵ����
        {
            for(auto km : m_indexValueTypeArray) {
                m_cbbValueType.AddString(km.strValueType);
            }
            m_cbbValueType.SetCurSel(2);
        }
        // �༭ֵ����
        {
            for(auto km : m_indexValueTypeArray) {
                m_cbbValueTypeEdit.AddString(km.strValueType);
            }
            m_cbbValueTypeEdit.SetCurSel(2);
        }

        // ���������ص�����
        // ���͵�ָ��Ϊ ������
        m_pFinder->SetCallbackFirst(FirstSearchRoutine, &m_pProcess);
        m_pFinder->SetCallbackNext(NextSearchRoutine, &m_pProcess);
    }

    // ������ʱ�� : ÿ�����һ�� Ŀ���ַ���е�����
    SetTimer(1, 1000, nullptr);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMemoryCheatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMemoryCheatDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMemoryCheatDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMemoryCheatDlg::PreTranslateMessage(MSG *pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CMemoryCheatDlg::OnBnClickedButtonProgress()
{
    // ��ʾ �����б�Ի���
    CDialogProgress dlg(this);
    if(IDOK != dlg.DoModal()) {
        // ȡ���������,��0
        m_dwProcessId = 0;
        return;
    }
    // ���ѡ��Ľ���id
    m_dwProcessId = CDialogProgress::m_dwProcessId;
    // �ѱ���������Ϊ Ŀ�������
    SetWindowText(CDialogProgress::m_strProcessName);
    // �򿪽���
    m_pFinder->OpenProcess(m_dwProcessId);
}

// �״�ɨ��
void CMemoryCheatDlg::OnBnClickedButtonFirst()
{
    // ��� û��ѡ�����,��ѡ��
    if(0 == m_dwProcessId) {
        OnBnClickedButtonProgress();
    }
    // �������0,˵��û��ѡ�����,ֱ�ӷ���
    if(0 == m_dwProcessId) {
        return;
    }

    // �ص������п��������Ƿ���� �ı�־
    m_bGoon = true;

    // �������
    m_lstAddressTemp.DeleteAllItems();

    // ���¿ؼ����ݵ�����
    UpdateData(TRUE);

    // Ŀ��ֵ
    CString sTarget;
    GetDlgItemText(IDC_EDIT_SEARCH_VALUE, sTarget);
    if(sTarget.IsEmpty()) {
        AfxMessageBox(_T("����������ֵ"));
        // ���� Ŀ��ֵ �ؼ� ��ý���
        GetDlgItem(IDC_EDIT_SEARCH_VALUE)->SetFocus();
        UpdateData(FALSE);
        m_bGoon = false;
        return;
    }

    // ��õ�ǰ��ť
    auto pBtnFirst = (CButton *)GetDlgItem(IDC_BUTTON_FIRST);
    pBtnFirst->EnableWindow(FALSE);
    // ��ȡ ���ҷ�Χ
    DWORD dwLimitBegin, dwLimitEnd;
    GetLimit(dwLimitBegin, dwLimitEnd);

    // ��ȡ��ǰѡ����
    int iIdx = m_cbbValueType.GetCurSel();

    // ��¼ �������
    bool bFind = false;

    // ����ѡ��Ĳ�ͬ��������,���в�ͬ��ʽ�Ĳ���
    switch(iIdx) {
        case 0: { // 1�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            BYTE bb = (BYTE)(ul & 0x000000FF);
            bFind = m_pFinder->FindFirst(m_dwProcessId, dwLimitBegin, dwLimitEnd, bb);
            break;
        }
        case 1: { // 2�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            SHORT bb = (SHORT)(ul & 0x0000FFFF);
            bFind = m_pFinder->FindFirst(m_dwProcessId, dwLimitBegin, dwLimitEnd, bb);
            break;
        }
        case 2: { // 4�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            DWORD bb = (DWORD)(ul & 0xFFFFFFFF);
            bFind = m_pFinder->FindFirst(m_dwProcessId, dwLimitBegin, dwLimitEnd, bb);
            break;
        }
        case 3: { // float
            TCHAR *szEndPtr = nullptr;
            float f = _tcstof(sTarget.GetString(), &szEndPtr);
            bFind = m_pFinder->FindFirst(m_dwProcessId, dwLimitBegin, dwLimitEnd, f);
            break;
        }
        case 4: { // double
            TCHAR *szEndPtr = nullptr;
            double d = _tcstod(sTarget.GetString(), &szEndPtr);
            bFind = m_pFinder->FindFirst(m_dwProcessId, dwLimitBegin, dwLimitEnd, d);
            break;
        }
        default:
            break;
    }

    // ��� ����������� FALSE,�˳�
    if(!bFind) {
        goto __End;
    }

    // ���ý������
    {
        const std::list<DWORD> &lst = m_pFinder->GetResults();
        int index = 1024; //�����ʾ1024���
        for(auto addr : lst) {
            if(index-- <= 0) {
                break;
            }
            int index = m_lstAddressTemp.InsertItem(0, _T(""));
            CString s;
            s.Format(_T("%08X"), addr);
            m_lstAddressTemp.SetItemText(index, 0, s);
        }
    }

    // ��������0
    m_pProcess.SetPos(0);

__End:
    // �ָ���ť ����״̬
    pBtnFirst->EnableWindow(TRUE);
    // ������������(false)
    m_bGoon = false;
    UpdateData(FALSE);
}

// �ٴ�ɨ��
void CMemoryCheatDlg::OnBnClickedButtonNext()
{
    // �ص������пؼ������Ƿ���� �ı�־
    m_bGoon = true;
    UpdateData(TRUE);
    // Ŀ��ֵ
    CString sTarget;
    GetDlgItemText(IDC_EDIT_SEARCH_VALUE, sTarget);
    if(sTarget.IsEmpty()) {
        AfxMessageBox(_T("������Ŀ��ֵ"));
        UpdateData(FALSE);
        m_bGoon = false;
        return;
    }

    // ��õ�ǰ��ť
    auto pBtnNext = (CButton *)GetDlgItem(IDC_BUTTON_NEXT);
    pBtnNext->EnableWindow(FALSE);

    //�������
    m_lstAddressTemp.DeleteAllItems();

    // ��ȡ��ǰѡ����
    int iIdx = m_cbbValueType.GetCurSel();

    // ��¼ �������
    bool bFind = false;

    // ����ѡ��Ĳ�ͬ��������,���в�ͬ��ʽ�Ĳ���
    switch(iIdx) {
        case 0: { // 1�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            BYTE bb = (BYTE)(ul & 0x000000FF);
            bFind = m_pFinder->FindNext(bb);
            break;
        }
        case 1: { // 2�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            SHORT bb = (SHORT)(ul & 0x0000FFFF);
            bFind = m_pFinder->FindNext(bb);
            break;
        }
        case 2: { // 4�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            DWORD bb = (DWORD)(ul & 0xFFFFFFFF);
            bFind = m_pFinder->FindNext(bb);
            break;
        }
        case 3: { // float
            TCHAR *szEndPtr = nullptr;
            float f = _tcstof(sTarget.GetString(), &szEndPtr);
            bFind = m_pFinder->FindNext(f);
            break;
        }
        case 4: { // double
            TCHAR *szEndPtr = nullptr;
            double d = _tcstod(sTarget.GetString(), &szEndPtr);
            bFind = m_pFinder->FindNext(d);
            break;
        }
        default:
            break;
    }

    // ��� ����������� FALSE,�˳�
    if(!bFind) {
        goto __End;
    }

    // ���ý������
    {
        const std::list<DWORD> &lst = m_pFinder->GetResults();
        int index = 1024;
        for(auto addr : lst) {
            if(index-- <= 0) {
                break;
            }
            int index = m_lstAddressTemp.InsertItem(0, _T(""));
            CString s;
            s.Format(_T("%08X"), addr);
            m_lstAddressTemp.SetItemText(index, 0, s);
        }
    }

    // ����������
    m_pProcess.SetPos(0);

__End:
    // �ָ���ť ����״̬
    pBtnNext->EnableWindow(TRUE);
    // ������������(false)
    m_bGoon = false;
    UpdateData(FALSE);
}

// ֹͣɨ��
void CMemoryCheatDlg::OnBnClickedButtonStop()
{
    // ������Ϊ0,�ص������ᵼ������������ǰ����
    m_bGoon = false;
}

// ��� ɨ�跶Χ
void CMemoryCheatDlg::GetLimit(DWORD &dwLimitBegin, DWORD &dwLimitEnd)
{
    // ��� �ı��� ����
    CString s0, s1;
    GetDlgItemText(IDC_EDIT_LIMIT_START, s0);
    GetDlgItemText(IDC_EDIT_LIMIT_END, s1);
    // ת���� DWORD ����
    _TCHAR *szEnd = NULL;
    dwLimitBegin = _tcstol(s0.GetString(), &szEnd, 0x10);
    dwLimitEnd = _tcstol(s1.GetString(), &szEnd, 0x10);
}

// ˫����ʱ��ַ�б�: ��ѡ���е����ļ��뵽 Ŀ���ַ�б�
void CMemoryCheatDlg::OnNMDblclkListAddressTemp(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nItem = pNMItemActivate->iItem;
    int iSubItem = pNMItemActivate->iSubItem;
    if(nItem >= 0 && iSubItem >= 0) {
        UpdateData(TRUE);
        // ��õ�ַ
        DWORD dwAddr = GetListItemValueDWORD(m_lstAddressTemp, nItem, iSubItem, 0x10);

        // ���ֵ����,��ֵ
        CString strValue, strValueType;

        // ��ȡ �ڴ�����
        strValue = ReadValue(m_cbbValueType, dwAddr);

        // ��� ֵ����
        int iIdx = m_cbbValueType.GetCurSel();
        m_cbbValueType.GetLBText(iIdx, strValueType);

        // ���ӵ� Ŀ���ַ�б��
        AddTargetListData(std::make_tuple(_T(""), dwAddr, strValueType, strValue));

        UpdateData(FALSE);
    }
    *pResult = 0;
}

// ���� Ŀ���ַ�б�
void CMemoryCheatDlg::OnNMClickListAddressTarget(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nItem = pNMItemActivate->iItem;
    int iSubItem = pNMItemActivate->iSubItem;
    if(nItem >= 0 && iSubItem >= 0) {
        CListCtrl &m_lst = m_lstAddressTarget;
        // ����б��е�ѡ���еĸ���ֵ,�����õ��༭��
        m_strDesEdit = m_lst.GetItemText(nItem, EListTargetIndexDescription);
        m_strAddressEdit = m_lst.GetItemText(nItem, EListTargetIndexAddress);
        m_strValueTypeEdit = m_lst.GetItemText(nItem, EListTargetIndexType);
        m_strValueEdit = m_lst.GetItemText(nItem, EListTargetIndexValue);
        UpdateData(FALSE);
    }
    *pResult = 0;
}

// ����m_lst�е�����
void CMemoryCheatDlg::AddTempListData(const TupleTempLineData &info)
{
    int index = m_lstAddressTemp.GetItemCount();
    // ����һ������
    index = m_lstAddressTemp.InsertItem(index, _T(""));
    // ��ַ DWORD ת���� �ַ������õ� �б����
    SetListItemValueFormat(m_lstAddressTemp, index, EListTempIndexAddress, _T("%08X"), std::get<0>(info));
}

// ���ĳ������
CMemoryCheatDlg::TupleTempLineData CMemoryCheatDlg::GetTempListData(int index)
{
    return std::make_tuple(GetListItemValueDWORD(m_lstAddressTemp, index, EListTargetIndexAddress, 0x10));
}

// ��������
void CMemoryCheatDlg::AddTargetListData(const TupleTargetLineData &info)
{
    int index = m_lstAddressTarget.GetItemCount();
    // ��������
    index = m_lstAddressTarget.InsertItem(index, std::get<0>(info));
    // ���õ�ַ
    SetListItemValueFormat(m_lstAddressTarget, index, EListTargetIndexAddress, _T("%08X"), std::get<1>(info));
    // ������������
    m_lstAddressTarget.SetItemText(index, EListTargetIndexType, std::get<2>(info));
    // ���� ֵ
    m_lstAddressTarget.SetItemText(index, EListTargetIndexValue, std::get<3>(info));
}

// ��������
void CMemoryCheatDlg::UpdTargetListData(int index, const TupleTargetLineData &info)
{
    // ��������
    m_lstAddressTarget.SetItemText(index, EListTargetIndexDescription, std::get<0>(info));
    // ���õ�ַ
    SetListItemValueFormat(m_lstAddressTarget, index, EListTargetIndexAddress, _T("%08X"), std::get<1>(info));
    // ���� ֵ����
    m_lstAddressTarget.SetItemText(index, EListTargetIndexType, std::get<2>(info));
    // ���� ֵ
    m_lstAddressTarget.SetItemText(index, EListTargetIndexValue, std::get<3>(info));
}

// ���ĳ������
CMemoryCheatDlg::TupleTargetLineData CMemoryCheatDlg::GetListTargetData(int index)
{
    return std::make_tuple(
               m_lstAddressTarget.GetItemText(index, EListTargetIndexDescription),
               GetListItemValueDWORD(m_lstAddressTarget, index, EListTargetIndexAddress, 0x10),
               m_lstAddressTarget.GetItemText(index, EListTargetIndexType),
               m_lstAddressTarget.GetItemText(index, EListTargetIndexValue)
           );
}

// ��� dword ���͵�ֵ
DWORD CMemoryCheatDlg::GetListItemValueDWORD(CListCtrl &lst, int nItem, int nSubItem, int _Radix/*=0x10*/)
{
    CString s;
    // ��ø��е��ı�
    s = lst.GetItemText(nItem, nSubItem);
    // ת���� DWORD ����
    TCHAR *szEndPtr = nullptr;
    DWORD d = _tcstoul(s.GetString(), &szEndPtr, _Radix);
    return d;
}

// ���ݴ���ĸ�ʽ������ �ı�, ʹ�÷������� printf
void CMemoryCheatDlg::SetListItemValueFormat(CListCtrl &lst, int nItem, int nSubItem, PCTSTR szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);
    TCHAR szBuf[1024] = { 0 };
    // ������������������ szFormat ָ���ĸ�ʽ,��ӡ�� szBuf��
    _vstprintf_s(szBuf, szFormat, ap);
    // �������ı�
    lst.SetItemText(nItem, nSubItem, szBuf);
    va_end(ap);
}

// ���Ӱ�ť : ���� �༭�������ݵ� Ŀ���ַ�б����
void CMemoryCheatDlg::OnBnClickedButtonAdd()
{
    UpdateData(TRUE);

    // ת���� DWORD ����
    TCHAR *szEndPtr;
    DWORD d;
    d = _tcstoul(m_strAddressEdit.GetString(), &szEndPtr, 0x10);
    // ���ӵ� Ŀ���ַ�б��
    AddTargetListData(std::make_tuple(m_strDesEdit, d, m_strValueTypeEdit, m_strValueEdit));

    UpdateData(FALSE);
}

// ɾ����ť : ɾ�� Ŀ���ַ�б����ѡ�е���
void CMemoryCheatDlg::OnBnClickedButtonDel()
{
    std::vector<int> lines;
    // ���ѡ����index,�����뵽 lines������
    POSITION pos = m_lstAddressTarget.GetFirstSelectedItemPosition();
    while(pos) {
        lines.push_back(m_lstAddressTarget.GetNextSelectedItem(pos));
    }
    // �Ӻ���ǰ,����ɾ����
    for(auto it = lines.rbegin(); it != lines.rend(); ++it) {
        m_lstAddressTarget.DeleteItem(*it);
    }
}

// ���水ť : ���� �༭�������ݵ� Ŀ���ַ�б����ѡ�е���
void CMemoryCheatDlg::OnBnClickedButtonSave()
{
    // ��ǰѡ����
    int index;
    {
        POSITION pos = m_lstAddressTarget.GetFirstSelectedItemPosition();
        if(!pos) {
            return;
        }
        index = m_lstAddressTemp.GetNextSelectedItem(pos);
        // û��ѡ����,����
        if(index < 0) {
            return;
        }
    }

    UpdateData(TRUE);

    // �޸��ڴ�����,�������б������
    {
        TCHAR *szEndPtr;
        DWORD dwAddr = _tcstoul(m_strAddressEdit.GetString(), &szEndPtr, 0x10);
        // �޸��ڴ�����
        WriteValue(m_cbbValueTypeEdit, dwAddr, m_strValueEdit);
        // ͬʱ �޸��б���е�����
        UpdTargetListData(index, std::make_tuple(m_strDesEdit, dwAddr, m_strValueTypeEdit, m_strValueEdit));
    }
    UpdateData(FALSE);
}

// �༭�� �������� �����仯: �ض���ַ����ֵ
void CMemoryCheatDlg::OnCbnSelchangeComboValueTypeEdit()
{
    //���ݵ�ַ,��ֵ����,���¶��ڴ��ֵ
    UpdateData(TRUE);
    // ��õ�ַ
    TCHAR *szEndPtr;
    DWORD dwAddr = (DWORD)_tcstoul(m_strAddressEdit.GetString(), &szEndPtr, 0x10);
    // ��ȡ������ �༭����ֵ
    m_strValueEdit = ReadValue(m_cbbValueTypeEdit, m_strAddressEdit);
    UpdateData(FALSE);
}

// �����б���е�ֵ
void CMemoryCheatDlg::UpdateTargetListValueByAddress()
{
    using namespace std;
    for(int i = 0; i < m_lstAddressTarget.GetItemCount(); ++i) {
        // ���������
        TupleTargetLineData dt = GetListTargetData(i);
        // �ڴ��ַ
        DWORD dwAddr = get<1>(dt);
        CString  valueType = get<2>(dt);
        // ��ȡ ���ڴ洦��ֵ
        CString value = ReadValue(valueType, dwAddr);
        // ���±�������:�仯��ֻ�� ֵ
        UpdTargetListData(i, std::make_tuple(get<0>(dt), dwAddr, valueType, value));
    }
}

// ��ʱ��, ��ʱ���� Ŀ���б�����г����ڴ��ַ����ֵ
void CMemoryCheatDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent) {
        case 1: {
            // ���µ�ǰ��ַ����ֵ
            UpdateTargetListValueByAddress();
            break;
        }
        default:
            break;
    }
    CDialogEx::OnTimer(nIDEvent);
}

// ����CComboBox��ͬѡ����, ���в�ͬ�� ��ȡ/д������
CString CMemoryCheatDlg::ReadValue(const CString &strValueType, DWORD dwAddr)
{
    // ���� ����,���������Ϣ
    for(auto km : m_indexValueTypeArray) {
        if(km.strValueType == strValueType) {
            //����������Ϣ���ڴ��ַ,��ȡ����
            return ReadValue(km.index, dwAddr);
        }
    }
    // �����ܵ���������.
    assert(false);
    return _T("");
}
CString CMemoryCheatDlg::ReadValue(int index, DWORD dwAddr)
{
    /*
        ���� �ڴ��ַ,�ô���ֵ ������������Ͳ�ͬ,��������ͬ������
        CComboBox ��ѡ��� �������Ͳ�ͬ,���ȡ�ķ�ʽ��ͬ
        ��: ѡ�� �±�Ϊ0,���� Ҫ�Ѵ˴���ֵ ����Ϊ BYTE����,ֻ��ȡһ�ֽ�
        ����� switch���ݲ�ͬ�� �±�, ���ò�ͬģ�庯��
    */
    CString strValue;
    switch(index) {
        case 0: { // 1�ֽ�
            BYTE bValue;
            if(m_pFinder->Read(dwAddr, bValue)) {
                strValue.Format(_T("%d"), bValue);
            }
            break;
        }
        case 1: { // 2�ֽ�
            SHORT bValue;
            if(m_pFinder->Read(dwAddr, bValue)) {
                strValue.Format(_T("%d"), bValue);
            }
            break;
        }
        case 2: { // 4�ֽ�
            DWORD bValue;
            if(m_pFinder->Read(dwAddr, bValue)) {
                strValue.Format(_T("%d"), bValue);
            }
            break;
        }
        case 3: { // float
            float bValue;
            if(m_pFinder->Read(dwAddr, bValue)) {
                strValue.Format(_T("%E"), bValue);
            }
            break;
        }
        case 4: {
            double bValue;
            if(m_pFinder->Read(dwAddr, bValue)) {
                strValue.Format(_T("%E"), bValue);
            }
            break;
        }
        default:
            break;
    }
    return strValue;
}
CString CMemoryCheatDlg::ReadValue(CComboBox &cbb, DWORD dwAddr)
{
    int iIdx = cbb.GetCurSel();
    return ReadValue(iIdx, dwAddr);
}
CString CMemoryCheatDlg::ReadValue(CComboBox &cbb, const CString &strAddr)
{
    TCHAR *szPtr;
    DWORD dwAddr = (DWORD)_tcstoul(strAddr.GetString(), &szPtr, 0x10);
    return ReadValue(cbb, dwAddr);
}
void CMemoryCheatDlg::WriteValue(CComboBox &cbb, const CString &strAddr, const CString &value)
{
    TCHAR *szPtr;
    DWORD dwAddr = (DWORD)_tcstoul(strAddr.GetString(), &szPtr, 0x10);
    WriteValue(cbb, dwAddr, value);
}
void CMemoryCheatDlg::WriteValue(CComboBox &cbb, DWORD dwAddr, const CString &value)
{
    /*
        ���� �ڴ��ַ,�ô���ֵ ������������Ͳ�ͬ,��������ͬ������
        CComboBox ��ѡ��� �������Ͳ�ͬ,��д��ĵķ�ʽ��ͬ(�ֽ�˳��,�ֽ�������)
        ��: ѡ�� �±�Ϊ0,���� Ҫ�Ѵ˴���ֵ ����Ϊ BYTE����,ֻ��д��1�ֽ�
        ����� switch���ݲ�ͬ�� �±�, ���ò�ͬģ�庯��
    */
    int iIdx = cbb.GetCurSel();
    const CString sTarget = value;
    switch(iIdx) {
        case 0: { // 1�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            BYTE bb = (BYTE)(ul & 0x000000FF);
            m_pFinder->Write(dwAddr, bb);
            break;
        }
        case 1: { // 2�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            SHORT bb = (SHORT)(ul & 0x0000FFFF);
            m_pFinder->Write(dwAddr, bb);
            break;
        }
        case 2: { // 4�ֽ�
            TCHAR *szEndPtr = nullptr;
            unsigned long ul = _tcstoul(sTarget.GetString(), &szEndPtr, 10);
            DWORD bb = (DWORD)(ul & 0xFFFFFFFF);
            m_pFinder->Write(dwAddr, bb);
            break;
        }
        case 3: { // float
            TCHAR *szEndPtr = nullptr;
            float f = _tcstof(sTarget.GetString(), &szEndPtr);
            m_pFinder->Write(dwAddr, f);
            break;
        }
        case 4: { // double
            TCHAR *szEndPtr = nullptr;
            double d = _tcstod(sTarget.GetString(), &szEndPtr);
            m_pFinder->Write(dwAddr, d);
            break;
        }
        default:
            break;
    }

}

// ע����Ϸ
void CMemoryCheatDlg::OnBnClickedButtonPlantInject()
{
    auto h = LoadLibrary(_T("pvz.dll"));
    typedef void(__stdcall * PFun)();
    PFun SetHook = (PFun) GetProcAddress(h, "SetHook");
    if(SetHook) {
        SetHook();
    }
    FreeLibrary(h);
}

