// DialogCalc03.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc03.h"
#include "afxdialogex.h"


// CDialogCalc03 �Ի���

IMPLEMENT_DYNAMIC(CDialogCalc03, CDialogEx)

CDialogCalc03::CDialogCalc03(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG3, pParent)
{
}

CDialogCalc03::~CDialogCalc03()
{
}

void CDialogCalc03::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogCalc03, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON5, &CDialogCalc03::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogCalc03::OnBnClickedButtonCalc)
END_MESSAGE_MAP()


// CDialogCalc03 ��Ϣ�������


void CDialogCalc03::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT01, _T("0"));
    SetDlgItemText(IDC_EDIT02, _T("0"));
    SetDlgItemText(IDC_EDIT03, _T("0"));
    SetDlgItemText(IDC_EDIT04, _T("0"));
    SetDlgItemText(IDC_EDIT05, _T("0"));
    SetDlgItemText(IDC_EDIT06, _T("1"));
    SetDlgItemText(IDC_EDIT07, _T("2"));
    SetDlgItemText(IDC_EDIT08, _T("0"));
    SetDlgItemText(IDC_EDIT09, _T("0"));
    SetDlgItemText(IDC_EDIT10, _T("0"));
    SetDlgItemText(IDC_EDIT11, _T("0"));
    SetDlgItemText(IDC_EDIT12, _T("0"));
    SetDlgItemText(IDC_EDIT13, _T("0"));
    SetDlgItemText(IDC_EDIT14, _T("0"));
    SetDlgItemText(IDC_EDIT15, _T("0"));
}


// ��ȡ�ؼ���ֵ,������Ϊ double
double CDialogCalc03::GetDlgItemValueD(UINT ID)
{
    CString str;
    GetDlgItemText(ID, str);
    PTSTR szEndPtr = nullptr;
    return _tcstod(str, &szEndPtr);
}

// ���ÿؼ���ֵ
void CDialogCalc03::SetDlgItemValueD(UINT ID, double val)
{
    CString str;
    str.Format(_T("%.2lf"), val);
    SetDlgItemText(ID, str);
}



void CDialogCalc03::OnBnClickedButtonCalc()
{
    /*
        ��ȡ����ֵ
    */
    // ���й����ʽ�
    double d1 = GetDlgItemValueD(IDC_EDIT01);
    // ��ͥ������
    double d2 = GetDlgItemValueD(IDC_EDIT02);
    // ÿ�¿����ڹ���֧��
    double d3 = GetDlgItemValueD(IDC_EDIT03);
    // �ƻ���������
    double d4 = GetDlgItemValueD(IDC_EDIT04);
    // �ƻ��������
    double d5 = GetDlgItemValueD(IDC_EDIT05);
    // ��˰˰��
    double d6 = GetDlgItemValueD(IDC_EDIT06);
    // ���ݹ���ά�޻�����ȡ����
    double d7 = GetDlgItemValueD(IDC_EDIT07);
    // ����ֵ�ĺϷ���
    {
        if(d4 < 1.0) {
            AfxMessageBox(_T("������Ϸ�����"));
            return;
        }
    }
    // ��ʼ����,����ֵ���ؼ�
    {
        // �ɹ������ܼ�
        double d8 = (d1 * 10000 + d3) * 1.8;
        SetDlgItemValueD(IDC_EDIT8, d8);
        // ��˰
        double d9 = d8 * d2 / 100.0;
        SetDlgItemValueD(IDC_EDIT9, d9);
        // �� ����
        double d10 = d1 / d4;
        SetDlgItemValueD(IDC_EDIT10, d10);
        // ��ʦ��
        double d11 = d10 * 0.02;
        SetDlgItemValueD(IDC_EDIT11, d11);
        // �ɹ����ݵ���
        double d12 = d10 / d5;
        SetDlgItemValueD(IDC_EDIT12, d12);
        // ����ά�޻���
        double d13 = d10 * 0.003;
        SetDlgItemValueD(IDC_EDIT13, d13);
        // ���շ�
        double d14 = d1 * 0.0035;
        SetDlgItemValueD(IDC_EDIT14, d14);
        // ��Ѻ�ǼǷ� :��ֵ�̶�
        //double d15 = 0;
        SetDlgItemText(IDC_EDIT15, _T("200 - 500"));
    }
}


BOOL CDialogCalc03::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    //�������
    OnBnClickedButtonClear();
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogCalc03::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogCalc03::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


BOOL CDialogCalc03::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
