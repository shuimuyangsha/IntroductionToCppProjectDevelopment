// ECSClient.h : main header file for the ECSCLIENT application
//

#if !defined(AFX_ECSCLIENT_H__441F964A_7A07_4BA1_902E_D0E5CCE38A28__INCLUDED_)
#define AFX_ECSCLIENT_H__441F964A_7A07_4BA1_902E_D0E5CCE38A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CECSClientApp:
// See ECSClient.cpp for the implementation of this class
//
#include "ECSClientDlg.h"
class CECSClientApp : public CWinApp
{
public:
	CECSClientApp();
	char m_chFullPath[MAX_PATH];	// 应用程序路径，不包含名称和扩展名,只是目录

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECSClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CECSClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSCLIENT_H__441F964A_7A07_4BA1_902E_D0E5CCE38A28__INCLUDED_)
