// ClientInfo.h: interface for the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_)
#define AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientSock.h"

// �ڷ��������տͻ�������ʱ��¼�ͻ�����Ϣ
class CClientInfo  
{
public:
	CString m_csClientIP;		// �ͻ���IP
	CSocket *m_pClientSock;		// �ͻ����׽��� 
	CClientInfo();
	virtual ~CClientInfo();
};

#endif // !defined(AFX_CLIENTINFO_H__EC1699CC_1A3A_451A_A7DD_704FC8261734__INCLUDED_)
