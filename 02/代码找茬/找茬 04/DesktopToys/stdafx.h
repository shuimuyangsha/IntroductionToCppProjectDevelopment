
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





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

// ����C++��׼��
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>

#include <assert.h>

using namespace std;

// ���� GDIPlus
#include <gdiplus.h>
#include <gdipluseffects.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

#include "ScopeGuard.h"

// ����ɫ͸�����õ���ɫ
#define BACK_GROUND_LAYER RGB(0x00, 0x00, 0x00)

// 
#define PI(n) (3.1415926f * n)
// ����->�Ƕ�
#define Radian2Degree(r) ((180.0f * (r)) / PI(1.0f))
// �Ƕ�->����
#define Degree2Radian(a) ((a) * PI(1.0f) / 180.0f)

#include "Game.h"
extern CGame* g_game;


