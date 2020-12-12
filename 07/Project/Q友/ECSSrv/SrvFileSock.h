#if !defined(AFX_SRVFILESOCK_H__27698C24_409C_4214_A204_6D75E638B1D7__INCLUDED_)
#define AFX_SRVFILESOCK_H__27698C24_409C_4214_A204_6D75E638B1D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrvFileSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CSrvFileSock command target

class CSrvFileSock : public CSocket
{
// Attributes
public:

// Operations
public:
	CSrvFileSock();
	virtual ~CSrvFileSock();

// Overrides
public:
	int FindAccountID(CString & csIP);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrvFileSock)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSrvFileSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVFILESOCK_H__27698C24_409C_4214_A204_6D75E638B1D7__INCLUDED_)
