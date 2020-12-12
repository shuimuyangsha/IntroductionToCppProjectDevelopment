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

#define fTitle          1  // ����
#define fMinButton      2  // ��С����ť
#define fCloseButton    4  // �رհ�ť
#define fAll            7  // ���б�ʶ

// ��ť״̬
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

    // �ĸ�TabCtrl���ӶԻ���
    CContralDlg     *m_pContralDlg; // ����������ģ��
    CDiskDlg        *m_pDiskDlg;    // ���̿ռ�����ģ��
    CLitterDlg      *m_pLitterDlg;  // �����ļ�����ģ��
    CTaskDlg        *m_pTaskDlg;    // ϵͳ�������ģ��

    CString m_Caption;              // �������
    CButtonState m_ButtonState;     // ��ť״̬
    int m_CaptionHeight;            // �������ĸ߶�
    int m_TitleDrawHeight;          // ������ʵ�ʵĻ��Ƹ߶�
    int m_ButtonWidth;              // ��ťλͼ���
    int m_ButtonHeight;             // ��ť�߶�
    int m_BorderWidth;              // �߿���
    int m_BorderHeight;             // �߿�߶�
    COLORREF m_CapitonColor;        // ����������ɫ
    CFont m_CaptionFont;            // ��������
    BOOL  m_IsDrawForm;             // �Ƿ��ػ水ť
    CRect m_TitleRc;                // ����������
    CRect m_MinRect;                // ��С����ť����
    CRect m_CloseRect;              // �رհ�ť����

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
