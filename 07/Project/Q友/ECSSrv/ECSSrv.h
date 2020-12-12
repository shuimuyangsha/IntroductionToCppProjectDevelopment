// ECSSrv.h : main header file for the ECSSRV application
//

#if !defined(AFX_ECSSRV_H__85A61A7F_6491_472A_8D9B_08DF86E14155__INCLUDED_)
#define AFX_ECSSRV_H__85A61A7F_6491_472A_8D9B_08DF86E14155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CECSSrvApp:
// See ECSSrv.cpp for the implementation of this class
//


class CECSSrvApp : public CWinApp
{
public:
	CECSSrvApp();
	CPtrList  m_ClientList;		// 记录客户端连接的套接字
	CPtrList  m_FileList;		// 记录发送文件时客户端连接的套接字
	char m_chFullPath[MAX_PATH];	// 应用程序路径，不包含名称和扩展名

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECSSrvApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CECSSrvApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSSRV_H__85A61A7F_6491_472A_8D9B_08DF86E14155__INCLUDED_)
