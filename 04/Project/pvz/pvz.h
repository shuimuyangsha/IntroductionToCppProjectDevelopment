// pvz.h : pvz DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CpvzApp
// �йش���ʵ�ֵ���Ϣ������� pvz.cpp
//

class CpvzApp : public CWinApp
{
public:
    CpvzApp();

// ��д
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

void __stdcall SetHook();


