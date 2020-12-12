#pragma once
#include <tchar.h>

#if 1
/// ////////////////////////////////////////////////////////////////////////////////
/// 带时间输出日志
#define LOG_LEN_MAX 1024*4

#define LogA(fmt, ...) do { \
    char szBuffer[LOG_LEN_MAX] = {}; \
    char szTime[]={"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; \
    GetLocalTime(&t);\
    sprintf_s(szTime, "%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    sprintf_s(szBuffer, "[%s][%s-%03d]" ## fmt ## "\r\n", szTime, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringA(szBuffer); \
} while(0)
#define LogW(fmt, ...) do { \
    wchar_t szBuffer[LOG_LEN_MAX] = {}; \
    wchar_t szTime[]={L"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; \
    GetLocalTime(&t);\
    swprintf_s(szTime, L"%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    swprintf_s(szBuffer, L"[%s][%s-%03d]" ## fmt ## L"\r\n", szTime, __FUNCTIONW__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringW(szBuffer); \
} while(0)

#ifndef BASE_FUN_NAME
#   if (defined UNICODE) || (defined _UNICODE)
#       define BASE_FUN_NAME __FUNCTIONW__
#   else
#       define BASE_FUN_NAME __FUNCTION__
#   endif
#endif

#define Log(fmt, ...) do { \
    TCHAR szBuffer[LOG_LEN_MAX] = {}; \
    TCHAR szTime[]={_T("2014/12/12 12:12:12.999    ")}; \
    SYSTEMTIME t; \
    GetLocalTime(&t);\
    _stprintf_s(szTime, _T("%4d/%02d/%02d %02d:%02d:%02d.%03d"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    _stprintf_s(szBuffer, _T("[%s][%s-%03d]") ## fmt ## _T("\r\n"), szTime, BASE_FUN_NAME, __LINE__, ##__VA_ARGS__); \
    OutputDebugString(szBuffer); \
} while(0)

#define LogMsgA(fmt, ...) do {\
    char szBuffer[LOG_LEN_MAX] = {}; \
    char szTime[]={"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; \
    GetLocalTime(&t);\
    sprintf_s(szTime, "%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    sprintf_s(szBuffer, "[%s][%s-%03d]" ## fmt ## "\r\n", szTime, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringA(szBuffer); \
    ::MessageBoxA(NULL, szBuffer, "LogMsgA", MB_OK); \
}while(0)

#define LogMsgW(fmt, ...) do {\
    wchar_t szBuffer[LOG_LEN_MAX] = {}; \
    wchar_t szTime[]={L"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; \
    GetLocalTime(&t);\
    swprintf_s(szTime, L"%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    swprintf_s(szBuffer, L"[%s][%s-%03d]" ## fmt ## L"\r\n", szTime, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringW(szBuffer); \
    ::MessageBoxW(NULL, szBuffer, L"LogMsgA", MB_OK); \
}while(0)

#define LogMsgTimeoutA(timeout, fmt, ...) do {\
    char szCaption[LOG_LEN_MAX] = {}; \
    sprintf_s(szCaption, "%d豪秒后退出", timeout); \
    char szTime[]={"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; GetLocalTime(&t);\
    sprintf_s(szTime, "%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    char szBuffer[LOG_LEN_MAX] = {}; \
    sprintf_s(szBuffer, "[%s][%s-%03d]" ## fmt ## "\r\n", szTime, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringA(szBuffer); \
    ::MessageBoxTimeoutA(NULL, szBuffer, szCaption, MB_OK, 0, timeout); \
}while(0)

#define LogMsgTimeoutW(timeout, fmt, ...) do {\
    wchar_t szCaption[LOG_LEN_MAX] = {}; \
    swprintf_s(szCaption, L"%d豪秒后退出", timeout); \
    wchar_t szTime[]={L"2014/12/12 12:12:12.999    "}; \
    SYSTEMTIME t; GetLocalTime(&t);\
    swprintf_s(szTime, L"%4d/%02d/%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); \
    wchar_t szBuffer[LOG_LEN_MAX] = {}; \
    swprintf_s(szBuffer, L"[%s][%s-%03d]" ## fmt ## L"\r\n", szTime, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringW(szBuffer); \
    ::MessageBoxTimeoutW(NULL, szBuffer, szCaption, MB_OK, 0, timeout); \
}while(0)
#endif
