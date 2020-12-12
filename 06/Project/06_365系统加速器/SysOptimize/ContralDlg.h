#if !defined(AFX_CONTRALDLG_H__8863A379_B3E1_49B5_8699_9290C71EC4B3__INCLUDED_)
#define AFX_CONTRALDLG_H__8863A379_B3E1_49B5_8699_9290C71EC4B3__INCLUDED_
#include "IconBtn.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContralDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContralDlg dialog

class CContralDlg : public CDialog
{
// Construction
public:
    CContralDlg(CWnd *pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CContralDlg)
    enum { IDD = IDD_CONTRAL_DIALOG };
    CIconBtn    m_Computer;
    CIconBtn    m_Timedate;
    CIconBtn    m_Sysdm;
    CIconBtn    m_Mouse;
    CIconBtn    m_Mmsys;
    CIconBtn    m_Keyboard;
    CIconBtn    m_Intl;
    CIconBtn    m_Hdwwiz;
    CIconBtn    m_Desk;
    CIconBtn    m_Appwiz;
    CIconBtn    m_Access;
    CIconBtn    m_Internet;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContralDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CContralDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnButinternet();
    afx_msg void OnButmmsys();
    afx_msg void OnButtimedate();
    afx_msg void OnButdesk();
    afx_msg void OnButaccess();
    afx_msg void OnButmouse();
    afx_msg void OnButkeyboard();
    afx_msg void OnButintl();
    afx_msg void OnButappwiz();
    afx_msg void OnButhdwwiz();
    afx_msg void OnButsysdm();
    afx_msg void OnButmodem();
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTRALDLG_H__8863A379_B3E1_49B5_8699_9290C71EC4B3__INCLUDED_)
