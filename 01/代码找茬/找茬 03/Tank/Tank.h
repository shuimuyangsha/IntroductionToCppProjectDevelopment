
// Tank.h : Tank Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTankApp:
// �йش����ʵ�֣������ Tank.cpp
//

class CTankApp : public CWinApp
{
public:
    CTankApp();


// ��д
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

// ʵ��

public:
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()

private:
    // ���� GDIPlus ����Ҫ�ı���
    ULONG_PTR m_tokenGdiplus;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartupOutput output;

};

extern CTankApp theApp;
