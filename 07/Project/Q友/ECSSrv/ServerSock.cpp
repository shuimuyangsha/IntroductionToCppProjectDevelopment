// ServerSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "ServerSock.h"
#include "ClientSock.h "
#include "ClientInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSock

CServerSock::CServerSock()
{
}

CServerSock::~CServerSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSock, CSocket)
	//{{AFX_MSG_MAP(CServerSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSock member functions

void CServerSock::OnAccept(int nErrorCode) 
{
	CClientSock* pClientSock = new CClientSock();
	ASSERT(pClientSock != NULL);
	SOCKADDR_IN clAddr;
	int nLen = sizeof(SOCKADDR_IN);
	// ���տͻ�������
	Accept(*pClientSock,(SOCKADDR*)&clAddr,&nLen);


	char chDisable= 1;
	setsockopt(pClientSock->m_hSocket,IPPROTO_TCP,TCP_NODELAY,&chDisable,sizeof(chDisable));
	DWORD dwEnable = 1;
	pClientSock->IOCtl(FIONBIO,&dwEnable);

	BOOL bEnable = TRUE;
	pClientSock->SetSockOpt(SO_OOBINLINE,&bEnable ,sizeof(BOOL));

	CPtrList *pTmp = &((CECSSrvApp*)AfxGetApp())->m_ClientList;
	// ����ͻ���Ϣ����
	CClientInfo *pClientInfo = new CClientInfo();
	// ���ÿͻ���Ϣ
	pClientInfo->m_pClientSock = pClientSock;
	char *pchIP = inet_ntoa(*(in_addr*)&clAddr.sin_addr); 
	pClientInfo->m_csClientIP = pchIP;

	POSITION pos = pTmp->GetHeadPosition();	
	int nCount = pTmp->GetCount();
	for (int i=0; i< nCount; i++)
	{
		POSITION psTmp = pos;
		CClientInfo *pClient = static_cast < CClientInfo *>(pTmp->GetNext(pos));
		if (pClientInfo != NULL)
		{
			if (strcmp(pchIP,pClient->m_csClientIP)==0)
			{
				pClient->m_pClientSock->ShutDown();
				pClient->m_pClientSock->Close();
				delete pClient;
				pTmp->RemoveAt(psTmp);
				pTmp->InsertBefore(pos,pClientInfo);
				return;
			}
		}
	}
	// ��¼�ͻ���Ϣ
	pTmp->AddTail(pClientInfo);
	CSocket::OnAccept(nErrorCode);
}
