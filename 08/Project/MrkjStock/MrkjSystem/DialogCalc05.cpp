// DialogCalc05.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc05.h"
#include "afxdialogex.h"


// CDialogCalc05 �Ի���

IMPLEMENT_DYNAMIC(CDialogCalc05, CDialogEx)

CDialogCalc05::CDialogCalc05(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG5, pParent)
{
}

CDialogCalc05::~CDialogCalc05()
{
}

void CDialogCalc05::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_cbbPattern);
}


BEGIN_MESSAGE_MAP(CDialogCalc05, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogCalc05::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogCalc05::OnBnClickedButtonCalc)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CDialogCalc05::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDialogCalc05 ��Ϣ�������


void CDialogCalc05::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogCalc05::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}
/*********************************************************************************/
// TODO
// ˰�ʱ�:...


// ���㹫ʽ : ��������
double __stdcall Fun1(double dTotal, double dOther, double dStart)
{
    double rate[] = {0.03, 0.1, 0.2, 0.25, 0.3, 0.35, 0.45};
    double mon[] = { 0, 105, 555, 1005, 2755, 5505, 13505 };
    auto GetIndex = [dStart](double mm)->int {
        if(mm - dStart >= 80000)
        {
            return 6;
        }
        if(mm - dStart >= 55000)
        {
            return 5;
        }
        if(mm - dStart >= 35000)
        {
            return 4;
        }
        if(mm - dStart >= 9000)
        {
            return 3;
        }
        if(mm - dStart >= 4500)
        {
            return 2;
        }
        if(mm - dStart >= 1500)
        {
            return 1;
        }
        return 0;
    };
    int index = GetIndex(dTotal);
    // Ӧ��˰�������н������ ��������һ�𡱣��۳�����������˰�ʣ�����۳���
    return (dTotal - dOther - dStart) * rate[index] - mon[index];
}

// ���㹫ʽ : �Ʋ�ת��
double __stdcall Fun2(double dTotal, double dReal, double dOther)
{
    return std::max<double>((dTotal - dReal - dOther) * 0.20, 0);
}
double __stdcall Fun3(double money0, double money1, double money2)
{
    return std::max<double>(money0 * 0.2, 0);
}

typedef double (__stdcall *PFUN)(double money0, double money1, double money2);
struct {
    CString text;
    PFUN pFun;
} TEXT_FUN[] = {
    {_T("����,н������"), Fun1},
    {_T("�Ʋ�ת������"), Fun2 },
    {_T("�Ʋ���������"), Fun3 },
};
// �������ֻ�ü��㹫ʽ
PFUN GetFun(const CString& str)
{
    for(auto km : TEXT_FUN) {
        if(km.text == str) {
            return km.pFun;
        }
    }
    return nullptr;
}

// ���㹫ʽ End.
/*********************************************************************************/

BOOL CDialogCalc05::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ��ʼ���ؼ�
    {
        CComboBox * p = (CComboBox*)GetDlgItem(IDC_COMBO1);
        assert(p);
        for each(auto km in TEXT_FUN) {
            p->AddString(km.text);
        }
        p->SetCurSel(0);
    }
    SetDlgItemText(IDC_EDIT1, _T("0"));
    SetDlgItemText(IDC_EDIT2, _T("0"));
    SetDlgItemText(IDC_EDIT3, _T("3500"));
    SetDlgItemText(IDC_EDIT4, _T("0"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogCalc05::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT1, _T("0"));
    SetDlgItemText(IDC_EDIT2, _T("0"));
    SetDlgItemText(IDC_EDIT3, _T("0"));
    SetDlgItemText(IDC_EDIT4, _T("0"));
}


void CDialogCalc05::OnBnClickedButtonCalc()
{
    // ��� ��������,��������������, ��ü��㷽ʽ
    CString str;
    CComboBox * p = (CComboBox*)GetDlgItem(IDC_COMBO1);
    assert(p);
    if(p->GetCurSel() >= 0) {
        p->GetLBText(p->GetCurSel(), str);
    }
    PFUN pFun = GetFun(str);
    if(pFun) {
        CString s1, s2, s3;
        GetDlgItemText(IDC_EDIT1, s1);
        GetDlgItemText(IDC_EDIT2, s2);
        GetDlgItemText(IDC_EDIT3, s3);
        using StringHelper::StringTo;
        double d1 = StringTo<double>(s1.GetString());
        double d2 = StringTo<double>(s2.GetString());
        double d3 = StringTo<double>(s3.GetString());
        double fRes = pFun(d1, d2, d3);
        CString s4;
        s4.Format(_T("%lf"), fRes);
        SetDlgItemText(IDC_EDIT4, s4);
    }
    else {
        CTools::MessageBoxFormat(_T("û�м��㹫ʽ:'%s'"), str.GetString());
    }
}



void CDialogCalc05::OnCbnSelchangeCombo1()
{
    UpdateData(TRUE);
    CWnd* pA = GetDlgItem(IDC_STATIC_A);
    CWnd* pB = GetDlgItem(IDC_STATIC_B);
    CWnd* pEdtA = GetDlgItem(IDC_EDIT2);
    CWnd* pEdtB = GetDlgItem(IDC_EDIT3);
    assert(pA);
    assert(pB);
    switch(m_cbbPattern.GetCurSel()) {
        case 0: {
                pA->SetWindowText(_T("�����:"));
                pB->SetWindowText(_T("������:"));
                pEdtA->EnableWindow(TRUE);
                pEdtB->EnableWindow(TRUE);
                break;
            }
        case 1: {
                pA->SetWindowText(_T("�Ʋ�ԭֵ:"));
                pB->SetWindowText(_T("��������:"));
                pEdtA->EnableWindow(TRUE);
                pEdtB->EnableWindow(TRUE);
                break;
            }
        case 2: {
                pEdtA->EnableWindow(FALSE);
                pEdtB->EnableWindow(FALSE);
                break;
                break;
            }
    }
}


BOOL CDialogCalc05::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
