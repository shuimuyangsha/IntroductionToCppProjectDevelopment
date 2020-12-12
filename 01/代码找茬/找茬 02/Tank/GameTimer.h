#pragma once
#include <windows.h>
class CGameTimer
{
public:

    explicit CGameTimer(unsigned int timeout = 3000, unsigned int timeval = 300)
        : m_timeout(timeout)
        , m_timeval(timeval)
    {
    }

    virtual ~CGameTimer()
    {
    }

    // ����ʱ���
    void SetTimeval(unsigned int timeval)
    {
        m_timeval = timeval;
    }

    // ������һ��ʱ��
    void SetLastTime()
    {
        m_timeLast = GetTickCount();
    }

    // ���� ��ʼʱ�� : Ϊ��ǰʱ��
    void StartTimer()
    {
        m_timeStart =  ::GetTickCount();
    }

    // �����ܵĳ�ʱʱ��
    void SetTimeout(unsigned int timeout)
    {
        m_timeout = timeout;
    }

    // �ж��Ƿ�ʱ(��ʱ��)
    bool IsTimeout() const
    {
        return GetTickCount() - m_timeStart >= m_timeout;
    }

    // �ж��Ƿ񵽴�ʱ���:������ˣ��Ƿ����
    bool IsTimeval(bool bUpdate = true)
    {
        if(GetTickCount() - m_timeLast >= m_timeval) {
            if(bUpdate) {
                m_timeLast = GetTickCount();
            }
            return true;
        }
        else {
            return false;
        }
    }

private:
    // ��ʱ����ʼʱ��
    unsigned int m_timeStart{ ::GetTickCount()};
    // �ܵĳ�ʱʱ��
    unsigned int m_timeout{ 3000};
    // ��һ�θ���ʱ��
    unsigned int m_timeLast{m_timeStart};
    // ʱ����
    unsigned int m_timeval{300};
};

// �ж�ʱ�����
class CGameTimeval
{
public:

    explicit CGameTimeval(unsigned int timeval = 300)
        : m_timeval(timeval)
    {
    }

    virtual ~CGameTimeval()
    {
    }

    void SetTimeval(unsigned int timeval)
    {
        m_timeval = timeval;
    }
    // ���� ��ʼʱ�� : Ϊ��ǰʱ��
    void SetLastTime()
    {
        m_timeLast = GetTickCount();
    }

    // �Ƿ񵽴�ʱ���:������ˣ��Ƿ����
    bool IsTimeval(bool bUpdate = true)
    {
        if(GetTickCount() - m_timeLast >= m_timeval) {
            if(bUpdate) {
                m_timeLast = GetTickCount();
            }
            return true;
        }
        else {
            return false;
        }
    }

private:

    unsigned int m_timeLast{ GetTickCount() };

    unsigned int m_timeval{300};
};


