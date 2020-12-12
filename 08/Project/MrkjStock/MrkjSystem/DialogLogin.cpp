// DialogLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogLogin.h"
#include "afxdialogex.h"


// CDialogLogin �Ի���

IMPLEMENT_DYNAMIC(CDialogLogin, CDialogEx)

CDialogLogin::CDialogLogin(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_LOGIN, pParent)
    , m_strUsername(_T(""))
    , m_strPassword(_T(""))
    , m_bRecordPass(FALSE)
{
}

CDialogLogin::~CDialogLogin()
{
}

void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
    DDX_Check(pDX, IDC_CHECK_RECORD, m_bRecordPass);
}


BEGIN_MESSAGE_MAP(CDialogLogin, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialogLogin::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialogLogin::OnBnClickedButtonCancel)
    ON_BN_CLICKED(IDC_CHECK_RECORD, &CDialogLogin::OnBnClickedCheckRecord)
END_MESSAGE_MAP()


// CDialogLogin ��Ϣ�������

BOOL CDialogLogin::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    std::wstring username = CFG.ReadString(L"��½�û�", L"�û���", L"");
    m_strUsername = username.c_str();
    BOOL bRecord = CFG.ReadBOOL(L"��½�û�", L"�Ƿ��¼����", 0);
    std::wstring password = CFG.ReadString(L"��½�û�", L"����", L"");
    if(bRecord) {
        m_strPassword = password.c_str();
    }
    m_bRecordPass = bRecord;
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogLogin::OnBnClickedButtonLogin()
{
    UpdateData(TRUE);
    // ��¼�������ļ�
    RecordInfo();
    // �����ݿ��н��жԱ�
    if(!DB.CheckUser(m_strUsername, m_strPassword)) {
        AfxMessageBox(_T("�û������������"));
        return;
    }
    // ��ѯ���ݿ��е�Ȩ����Ϣ
    if(!DB.QueryUserByUsername(g_loginUser, m_strUsername)) {
        AfxMessageBox(_T("��ȡ�û���Ϣʧ��"));
        return;
    }
    // �˳���ǰ�Ի���
    OnOK();
}

void CDialogLogin::OnBnClickedButtonCancel()
{
    // ���ø÷���,�����˳�CDialog����
    OnCancel();
}

void CDialogLogin::RecordInfo()
{
    // ����"�û���"�������ļ���
    CFG.SaveFormat(L"��½�û�", L"�û���", L"%s", m_strUsername.GetString());
    // ����"�Ƿ��¼����"�������ļ���
    CFG.SaveFormat(L"��½�û�", L"�Ƿ��¼����", L"%d", m_bRecordPass);
    // ��� ѡ������ "��¼��½����",���¼���뵽�����ļ���
    if(m_bRecordPass) {
        CFG.SaveFormat(L"��½�û�", L"����", L"%s", m_strPassword.GetString());
    }
}

void CDialogLogin::OnBnClickedCheckRecord()
{
    UpdateData(TRUE);
    RecordInfo();
    UpdateData(FALSE);
}


void CDialogLogin::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���

    CDialogEx::OnOK();
}


void CDialogLogin::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���

    CDialogEx::OnCancel();
}


BOOL CDialogLogin::PreTranslateMessage(MSG* pMsg)
{
    return CDialogEx::PreTranslateMessage(pMsg);
}
