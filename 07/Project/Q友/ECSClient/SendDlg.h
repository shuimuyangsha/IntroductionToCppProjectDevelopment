#if !defined(AFX_SENDDLG_H__D43EB33A_BC3D_4BF9_BCA7_E13FB5011653__INCLUDED_)
#define AFX_SENDDLG_H__D43EB33A_BC3D_4BF9_BCA7_E13FB5011653__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendDlg.h : header file
//
#include "RvFileSock.h"
#include "Message.h"
#include <richole.h>
/////////////////////////////////////////////////////////////////////////////
// CSendDlg dialog
const DWORD dwSendLen = 1024*6;	// 每次发送6k的数据

#import "Gif.dll" named_guids

class CSendDlg : public CDialog
{
// Construction
public:
	void OnSendEditClear();
	CString ExtractFileName(CString csFullName);
	void OnSaveImage();
	void OnSendEditRBtnUp(UINT nFlags, CPoint point);
	CString ExtractFilePath(CString csFullPath);
	void OnSendImage();
	void OnCancel();
	BOOL InsertImage(IRichEditOle *lpRichEditOle,CString &csFileName);
	void SendFile(CSocket & sock,int nFileType = ST_FILE);
	CSendDlg(CWnd* pParent = NULL);   // standard constructor
	int  m_UserID;						// 发送对话框关联的目标ID
	CRvFileSock m_csSock;
	DWORD dwSendCount;
	int  m_nIndex;
	BOOL m_bSendFile;
	CString m_csFileName;
	CString m_csFullName;
	char chFileName[MAX_PATH];
	CRichEditCtrl * m_pShareEdit;
// Dialog Data
	//{{AFX_DATA(CSendDlg)
	enum { IDD = IDD_SENDDLG_DIALOG };
	CButton	m_SendFile;
	CProgressCtrl	m_Progress;
	CRichEditCtrl	m_ShowEdit;
	CRichEditCtrl	m_SendEdit;
	CEdit	m_LocalName;
	CEdit	m_LocalID;
	CComboBox	m_DesName;
	CComboBox	m_DesID;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendDlg)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnQuit();
	afx_msg void OnBtSend();
	afx_msg void OnSendFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnSendImg();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDDLG_H__D43EB33A_BC3D_4BF9_BCA7_E13FB5011653__INCLUDED_)
