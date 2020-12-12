#if !defined(AFX_LOGIN_H__C8EBD615_0F08_4A27_8C38_5B6F35817BF3__INCLUDED_)
#define AFX_LOGIN_H__C8EBD615_0F08_4A27_8C38_5B6F35817BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog
#include "ECSClientDlg.h"
class CLogin : public CDialog
{
// Construction
public:
    CLogin(CECSClientDlg *MainDlg = NULL,CWnd* pParent = NULL);   // standard constructor
    CECSClientDlg *m_pMainDlg;
    BOOL m_bConnect;
// Dialog Data
    //{{AFX_DATA(CLogin)
    enum { IDD = IDD_LOGIN_DIALOG };
    CEdit	m_PassWord;
    CEdit	m_UserID;
    CEdit	m_SvrPort;
    CIPAddressCtrl	m_SvrIP;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLogin)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CLogin)
    virtual void OnOK();
    afx_msg void OnBtCancel();
    virtual BOOL OnInitDialog();
    afx_msg void OnBtLogin();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__C8EBD615_0F08_4A27_8C38_5B6F35817BF3__INCLUDED_)
