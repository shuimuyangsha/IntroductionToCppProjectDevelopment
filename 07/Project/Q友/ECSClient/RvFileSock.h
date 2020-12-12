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

// ��������ļ����׽���
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
	BOOL m_bRvFull;	// �������ݱ��Ƿ��������
	char* m_pchBuffer;	// ���ݻ�������������ݱ��������������ݽ�����ɣ�������������ݱ�
	int m_nRvNum;	// ������ݱ�����������¼�Ѿ����յ�������
	int m_nPackSize;// ���ݱ��Ĵ�С
	CMessage m_RvMsg;// ��¼֮ǰ���յ����ݱ��е�CMessage�ṹ
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
