// DialogMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pvz.h"
#include "DialogMain.h"
#include "afxdialogex.h"

#include <vector>

// ģ����I��D
#define ID_MFZ (0x30)

// CDialogMain �Ի���

IMPLEMENT_DYNAMIC(CDialogMain, CDialogEx)

CDialogMain::CDialogMain(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG1, pParent)
    , m_dwSunlight(9527)
    , m_dwGold(9528)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDialogMain::~CDialogMain()
{
}

void CDialogMain::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SUNLIGHT, m_dwSunlight);
    DDX_Control(pDX, IDC_COMBO_PLANTS, m_comboPlants);
    DDX_Control(pDX, IDC_COMBO_CROPX, m_comboCropX);
    DDX_Control(pDX, IDC_COMBO_CROPY, m_comboCropY);
    DDX_Text(pDX, IDC_EDIT_GOLD, m_dwGold);
    DDX_Control(pDX, IDC_COMBO_PLANTS2, m_comboPlants2);
}


BEGIN_MESSAGE_MAP(CDialogMain, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_NOCD, &CDialogMain::OnBnClickedButtonNocd)
    ON_BN_CLICKED(IDC_BUTTON_ANYWHERE, &CDialogMain::OnBnClickedButtonAnywhere)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_COLLECT, &CDialogMain::OnBnClickedButtonAutoCollect)
    ON_BN_CLICKED(IDC_BUTTON_SUNLIGHT, &CDialogMain::OnBnClickedButtonSunlight)
    ON_BN_CLICKED(IDC_BUTTON_CROP, &CDialogMain::OnBnClickedButtonCrop)
    ON_BN_CLICKED(IDC_BUTTON_CROP2, &CDialogMain::OnBnClickedButtonCropRow)
    ON_BN_CLICKED(IDC_BUTTON_CROP3, &CDialogMain::OnBnClickedButtonColumn)
    ON_BN_CLICKED(IDC_BUTTON_ALL_ZOMBIES, &CDialogMain::OnBnClickedButtonAllZombies)
    ON_BN_CLICKED(IDC_BUTTON_REMOVEPLANT, &CDialogMain::OnBnClickedButtonRemoveplant)
    ON_BN_CLICKED(IDC_BUTTON_GOLD, &CDialogMain::OnBnClickedButtonGold)
    ON_CBN_SELCHANGE(IDC_COMBO_PLANTS, &CDialogMain::OnCbnSelchangeComboPlants)
END_MESSAGE_MAP()


// CDialogMain ��Ϣ�������

static bool CompareCode(DWORD dwAddr, const unsigned char code[], size_t len)
{
    if(0 == memcmp((void *)dwAddr, code, len)) {
        return true;
    }
    return false;
}

