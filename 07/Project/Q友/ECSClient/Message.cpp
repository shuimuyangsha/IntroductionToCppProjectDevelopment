// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ECSClient.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMessage::CMessage()
{
	memset(m_chAccount,0,ACCOUNTLEN);
	memset(m_chContent,0,CONTENTLEN);
	memset(m_chPassword,0,PASSWRODLEN);
	memset(m_nAccountID,0,MAXACCOUNT);
	m_nSubType = ST_UNKNOWN;
	m_nMsgType = MT_UNKNOWN;
	m_pchData = NULL;
	dwDatalen = 0;
	m_nSrcID = 0;
	m_nFlag = 0;
	m_dwFileLen = 0;
}

CMessage::~CMessage()
{
	if (m_pchData != NULL)
	{
		delete []m_pchData;
		m_pchData = NULL;
	}
	memset(m_chAccount,0,ACCOUNTLEN);
	memset(m_chContent,0,CONTENTLEN);
	memset(m_chPassword,0,PASSWRODLEN);
	memset(m_nAccountID,0,MAXACCOUNT);
	m_nSubType = ST_UNKNOWN;
	m_nMsgType = MT_UNKNOWN;
	dwDatalen = 0;
	m_nSrcID = 0;
	m_nFlag = 0;
	m_dwFileLen = 0;
	










	
}