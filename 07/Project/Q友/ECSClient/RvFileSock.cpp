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
		char* pBuffer = new char[dwMaxNum];	// 定义一个100K的缓冲区

		memset(pBuffer,0,dwMaxNum);
		CMessage Msg;
		DWORD nLen = Receive(pBuffer,dwMaxNum);
		if (m_bRvFull == FALSE)
		{
			// 与之前接收的数据组合为一个完整的数据报

			char* pchTmp = m_pchBuffer;
			pchTmp += m_nRvNum;
			memcpy(pchTmp,pBuffer,dwMaxNum);
			pchTmp = NULL;
			m_nRvNum += nLen;
			if (m_nRvNum >= m_nPackSize)	// 完整的数据报接收完成

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
			if((m_RvMsg.m_nSubType==ST_FILE ||m_RvMsg.m_nSubType==ST_IMAGE) && dwMaxNum < m_RvMsg.dwDatalen )// 包含CMessage结构，但是数据不完整		

			{
				m_bRvFull = FALSE;	
				m_pchBuffer = new char[m_RvMsg.dwDatalen];
				memset(m_pchBuffer,0,m_RvMsg.dwDatalen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);
				m_nRvNum = dwMaxNum;
				m_nPackSize = m_RvMsg.dwDatalen;
			}
			else if ((m_RvMsg.m_nSubType==ST_FILE ||m_RvMsg.m_nSubType==ST_IMAGE) && dwMaxNum == m_RvMsg.dwDatalen )	// 接收完整的数据报

			{
				m_nPackSize = m_RvMsg.dwDatalen;
				m_nRvNum = 0;
				m_pchBuffer = new char[m_RvMsg.dwDatalen];
				memset(m_pchBuffer,0,m_RvMsg.dwDatalen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);
			}
			else if (m_RvMsg.m_nSubType != ST_FILE && m_RvMsg.m_nSubType != ST_IMAGE)	// 传输的不是文件数据，只是一个CMessage结构数据

			{
				m_nPackSize = nLen;
				m_nRvNum = 0;
				m_pchBuffer = new char[nLen];
				memset(m_pchBuffer,0,nLen);
				memcpy(m_pchBuffer,pBuffer,dwMaxNum);			
			}
		}
		if (m_bRvFull==FALSE)	// 数据报没有接收完成，需要继续接收数据

		{
			delete []pBuffer;
			return;
		}

		// 读取消息源账户ID

		int nID = m_RvMsg.m_nSrcID;
		// 判断与源账户ID关联的发送窗口是否存在

		if (m_pDlg==NULL)
		{
			POSITION pos = NULL;
			CECSClientDlg *pMainDlg = (CECSClientDlg *)AfxGetMainWnd();
			if (pMainDlg->SearchSendDlg(nID,pos)==FALSE)
			{
				m_pDlg = new CSendDlg();
				// 关联对话框的目标ID

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
				// 添加对方账户ID

				CString csID;
				csID.Format("%d",nID);
				m_pDlg->m_DesID.ResetContent();	// 清空数据

				m_pDlg->m_DesID.AddString(csID);
				m_pDlg->m_DesID.SetCurSel(0);

				CString csName;
				BOOL bRet = pMainDlg->FindUserName(nID,csName);
				if (bRet)
				{
					// 添加对方账户名

					m_pDlg->m_DesName.ResetContent();	// 清空数据

					m_pDlg->m_DesName.AddString(csName);
					m_pDlg->m_DesName.SetCurSel(0);
				}

				// 添加本地账户ID和名称

				CString csLocalID;
				csLocalID.Format("%d",pMainDlg->m_nLoginUser);
				m_pDlg->m_LocalID.SetWindowText(csLocalID);
				CString csLocalName = "";
				bRet = pMainDlg->FindUserName(pMainDlg->m_nLoginUser,csLocalName);

				if (bRet)
				{
					m_pDlg->m_LocalName.SetWindowText(csLocalName);
				}
				// 显示窗口

				m_pDlg->ShowWindow(SW_SHOW);
				// 在其他发送窗口之上显示当前发送窗口

				m_pDlg->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);	
			}		
		}

		if (m_RvMsg.m_nMsgType==MT_SENDDATA)
		{
			if (m_RvMsg.m_nSubType==ST_FILERESPONSE)	// 对方同意了接收文件

			{
				// 开始发送文件

				m_pDlg->SendFile(*this);
			}
			else if (m_RvMsg.m_nSubType==ST_IMAGERESPONSE)
			{
				// 开始图像文件

				m_pDlg->SendFile(*this,ST_IMAGE);	
			}
			else if (m_RvMsg.m_nSubType==ST_FILEDENY)	// 对方拒绝接收文件

			{
				Msg.m_nMsgType = MT_DISCONNECT;
				Msg.m_nSubType = ST_UNKNOWN;
				Send(&Msg,sizeof(CMessage));
				ShutDown();
				Close();
				m_pDlg->MessageBox("对方拒绝了接收文件");
			}
			else if (m_RvMsg.m_nSubType==ST_FILE || m_RvMsg.m_nSubType==ST_IMAGE)	// 接收文件

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
					// 创建目录

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
							// 显示接收的图像文件

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
							m_pDlg->MessageBox("文件接收完成!","提示");	
						}
					}
				}
				else if (m_RvMsg.m_nFlag==0)	// 文件传输过程中

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
				else if (m_RvMsg.m_nFlag==1)	// 文件传输结束

				{
					char* pTp = m_pchBuffer + sizeof(CMessage);
					m_File.Write(pTp,m_nPackSize-sizeof(CMessage));
					m_bFirst = TRUE;
					m_File.Close();
					pTp = NULL;
					// 向服务器发送断开连接的消息

					Msg.m_nMsgType = MT_DISCONNECT;
					Msg.m_nSubType = ST_UNKNOWN;
					Send(&Msg,sizeof(CMessage));
					ShutDown();
					Close();
					if (m_RvMsg.m_nSubType==ST_IMAGE)
					{
						// 显示接收的图像文件

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
						m_pDlg->MessageBox("文件接收完成!","提示");
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
