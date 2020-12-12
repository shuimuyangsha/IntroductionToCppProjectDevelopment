#pragma once

namespace StringHelper
{
    //字符转码

    inline int _GetLen_M2W(__in   UINT CodePage, __in   LPCSTR lpMultiByteStrM)
    {
        return ::MultiByteToWideChar(CodePage, 0, lpMultiByteStrM, -1, NULL, 0);
    }
    inline int _GetLen_W2M(__in   UINT CodePage, __in   LPCWSTR lpWideCharStr)
    {
        return ::WideCharToMultiByte(CodePage,
                                     0,
                                     lpWideCharStr,
                                     -1,
                                     NULL,
                                     0,
                                     NULL,
                                     NULL);
    }
    inline wchar_t *_ANSIToUnicode(const char *str)
    {
        wchar_t *pUnicode = NULL;
        try {
            int  unicodeLen = _GetLen_M2W(CP_ACP, str);
            if(unicodeLen <= 0) {
                throw("计算需要长度为0");
            }
            pUnicode = new  wchar_t[unicodeLen + 1];
            memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
            if(0 == ::MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)pUnicode, unicodeLen)) {
                throw("转换出错");
            }
        }
        catch(...) {
            TRACE("发生异常\r\n");
            if(pUnicode) {
                delete[]pUnicode;
                pUnicode = NULL;
            }
        }
        return pUnicode;
    }
    inline char    *_UnicodeToAnsi(const wchar_t *str)
    {
        char     *pElementText = NULL;
        try {
            int iTextLen = _GetLen_W2M(CP_ACP, str);
            if(iTextLen <= 0) {
                throw("计算需要长度为0");
            }
            pElementText = new char[iTextLen + 1];
            memset((void *)pElementText, 0, sizeof(char) * (iTextLen + 1));
            if(0 == ::WideCharToMultiByte(CP_ACP, 0, str, -1, pElementText, iTextLen, NULL, NULL)) {
                throw("转换出错");
            }
        }
        catch(...) {
            TRACE("发生异常\r\n");
            if(!pElementText) {
                delete[] pElementText;
                pElementText = NULL;
            };
        }
        return pElementText;
    }

    inline wchar_t *_UTF8ToUnicode(const char *str)
    {
        wchar_t *pUnicode = NULL;
        try {
            int  unicodeLen = _GetLen_M2W(CP_UTF8, str);
            if(unicodeLen <= 0) {
                throw("计算需要长度为0");
            }
            pUnicode = new  wchar_t[unicodeLen + 1];
            memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
            if(0 == ::MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)pUnicode, unicodeLen)) {
                throw("转换出错");
            }
        }
        catch(...) {
            TRACE("发生异常\r\n");
            if(pUnicode) {
                delete[]pUnicode;
                pUnicode = NULL;
            }
        }
        return pUnicode;
    }
    inline char    *_UnicodeToUTF8(const wchar_t *str)
    {
        char     *pElementText = NULL;
        try {
            int iTextLen = _GetLen_W2M(CP_UTF8, str);
            if(iTextLen <= 0) {
                throw("计算需要长度为0");
            }
            pElementText = new char[iTextLen + 1];
            memset((void *)pElementText, 0, sizeof(char) * (iTextLen + 1));
            if(0 == ::WideCharToMultiByte(CP_UTF8, 0, str, -1, pElementText, iTextLen, NULL, NULL)) {
                throw("转换出错");
            }
        }
        catch(...) {
            TRACE("发生异常\r\n");
            if(!pElementText) {
                delete[] pElementText;
                pElementText = NULL;
            };
        }
        return pElementText;
    }

    inline std::wstring ANSIToUnicode(const std::string  &str)
    {
        int  len = 0;
        len = str.length();
        int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
                                                0,
                                                str.c_str(),
                                                -1,
                                                NULL,
                                                0);
        wchar_t   *pUnicode = new  wchar_t[unicodeLen + 1];
        memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
        ::MultiByteToWideChar(CP_ACP,
                              0,
                              str.c_str(),
                              -1,
                              (LPWSTR)pUnicode,
                              unicodeLen);
        std::wstring  rt;
        rt = (wchar_t *)pUnicode;
        delete[] pUnicode;
        return  rt;
    }
    inline std::string  UnicodeToANSI(const std::wstring &str)
    {
        char     *pElementText;
        int    iTextLen;
        // wide char to multi char
        iTextLen = WideCharToMultiByte(CP_ACP,
                                       0,
                                       str.c_str(),
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
        pElementText = new char[iTextLen + 1];
        memset((void *)pElementText, 0, sizeof(char) * (iTextLen + 1));
        ::WideCharToMultiByte(CP_ACP,
                              0,
                              str.c_str(),
                              -1,
                              pElementText,
                              iTextLen,
                              NULL,
                              NULL);
        std::string strText;
        strText = pElementText;
        delete[] pElementText;
        return strText;
    }
    inline std::wstring UTF8ToUnicode(const std::string  &str)
    {
        int  len = 0;
        len = str.length();
        int  unicodeLen = ::MultiByteToWideChar(CP_UTF8,
                                                0,
                                                str.c_str(),
                                                -1,
                                                NULL,
                                                0);
        wchar_t   *pUnicode = new  wchar_t[unicodeLen + 1];
        memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
        ::MultiByteToWideChar(CP_UTF8,
                              0,
                              str.c_str(),
                              -1,
                              (LPWSTR)pUnicode,
                              unicodeLen);
        std::wstring  rt;
        rt = (wchar_t *)pUnicode;
        delete[] pUnicode;
        return  rt;
    }
    inline std::string  UnicodeToUTF8(const std::wstring &str)
    {
        char     *pElementText;
        int    iTextLen;
        // wide char to multi char
        iTextLen = WideCharToMultiByte(CP_UTF8,
                                       0,
                                       str.c_str(),
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
        pElementText = new char[iTextLen + 1];
        memset((void *)pElementText, 0, sizeof(char) * (iTextLen + 1));
        ::WideCharToMultiByte(CP_UTF8,
                              0,
                              str.c_str(),
                              -1,
                              pElementText,
                              iTextLen,
                              NULL,
                              NULL);
        std::string strText;
        strText = pElementText;
        delete[] pElementText;
        return strText;
    }
    inline std::string  UTF8ToANSI(const std::string &str)
    {
        std::string s("");
        const char *pAnsi = NULL;
        const wchar_t *pUnicode = NULL;
        pUnicode = _UTF8ToUnicode(str.c_str());
        if(!pUnicode) {
            goto __End;
        }
        pAnsi = _UnicodeToAnsi(pUnicode);
        if(!pAnsi) {
            goto __End;
        }
        s.append(pAnsi);
__End:
        return s;
    }
    inline std::string  ANSIToUTF8(const std::string &str)
    {
        std::string s("");
        char *pUTF8 = NULL;
        wchar_t *pUnicode = NULL;
        pUnicode = _ANSIToUnicode(str.c_str());
        if(!pUnicode) {
            goto __End;
        }
        pUTF8 = _UnicodeToUTF8(pUnicode);
        if(!pUTF8) {
            goto __End;
        }
        s.append(pUTF8);
__End:
        if(pUnicode) {
            delete[] pUnicode;
        }
        if(pUTF8) {
            delete[] pUTF8;
        }
        return s;
    }

    inline std::wstring ANSIToUnicode(const char *str)
    {
        std::wstring s(L"");
        if(!str) {
            return s;
        }
        const wchar_t *pUnicode = _ANSIToUnicode(str);
        if(!pUnicode) {
            return s;
        }
        s.append(pUnicode);
        delete[] pUnicode;
        return s;
    }
    inline std::string  UnicodeToANSI(const wchar_t *str)
    {
        std::string s("");
        if(!str) {
            return s;
        }
        const char *pAnsi = _UnicodeToAnsi(str);
        if(!pAnsi) {
            return s;
        }
        s.append(pAnsi);
        delete[] pAnsi;
        return s;
    }
    inline std::wstring UTF8ToUnicode(const char *str)
    {
        std::wstring s(L"");
        if(!str) {
            return s;
        }
        const wchar_t *pUnicode = _UTF8ToUnicode(str);
        if(!pUnicode) {
            return s;
        }
        s.append(pUnicode);
        delete[] pUnicode;
        return s;
    }
    inline std::string  UnicodeToUTF8(const wchar_t *str)
    {
        std::string s("");
        if(!str) {
            return s;
        }
        const char *pUTF8 = _UnicodeToUTF8(str);
        if(!pUTF8) {
            return s;
        }
        s.append(pUTF8);
        delete[] pUTF8;
        return s;
    }
    inline std::string  UTF8ToANSI(const char *str)
    {
        std::string s("");
        if(!str) {
            return s;
        }
        const char *pAnsi = NULL;
        const wchar_t *pUnicode = NULL;
        pUnicode = _UTF8ToUnicode(str);
        if(!pUnicode) {
            goto __End;
        }
        pAnsi = _UnicodeToAnsi(pUnicode);
        if(!pAnsi) {
            goto __End;
        }
        s.append(pAnsi);
__End:
        if(pAnsi) {
            delete[] pAnsi;
        }
        if(pUnicode) {
            delete[] pUnicode;
        }
        return s;
    }
    inline std::string  ANSIToUTF8(const char *str)
    {
        std::string s("");
        if(!str) {
            return s;
        }
        char *pUTF8 = NULL;
        wchar_t *pUnicode = NULL;
        pUnicode = _ANSIToUnicode(str);
        if(!pUnicode) {
            goto __End;
        }
        pUTF8 = _UnicodeToUTF8(pUnicode);
        if(!pUTF8) {
            goto __End;
        }
        s.append(pUTF8);
__End:
        if(pUnicode) {
            delete[] pUnicode;
        }
        if(pUTF8) {
            delete[] pUTF8;
        }
        return s;
    }

    inline void StringReplaceAll(std::string &str, const std::string &strOld, const std::string &strNew)
    {
        using std::string;
        const string::size_type lenOld = strOld.length();
        const string::size_type lenNew = strNew.length();
        if(lenOld == 0) {
            return;
        }
        for(string::size_type pos = 0; (pos = str.find(strOld, pos)) != string::npos; pos += lenNew) {
            str.replace(pos, lenOld, strNew);
        }
    }



    inline bool StringReplaceAll(std::wstring &str, const std::wstring &strold, const std::wstring &strnew)
    {
        try {
            using namespace std;
            std::wstring::size_type pos = 0;
            while((pos = str.find(strold, pos)) != wstring::npos) {
                str.replace(pos, strold.length(), strnew);
                pos += strnew.length();
            }
            return true;
        }
        catch(...) {
            TRACE("发生异常\r\n");
            return false;
        }
    }

    inline void to_upper(std::string &str)
    {
        using namespace std;
        transform(str.begin(), str.end(), str.begin(), [](std::string::value_type & chr)->std::string::value_type {
            return toupper(chr);
        });
    }
    inline void to_upper(std::wstring &str)
    {
        using namespace std;
        transform(str.begin(), str.end(), str.begin(), [](std::wstring::value_type & chr)->std::wstring::value_type {
            return towupper(chr);
        });
    }
    inline void to_lower(std::string &str)
    {
        using namespace std;
        transform(str.begin(), str.end(), str.begin(), [](std::string::value_type & chr)->std::string::value_type {
            return tolower(chr);
        });
    }
    inline void to_lower(std::wstring &str)
    {
        using namespace std;
        transform(str.begin(), str.end(), str.begin(), [](std::wstring::value_type & chr)->std::wstring::value_type {
            return towlower(chr);
        });
    }

    //拆分字符串
    inline void SplitString(std::vector<std::string> &str_array, const std::string &str, const char *seps = ",")
    {
        str_array.clear();
        std::string _str(str);
        char *s = const_cast<char *>(_str.c_str());
        char *token = NULL, *token_next = NULL;
        token = strtok_s(s, seps, &token_next);
        while(token) {
            str_array.push_back(token);
            token = strtok_s(NULL, seps, &token_next);
        }
    }
    inline void SplitString(std::vector<std::wstring> &str_array, const std::wstring &str, const wchar_t *seps = L",")
    {
        str_array.clear();
        std::wstring _str(str);
        wchar_t *s = const_cast<wchar_t *>(_str.c_str());
        wchar_t *token = NULL, *token_next = NULL;
        token = wcstok_s(s, seps, &token_next);
        while(token) {
            str_array.push_back(token);
            token = wcstok_s(NULL, seps, &token_next);
        }
    }

    //拆分，并获得第i个子串
    inline std::string GetSubString(const std::string &str, const char *seps = ",", size_t index = 0)
    {
        std::string s(str);
        char *token = NULL, *token_next = NULL;
        token = strtok_s(const_cast<char *>(s.c_str()), seps, &token_next);
        while(token) {
            if(index == 0) {
                return token;
            }
            --index;
            token = strtok_s(NULL, seps, &token_next);
        }
        return "";
    }
    inline std::wstring GetSubString(const std::wstring &str, const wchar_t *seps = L",", size_t index = 0)
    {
        std::wstring s(str);
        wchar_t *token = NULL, *token_next = NULL;
        token = wcstok_s(const_cast<wchar_t *>(s.c_str()), seps, &token_next);
        while(token) {
            if(index == 0) {
                return token;
            }
            --index;
            token = wcstok_s(NULL, seps, &token_next);
        }
        return L"";
    }

    inline std::wstring ToString(float val, size_t pre)
    {
        using namespace std;
        std::wstringstream ss;
        ss << fixed << setprecision(pre);
        ss.clear();
        ss << val;
        return ss.str();
    }

    template<typename TRet>
    inline TRet StringTo(const wchar_t *szVal)
    {
        using namespace std;
        std::wstringstream ss(szVal);
        TRet d;
        ss >> d;
        return d;
    }
    template<typename T>
    inline std::wstring ToString(const T &val)
    {
        using namespace std;
        std::wstringstream ss;
        ss << val;
        return ss.str();
    }
}

