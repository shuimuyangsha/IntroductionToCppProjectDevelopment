
// DesktopToys.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDesktopToysApp: 
// �йش����ʵ�֣������ DesktopToys.cpp
//

class CDesktopToysApp : public CWinApp
{
public:
	CDesktopToysApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    // ���� GDIPlus ����Ҫ�ı���
    ULONG_PTR m_tokenGdiplus;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartupOutput output;

};

extern CDesktopToysApp theApp;