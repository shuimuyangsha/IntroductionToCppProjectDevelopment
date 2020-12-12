// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "ClientSock.h"
#include "Message.h"
#include "ECSClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSock

CClientSock::CClientSock()
{

}

CClientSock::~CClientSock()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSock, CSocket)
    //{{AFX_MSG_MAP(CClientSock)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif  // 0

/////////////////////////////////////////////////////////////////////////////
// CClientSock member functions

void CClientSock::OnReceive(int nErrorCode)
{
    CSocket::OnReceive(nErrorCode);
    DWORD dwMaxNum = 0;
    if(IOCtl(SIOCATMARK, &dwMaxNum) == FALSE) { // 有外带数据需要读取
        char *pBuffer = new char[dwMaxNum]; // 定义一个5K的缓冲区
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        int nLen = Receive(pBuffer, dwMaxNum, MSG_OOB);
        memcpy(&Msg, pBuffer, sizeof(Msg));
        char *pchFile = NULL;
        if(Msg.m_nMsgType == MT_NETTEST) {  // 测试网络状态
            // 发送应答信息
            Msg.m_nMsgType = MT_NETCONNECT;
            Send(&Msg, sizeof(CMessage), MSG_OOB);
        }
        else if(Msg.m_nMsgType == MT_NETCONNECT) {  // 某个用户处于连接状态
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 1);
            return;
        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // 某个用户处于掉线状态
            // 进行掉线状态处理
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 0);
            return;
        }
        delete  [] pBuffer;
    }
    else if(IOCtl(FIONREAD, &dwMaxNum)) {
        CECSClientDlg *pMainDlg = (CECSClientDlg *)AfxGetMainWnd();
        pMainDlg->m_nTestNum = 0;

        char *pBuffer = new char[dwMaxNum];
        memset(pBuffer, 0, dwMaxNum);
        CMessage Msg;
        int nLen = Receive(pBuffer, dwMaxNum);
        memcpy(&Msg, pBuffer, sizeof(Msg));
        char *pchFile = NULL;
        if(nLen != sizeof(CMessage)) {      // 消息中包含文件数据
            pchFile = pBuffer + sizeof(CMessage);
        }
        if(Msg.m_nMsgType == MT_NETTEST) {  // 测试网络状态
            // 发送应答信息
            Msg.m_nMsgType = MT_NETCONNECT;
            Msg.m_nAccountID[0] = pMainDlg->m_nLoginUser;
            Send(&Msg, sizeof(CMessage), MSG_OOB);

        }
        else if(Msg.m_nMsgType == MT_NETCONNECT) {  // 某个用户处于连接状态
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 1);
            return;

        }
        else if(Msg.m_nMsgType == MT_DISCONNECT) {  // 某个用户处于掉线状态
            // 进行掉线状态处理
            CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
            delete [] pBuffer;
            pDlg->SetUserState(Msg.m_nAccountID[0], 0);
            return;
        }
        else if(Msg.m_nMsgType == MT_CONFIRM) { // 确认信息
            if(Msg.m_nSubType == ST_LOGINCONFIRM)   // 登录确认信息

            {
                if(strcmp(Msg.m_chContent, "success") == 0) {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_bLogined = TRUE;
                    // AfxMessageBox("登录成功!",0,0);

                }
                else if(strcmp(Msg.m_chContent, "failed") == 0) {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_nLoginUser = 0;
                    pDlg->m_bLogined = FALSE;
                    AfxMessageBox("用户名或密码错误!", 0, 0);
                }
                else if(strcmp(Msg.m_chContent, "logined") == 0) // 账户已在别处登录

                {
                    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                    pDlg->m_nLoginUser = 0;
                    pDlg->m_bLogined = FALSE;
                    AfxMessageBox("账户已在别处登录!", 0, 0);
                }
            }
        }
        else if(Msg.m_nMsgType == MT_SENDDATA) { // 接收数据
            if(Msg.m_nSubType == ST_TEXT)       // 文本数据

            {
                CString csText = Msg.m_chContent;// 判断文本是否为空

                if(csText.IsEmpty()) {
                    return;
                }
                // 如果之前没有与该账户建立通信对话框，则建立通信对话框

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                int nDesID = Msg.m_nSrcID;      // 获取消息的源账户ID

                POSITION pos = NULL;
                CSendDlg *pSdDlg = NULL;
                CString csName = "";
                CString csID = "";
                if(pDlg->SearchSendDlg(nDesID, pos) == FALSE) // 查找发送的对话框是否存在

                {
                    pSdDlg = new CSendDlg();
                    pSdDlg->m_UserID = nDesID;
                    pSdDlg->Create(IDD_SENDDLG_DIALOG);
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                    // 添加对方账户ID

                    csID.Format("%d", Msg.m_nSrcID);
                    pSdDlg->m_DesID.ResetContent();
                    pSdDlg->m_DesID.AddString(csID);
                    pSdDlg->m_DesID.SetCurSel(0);

                    BOOL bRet = pDlg->FindUserName(Msg.m_nSrcID, csName);
                    // 添加对方账户名

                    if(bRet) {
                        pSdDlg->m_DesName.ResetContent();
                        pSdDlg->m_DesName.AddString(csName);
                        pSdDlg->m_DesName.SetCurSel(0);
                    }
                    // 添加本地账户ID和名称

                    CString csLocalID;
                    csLocalID.Format("%d", pDlg->m_nLoginUser);
                    pSdDlg->m_LocalID.SetWindowText(csLocalID);

                    CString csLocalName = "";
                    bRet = pDlg->FindUserName(pDlg->m_nLoginUser, csLocalName);
                    if(bRet) {
                        pSdDlg->m_LocalName.SetWindowText(csLocalName);
                    }
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                }
                else if(pSdDlg == NULL && pos != NULL) {
                    pSdDlg = (CSendDlg *)pDlg->m_pSdDlgList.GetAt(pos);
                }
                // 显示发送窗口

                if(pSdDlg != NULL) {
                    // 添加发送数据


                    pSdDlg->m_ShowEdit.SetSel(-1, -1);
                    csID.Format("%d", Msg.m_nSrcID);
                    CString csUsrInfo = csID;
                    if(csName.IsEmpty()) {
                        pDlg->FindUserName(Msg.m_nSrcID, csName);
                    }

                    csUsrInfo += "(";
                    csUsrInfo += csName;
                    csUsrInfo += ") ";
                    CString csTime;
                    CTime ctNow = CTime::GetCurrentTime();
                    csTime = ctNow.Format("%H:%M:%S");
                    csUsrInfo += csTime;

                    CHARFORMAT cf;
                    memset(&cf, 0, sizeof(CHARFORMAT));
                    BOOL m_bSelect = (pSdDlg->m_ShowEdit.GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
                    if(m_bSelect) {
                        pSdDlg->m_ShowEdit.GetSelectionCharFormat(cf);
                    }
                    else {
                        pSdDlg->m_ShowEdit.GetDefaultCharFormat(cf);
                    }
                    // 判断是否选择了内容


                    cf.dwMask = CFM_COLOR;
                    cf.dwEffects = CFE_BOLD;
                    cf.crTextColor = RGB(0, 0, 255);
                    if(m_bSelect) {
                        pSdDlg->m_ShowEdit.SetSelectionCharFormat(cf);
                    }
                    else {
                        pSdDlg->m_ShowEdit.SetWordCharFormat(cf);
                    }

                    pSdDlg->m_ShowEdit.ReplaceSel(csUsrInfo);

                    pSdDlg->m_ShowEdit.SetSel(-1, -1);
                    pSdDlg->m_ShowEdit.ReplaceSel("\n");

                    pSdDlg->m_ShowEdit.GetDefaultCharFormat(cf);
                    pSdDlg->m_ShowEdit.SetSelectionCharFormat(cf);
                    csText.TrimLeft();
                    csText.TrimRight();
                    if(!csText.IsEmpty()) {
                        pSdDlg->m_ShowEdit.ReplaceSel(Msg.m_chContent);
                        pSdDlg->m_ShowEdit.SetSel(-1, -1);  // 在末尾添加换行

                        pSdDlg->m_ShowEdit.ReplaceSel("\n");
                    }
                    // 显示窗口

                    pSdDlg->ShowWindow(SW_SHOW);
                    // 在其他发送窗口之上显示当前发送窗口

                    pSdDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                }
            }
            else if(Msg.m_nSubType == ST_FILE)      // 文件数据

            {
                CECSClientApp *pApp = (CECSClientApp *)AfxGetApp();
                char chName[MAX_PATH] = {0};
                strcpy(chName, pApp->m_chFullPath);
                strcat(chName, "\\File\\");
                // 创建目录

                CreateDirectory(chName, NULL);
                strcat(chName, Msg.m_chContent);
                // 读取文件名

                CFile file(chName, CFile::modeCreate | CFile::modeReadWrite);

                file.Write(pchFile, Msg.dwDatalen);
                file.Close();
                // 初始化人员列表

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                pDlg->LoadOrgFromFile((_bstr_t)(char *)chName);
            }
            else if(Msg.m_nSubType == ST_FILEREQUEST || Msg.m_nSubType == ST_IMAGEREQUEST)  // 对方有文件发送

            {
                // 查找文件发送窗口

                // 如果之前没有与该账户建立通信对话框，则建立通信对话框

                CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
                int nDesID = Msg.m_nSrcID;      // 获取消息的源账户ID

                POSITION pos = NULL;
                CSendDlg *pSdDlg = NULL;
                CString csName = "";
                CString csID = "";
                if(pDlg->SearchSendDlg(nDesID, pos) == FALSE) // 查找发送的对话框是否存在

                {
                    pSdDlg = new CSendDlg();
                    pSdDlg->m_UserID = nDesID;
                    pSdDlg->Create(IDD_SENDDLG_DIALOG);
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                    // 添加对方账户ID

                    csID.Format("%d", Msg.m_nSrcID);
                    pSdDlg->m_DesID.ResetContent();
                    pSdDlg->m_DesID.AddString(csID);
                    pSdDlg->m_DesID.SetCurSel(0);

                    BOOL bRet = pDlg->FindUserName(Msg.m_nSrcID, csName);
                    // 添加对方账户名

                    if(bRet) {
                        pSdDlg->m_DesName.ResetContent();
                        pSdDlg->m_DesName.AddString(csName);
                        pSdDlg->m_DesName.SetCurSel(0);
                    }
                    // 添加本地账户ID和名称

                    CString csLocalID;
                    csLocalID.Format("%d", pDlg->m_nLoginUser);
                    pSdDlg->m_LocalID.SetWindowText(csLocalID);

                    CString csLocalName = "";
                    bRet = pDlg->FindUserName(pDlg->m_nLoginUser, csLocalName);
                    if(bRet) {
                        pSdDlg->m_LocalName.SetWindowText(csLocalName);
                    }
                    pDlg->m_pSdDlgList.AddTail(pSdDlg);
                }
                else if(pSdDlg == NULL && pos != NULL) {
                    pSdDlg = (CSendDlg *)pDlg->m_pSdDlgList.GetAt(pos);
                }
                // 显示窗口

                pSdDlg->ShowWindow(SW_SHOW);
                // 在其他发送窗口之上显示当前发送窗口

                pSdDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


                if(Msg.m_nSubType == ST_IMAGEREQUEST)   // 表示直接接收图像文件

                {
                    CString csLocalID;
                    pSdDlg->m_LocalID.GetWindowText(csLocalID);
                    int nSrcID = Msg.m_nSrcID;

                    pSdDlg->m_csSock.ShutDown();
                    pSdDlg->m_csSock.Close();

                    Msg.m_nSubType = ST_IMAGEREQUEST;
                    Msg.m_nSrcID = atoi(csLocalID);
                    Msg.m_nAccountID[0] = nSrcID;
                    Msg.m_nSubType = ST_IMAGERESPONSE;  // 图像数据




                    int nPort = 0;
                    BOOL bRet = FALSE;
create:
                    nPort = 4000 + rand() % 3000;
                    bRet = pSdDlg->m_csSock.Create(nPort);
                    if(bRet == FALSE) {
                        goto create;
                    }

                    int nSize = PACKAGESIZE;
                    pSdDlg->m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
                    pSdDlg->m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));
connect:
                    bRet = pSdDlg->m_csSock.Connect(pDlg->m_ServerIP, 601);
                    if(bRet == FALSE) {
                        goto connect;
                    }
                    Send(&Msg, sizeof(CMessage));

                }
                else {
                    if(pSdDlg->MessageBox("是否同意接收文件?", "提示", MB_YESNO) == IDYES) {
                        CString csLocalID;
                        pSdDlg->m_LocalID.GetWindowText(csLocalID);
                        int nSrcID = Msg.m_nSrcID;
                        Msg.m_nSrcID = atoi(csLocalID);
                        Msg.m_nAccountID[0] = nSrcID;
                        Msg.m_nSubType = ST_FILERESPONSE;


                        pSdDlg->m_csSock.ShutDown();
                        pSdDlg->m_csSock.Close();

                        int nPort = 0;
                        BOOL bRet = FALSE;
createimg:
                        nPort = 4000 + rand() % 3000;
                        bRet = pSdDlg->m_csSock.Create(nPort);
                        if(bRet == FALSE) {
                            goto createimg;
                        }

                        int nSize = PACKAGESIZE;
                        pSdDlg->m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
                        pSdDlg->m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));
connectimg:
                        bRet = pSdDlg->m_csSock.Connect(pDlg->m_ServerIP, 601);
                        if(bRet == FALSE) {
                            goto connectimg;
                        }
                        Send(&Msg, sizeof(CMessage));
                    }
                    else    // 发送拒绝信息

                    {
                        CString csLocalID;
                        pSdDlg->m_LocalID.GetWindowText(csLocalID);
                        int nSrcID = Msg.m_nSrcID;
                        Msg.m_nSrcID = atoi(csLocalID);
                        Msg.m_nAccountID[0] = nSrcID;
                        Msg.m_nSubType = ST_FILEDENY;
                        Send(&Msg, sizeof(CMessage));
                    }
                }
            }
        }
        delete [] pBuffer;
        CSocket::OnReceive(nErrorCode);
    }
}
