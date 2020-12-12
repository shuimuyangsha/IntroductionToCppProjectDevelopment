#pragma once

#include "DMenu.h"
#include "Shooter.h"

class IMark;

class CGame
{
public:
    /* 构造函数:
     * hWnd : 游戏窗口窗口
     * x,y: 位置
     * w,h: 窗口宽高
     */
    CGame(HWND hWnd, float x, float y, float w, float h);
    ~CGame();

public:
    // 游戏的一帧
    bool EnterFrame(DWORD dwTime);

    /// ////////////////////////////////////////////////////////////////////////
    /// 处理鼠标消息
    // 鼠标左键按下
    void OnLButtonDown(UINT nFlags, CPoint point);
    // 鼠标左键抬起
    void OnLButtonUp(UINT nFlags, CPoint point);
    // 鼠标左键双击
    void OnLButtonDblClk(UINT nFlags, CPoint point);
    // 鼠标右键按下
    void OnRButtonDown(UINT nFlags, CPoint point);
    // 鼠标右键抬起
    void OnRButtonUp(UINT nFlags, CPoint point);
    // 鼠标右键双击
    void OnRButtonDblClk(UINT nFlags, CPoint point);
    // 鼠标移动
    void OnMouseMove(UINT nFlags, CPoint point);

    // 按下 ESC 键时的处理: 返回FALSE 不处理，则由父窗口处理
    BOOL OnESC();

	// 设置 进入游戏阶段，并设置工具
	void SetStatusNormal (std::shared_ptr<CShooter> pTool, BOOL bCursor = FALSE)
	{
		// 设置使用的工具
		m_pTool = pTool;
		// 设置游戏状态
		m_eStatus = EGameStatusNormal;
		// 隐藏鼠标
		if (!bCursor) {
			while (true) {
				int i = ShowCursor (FALSE);
				TRACE ("隐藏光标 %d \r\n", i);
				if (i < 0) {
					break;
				}
			}
		}
		else {
			int i = ShowCursor (bCursor);
			TRACE ("显示光标 %d \n", i);
		}
	}

    // 增加一个 破坏物
    void Append(std::shared_ptr<IMark> pMark)
    {
        m_vMarks.push_back(pMark);
    }

    // 获得游戏窗口的宽度
    float GetWidth() const
    {
        return m_width;
    }

    // 获得游戏窗口的高度
    float GetHeigth() const
    {
        return m_height;
    }

    // 获得范围
    RectF GetRectF() const
    {
        return RectF(m_x, m_y, m_width, m_height);
    }
private:
    HWND m_hWnd;
    // 窗口的起始位置
    float m_x;
    float m_y;

    // 游戏窗口的宽高
    float m_width;
    float m_height;

    // 游戏只分为两个阶段: 菜单选择阶段，正式游戏阶段
    typedef enum EGameStatus {EGameStatusSelect, EGameStatusNormal} EGameStatus;
    EGameStatus  m_eStatus{EGameStatusSelect};

private:
    // 客户区的大小
    CRect m_rcClient;

    // 记录 游戏每秒多少帧
    int m_fps{ 0 };
    // 输出FPS文字用的画刷
    SolidBrush m_brush{Color(254, 0xFF, 0x00, 0x00)};
    // 输出FPS文字字体
    Gdiplus::Font m_font{L"宋体", 10.0};
    // 在左上角显示
    PointF origin{0.0f, 0.0f};

    // 画出所有对象
    void Draw();

    // 输出fps
    void DrawFps(Gdiplus::Graphics &gh);

    // 破坏窗口留下的东西
    std::vector<std::shared_ptr<IMark>> m_vMarks;

    // 背景图
    Gdiplus::Bitmap *m_imgBk;

    // 菜单
    std::shared_ptr<CDMenu> m_menu;

    // 当前选择的工具
    std::shared_ptr<CShooter> m_pTool;
};

