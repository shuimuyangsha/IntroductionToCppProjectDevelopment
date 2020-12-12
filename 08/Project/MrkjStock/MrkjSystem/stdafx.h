
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define  CHECK_LOGIN 1 // �����Ƿ���� ��½Ȩ�޼��



/********************************************************************************/
// # libcurl
#define BUILDING_LIBCURL
#define HTTP_ONLY
#include "../3thd/libcurl/include/curl/curl.h"
#if (defined DEBUG || defined _DEBUG)
#  pragma comment(lib, "../3thd/libcurl/libcurld.lib")
#else
#  pragma comment(lib, "../3thd/libcurl/libcurl.lib")
#endif
#pragma comment(lib, "ws2_32.lib")  // libcurl ��Ҫ�Ŀ�
#pragma comment(lib, "winmm.lib")   // libcurl ��Ҫ�Ŀ�
#pragma comment(lib, "wldap32.lib") // libcurl ��Ҫ�Ŀ�


#include <tchar.h>

// C++ ��׼��
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <tuple>
#include <algorithm>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <memory>
#include <functional>
#include <iterator>
using namespace std;

#include "resource.h"


// GDI
#pragma comment( lib, "gdiplus.lib" )
#include "gdiplus.h"
using namespace Gdiplus;


#include "StringHelper.h"
#include "Config.h"
#include "Log.h"

#include "MRKJDatabase.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) do { if(ptr) {delete ptr; ptr = nullptr;} }while(0)
#endif


extern CMRKJDatabase::DBEntryUser g_loginUser;