BOOL CDialogMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // ȷ����ť״̬
    {

        // ȥ��ֲ��CD
        {
            const DWORD dwBaseAddr = 0x00488250;
            const DWORD dwBaseAddr2 = 0x00488688;
            bool b1 = false, b2 = false;
            // ��������
            {
                const unsigned char code[] = { 0xEB, 0x24 };
                b1 = CompareCode(dwBaseAddr, code, _countof(code));
            }
            // �ɵ������
            {
                const unsigned char code[] = { 0xE9, 0x7A, 0x01, 0x00, 0x00, 0x90 };
                b2 = CompareCode(dwBaseAddr2, code, _countof(code));
            }
            if(b1 && b2) {
                SetDlgItemText(IDC_BUTTON_NOCD, _T("��CD(�ָ�)"));
            }
            else {
                SetDlgItemText(IDC_BUTTON_NOCD, _T("��CD"));
            }
        }

        // �ص���ֲ
        {
            const DWORD dwBaseAddr = 0x0040FE2F;

            // �Ƿ���Է���ֲ��
            const unsigned char code[] = { 0xE9, 0x20, 0x09, 0x00, 0x00, 0x90 };
            if(CompareCode(dwBaseAddr, code, _countof(code))) {
                SetDlgItemText(IDC_BUTTON_ANYWHERE, _T("�ص���ֲ(�ָ�)"));
            }
            else {
                SetDlgItemText(IDC_BUTTON_ANYWHERE, _T("�ص���ֲ"));
            }
        }

        // �Զ��ռ�����
        {
            const DWORD dwBaseAddr = 0x0043158F;
            // �ж��Ƿ������⣬��Ǯ��
            const unsigned char code[] = { 0x74, 0x08 };
            if(CompareCode(dwBaseAddr, code, _countof(code))) {
                SetDlgItemText(IDC_BUTTON_AUTO_COLLECT, _T("�Զ��ռ�(�ָ�)"));
            }
            else {

                SetDlgItemText(IDC_BUTTON_AUTO_COLLECT, _T("�Զ��ռ������Ǯ"));
            }
        }

        UpdateData(FALSE);
    }

    // ��ʼ���б�
    {
        // ����ֲ��
        {
            CString str[] = { _T(" 0:�㶹����"), _T(" 1:���տ�"), _T(" 2:ӣ��ը��"), _T(" 3:���ǽ"),
                              _T(" 4:������"), _T(" 5:��������"), _T(" 6:���컨"), _T(" 7:˫������"),

                              _T(" 8:С�繽"), _T(" 9:���⹽"), _T("10:���繽"), _T("11:Ĺ��������"),
                              _T("12:�Ȼ�"), _T("13:��С��"), _T("14:������"), _T("15:����"),

                              _T("16:˯��"), _T("17:�ѹ�"), _T("18:��������"), _T("19:���ƺ���"),
                              _T("20:������"), _T("21:�ش�"), _T("22:�����׮"), _T("23:�߼��"),

                              _T("24:��Ģ��"), _T("25:·�ƻ�"), _T("26:������"), _T("27:��Ҷ��"),
                              _T("28:�Ѽ�����"), _T("29:����"), _T("30:�Ϲ�ͷ"), _T("31:������"),

                              _T("32:���Ĳ�Ͷ��"), _T("33:����"), _T("34:����Ͷ��"), _T("35:���ȶ�"),
                              _T("36:����"), _T("37:Ҷ�ӱ���ɡ"), _T("38:��յ��"), _T("39:����Ͷ��"),

                              _T("40:��ǹ����"), _T("41:˫�����տ�"), _T("42:����Ģ��"), _T("43:����"),
                              _T("44:��������"), _T("45:�����"), _T("46:�ش���"), _T("47:���׼�ũ��"),

                              _T("48:ģ����"), _T("49:��ը���"), _T("50:�޴���ǽ")
                            };
            for(auto s : str) {
                m_comboPlants.AddString(s);
            }

            m_comboPlants.SetCurSel(0);
        }
        // ��ģ��ֲ��
        {
            CString str[] = { _T(" 0:�㶹����"), _T(" 1:���տ�"), _T(" 2:ӣ��ը��"), _T(" 3:���ǽ"),
                              _T(" 4:������"), _T(" 5:��������"), _T(" 6:���컨"), _T(" 7:˫������"),

                              /*_T(" 8:С�繽"), _T(" 9:���⹽"), _T("10:���繽"), _T("11:Ĺ��������"),
                              _T("12:�Ȼ�"), _T("13:��С��"), _T("14:������"), _T("15:����"),*/

                              _T("16:˯��"), _T("17:�ѹ�"), _T("18:��������"), _T("19:���ƺ���"),
                              _T("20:������"), _T("21:�ش�"), _T("22:�����׮"), _T("23:�߼��"),

                              /*_T("24:��Ģ��"), _T("25:·�ƻ�"), */_T("26:������"), _T("27:��Ҷ��"),
                              _T("28:�Ѽ�����"), _T("29:����"), _T("30:�Ϲ�ͷ"), /*_T("31:������"),*/

                              _T("32:���Ĳ�Ͷ��"), /*_T("33:����"),*/ _T("34:����Ͷ��"), _T("35:���ȶ�"),
                              _T("36:����"), _T("37:Ҷ�ӱ���ɡ"), _T("38:��յ��"), _T("39:����Ͷ��")/*,

                              _T("40:��ǹ����"), _T("41:˫�����տ�"), _T("42:����Ģ��"), _T("43:����"),
                              _T("44:��������"), _T("45:�����"), _T("46:�ش���"), _T("47:���׼�ũ��"),

                              _T("48:ģ����"), _T("49:��ը���"), _T("50:�޴���ǽ")*/
                            };
            // ����ֲ��
            for(auto s : str) {
                m_comboPlants2.AddString(s);
            }
            m_comboPlants2.SetCurSel(0);
        }


        // ��:���6��
        for(int x = 0; x <= 5; ++x) {
            CString str;
            str.Format(_T("%d"), x);
            m_comboCropX.AddString(str.GetString());
        }
        m_comboCropX.SetCurSel(0);

        // ��:���9��
        for(int y = 0; y <= 8; ++y) {
            CString str;
            str.Format(_T("%d"), y);
            m_comboCropY.AddString(str.GetString());
        }
        m_comboCropY.SetCurSel(0);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
// �쳣: OCX ����ҳӦ���� FALSE
}

// д�ڴ�:�޸Ĵ���
static bool WriteCode(DWORD dwAddr, const unsigned char code[], size_t len)
{
    DWORD dwOld;
    if(!VirtualProtect((LPVOID)dwAddr, len, PAGE_EXECUTE_READWRITE, &dwOld)) {
        return false;
    }

    memcpy_s((void *)dwAddr, len, code, len);

    DWORD dwNew;
    if(VirtualProtect((LPVOID)dwAddr, len, dwOld, &dwNew)) {
        return true;
    }
    return false;
}

