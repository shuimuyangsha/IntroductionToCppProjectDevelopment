// CreateReg.h : main header file for the CREATEREG application
//

#if !defined(AFX_CREATEREG_H__AFCD3F08_C8C8_4E36_929D_84BE5281043F__INCLUDED_)
#define AFX_CREATEREG_H__AFCD3F08_C8C8_4E36_929D_84BE5281043F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCreateRegApp:
// See CreateReg.cpp for the implementation of this class
//

class CCreateRegApp : public CWinApp
{
public:
	CCreateRegApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateRegApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCreateRegApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEREG_H__AFCD3F08_C8C8_4E36_929D_84BE5281043F__INCLUDED_)
