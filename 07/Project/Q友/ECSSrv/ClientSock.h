#if !defined(AFX_CLIENTSOCK_H__FFF523B1_A2C8_4618_AF51_8483FE1CAE72__INCLUDED_)
#define AFX_CLIENTSOCK_H__FFF523B1_A2C8_4618_AF51_8483FE1CAE72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientSock command target

class CClientSock : public CSocket
{
// Attributes
public:

// Operations
public:
    CClientSock();
    virtual ~CClientSock();
    CString m_csAccountID;          // �ͻ����ʺ�,���û���¼���¼���ʺ�
    int m_nTestNum;                 // ������Լ���
    BOOL m_bTesting;                // �Ƿ�Ϊ���������
// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CClientSock)
public:
    virtual void OnReceive(int nErrorCode);
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CClientSock)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCK_H__FFF523B1_A2C8_4618_AF51_8483FE1CAE72__INCLUDED_)
