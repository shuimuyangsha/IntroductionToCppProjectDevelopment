#if !defined(AFX_RVFILESOCK_H__DF53B00B_7271_4B97_AD3E_5FBE8D2E33AC__INCLUDED_)
#define AFX_RVFILESOCK_H__DF53B00B_7271_4B97_AD3E_5FBE8D2E33AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RvFileSock.h : header file
//

#include "Message.h"

/////////////////////////////////////////////////////////////////////////////
// CRvFileSock command target

// 定义接收文件的套接字
class CSendDlg;
class CRvFileSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CRvFileSock();
	virtual ~CRvFileSock();
	int m_nIndex;
	CSendDlg* m_pDlg;
	CFile m_File;
	BOOL m_bFirst;
	BOOL m_bRvFull;	// 单个数据报是否接收完整
	char* m_pchBuffer;	// 数据缓冲区，如果数据报不完整，待数据接收完成，组合完整的数据报
	int m_nRvNum;	// 如果数据报不完整，记录已经接收的数据量
	int m_nPackSize;// 数据报的大小
	CMessage m_RvMsg;// 记录之前接收的数据报中的CMessage结构
	char m_chName[MAX_PATH];
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRvFileSock)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRvFileSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RVFILESOCK_H__DF53B00B_7271_4B97_AD3E_5FBE8D2E33AC__INCLUDED_)
