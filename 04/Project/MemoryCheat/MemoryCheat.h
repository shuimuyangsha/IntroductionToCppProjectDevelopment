
// MemoryCheat.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMemoryCheatApp:
// �йش����ʵ�֣������ MemoryCheat.cpp
//

class CMemoryCheatApp : public CWinApp
{
public:
    CMemoryCheatApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CMemoryCheatApp theApp;