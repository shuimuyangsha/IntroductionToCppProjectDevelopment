
// DialogMain.h : ͷ�ļ�
//

#pragma once

#include "Game.h"

#include "DesktopToysDlg.h"

// CDialogMain �Ի���
class CDialogMain : public CDialogEx
{
    // ����
public:
    CDialogMain(CWnd *pParent = NULL);  // ��׼���캯��

                                            // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_MAIN};
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��


                                                        // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
