
// stdafx.cpp : 只包括标准包含文件的源文件
// MrkjSystem.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


#if CHECK_LOGIN
CMRKJDatabase::DBEntryUser g_loginUser;
#else
CMRKJDatabase::DBEntryUser g_loginUser {L"1", L"admin", L"admin", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1", L"1"};
#endif
