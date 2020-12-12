// DialogCalc02.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogCalc02.h"
#include "afxdialogex.h"


// CDialogCalc02 �Ի���

IMPLEMENT_DYNAMIC(CDialogCalc02, CDialogEx)

CDialogCalc02::CDialogCalc02(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG2, pParent)
    , m_strCode(_T(""))
    , m_strName(_T(""))
{
}

CDialogCalc02::~CDialogCalc02()
{
}

void CDialogCalc02::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_strCode);
    DDX_Text(pDX, IDC_EDIT2, m_strName);
}


BEGIN_MESSAGE_MAP(CDialogCalc02, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CDialogCalc02::OnEnKillfocusEdit1)
    ON_BN_CLICKED(IDC_BUTTON5, &CDialogCalc02::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON4, &CDialogCalc02::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDialogCalc02 ��Ϣ�������


void CDialogCalc02::OnEnKillfocusEdit1()
{
    UpdateData();
    std::wstring name;
    if(DB.QueryStockNameByCode(m_strCode, name)) {
        SetDlgItemText(IDC_EDIT2, name.c_str());
    }
}

void CDialogCalc02::OnBnClickedButton5()
{
    SetDlgItemText(IDC_EDIT1, _T(""));
    SetDlgItemText(IDC_EDIT2, _T(""));
    SetDlgItemText(IDC_EDIT9, _T(""));
    SetDlgItemText(IDC_EDIT10, _T(""));
    SetDlgItemText(IDC_EDIT11, _T(""));
}


void CDialogCalc02::OnBnClickedButton4()
{
    UpdateData();
    /* ����ʽ:
        ��ͣ  = �۸� * 1.1
        ��ͣ  = �۸� * 0.9
    */
    float fPrice;
    {
        CString sPrice;
        GetDlgItemText(IDC_EDIT9, sPrice);
        TCHAR * szEnd = nullptr;
        fPrice = _tcstof(sPrice, &szEnd);
    }
    float fDailyLimit = fPrice * 1.1f;
    float fDownLimit = fPrice * 0.9f;
    // �������� ���� ��λС��
    using namespace std;
    SetDlgItemText(IDC_EDIT10, StringHelper::ToString(fDailyLimit, 2).c_str());
    SetDlgItemText(IDC_EDIT11, StringHelper::ToString(fDownLimit, 2).c_str());
    UpdateData(FALSE);
}


void CDialogCalc02::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogCalc02::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}
BOOL CDialogCalc02::PreTranslateMessage(MSG* pMsg)
{
    // ��ֹ ESC/�س��� �Զ��˳�
    if(pMsg->message == WM_KEYFIRST) {
        if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
            return TRUE; // ���ط�0,�������Ϣ���������зַ�
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
