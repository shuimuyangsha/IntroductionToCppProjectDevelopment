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
    CString  m_csIP;        // 客户ip
    CString m_csAccountID;  // 客户端帐号
    CFile m_File;
    BOOL m_bFirst;  // 是否首次接收数据
    BOOL m_bRvFull; // 单个数据报是否接收完整
    char *m_pchBuffer;  // 数据缓冲区，如果数据报不完整，待数据接收完成，组合完整的数据报
    int m_nRvNum;   // 如果数据报不完整，记录已经接收的数据量
    int m_nPackSize;// 数据报的大小
    CMessage m_RvMsg;// 记录之前接收的数据报中的CMessage结构
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
