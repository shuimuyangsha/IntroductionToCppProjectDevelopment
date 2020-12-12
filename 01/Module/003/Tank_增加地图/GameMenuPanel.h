#pragma once

#include <tuple>
#include <vector>
#include "GameEntry.h"

using namespace std;

class CGame;

class CGameMenuPanel : public CGameEntryMoveable
{
public:
    CGameMenuPanel();
    ~CGameMenuPanel();

    void SetParent(CGame *g)
    {
        m_pParent = g;
    };

    CGame *m_pParent{ nullptr };

    // 画
    virtual void Draw(Graphics &gh) const;


    // 自己的范围
    RectF m_rect{0, 0, 800, 600};

    // 处理 鼠标移动事件
    void OnMouseMove(UINT nFlags, CPoint point);

    // 处理 鼠标左键抬起事件
    void OnLButtonUp(UINT nFlags, CPoint point);

    // 背景图
    Image *m_imgBackground;

	// 菜单项
	struct {
		vector<tuple<Image *, RectF>> vImgInfoPtr;
		int index;
	} m_menuItems[2];
};
