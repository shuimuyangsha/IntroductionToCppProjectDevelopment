// ClientInfo.h: interface for the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_)
#define AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientSock.h"

// 在服务器接收客户端连接时记录客户端信息
class CClientInfo  
{
public:
	CString m_csClientIP;		// 客户端IP
	CSocket *m_pClientSock;		// 客户端套接字 
	CClientInfo();
	virtual ~CClientInfo();
};

#endif // !defined(AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_)
