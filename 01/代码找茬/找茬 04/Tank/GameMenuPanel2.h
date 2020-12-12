#pragma once
#include "GameEntry.h"


class CGame;
class CGameMenuPanel2 : CGameEntryMoveable
{
public:
    CGameMenuPanel2();
    ~CGameMenuPanel2();

    CGame *m_pParent{ nullptr };

    // 画
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // 处理 鼠标移动事件
    void OnMouseMove(UINT nFlags, CPoint point);

    // 处理 鼠标左键抬起事件
    void OnLButtonUp(UINT nFlags, CPoint point);

    // 菜单 图片
    Image *m_img{nullptr};

    // 绘图状态
    bool m_bNormal{true};
};
