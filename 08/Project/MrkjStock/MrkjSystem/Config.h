#pragma once

#include "stdafx.h"
#include "string"
#include "Tools.h"


class CConfig
{
    const std::wstring FILENAME{ L"sys.ini" };
    CConfig()
    {
        std::wstring strFilepath = CTools::GetDir();
        if(PathFileExists((strFilepath + L"\\" + FILENAME).c_str())) {
            m_strFilepath = (strFilepath + L"\\" + FILENAME);
            return;
        }
        AfxMessageBox(_T("没发现配置文件"));
    }

    ~CConfig()
    {
    }

    // 保存配置文件的路径
    std::wstring m_strFilepath;

public:
    // 返回本类的实例
    static CConfig& Instance()
    {
        static CConfig cfg;
        return cfg;
    }

    // 读取字符串
    std::wstring ReadString(const TCHAR *szApp, const TCHAR *szKey, const TCHAR* szDefault = _T("")) const
    {
        std::wstring strRet = _T("");
        DWORD dwBufSize = 64;
        TCHAR *szBuffer = nullptr;
        for(int _ = 0; _ < 10; ++_) {
            szBuffer = new TCHAR[dwBufSize + 1];
            ZeroMemory(szBuffer, (1 + dwBufSize) + sizeof(TCHAR));
            DWORD dwRead = ::GetPrivateProfileString(szApp, szKey, szDefault, szBuffer, 1 + dwBufSize , m_strFilepath.c_str());
            //读入的长度 == 缓冲区长度-sizeof(TCHAR),可能有未读完的数据
            if(dwRead == dwBufSize) {
                // 删除原来的缓冲区,并把长度X4
                delete[] szBuffer;
                szBuffer = nullptr;
                dwBufSize *= 4;
            }
            else {
                strRet = szBuffer;
                delete[] szBuffer;
                szBuffer = nullptr;
                break;
            }
        }
        if(szBuffer) {
            delete [] szBuffer;
            szBuffer = nullptr;
        }
        return strRet;
    }

    // 读取字符串
    std::string ReadString(const char *szApp, const char *szKey, const char* szDefault = "") const
    {
        //这种做法,只能读到1023个字符
        char buf[1024] = {};
        DWORD cbRead = 0;
        cbRead = ::GetPrivateProfileStringA(szApp, szKey, szDefault, buf
                                            , _countof(buf), StringHelper::UnicodeToANSI(m_strFilepath).c_str());
        if(cbRead == 0) {
            return szDefault;
        }
        else {
            buf[cbRead] = '\0';
            return buf;
        }
    }

    // 读取 BOOl 类型值(只能是 1 or 0)
    BOOL ReadBOOL(const wchar_t *szApp, const wchar_t *szKey, BOOL bDefault = FALSE) const
    {
        return GetPrivateProfileInt(szApp, szKey, bDefault, m_strFilepath.c_str());
    }

    // 保存
    void SaveFormat(const TCHAR *const szAppName, TCHAR const* const szKeyName, TCHAR const* const szFormat, ...)
    {
        std::wstring strContent = L"";
        //格式化字符串
        {
            va_list args;
            int len;
            TCHAR *buffer = NULL;
            va_start(args, szFormat);
            len = _vsctprintf(szFormat, args) + 1;
            buffer = new TCHAR[len * sizeof(TCHAR)];
            if(!buffer) {
                goto _fend;
            }
            _vstprintf_s(buffer, len, szFormat, args);
            buffer[len - 1] = _T('\0');
            strContent = buffer;
_fend:
            if(buffer) {
                delete[] buffer;
            }
        }
        if(strContent.empty()) {
            return;
        }
        WritePrivateProfileString(szAppName, szKeyName, strContent.c_str(), m_strFilepath.c_str());
    }

};
// 返因全局唯一实例
#define CFG (CConfig::Instance())
