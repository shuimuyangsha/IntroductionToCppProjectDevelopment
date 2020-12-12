
// MrkjSystem.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "MrkjSystemDlg.h"
#include "DialogLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMrkjSystemApp

BEGIN_MESSAGE_MAP(CMrkjSystemApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMrkjSystemApp ����

CMrkjSystemApp::CMrkjSystemApp()
{
    // ֧����������������
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMrkjSystemApp ����

CMrkjSystemApp theApp;


// CMrkjSystemApp ��ʼ��

BOOL CMrkjSystemApp::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    if(!AfxSocketInit()) {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }
    AfxEnableControlContainer();
    // ���� shell ���������Է��Ի������
    // �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
    CShellManager *pShellManager = new CShellManager;
    // ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    // ��ʼ��com
    CoInitialize(NULL);
    // GDI+ ��ʼ��
    GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
    // ���������ļ�
    {
        //CFG.ReadString(L"", L"", L"");
    }
    // �������ݿ�
    {
        //����,�û���,����
        std::string host = CFG.ReadString("���ݿ�", "����", "AGAIN");
        std::string database = CFG.ReadString("���ݿ�", "���ݿ�", "MrkjStock");
        std::string username = CFG.ReadString("���ݿ�", "�û�", "sa");
        std::string password = CFG.ReadString("���ݿ�", "����", "");
        if(!DB.Connect(host.c_str(), database.c_str(), username.c_str(), password.c_str())) {
            AfxMessageBox(_T("�������ݿ�ʧ��"));
            return FALSE;
        }
    }
#if CHECK_LOGIN
    CDialogLogin dlg;
    if(IDOK == dlg.DoModal()) {
#endif
        CMrkjSystemDlg dlg;
        m_pMainWnd = &dlg;
        INT_PTR nResponse = dlg.DoModal();
        if(nResponse == IDOK) {
        }
        else if(nResponse == IDCANCEL) {
            // TODO: �ڴ˷��ô����ʱ��
            //  ��ȡ�������رնԻ���Ĵ���
        }
        else if(nResponse == -1) {
            TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
            TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
        }
#if CHECK_LOGIN
    }
#endif
    // ɾ�����洴���� shell ��������
    if(pShellManager != NULL) {
        delete pShellManager;
    }
    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}



int CMrkjSystemApp::ExitInstance()
{
    GdiplusShutdown(m_gdiplusToken);
    CoUninitialize();
    return CWinApp::ExitInstance();
}
