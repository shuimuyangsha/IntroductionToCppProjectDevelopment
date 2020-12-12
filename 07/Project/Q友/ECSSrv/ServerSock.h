#if !defined(AFX_SERVERSOCK_H__CB3938C0_9650_4C1E_9AB7_C0E6DF7D7F50__INCLUDED_)
#define AFX_SERVERSOCK_H__CB3938C0_9650_4C1E_9AB7_C0E6DF7D7F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerSock command target

class CServerSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CServerSock();
	virtual ~CServerSock();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSock)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCK_H__CB3938C0_9650_4C1E_9AB7_C0E6DF7D7F50__INCLUDED_)
