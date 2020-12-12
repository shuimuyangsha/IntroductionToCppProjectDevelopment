// RvFileSock.cpp : implementation file
//

#include "stdafx.h"
#include "ecsclient.h"
#include "RvFileSock.h"
#include "Message.h"
#include "SendDlg.h"
#include "ECSClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRvFileSock

CRvFileSock::CRvFileSock()
{
	m_nIndex = 0;
	m_pDlg = NULL;
	m_bFirst = TRUE;
	m_bRvFull = TRUE;
	m_nPackSize = 0;
	m_nRvNum = 0;
	m_pchBuffer = NULL;
}

CRvFileSock::~CRvFileSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRvFileSock, CSocket)
	//{{AFX_MSG_MAP(CRvFileSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRvFileSock member functions

void CRvFileSock::OnReceive(int nErrorCode) 
{
	CSocket::OnReceive(nErrorCode);
	DWORD dwMaxNum = 0;
	if (IOCtl(FIONREAD,&dwMaxNum))
	{
		char* pBuffer = new char[dwMaxNum];	// ����һ��100K�Ļ�����

		memset(pBuffer,0,dwMaxNum);
		CMessage Msg;
		DWORD nLen = Receive(pBuffer,dwMaxNum);
		if (m_bRvFull == FALSE)
		{
			// ��֮ǰ���յ��������Ϊһ�����������ݱ�

			char* pchTmp = m_pchBuffer;
			pchTmp += m_nRvNum;
			memcpy(pchTmp,pBuffer,dwMaxNum);
			pchTmp = NULL;
			m_nRvNum += nLen;
			if (m_nRvNum >= m_nPackSize)	// ���������ݱ��������

			{
				m_bRvFull = TRUE;
				m_nRvNum = 0;
			}
		}
		else
		{
			m_bRvFull = TRUE;
			memcpy(&Msg,pBuffer,sizeof(CMessage));
			memcpy(&m_RvMsg,pBuffer,sizeof(CMessage));
			if((m_RvMsg.m_nSubType==ST_FILE ||m_RvMsg.m_nSubType==ST_IMAGE) && dwMaxNum < m_RvMsg.dwDatalen )// ����CMessage�ṹ���������ݲ�����		

			{
				m_bRvFull = FALSE;	
				m_pchBuffer = new char[m_RvMsg.dwDatalen];
				memset(m_pchBuffer,0,m_RvMsg.dwDatalen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);
				m_nRvNum = dwMaxNum;
				m_nPackSize = m_RvMsg.dwDatalen;
			}
			else if ((m_RvMsg.m_nSubType==ST_FILE ||m_RvMsg.m_nSubType==ST_IMAGE) && dwMaxNum == m_RvMsg.dwDatalen )	// �������������ݱ�

			{
				m_nPackSize = m_RvMsg.dwDatalen;
				m_nRvNum = 0;
				m_pchBuffer = new char[m_RvMsg.dwDatalen];
				memset(m_pchBuffer,0,m_RvMsg.dwDatalen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);
			}
			else if (m_RvMsg.m_nSubType != ST_FILE && m_RvMsg.m_nSubType != ST_IMAGE)	// ����Ĳ����ļ����ݣ�ֻ��һ��CMessage�ṹ����

			{
				m_nPackSize = nLen;
				m_nRvNum = 0;
				m_pchBuffer = new char[nLen];
				memset(m_pchBuffer,0,nLen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);			
			}
		}
		if (m_bRvFull==FALSE)	// ���ݱ�û�н�����ɣ���Ҫ������������

		{
			delete []pBuffer;
			return;
		}

		// ��ȡ��ϢԴ�˻�ID

		int nID = m_RvMsg.m_nSrcID;
		// �ж���Դ�˻�ID�����ķ��ʹ����Ƿ����

		if (m_pDlg==NULL)
		{
			POSITION pos = NULL;
			CECSClientDlg *pMainDlg = (CECSClientDlg *)AfxGetMainWnd();
			if (pMainDlg->SearchSendDlg(nID,pos)==FALSE)
			{
				m_pDlg = new CSendDlg();
				// �����Ի����Ŀ��ID

				m_pDlg->m_UserID = nID;
				m_pDlg->Create(IDD_SENDDLG_DIALOG);
				pMainDlg->m_pSdDlgList.AddTail(m_pDlg);
			}
			else if (m_pDlg == NULL && pos != NULL)
			{
				m_pDlg= (CSendDlg *)pMainDlg->m_pSdDlgList.GetAt(pos);
			}
			if (m_pDlg != NULL)
			{
				// ��ӶԷ��˻�ID

				CString csID;
				csID.Format("%d",nID);
				m_pDlg->m_DesID.ResetContent();	// �������

				m_pDlg->m_DesID.AddString(csID);
				m_pDlg->m_DesID.SetCurSel(0);

				CString csName;
				BOOL bRet = pMainDlg->FindUserName(nID,csName);
				if (bRet)
				{
					// ��ӶԷ��˻���

					m_pDlg->m_DesName.ResetContent();	// �������

					m_pDlg->m_DesName.AddString(csName);
					m_pDlg->m_DesName.SetCurSel(0);
				}

				// ��ӱ����˻�ID������

				CString csLocalID;
				csLocalID.Format("%d",pMainDlg->m_nLoginUser);
				m_pDlg->m_LocalID.SetWindowText(csLocalID);
				CString csLocalName = "";
				bRet = pMainDlg->FindUserName(pMainDlg->m_nLoginUser,csLocalName);

				if (bRet)
				{
					m_pDlg->m_LocalName.SetWindowText(csLocalName);
				}
				// ��ʾ����

				m_pDlg->ShowWindow(SW_SHOW);
				// ���������ʹ���֮����ʾ��ǰ���ʹ���

				m_pDlg->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);	
			}		
		}

		if (m_RvMsg.m_nMsgType==MT_SENDDATA)
		{
			if (m_RvMsg.m_nSubType==ST_FILERESPONSE)	// �Է�ͬ���˽����ļ�

			{
				// ��ʼ�����ļ�

				m_pDlg->SendFile(*this);
			}
			else if (m_RvMsg.m_nSubType==ST_IMAGERESPONSE)
			{
				// ��ʼͼ���ļ�

				m_pDlg->SendFile(*this,ST_IMAGE);	
			}
			else if (m_RvMsg.m_nSubType==ST_FILEDENY)	// �Է��ܾ������ļ�

			{
				Msg.m_nMsgType = MT_DISCONNECT;
				Msg.m_nSubType = ST_UNKNOWN;
				Send(&Msg,sizeof(CMessage));
				ShutDown();
				Close();
				m_pDlg->MessageBox("�Է��ܾ��˽����ļ�");
			}
			else if (m_RvMsg.m_nSubType==ST_FILE || m_RvMsg.m_nSubType==ST_IMAGE)	// �����ļ�

			{
				if (m_bFirst|| m_RvMsg.m_nFlag==-1)
				{
					m_bFirst = FALSE;
					CECSClientApp *pApp = (CECSClientApp *)AfxGetApp();
					// char chName[MAX_PATH] = {0};

					strcpy(m_chName,pApp->m_chFullPath);
					if (m_RvMsg.m_nSubType==ST_FILE )
					{
						strcat(m_chName,"\\File\\");
					}
					else
					{
						strcat(m_chName,"\\Images\\");
					}
					// ����Ŀ¼

					CreateDirectory(m_chName,NULL);
					strcat(m_chName,m_RvMsg.m_chContent);

					m_File.Open(m_chName,CFile::modeCreate|CFile::modeReadWrite);
					char* pchTmp = m_pchBuffer + sizeof(CMessage);
					m_File.Write(pchTmp,m_nPackSize-sizeof(CMessage));
					pchTmp = NULL;
					if (m_RvMsg.m_nSubType==ST_FILE && m_pDlg != NULL)
					{
						m_pDlg->m_Progress.SetRange32(0,m_RvMsg.m_dwFileLen);
						m_pDlg->m_Progress.ShowWindow(SW_SHOW);
						m_pDlg->m_Progress.SetPos(m_nPackSize-sizeof(CMessage));
					
					}
					if (m_RvMsg.m_dwFileLen==m_nPackSize-sizeof(CMessage))
					{
						m_File.Close();
						m_bFirst = TRUE;
						Msg.m_nMsgType = MT_DISCONNECT;
						Msg.m_nSubType = ST_UNKNOWN;
						Send(&Msg,sizeof(CMessage));
						ShutDown();
						Close();

						if (m_RvMsg.m_nSubType==ST_IMAGE)
						{
							// ��ʾ���յ�ͼ���ļ�

							if(m_pDlg != NULL)
							{
								CString csFile = m_chName;// m_RvMsg.m_chContent;

								IRichEditOle *lpRichOle = m_pDlg->m_ShowEdit.GetIRichEditOle();
								if (lpRichOle != NULL)
								{
									m_pDlg->m_ShowEdit.SetSel(-1,-1);
									m_pDlg->InsertImage(lpRichOle,csFile);
									lpRichOle->Release();
									lpRichOle = NULL;
								}
								m_pDlg->m_ShowEdit.SetSel(-1,-1);
								m_pDlg->m_ShowEdit.ReplaceSel("\n");
							}
						}
						else if (m_RvMsg.m_nSubType==ST_FILE)
						{
							m_pDlg->m_Progress.SetPos(0);
							m_pDlg->m_Progress.ShowWindow(SW_HIDE);
							m_pDlg->MessageBox("�ļ��������!","��ʾ");	
						}
					}
				}
				else if (m_RvMsg.m_nFlag==0)	// �ļ����������

				{					
					char* pTmp = m_pchBuffer + sizeof(CMessage);
					m_File.Write(pTmp,m_nPackSize-sizeof(CMessage));
					pTmp = NULL;
					if (m_RvMsg.m_nSubType==ST_FILE && m_pDlg != NULL)
					{
						int nPos = m_pDlg->m_Progress.GetPos();
						m_pDlg->m_Progress.SetPos(nPos+m_nPackSize-sizeof(CMessage));
					
					}
				}
				else if (m_RvMsg.m_nFlag==1)	// �ļ��������

				{
					char* pTp = m_pchBuffer + sizeof(CMessage);
					m_File.Write(pTp,m_nPackSize-sizeof(CMessage));
					m_bFirst = TRUE;
					m_File.Close();
					pTp = NULL;
					// ����������ͶϿ����ӵ���Ϣ

					Msg.m_nMsgType = MT_DISCONNECT;
					Msg.m_nSubType = ST_UNKNOWN;
					Send(&Msg,sizeof(CMessage));
					ShutDown();
					Close();
					if (m_RvMsg.m_nSubType==ST_IMAGE)
					{
						// ��ʾ���յ�ͼ���ļ�

						if(m_pDlg != NULL)
						{
							m_pDlg->m_ShowEdit.SetSel(-1,-1);
							CString csFile = m_chName;// m_RvMsg.m_chContent;

							IRichEditOle *lpRichOle = m_pDlg->m_ShowEdit.GetIRichEditOle();
							if (lpRichOle != NULL)
							{
								m_pDlg->InsertImage(lpRichOle,csFile);
								lpRichOle->Release();
								lpRichOle = NULL;
							}
							m_pDlg->m_ShowEdit.SetSel(-1,-1);
							m_pDlg->m_ShowEdit.ReplaceSel("\n");
						}
					}
					else if (m_RvMsg.m_nSubType==ST_FILE )
					{
						m_pDlg->m_Progress.SetPos(0);
						m_pDlg->m_Progress.ShowWindow(SW_HIDE);
						m_pDlg->MessageBox("�ļ��������!","��ʾ");
					}
				}
			}
		}
		if (m_bRvFull==TRUE && m_pchBuffer != NULL)
		{
			delete [] m_pchBuffer;
			m_pchBuffer = NULL;
		}
		delete [] pBuffer;
	}	
}
