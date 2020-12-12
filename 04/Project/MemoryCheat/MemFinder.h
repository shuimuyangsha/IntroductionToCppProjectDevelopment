#pragma once
#include <windows.h>
#include <list>
#include <algorithm>


class CMemFinder
{
public:
    CMemFinder()
    {
        // 获得一页内存的大小
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        m_dwPageSize = info.dwPageSize;
    }
    ~CMemFinder()
    {
        // 关闭进程句柄
        SafeCloseHandle();
    }

    // 打开进程
    bool OpenProcess(DWORD dwProcessId)
    {
        // 如果进程是打开的,关闭
        if(IsValidHandle()) {
            SafeCloseHandle();
        }

        // 打开
        m_hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE |
                                   PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD |
                                   PROCESS_QUERY_INFORMATION,
                                   FALSE, dwProcessId);
        if(IsValidHandle()) {
            return true;
        }
        else {
            SafeCloseHandle();
            return false;
        }
    }

    /* 初次扫描
        1. 打开进程
        2. 扫描范围内的内存,寻找可读的,使用中的内存
        3. 与目标值进行比较
    */
    template<typename T>
    bool FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
    {
        m_arList.clear();
        return _FindFirst(dwProcessId, dwBegin, dwEnd, value);
    }

    /*
        在 FindFirst结果的基础上, 继续进行比较
    */
    template<typename T>
    bool FindNext(T value)
    {
        return _FindNext(value);
    }

    // 读内存
    template<typename T>
    bool Read(DWORD dwAddr, T &val)
    {
        if(::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, &val, sizeof(val), NULL)) {
            return true;
        }
        return false;
    }

    bool Read(DWORD dwAddr, unsigned char val[], size_t len)
    {
        if(::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, &val[0], len, NULL)) {
            return true;
        }
        return false;
    }

    // 写内存
    template<typename T>
    bool Write(DWORD dwAddr, T val)
    {
        if(::WriteProcessMemory(m_hProcess, (LPVOID)dwAddr, &val, sizeof(val), nullptr)) {
            return true;
        }
        return false;
    }

    bool Write(DWORD dwAddr, unsigned char code[], size_t len)
    {
        if(::WriteProcessMemory(m_hProcess, (LPVOID)dwAddr, &code[0], len, nullptr)) {
            return true;
        }
        return false;
    }

    // 调用CALL
    bool RemoteCall(unsigned char code[], size_t len, /*函数*/
                    unsigned char para[], size_t paraLen /*参数*/
                   )
    {
        if(!IsValidHandle()) {

        }
        // 申请代码内存
        PVOID mFuncAddr = ::VirtualAllocEx(m_hProcess, NULL, len,  MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if(nullptr == mFuncAddr) {
            return false;
        }
        // 申请函数参数内存
        PVOID ParamAddr = ::VirtualAllocEx(m_hProcess, NULL, len,  MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if(nullptr == ParamAddr) {
            return false;
        }

        // 写入代码，和参数
        this->Write((DWORD)mFuncAddr, code, len);
        this->Write((DWORD)ParamAddr, para, paraLen);

        // 创建远程线程
        DWORD dwThreadId;
        HANDLE hThread = ::CreateRemoteThread(m_hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)mFuncAddr, ParamAddr, 0, &dwThreadId);

        // 等待执行完毕
        if(hThread && hThread != INVALID_HANDLE_VALUE) {
            WaitForSingleObject(hThread, 1000);
        }

        // 释放内存
        VirtualFreeEx(m_hProcess, mFuncAddr, len, MEM_RELEASE);
        VirtualFreeEx(m_hProcess, ParamAddr, paraLen, MEM_RELEASE);
        return true;
    }


    // 设置初次扫描的回调函数
    virtual void SetCallbackFirst(bool(__stdcall *pGoonFirst)(void *pArgs, size_t nAddrCount, size_t index), void *pArgs)
    {
        m_pGoonFirst = pGoonFirst;
        m_pArgsFirst = pArgs;
    }

    // 设置初次以外扫描的回调函数
    virtual void SetCallbackNext(bool(__stdcall *pGoonNext)(void *pArgs, size_t nAddrCount, size_t index), void *pArgs)
    {
        m_pGoonNext = pGoonNext;
        m_pArgsNext = pArgs;
    }

    // 获得结果
    virtual const std::list<DWORD> &GetResults() const
    {
        return m_arList;
    }

private:

    // 真正的查找函数
    template<typename T>
    bool _FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
    {

        HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, dwProcessId);
        m_hProcess = hProcess;
        if(hProcess == NULL) {
            return false;
        }

        // 目标值的长度
        const size_t len = sizeof(value);
        const void *pValue = &value;
        MEMORY_BASIC_INFORMATION mbi;
        DWORD dwBaseAddress = dwBegin;
        do {
            // 如果查询内存属性失败: 越过此页,进行一下页的查询
            if(0 == VirtualQueryEx(hProcess, (LPVOID)dwBaseAddress, &mbi, sizeof(mbi))) {
                dwBaseAddress += m_dwPageSize;
                continue;
            }

            // 下一步的搜索地址
            dwBaseAddress = (DWORD)mbi.BaseAddress + mbi.RegionSize;

            // 回调函数
            if(!m_pGoonFirst || !m_pGoonFirst(m_pArgsFirst, dwEnd - dwBegin, dwBaseAddress - dwBegin)) {
                return false;
            }

            // 判数内存属性
            if(mbi.State != MEM_COMMIT || (mbi.Protect != PAGE_READWRITE &&
                                           mbi.Protect != PAGE_READONLY &&
                                           mbi.Protect != PAGE_EXECUTE_READ &&
                                           mbi.Protect != PAGE_EXECUTE_READWRITE)) {
                //跳过未分配或不可读的区域
                continue;
            }


            // 读亽内容
            DWORD dwReadSize;
            char *Buf = new char[mbi.RegionSize];
            if(ReadProcessMemory(hProcess, (LPVOID)mbi.BaseAddress, Buf, mbi.RegionSize, &dwReadSize) == 0) {
                delete[] Buf;
                CloseHandle(hProcess);
                return false;
            }

            //搜索这块内存区域
            {
                DWORD dwBaseAddr = (DWORD)mbi.BaseAddress;
                for(size_t i = 0; i < mbi.RegionSize - len; ++i) {
                    void *p = &Buf[i];
                    // 等于要查找的值？
                    if(memcmp(p, pValue, len) == 0) {
                        m_arList.push_back(dwBaseAddr + i);
                    }
                }
            }
            // 删除内存区域
            delete[] Buf;
        }
        while(dwBaseAddress < dwEnd);
        return true;
    }

    // 真正的查找函数
    template<typename T>
    bool _FindNext(T value)
    {
        // 目标值的长度
        const size_t len = sizeof(value);
        const void *pValue = &value;

        // 己存地址数量
        size_t cnt = m_arList.size();
        size_t index = 0;
        std::list<DWORD> dwTemp;
        for(auto addr : m_arList) {
            // 如果有回调函数,并且回调函数返回 FALSE,则退出查找
            if(this->m_pGoonNext && !this->m_pGoonNext(this->m_pArgsNext, cnt, index++)) {
                return false;
            }
            // 处理消息
            WaitForIdle();

            // 读入内容
            T tReadValue;
            if(!::ReadProcessMemory(m_hProcess, (LPCVOID)addr, &tReadValue, len, NULL)) {
                continue; // 没有读取成功
            }

            // 和目标值进行比较
            if(0 == memcmp(pValue, &tReadValue, len)) {
                // 值相等, 保留
                dwTemp.push_back(addr);
            }
        }
        // 保存本次结果
        m_arList = dwTemp;

        return !m_arList.empty();
    }

    // 看看有没有消息.有就取出来,并分发
    void WaitForIdle()
    {
        MSG msg;
        while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    // 是否是合法的 进程句柄
    bool IsValidHandle()
    {
        return (m_hProcess && INVALID_HANDLE_VALUE != m_hProcess);
    }

    // 关闭进程句柄
    void SafeCloseHandle()
    {
        CloseHandle(m_hProcess);
        m_hProcess = INVALID_HANDLE_VALUE;
    }

    // 目标进程句柄
    HANDLE m_hProcess{ INVALID_HANDLE_VALUE };
    // 搜索的结果
    std::list<DWORD> m_arList;
    // 回调函数
    typedef bool(__stdcall *PFUN_CALLBACK)(void *pArgs, size_t nPageCount, size_t index);
    // 首次扫描 回调函数
    PFUN_CALLBACK m_pGoonFirst{ nullptr };
    // 下次扫描 回调函数
    PFUN_CALLBACK m_pGoonNext{ nullptr };
    // 回调函数的参数 : 为用户设置回调函数时,传过来的指针,当本类内调用回调函数时,原封不动传回去
    void *m_pArgsFirst{ nullptr };
    void *m_pArgsNext{ nullptr };

    DWORD m_dwPageSize;
};
