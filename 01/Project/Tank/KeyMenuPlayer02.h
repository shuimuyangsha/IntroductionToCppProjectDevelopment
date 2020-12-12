#pragma once
#include "GameEntry.h"
#include "GameTimer.h"

class CGame;
class CKeyMenuPlayer02 : public CGameEntryMoveable
{
public:
    CKeyMenuPlayer02();
    ~CKeyMenuPlayer02();

    // »­
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // Í£Ö¹¶¯»­
    void SetStop(bool bStop = true)
    {
        m_bStop = bStop;
    };
    bool GetStop() const
    {
        return m_bStop;
    }

private:
    mutable CGameTimeval m_time{300};
    // ²Ëµ¥ Í¼Æ¬
    Image *m_img[2];

    // ÊÇ·ñÍ£Ö¹¶¯»­×´Ì¬
    bool m_bStop{false};
};

