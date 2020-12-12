#if !defined(AFX_CLIENTFILESOCK_H__F9FBC086_3335_456E_8B84_A0F6641C504F__INCLUDED_)
#define AFX_CLIENTFILESOCK_H__F9FBC086_3335_456E_8B84_A0F6641C504F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientFileSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientFileSock command target

class CClientFileSock : public CSocket
{
// Attributes
public:

// Operations
public:
    CClientFileSock();
    virtual ~CClientFileSock();
    CString  m_csIP;        // �ͻ�ip
    CString m_csAccountID;  // �ͻ����ʺ�
    CFile m_File;
    BOOL m_bFirst;  // �Ƿ��״ν�������
    BOOL m_bRvFull; // �������ݱ��Ƿ��������
    char *m_pchBuffer;  // ���ݻ�������������ݱ��������������ݽ�����ɣ�������������ݱ�
    int m_nRvNum;   // ������ݱ�����������¼�Ѿ����յ�������
    int m_nPackSize;// ���ݱ��Ĵ�С
    CMessage m_RvMsg;// ��¼֮ǰ���յ����ݱ��е�CMessage�ṹ
// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CClientFileSock)
public:
    virtual void OnReceive(int nErrorCode);
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CClientFileSock)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTFILESOCK_H__F9FBC086_3335_456E_8B84_A0F6641C504F__INCLUDED_)
