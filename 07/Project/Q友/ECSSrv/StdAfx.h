// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__523B7C5A_389E_4B39_9671_AE57075C7D97__INCLUDED_)
#define AFX_STDAFX_H__523B7C5A_389E_4B39_9671_AE57075C7D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Afxsock.h"
#include "Message.h"
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" \
	rename_namespace ("ADODB") \
rename("EOF","ADOEOF")
using namespace ADODB;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__523B7C5A_389E_4B39_9671_AE57075C7D97__INCLUDED_)
