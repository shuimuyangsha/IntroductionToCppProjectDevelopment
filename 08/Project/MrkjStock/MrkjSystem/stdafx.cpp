
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// MrkjSystem.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


#if CHECK_LOGIN
CMRKJDatabase::DBEntryUser g_loginUser;
#else
CMRKJDatabase::DBEntryUser g_loginUser {L"1", L"admin", L"admin", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1"};
#endif
