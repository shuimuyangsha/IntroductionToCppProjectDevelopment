// SrvFileSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSSrv.h"
#include "SrvFileSock.h"
#include "ClientFileSock.h"
#include "ClientInfo.h"
#include "ECSSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrvFileSock

CSrvFileSock::CSrvFileSock()
{
}

CSrvFileSock::~CSrvFileSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSrvFileSock, CSocket)
	//{{AFX_MSG_MAP(CSrvFileSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSrvFileSock member functions

void CSrvFileSock::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CClientFileSock* pClientSock = new CClientFileSock();
	ASSERT(pClientSock != NULL);
	SOCKADDR_IN clAddr;
	int nLen = sizeof(SOCKADDR_IN);
	// 接收客户端连接
		// 10240+sizeof(CMessage);
	int csSize;
	int nSize = PACKAGESIZE;

	Accept(*pClientSock,(SOCKADDR*)&clAddr,&nLen);
	pClientSock->SetSockOpt(SO_RCVBUF,&nSize,sizeof(nSize));
	pClientSock->GetSockOpt(SO_RCVBUF,&nSize,&csSize);

	DWORD dwEnable = 1;
	pClientSock->IOCtl(FIONBIO,&dwEnable);

	char chDisable= 1;
	
	pClientSock->SetSockOpt(SO_RCVBUF,&nSize,sizeof(nSize));
	pClientSock->GetSockOpt(SO_RCVBUF,&nSize,&csSize);

	CPtrList *pTmp = &((CECSSrvApp*)AfxGetApp())->m_FileList;
	// 构造客户信息对象
	CClientInfo *pClientInfo = new CClientInfo();
	// 设置客户信息
	pClientInfo->m_pClientSock = pClientSock;
	char *pchIP = inet_ntoa(*(in_addr*)&clAddr.sin_addr); 
	pClientInfo->m_csClientIP = pchIP;
	pClientSock->m_csIP = pchIP;

	CString csID;
	csID.Format("%d",FindAccountID(pClientSock->m_csIP));
	// 根据ip查找用户账户ID
	pClientSock->m_csAccountID =csID ;


	POSITION pos = pTmp->GetHeadPosition();	
	int nCount = pTmp->GetCount();
	for (int i=0; i< nCount; i++)
	// while(pos)
	{
		POSITION psTmp = pos;
		CClientInfo *pClient = static_cast < CClientInfo *>(pTmp->GetNext(pos));
		if (pClientInfo != NULL)
		{
			int nID = atoi(((CClientFileSock*)pClient->m_pClientSock)->m_csAccountID);
			if (nID == atoi(csID))
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
	// 记录客户信息
	pTmp->AddTail(pClientInfo);
	CSocket::OnAccept(nErrorCode);
}

int CSrvFileSock::FindAccountID(CString &csIP)
{
	CPtrList* pList = &((CECSSrvApp*)AfxGetApp())->m_ClientList;
	int nCount = pList->GetCount();
	POSITION pos = pList->GetHeadPosition();	
	if(pos)
	{
		pos = pList->GetHeadPosition();
		// 获取目标对象的套接字
		for(int j=0; j<nCount; j++)
		{
			CClientInfo *pClientInfo = static_cast < CClientInfo *>(pList->GetNext(pos));
			if (pClientInfo != NULL)
				if (csIP==pClientInfo->m_csClientIP)
				{
					int nID = atoi(((CClientSock*)pClientInfo->m_pClientSock)->m_csAccountID);
					return nID;
				}
		}
	}
	return 0;
}
