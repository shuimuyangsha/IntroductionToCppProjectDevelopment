
// MrkjSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMrkjSystemApp:
// �йش����ʵ�֣������ MrkjSystem.cpp
//

class CMrkjSystemApp : public CWinApp
{
public:
    CMrkjSystemApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();

private:
    // ��ʼ��GDI+��Ҫ�ı���
    GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken;
};

extern CMrkjSystemApp theApp;