// ���� ��ֲ��û��CD
void CDialogMain::OnBnClickedButtonNocd()
{
    const DWORD dwBaseAddr = 0x00488250;
    const DWORD dwBaseAddr2 = 0x00488688;
    CString str;
    GetDlgItemText(IDC_BUTTON_NOCD, str);
    if(str == _T("��CD")) {
        // ��������
        {
            const unsigned char code[] = { 0xEB, 0x24 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        // �ɵ������
        {
            const unsigned char code[] = { 0xE9, 0x7A, 0x01, 0x00, 0x00, 0x90 };
            WriteCode(dwBaseAddr2, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_NOCD, _T("��CD(�ָ�)"));
    }
    else {
        // ��������
        {
            const unsigned char code[] = { 0x75, 0x24 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        // �ɵ������
        {
            const unsigned char code[] = { 0x0F, 0x85, 0x79, 0x01, 0x00, 0x00 };
            WriteCode(dwBaseAddr2, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_NOCD, _T("��CD"));
    }
}

// ���� �����ص���ֲ
void CDialogMain::OnBnClickedButtonAnywhere()
{
    const DWORD dwBaseAddr = 0x0040FE2F;

    CString str;
    GetDlgItemText(IDC_BUTTON_ANYWHERE, str);
    if(str == _T("�ص���ֲ")) {
        // �Ƿ���Է���ֲ��
        {
            const unsigned char code[] = { 0xE9, 0x20, 0x09, 0x00, 0x00, 0x90 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_ANYWHERE, _T("�ص���ֲ(�ָ�)"));
    }
    else {
        // �Ƿ���Է���ֲ��
        {
            const unsigned char code[] = { 0x0F, 0x84, 0x1F, 0x09, 0x00, 0x00 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_ANYWHERE, _T("�ص���ֲ"));
    }
}

// ���� �����Զ��ռ����⣬��Ǯ
void CDialogMain::OnBnClickedButtonAutoCollect()
{
    const DWORD dwBaseAddr = 0x0043158F;

    CString str;
    GetDlgItemText(IDC_BUTTON_AUTO_COLLECT, str);
    if(str == _T("�Զ��ռ������Ǯ")) {
        // �ж��Ƿ������⣬��Ǯ��
        {
            const unsigned char code[] = { 0x74, 0x08 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_AUTO_COLLECT, _T("�Զ��ռ�(�ָ�)"));
    }
    else {
        // �ж��Ƿ������⣬��Ǯ��
        {
            const unsigned char code[] = { 0x75, 0x08 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_AUTO_COLLECT, _T("�Զ��ռ������Ǯ"));
    }
}

// ���� ��ʬȫ������
void CDialogMain::OnBnClickedButtonAllZombies()
{
    const DWORD dwBaseAddr = 0x00413FD9;
    CString str;
    GetDlgItemText(IDC_BUTTON_ALL_ZOMBIES, str);
    if(str == _T("��ʬȫ������")) {
        {
            const unsigned char code[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_ALL_ZOMBIES, _T("��ʬȫ������(�ָ�)"));
    }
    else {
        {
            const unsigned char code[] = { 0x0F, 0x85, 0x0D, 0x01, 0x00, 0x00 };
            WriteCode(dwBaseAddr, code, _countof(code));
        }
        SetDlgItemText(IDC_BUTTON_ALL_ZOMBIES, _T("��ʬȫ������"));
    }
}

// ���� ��������
void CDialogMain::OnBnClickedButtonSunlight()
{
    UpdateData(TRUE);
    DWORD dwValue = m_dwSunlight;
    UpdateData(FALSE);


    // ���������ڴ��ַ:
    // [[0x006A9EC0] + 0x768 ] + 5560

    DWORD dwTemp;
    dwTemp = *(DWORD *)0x006A9EC0;
    dwTemp = *(DWORD *)(0x768 + dwTemp);
    // �ڴ��ַ
    DWORD dwAddr = 0x5560 + dwTemp;

    // д���µ���������
    *(DWORD *)(dwAddr) = dwValue;
}

// ���� ��������
void CDialogMain::OnBnClickedButtonGold()
{
    UpdateData(TRUE);
    DWORD dwValue = m_dwGold;
    UpdateData(FALSE);


    // ��Ǯ�����ڴ��ַ:
    // [[0x006A9EC0] + 0x82C] + 0x28

    DWORD dwTemp;
    dwTemp = *(DWORD *)0x006A9EC0;
    dwTemp = *(DWORD *)(0x82C + dwTemp);
    // �ڴ��ַ
    DWORD dwAddr = 0x28 + dwTemp;

    // д���µĽ�Ǯ����
    *(DWORD *)(dwAddr) = dwValue;
}

// ������ֲCALL
static void _CropCall(DWORD dwType, DWORD dwX, DWORD dwY, DWORD dwIDTarget = 0xFFFFffff)
{
    __try {
        __asm {
            pushad;
            pushfd;
            push dwIDTarget; // ���ģ���ߣ����Ǳ�ģ�õ�ֲ���ID
            push dwType;      // ֲ������
            push dwX;      // x����

            mov eax, 0x006A9EC0;
            mov eax, dword ptr[eax];
            add eax, 0x768;
            mov eax, dword ptr[eax];
            push eax;

            mov eax, dwY; // y����

            mov esi, 0x0040D120; // ����CALL
            call esi;

            popfd;
            popad;
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {

    }
}
static void CropCall(DWORD dwType, DWORD dwX, DWORD dwY, DWORD dwIDTarget = 0xFFFFffff)
{
    // �����ģ����
    if(dwType == ID_MFZ) {
        _CropCall(dwType, dwX, dwY, dwIDTarget);
    }
    else {
        _CropCall(dwType, dwX, dwY);
    }
}


// ���ĳλ�õ�ֵ��
static DWORD GetHitPlants(int row, int col)
{
    int a = row, b = col;
    DWORD dwArr[10] = {0};
    __asm {
        pushfd;
        pushad;

        push b;
        push a;
        mov edx, 0x6A9EC0;
        mov edx, dword ptr[edx];
        mov edx, dword ptr[edx + 0x768];
        mov esi, edx;
        lea ebx, dwArr;
        mov eax, 0x0040D2A0;
        call eax;

        popad;
        popfd;
    };
    return dwArr[3];
}

// ����ֲ��
static void RemovePlant(int row, int col)
{
    __try {
        DWORD dwBase = GetHitPlants(row, col);
        if(dwBase) {
            __asm {
                pushfd;
                pushad;

                mov ebp, dwBase;
                push ebp;
                mov eax, 0x004679B0;
                call eax;

                popad;
                popfd;

            }
        }
        else {
            // ָ��λ��û��ֲ�ʲôҲ����
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {

    }
}


DWORD CDialogMain::GetPlants2Id()
{
    CString str;
    int index = m_comboPlants2.GetCurSel();
    m_comboPlants2.GetLBText(index, str);
    if(str.IsEmpty()) {
        return 0xFFFFffff;
    }
    DWORD dwId = 0xFFFFffff;
    {
        // ��������, ת�� ֲ��ID
        int index = str.Find(_T(":"));
        if(index < 0) {
            return 0xFFFFffff;
        }
        PTSTR szEndPtr;
        dwId = _tcstol(str.Mid(0, index).GetString(), &szEndPtr, 0xA);
    }
    return dwId;
}

// ��ֲһ��ֲ��
void CDialogMain::OnBnClickedButtonCrop()
{
    UpdateData(TRUE);
    if(m_comboCropY.GetCurSel() >= 0 && m_comboCropX.GetCurSel() >= 0) {
        CropCall(m_comboPlants.GetCurSel(), m_comboCropY.GetCurSel(),
                 m_comboCropX.GetCurSel(), GetPlants2Id());
    }
    UpdateData(FALSE);
}

// ��ֲһ��
void CDialogMain::OnBnClickedButtonCropRow()
{
    UpdateData(TRUE);
    DWORD dwId = GetPlants2Id();
    for(DWORD col = 0; col < 9; ++col) {
        CropCall(m_comboPlants.GetCurSel(), col, m_comboCropX.GetCurSel(), dwId);
    }
    UpdateData(FALSE);
}

// ��ֲһ��
void CDialogMain::OnBnClickedButtonColumn()
{
    UpdateData(TRUE);
    DWORD dwId = GetPlants2Id();
    for(DWORD row = 0; row < 6; ++row) {
        CropCall(m_comboPlants.GetCurSel(), m_comboCropY.GetCurSel(), row, dwId);
    }
    UpdateData(FALSE);
}

// �Ƴ�ֲ��
void CDialogMain::OnBnClickedButtonRemoveplant()
{
    RemovePlant(m_comboCropX.GetCurSel(), m_comboCropY.GetCurSel());
}

// ѡ����ֲֲ��
void CDialogMain::OnCbnSelchangeComboPlants()
{
    int index = m_comboPlants.GetCurSel();
    if(index == ID_MFZ) {
        m_comboPlants2.EnableWindow(TRUE);
    }
    else {
        m_comboPlants2.EnableWindow(FALSE);
    }
}
