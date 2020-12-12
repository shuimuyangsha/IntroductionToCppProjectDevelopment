// ECSSrv.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "ECSSrvDlg.h"
#include "ClientInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECSSrvApp

BEGIN_MESSAGE_MAP(CECSSrvApp, CWinApp)
	//{{AFX_MSG_MAP(CECSSrvApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECSSrvApp construction

CECSSrvApp::CECSSrvApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CECSSrvApp object

CECSSrvApp theApp;
HANDLE hMutex;
/////////////////////////////////////////////////////////////////////////////
// CECSSrvApp initialization

BOOL CECSSrvApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	hMutex = CreateMutex(NULL,FALSE,"Run");
	if (GetLastError()==ERROR_ALREADY_EXISTS)	// 有一个实例已经运行
	{
		return FALSE;
	}


	char chFullName[MAX_PATH] = {0};
	// 获取应用程序名称，包含路径
	GetModuleFileName(NULL,chFullName,MAX_PATH);
	char *pchFind = strrchr(chFullName,'\\');
	int nPos = pchFind - chFullName +1;
	strncpy(m_chFullPath,chFullName,nPos-1);

	// 初始化COM库
	AfxOleInit();

    AfxSocketInit();

	CECSSrvDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CECSSrvApp::ExitInstance() 
{
	// 释放客户端信息列表
	CloseHandle(hMutex);
	POSITION pos = m_ClientList.GetHeadPosition();
	if(pos)
	{
		CClientInfo *m_pClientInfo;
		for(int i=0; i<m_ClientList.GetCount(); i++)
		{
			if (pos != NULL)
			{
				m_pClientInfo = static_cast <CClientInfo *>(m_ClientList.GetNext(pos));
				ASSERT(m_pClientInfo != NULL);
				delete m_pClientInfo;
			}
		}
	}	
	pos = m_FileList.GetHeadPosition();
	CClientInfo *m_pClientInfo;
	for(int i=0; i<m_ClientList.GetCount(); i++)
	{
		if(pos != NULL)
		{
			m_pClientInfo = static_cast <CClientInfo *>(m_FileList.GetNext(pos));
			ASSERT(m_pClientInfo != NULL);
			delete m_pClientInfo;
		}
	}
	return CWinApp::ExitInstance();
}
