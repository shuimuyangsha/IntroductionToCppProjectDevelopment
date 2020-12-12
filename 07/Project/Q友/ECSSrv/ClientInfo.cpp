// ClientInfo.cpp: implementation of the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ECSSrv.h"
#include "ClientInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientInfo::CClientInfo()
{
	m_csClientIP = "";
	m_pClientSock = NULL;
}

CClientInfo::~CClientInfo()
{
	if (m_pClientSock != NULL)
	{
		delete m_pClientSock;
		m_pClientSock = NULL;
	}
m_csClientIP = "";
}
