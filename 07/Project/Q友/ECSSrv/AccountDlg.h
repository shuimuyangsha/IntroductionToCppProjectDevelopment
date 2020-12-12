#if !defined(AFX_ACCOUNTDLG_H__52494BA0_F8D9_4F7E_A888_3F0573A67910__INCLUDED_)
#define AFX_ACCOUNTDLG_H__52494BA0_F8D9_4F7E_A888_3F0573A67910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog
#include "DataManage.h"

#import "C:\\WINDOWS\\system32\\msxml6.dll"
using namespace MSXML2;

class CAccountDlg : public CDialog
{
    // Construction
public:
    HTREEITEM GetDepart(int DepartID, HTREEITEM hCurrent);
    void LoadAccountInfo();
    void ClearAcInfo();
    void FindNode(HTREEITEM hParent, int ParentID, MSXML2::IXMLDOMNodePtr pParentNode);
    void LoadDepartInfo();
    CDataManage m_DBMng;
    BOOL m_InitSucess;
    CImageList m_ImageList;
    CAccountDlg(CWnd *pParent = NULL);    // standard constructor
    MSXML2::IXMLDOMDocumentPtr pXMLDoc;
    // Dialog Data
    //{{AFX_DATA(CAccountDlg)
    enum { IDD = IDD_ACCOUNTDLG_DIALOG };
    CEdit   m_UserID;
    CEdit   m_User;
    CComboBox   m_Sex;
    CEdit   m_HanderShip;
    CTreeCtrl   m_DPList;
    CEdit   m_DepartID;
    CEdit   m_Depart;
    CEdit   m_Password;
    CEdit   m_AccountName;
    CEdit   m_AccountID;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAccountDlg)
public:
    virtual BOOL PreTranslateMessage(MSG *pMsg);
protected:
    virtual void DoDataExchange(CDataExchange *pDX);     // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
public:

    // Generated message map functions
    //{{AFX_MSG(CAccountDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangedDplist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBtadd();
    afx_msg void OnBtDelete();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTDLG_H__52494BA0_F8D9_4F7E_A888_3F0573A67910__INCLUDED_)
