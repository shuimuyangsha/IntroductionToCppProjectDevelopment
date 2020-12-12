// SysOptimizeDlg.h : header file
//

#if !defined(AFX_SYSOPTIMIZEDLG_H__368DD37B_D0F5_4840_935F_3A3889E04035__INCLUDED_)
#define AFX_SYSOPTIMIZEDLG_H__368DD37B_D0F5_4840_935F_3A3889E04035__INCLUDED_
#include "ContralDlg.h"
#include "DiskDlg.h"
#include "LitterDlg.h"
#include "TaskDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define fTitle          1  // 标题
#define fMinButton      2  // 最小化按钮
#define fCloseButton    4  // 关闭按钮
#define fAll            7  // 所有标识

// 按钮状态
enum CButtonState {bsNone, bsMin, bsClose};
/////////////////////////////////////////////////////////////////////////////
// CSysOptimizeDlg dialog

class CSysOptimizeDlg : public CDialog
{
// Construction
public:
    void DrawCaption();
    void DrawDialog(UINT Flags);
    void DestroyWindowBox(int num);
    void CreateDialogBox(int num);
    CSysOptimizeDlg(CWnd *pParent = NULL);  // standard constructor
    int m_Num;

    // 四个TabCtrl的子对话框
    CContralDlg     *m_pContralDlg; // 控制面板操作模块
    CDiskDlg        *m_pDiskDlg;    // 磁盘空间整理模块
    CLitterDlg      *m_pLitterDlg;  // 垃圾文件清理模块
    CTaskDlg        *m_pTaskDlg;    // 系统任务管理模块

    CString m_Caption;              // 窗体标题
    CButtonState m_ButtonState;     // 按钮状态
    int m_CaptionHeight;            // 标题栏的高度
    int m_TitleDrawHeight;          // 标题栏实际的绘制高度
    int m_ButtonWidth;              // 按钮位图宽度
    int m_ButtonHeight;             // 按钮高度
    int m_BorderWidth;              // 边框宽度
    int m_BorderHeight;             // 边框高度
    COLORREF m_CapitonColor;        // 标题字体颜色
    CFont m_CaptionFont;            // 标题字体
    BOOL  m_IsDrawForm;             // 是否重绘按钮
    CRect m_TitleRc;                // 标题栏区域
    CRect m_MinRect;                // 最小化按钮区域
    CRect m_CloseRect;              // 关闭按钮区域

    CBitmap m_BKGround;
// Dialog Data
    //{{AFX_DATA(CSysOptimizeDlg)
    enum { IDD = IDD_SYSOPTIMIZE_DIALOG };
    CStatic m_Select;
    CStatic m_Frame;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSysOptimizeDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CSysOptimizeDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    afx_msg void OnStacontral();
    afx_msg void OnStadisk();
    afx_msg void OnStalitter();
    afx_msg void OnStatask();
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNcPaint();
    afx_msg void OnMove(int x, int y);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSOPTIMIZEDLG_H__368DD37B_D0F5_4840_935F_3A3889E04035__INCLUDED_)
