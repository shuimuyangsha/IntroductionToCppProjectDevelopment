// DialogAbout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogAbout.h"
#include "afxdialogex.h"


// CDialogAbout �Ի���

IMPLEMENT_DYNAMIC(CDialogAbout, CDialogEx)

CDialogAbout::CDialogAbout(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_ABOUT, pParent)
    , m_strAboutText(_T(""))
    , m_strCopyright(_T(""))
    , m_strPhone(_T(""))
    , m_strUrl(_T(""))
{
}

CDialogAbout::~CDialogAbout()
{
}

void CDialogAbout::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_ABOUT_TXT, m_strAboutText);
    DDX_Text(pDX, IDC_STATIC_ABOUT_COPYRIGHT, m_strCopyright);
    DDX_Text(pDX, IDC_STATIC_ABOUT_PHONE, m_strPhone);
    DDX_Text(pDX, IDC_STATIC_ABOUT_URL, m_strUrl);
}


BEGIN_MESSAGE_MAP(CDialogAbout, CDialogEx)
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogAbout ��Ϣ�������

BOOL CDialogAbout::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ����
    {
        CString str;
        str.LoadString(IDS_STRING_SYSTEM_NAME);
        str = L"����" + str;
        SetWindowText(str);
    }
    // ͼ��
    {
        // û��
    }
    //  �����ַ���
    {
        m_strAboutText.LoadString(IDS_STRING_ABOUT_TEXT);
        m_strPhone.LoadString(IDS_STRING_ABOUT_PHONE);
        m_strUrl.LoadString(IDS_STRING_ABOUT_URL);
        m_strCopyright.LoadString(IDS_STRING_ABOUT_COPYRIGHT);
    }
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogAbout::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


BOOL CDialogAbout::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogAbout::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnOK();
}


void CDialogAbout::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


HBRUSH CDialogAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}
