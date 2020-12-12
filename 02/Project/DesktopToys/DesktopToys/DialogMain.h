
// DialogMain.h : 头文件
//

#pragma once

#include "Game.h"

#include "DesktopToysDlg.h"

// CDialogMain 对话框
class CDialogMain : public CDialogEx
{
    // 构造
public:
    CDialogMain(CWnd *pParent = NULL);  // 标准构造函数

                                            // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_MAIN};
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持


                                                        // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual BOOL PreTranslateMessage(MSG *pMsg);

    DECLARE_MESSAGE_MAP()

public:
#if 0
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
#endif

private:
    //std::shared_ptr<CGame> m_pGame;
    std::shared_ptr<CDesktopToysDlg> m_dlg;
};
