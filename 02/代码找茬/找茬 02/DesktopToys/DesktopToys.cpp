
// DesktopToys.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DesktopToys.h"
#include "DesktopToysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesktopToysApp

BEGIN_MESSAGE_MAP(CDesktopToysApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDesktopToysApp ����

CDesktopToysApp::CDesktopToysApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDesktopToysApp ����

CDesktopToysApp theApp;


// CDesktopToysApp ��ʼ��

BOOL CDesktopToysApp::InitInstance()
{
    CWinApp::InitInstance();


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

    // GDI+ ��ʼ��
    Status s = GdiplusStartup(&m_tokenGdiplus, &input, &output);


    CDesktopToysDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: �ڴ˷��ô����ʱ��
        //  ��ȷ�������رնԻ���Ĵ���
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: �ڴ˷��ô����ʱ��
        //  ��ȡ�������رնԻ���Ĵ���
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
        TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
    }

    // ɾ�����洴���� shell ��������
    if (pShellManager != NULL)
    {
        delete pShellManager;
    }

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}



int CDesktopToysApp::ExitInstance()
{
    // ж�� GDI+
    GdiplusShutdown(m_tokenGdiplus);
    return CWinApp::ExitInstance();
}
