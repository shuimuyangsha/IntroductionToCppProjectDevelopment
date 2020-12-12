// ECSSrvDlg.h : header file
//

#if !defined(AFX_ECSSRVDLG_H__668B2BA9_5575_42DD_B51C_2C652662E343__INCLUDED_)
#define AFX_ECSSRVDLG_H__668B2BA9_5575_42DD_B51C_2C652662E343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CECSSrvDlg dialog
// #include "winsock2.h"
#pragma comment (lib,"ws2_32.lib")
#define MAXNUM 200
#define PACKAGESIZE  7628

#include "DataManage.h"

#include "ServerSock.h"
#include "SrvFileSock.h"
class CECSSrvDlg : public CDialog
{
// Construction
public:
	BOOL VerifyUser(int nID,char* pchPassword);
	CECSSrvDlg(CWnd* pParent = NULL);	// standard constructor

	CDataManage m_DataMgn;
	CServerSock m_ServerSock;
	CSrvFileSock m_FileSock;
// Dialog Data
	//{{AFX_DATA(CECSSrvDlg)
	enum { IDD = IDD_ECSSRV_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECSSrvDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CECSSrvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDepartment();
	afx_msg void OnBtaccount();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSSRVDLG_H__668B2BA9_5575_42DD_B51C_2C652662E343__INCLUDED_)
