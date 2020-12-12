// ECSClientDlg.h : header file
//

#if !defined(AFX_ECSCLIENTDLG_H__C1DDC315_5DF7_4A6A_BC5F_D7441D2359E7__INCLUDED_)
#define AFX_ECSCLIENTDLG_H__C1DDC315_5DF7_4A6A_BC5F_D7441D2359E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClientSock.h"
#include "RvFileSock.h"
#include "SendDlg.h"
#include "BmpTree.h"
#include "SkinScroll.h"
/////////////////////////////////////////////////////////////////////////////
// CECSClientDlg dialog
#pragma comment (lib,"ws2_32.lib")

#import "C:\\WINDOWS\\system32\\msxml6.dll"
using namespace MSXML2;

#define PACKAGESIZE 7628

class CECSClientDlg : public CDialog
{
// Construction
public:
    void SetAllUserState(HTREEITEM hParent, int nState);
    void SetUserState(int nUserID, int nState);

    void OnCancel();
    BOOL FindNode(HTREEITEM hParent, int nUserID, CString &csName, HTREEITEM &hRetItem);
    BOOL FindUserName(int nUserID, CString &csName);
    BOOL SearchSendDlg(int nID, POSITION &ptPos);
    void FindSubNode(MSXML2::IXMLDOMElementPtr pParentNode , HTREEITEM hParentItem);
    void LoadOrgFromFile(BSTR csFileName);
    CECSClientDlg(CWnd *pParent = NULL);    // standard constructor
    CClientSock m_ClientSock;

    MSXML2::IXMLDOMDocumentPtr m_pXMLDoc;

    CImageList m_ImageList;
    int m_nLoginUser;       // 当前登录的用户ID
    CString m_Password;     // 记录用户登录密码
    int m_nPort;            // 记录登录的端口号
    CString m_ServerIP;     // 记录服务器IP
    CRvFileSock m_RvSock;   // 用于接收文件
    CPtrList m_pSdDlgList;  // 记录与其他用户交流时的对话框
    BOOL m_bLogined;        // 登录成功或失败
    int m_nTestNum;         // 网络测试计数
// Dialog Data
    //{{AFX_DATA(CECSClientDlg)
    enum { IDD = IDD_ECSCLIENT_DIALOG };
    CBmpTree    m_ACList;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CECSClientDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CECSClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    afx_msg void OnSelchangedAclist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkAclist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnClose();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSCLIENTDLG_H__C1DDC315_5DF7_4A6A_BC5F_D7441D2359E7__INCLUDED_)
