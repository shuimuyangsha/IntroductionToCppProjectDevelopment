// ECSClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ECSClient.h"
#include "ECSClientDlg.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECSClientApp

BEGIN_MESSAGE_MAP(CECSClientApp, CWinApp)
    //{{AFX_MSG_MAP(CECSClientApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!

    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSClientApp construction

CECSClientApp::CECSClientApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CECSClientApp object

CECSClientApp theApp;
HANDLE hMutex;
/////////////////////////////////////////////////////////////////////////////
// CECSClientApp initialization

BOOL CECSClientApp::InitInstance()
{
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();          // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();    // Call this when linking to MFC statically
#endif


    // ��ȡӦ�ó������ƣ�����·��
    {
        char chFullName[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, chFullName, MAX_PATH);
        char *pchFind = strrchr(chFullName, '\\');
        int nPos = pchFind - chFullName + 1;
        strncpy(m_chFullPath, chFullName, nPos - 1);
    }

    // �ж��Ƿ񼺾�����
#ifndef _DEBUG
    {
        hMutex = CreateMutex(NULL, FALSE, "RunClient");
        if(GetLastError() == ERROR_ALREADY_EXISTS) {     // ��һ��ʵ���Ѿ�����
            return FALSE;
        }
    }
#endif

    // ��ʼ��COM��
    CoInitialize(NULL);

    // ��ʼ���׽���
    AfxSocketInit();

    // ��ʼ��RichEditCtrl
    AfxInitRichEdit();

    // ������
    CECSClientDlg dlg;
    dlg.Create(IDD_ECSCLIENT_DIALOG);
    m_pMainWnd = &dlg;
    dlg.ShowWindow(SW_HIDE);

    // ��¼�Ի���
    CLogin login(&dlg);
    if(login.DoModal() == IDCANCEL) {
        return FALSE;
    }

    // ��������ʾ
    dlg.ShowWindow(SW_SHOW);

    int nResponse = dlg.RunModalLoop();
    if(nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if(nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }
    //
    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

int CECSClientApp::ExitInstance()
{
    if(hMutex) {
        CloseHandle(hMutex);
    }
    // ж��
    CoUninitialize();

    return CWinApp::ExitInstance();
}
