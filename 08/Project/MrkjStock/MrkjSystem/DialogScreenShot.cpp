// DialogScreenShot.cpp : 实现文件
//

#include "stdafx.h"
#include "MrkjSystem.h"
#include "DialogScreenShot.h"
#include "afxdialogex.h"
#include "ScreenTools.h"

// CDialogScreenShot 对话框

IMPLEMENT_DYNAMIC(CDialogScreenShot, CDialogEx)

CDialogScreenShot::CDialogScreenShot(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_SCREEN_SHOT, pParent)
{
}

CDialogScreenShot::~CDialogScreenShot()
{
}

void CDialogScreenShot::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogScreenShot, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CDialogScreenShot 消息处理程序

static CPoint ptStart{ -1, -1}; //第一个点
static CPoint ptLast{ -1, -1}; //第二个点(本次鼠标移动之前的)

BOOL CDialogScreenShot::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 获得屏幕分辨率
    int w = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    // 设置本窗口大小, 遮挡整个屏幕
    MoveWindow(0, 0, w, y);
    // 背景画刷
    m_bkBrush.CreateSolidBrush(RGB(0, 100, 100));
    // 设置为透明窗口
    ModifyStyleEx(0, WS_EX_LAYERED);
    SetLayeredWindowAttributes(0, 100, LWA_ALPHA/*窗口透明*/);
    // 设置鼠标
    m_hCursorOld = SetCursor(hCursor);
    // 初始值
    ptLast.x = ptLast.y = -1;
    ptStart.x = ptStart.y = -1;
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDialogScreenShot::OnLButtonDown(UINT nFlags, CPoint point)
{
    //记录第一个点
    ptStart = point;
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDialogScreenShot::AdjustRect(CRect& rc)
{
    // 系统 屏幕分辨率
    int m_iXScreen{ GetSystemMetrics(SM_CXSCREEN)};
    int m_iYScreen{ GetSystemMetrics(SM_CYSCREEN)};
#define ADJ(a, _min, _max) do { if (a<_min) {a = _min; } if (a>_max) {a=_max;}} while(0)
    ADJ(rc.left, 0, m_iXScreen);
    ADJ(rc.top, 0, m_iYScreen);
    ADJ(rc.right, 0, m_iXScreen);
    ADJ(rc.bottom, 0, m_iYScreen);
}

void CDialogScreenShot::OnLButtonUp(UINT nFlags, CPoint point)
{
    TRACE("%d,%d\r\n", point.x, point.y);
    //获取坐标区域,截图
    CRect rc = { ptStart.x, ptStart.y, point.x, point.y };
    // 判断 选取区域是否正确
    rc.NormalizeRect();
    if(!(rc.IsRectEmpty() && rc.IsRectNull())) {
        m_RectSelect = rc;
        AdjustRect(m_RectSelect);
    }
    //恢复鼠标
    SetCursor(m_hCursorOld);
    // 恢复默认值
    ptLast.x = ptLast.y = -1;
    ptStart.x = ptStart.y = -1;
    CDialogEx::OnLButtonUp(nFlags, point);
    // 根据选取值,确定返回结果(IDOK or IDCANCEL)
    m_RectSelect.NormalizeRect();
    if(m_RectSelect.IsRectEmpty() || m_RectSelect.IsRectNull()) {
        OnCancel();
    }
    else {
        OnOK();
    }
}

void CDialogScreenShot::OnMouseMove(UINT nFlags, CPoint point)
{
    // 每次移动都要重新设置光标
    SetCursor(hCursor);
    // 如果 左键是是按下状态
    if(nFlags == MK_LBUTTON) {
        CClientDC aDc(this);
        // 返色模式
        aDc.SetROP2(R2_NOTXORPEN);
        // 重绘旧图形
        if(ptLast.x > 0 && ptLast.y > 0) {
            RECT rc = {ptStart.x, ptStart.y, ptLast.x, ptLast.y};
            // 擦除上次的矩型
            aDc.Rectangle(&rc);
            // 以背景色重绘
            aDc.FillRect(&rc, &m_bkBrush);
        }
        //绘制本次的
        {
            RECT rc = {ptStart.x, ptStart.y, point.x, point.y};
            aDc.Rectangle(&rc);
            aDc.FillSolidRect(&rc, RGB(255, 0, 0));
        }
        //保存本次的
        ptLast = point;
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

HBRUSH CDialogScreenShot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if(nCtlColor == CTLCOLOR_DLG) {
        return (HBRUSH)m_bkBrush; //返回新画笔,重绘背景色
    }
    return hbr;
}

BOOL CDialogScreenShot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    return 0; //解决光标不停的闪的问题
    //return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

// 返回选取的区域
CRect CDialogScreenShot::GetSelectRect() const
{
    return m_RectSelect;
}


void CDialogScreenShot::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CDialogScreenShot::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
