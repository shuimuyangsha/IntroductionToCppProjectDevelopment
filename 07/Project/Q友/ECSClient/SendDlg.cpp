// SendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECSClient.h"
#include "SendDlg.h"
#include "Message.h"
#include "ECSClientDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendDlg dialog


CSendDlg::CSendDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CSendDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSendDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_UserID = -1;
    m_bSendFile = FALSE;
    m_pShareEdit = NULL;
}


void CSendDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSendDlg)
    DDX_Control(pDX, IDC_SEND_FILE, m_SendFile);
    DDX_Control(pDX, IDC_FILEPROGRESS, m_Progress);
    DDX_Control(pDX, IDC_SHOWLIST, m_ShowEdit);
    DDX_Control(pDX, IDC_SENDLIST, m_SendEdit);
    DDX_Control(pDX, IDC_LOCALNAME, m_LocalName);
    DDX_Control(pDX, IDC_LOCALID, m_LocalID);
    DDX_Control(pDX, IDC_DESNAME, m_DesName);
    DDX_Control(pDX, IDC_DESID, m_DesID);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendDlg, CDialog)
    //{{AFX_MSG_MAP(CSendDlg)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_CLOSE, OnQuit)
    ON_BN_CLICKED(IDC_BTSEND, OnBtSend)
    ON_BN_CLICKED(IDC_SEND_FILE, OnSendFile)
    ON_BN_CLICKED(IDC_CHOOSE_IMG, OnSendImg)
    ON_WM_TIMER()
    ON_COMMAND(ID_RICH_SAVEIMAGE, OnSaveImage)
    ON_COMMAND(ID_RICH_CLEARALL, OnSendEditClear)

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendDlg message handlers

BOOL CSendDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetTimer(1, 800, NULL);
    return TRUE;
}


void CSendDlg::OnOK()
{

}

BOOL CSendDlg::PreTranslateMessage(MSG *pMsg)
{
    // ��ֹ��ESC���رնԻ���
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        return TRUE;
    }

    if(pMsg->hwnd == m_SendEdit.m_hWnd && pMsg->message == WM_RBUTTONDOWN) {

        CPoint pt;
        pt.x = LOWORD(pMsg->lParam);
        pt.y = HIWORD(pMsg->lParam);
        m_pShareEdit = &m_SendEdit;
        OnSendEditRBtnUp(pMsg->wParam, pt);
    }
    else if(pMsg->hwnd == m_ShowEdit.m_hWnd && pMsg->message == WM_RBUTTONDOWN) {
        CPoint pt;
        pt.x = LOWORD(pMsg->lParam);
        pt.y = HIWORD(pMsg->lParam);
        m_pShareEdit = &m_ShowEdit;
        OnSendEditRBtnUp(pMsg->wParam, pt);
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CSendDlg::OnClose()
{
    CDialog::OnClose();
}

void CSendDlg::OnQuit()
{
    OnCancel();
}

// �������� : ��ť�¼�
void CSendDlg::OnBtSend()
{
    // ��ȡ�ı�
    CString csText;
    m_SendEdit.GetWindowText(csText);

    // ��������а���ֻ����ͼ�����ı�Ϊ" "�����û�����ʱ����ʾ�ı���
    // ֻ��ʾ�Է��ı��/ʱ�����Ϣ
    if(csText.IsEmpty()) {
        return;
    }

    // ��ȡ�û�ID
    CString csUserID;
    m_DesID.GetWindowText(csUserID);
    if(csUserID.IsEmpty()) {
        return;
    }

    // �����������е��׽��ַ�������
    // ��ȡ���̵�������
    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
    // û���͵���Ϣ
    CMessage cmSendInfo;
    cmSendInfo.m_nMsgType = MT_SENDDATA;
    cmSendInfo.m_nSrcID = pDlg->m_nLoginUser;
    cmSendInfo.m_nSubType = ST_TEXT;
    cmSendInfo.m_nAccountID[0] = atoi(csUserID);    // ����Ŀ���û�
    strncpy(cmSendInfo.m_chContent, csText, csText.GetLength());

    // �����ı�����
    pDlg->m_ClientSock.Send(&cmSendInfo, sizeof(CMessage));

    // ����ʾ�������������
    // ������û���Ϣ
    CString csUsrInfo, csName;
    m_LocalID.GetWindowText(csUsrInfo);
    m_LocalName.GetWindowText(csName);
    csUsrInfo += "(";
    csUsrInfo += csName;
    csUsrInfo += ") ";
    CString csTime;
    CTime ctNow = CTime::GetCurrentTime();
    csTime = ctNow.Format("%H:%M:%S");
    csUsrInfo += csTime;
    m_ShowEdit.SetSel(-1, -1);
    CHARFORMAT cf;
    memset(&cf, 0, sizeof(CHARFORMAT));
    BOOL m_bSelect = (m_ShowEdit.GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
    if(m_bSelect) {
        m_ShowEdit.GetSelectionCharFormat(cf);
    }
    else {
        m_ShowEdit.GetDefaultCharFormat(cf);
    }

    // �ж��Ƿ�ѡ��������
    cf.dwMask = CFM_COLOR;
    cf.dwEffects = CFE_BOLD;
    cf.crTextColor = RGB(0, 0, 255);
    if(m_bSelect) {
        m_ShowEdit.SetSelectionCharFormat(cf);
    }
    else {
        m_ShowEdit.SetWordCharFormat(cf);
    }

    m_ShowEdit.ReplaceSel(csUsrInfo);

    m_ShowEdit.SetSel(-1, -1);
    m_ShowEdit.GetDefaultCharFormat(cf);
    m_ShowEdit.SetSelectionCharFormat(cf);

    m_ShowEdit.ReplaceSel("\n");
    m_ShowEdit.SetSel(-1, -1);
    m_ShowEdit.ReplaceSel(csText);


    // �������ͼ����Ϣ����ͼ����ӵ���ʾ������
    IRichEditOle *pRichOle = m_SendEdit.GetIRichEditOle();
    LONG lNum = 0;
    if(pRichOle != NULL) {
        lNum = pRichOle->GetObjectCount();
        IRichEditOle *pShowEditOle = m_ShowEdit.GetIRichEditOle();
        IOleClientSite *lpOleClientSite = NULL;
        if(lNum != 0) {
            m_ShowEdit.SetSel(-1, -1);
        }
        for(LONG i = 0; i < lNum; i++) {

            if(pShowEditOle != NULL) {
                pShowEditOle->GetClientSite(&lpOleClientSite);
            }

            REOBJECT reObject;
            ZeroMemory(&reObject, sizeof(REOBJECT)); // ��ʼ��һ����

            reObject.cbStruct = sizeof(REOBJECT);

            pRichOle->GetObject(i, &reObject, REO_GETOBJ_ALL_INTERFACES);
            if(lpOleClientSite != NULL) {
                reObject.polesite = lpOleClientSite ;
                reObject.cp = REO_CP_SELECTION; // REO_IOB_SELECTION;

                reObject.dwFlags = reObject.dwFlags | REO_BLANK;

                GIFLib::ICGifPtr lpAnimator = NULL;
                reObject.poleobj->QueryInterface(GIFLib::IID_ICGif, (void **)&lpAnimator);
                if(lpAnimator != NULL) {
                    // ��ȡ����������ļ�����

                    char chFullName[MAX_PATH] = {0};
                    strcpy(chFullName, (char *)lpAnimator->GetFileName());
                    m_csFullName = chFullName;
                    CString csTmp = ExtractFilePath(m_csFullName);
                    strcpy(chFileName, csTmp);
                    // �����������ͼ������
                    OnSendImage();
                }

                reObject.poleobj->SetClientSite(NULL);
                reObject.poleobj->SetClientSite(lpOleClientSite);
                pShowEditOle->InsertObject(&reObject);

                m_ShowEdit.RedrawWindow();          // ˢ�´���

                OleSetContainedObject(reObject.poleobj, TRUE);
                lpOleClientSite->SaveObject();
                reObject.pstg->Release();
                reObject.poleobj->Release();
            }
        }
        pRichOle->Release();
        if(pShowEditOle != NULL) {
            pShowEditOle->Release();
        }
        if(lpOleClientSite != NULL) {
            lpOleClientSite->Release();
        }
        pRichOle = NULL;
        if(lNum != 0) {
            m_ShowEdit.SetSel(-1, -1);
            m_ShowEdit.ReplaceSel("\n");
        }
    }
    if(lNum == 0) { // û��ͼ�����
        m_ShowEdit.SetSel(-1, -1);
        m_ShowEdit.ReplaceSel("\n");
    }

    // ��շ�����Ϣ�������
    m_SendEdit.SetWindowText("");
    m_SendEdit.Clear();
}

// �����ļ� ��ť�¼�
void CSendDlg::OnSendFile()
{
    // ���Ƚ��ļ����͵������������ɷ�����ת��
    // �ļ��ķ�����ͨ����������ʱͨ��ʵ�ֵ�
    // ���ļ�����ʱ��ʱ��������
    char chName[MAX_PATH] = {0};
    gethostname(chName, MAX_PATH);
    hostent *phost = gethostbyname(chName);
    char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);

    m_csSock.ShutDown();
    m_csSock.Close();

    // ѡ��Ҫ���͵��ļ�
    CFileDialog cfDlg(TRUE);
    if(cfDlg.DoModal() == IDOK) {
        BOOL bRet = FALSE;
        int nPort = 0;
        m_csFileName = cfDlg.GetFileName();
        strcpy(chFileName, m_csFileName);
        m_csFullName = cfDlg.GetPathName();

        // ������Ϣ
        CMessage Msg;
        m_csSock.ShutDown();
        m_csSock.Close();

create:
        nPort = 4000 + rand() % 3000;
        bRet = m_csSock.Create(nPort);

        if(bRet == FALSE) {
            goto create;
        }
        char chDisable = 1;

        setsockopt(m_csSock.m_hSocket, IPPROTO_TCP, TCP_NODELAY, &chDisable, sizeof(chDisable));
        int nSize = PACKAGESIZE;
        // ���÷��ͺͽ��ջ�����
        m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
        m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));

        DWORD dwEnable = 1;
        m_csSock.IOCtl(FIONBIO, &dwEnable);

        CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();

        // ���ӷ�����
connect:
        bRet = m_csSock.Connect(pDlg->m_ServerIP, 601);
        if(bRet == FALSE) {
            goto connect;
        }

        // ���ȷ���������Ϣ
        CString csDesID, csSrcID;
        m_DesID.GetWindowText(csDesID);
        m_LocalID.GetWindowText(csSrcID);
        m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
        m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));

        Msg.m_nMsgType = MT_SENDDATA;
        Msg.m_nSubType = ST_FILEREQUEST;
        Msg.m_nAccountID[0] = atoi(csDesID);    // ����Ŀ�����

        Msg.m_nSrcID = atoi(csSrcID);

        m_csSock.Send(&Msg, sizeof(CMessage));  // ����������Ϣ

        // �����ļ�
        SendFile (m_csSock, ST_FILE);

        // ��ֹ���ʹ����еĶ���ѡ��

        m_ShowEdit.SetSel(-1, 0);
        m_ShowEdit.Invalidate();
    }
}

void CSendDlg::SendFile(CSocket &sock, int nFileType)
{
    CFile file;
    file.Open(m_csFullName, CFile::modeRead);
    DWORD dwLen = file.GetLength();
    dwSendCount = dwLen / dwSendLen;


    CMessage Msg;
    // ��¼�ļ���

    strcpy(Msg.m_chContent, chFileName);

    Msg.m_nMsgType = MT_SENDDATA;
    Msg.m_nSubType = nFileType;

    CString csDesID, csSrcID;
    m_DesID.GetWindowText(csDesID);
    Msg.m_nAccountID[0] = atoi(csDesID);
    Msg.m_dwFileLen = dwLen;
    m_LocalID.GetWindowText(csSrcID);
    Msg.m_nSrcID = atoi(csSrcID);
    m_Progress.ShowWindow(SW_SHOW);
    int nMod = dwLen % dwSendLen;// ���һ�η����ļ����ֽ���

    if(nMod != 0) {
        dwSendCount += 1;
    }

    char *pchBuffer = NULL;
    for(int i = 0; i < dwSendCount; i++) {
        if(i == 0) {
            m_Progress.SetRange32(0, dwSendCount);
            Msg.m_nFlag = -1;
        }
        else if(i == dwSendCount - 1) {
            Msg.m_nFlag = 1;
        }
        else {
            Msg.m_nFlag = 0;
        }
        DWORD dwMsg;
        if(i == dwSendCount - 1 && nMod != 0) {
            dwMsg = nMod + sizeof(CMessage);
        }
        else if(i != dwSendCount - 1 || nMod != 0) {
            dwMsg = dwSendLen + sizeof(CMessage);
        }
        pchBuffer = new char[dwMsg];
        Msg.dwDatalen = dwMsg;  // ���ݱ��Ĵ�С

        memcpy(pchBuffer, &Msg, sizeof(CMessage));
        char *pchTmp = pchBuffer;
        pchTmp += sizeof(CMessage);

        file.Read(pchTmp, dwMsg - sizeof(CMessage));
        int nlen =  sock.Send(pchBuffer, dwMsg);
        m_Progress.SetPos(i);
        delete [] pchBuffer;
    }
    file.Close();

    m_Progress.SetPos(0);
    m_Progress.ShowWindow(SW_HIDE);
    if(nFileType == ST_FILE) {
        MessageBox("�ļ��������!");
    }
}

// ѡ��ͼƬ ��ť�¼�
void CSendDlg::OnSendImg()
{
    CFileDialog flDlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ͼƬ�ļ�|*.bmp;*.gif;*.jpg;*.jpeg;*.ico;||", this);
    if(flDlg.DoModal() == IDOK) {
        CString csFile = flDlg.GetPathName();
        IRichEditOle *lpRichOle = m_SendEdit.GetIRichEditOle();
        if(lpRichOle != NULL) {
            InsertImage(lpRichOle, csFile);
            lpRichOle->Release();
            lpRichOle = NULL;
        }
    }
    // ��ֹ���ʹ����еĶ���ѡ��
    m_ShowEdit.SetSel(-1, 0);
    m_ShowEdit.Invalidate();
}

// ��ָ������Ϣ���в���ͼƬ
BOOL CSendDlg::InsertImage(IRichEditOle *lpRichEditOle, CString &csFileName)
{
    IStorage *lpStorage = NULL;        // �洢�ӿ�

    IOleObject *lpOleObject = NULL;    // OLE object

    LPLOCKBYTES lpLockBytes = NULL;
    IOleClientSite *lpOleClientSite = NULL;
    GIFLib::ICGifPtr lpAnimator;
    CLSID clsid;
    REOBJECT reobject;
    HRESULT hr;
    if(lpRichEditOle == NULL) {
        return FALSE;
    }

    hr = ::CoInitialize(NULL);
    if(FAILED(hr)) {
        _com_issue_error(hr);
    }
    hr = lpAnimator.CreateInstance(GIFLib::CLSID_CGif);
    if(FAILED(hr)) {
        _com_issue_error(hr);
    }
    lpRichEditOle->GetClientSite(&lpOleClientSite);
    try {
        // ��ȡOLE����ӿ�

        hr = lpAnimator->QueryInterface(IID_IOleObject, (void **)&lpOleObject);
        if(FAILED(hr)) {
            AfxMessageBox("Error QueryInterface");
        }
        hr = lpOleObject->GetUserClassID(&clsid);
        if(FAILED(hr)) {
            AfxMessageBox("Error GetUserClassID");
        }
        // ��ֹ���ִ�����ʾ

        lpOleObject->SetClientSite(NULL);
        lpOleObject->SetClientSite(lpOleClientSite);

        hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes); // ����LOCKBYTE����

        if(FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        ASSERT(lpLockBytes != NULL);
        hr = ::StgCreateDocfileOnILockBytes(lpLockBytes,
                                            STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE,
                                            0, &lpStorage);
        if(FAILED(hr)) {
            VERIFY(lpLockBytes->Release() == 0);
            lpLockBytes = NULL;
            AfxThrowOleException(hr);
        }
        ZeroMemory(&reobject, sizeof(REOBJECT));   // ��ʼ��һ����

        reobject.cbStruct = sizeof(REOBJECT);
        reobject.clsid = clsid;
        reobject.cp = REO_CP_SELECTION;
        reobject.dvaspect = DVASPECT_CONTENT;
        reobject.dwFlags = REO_BLANK;
        reobject.poleobj = lpOleObject;
        reobject.polesite = lpOleClientSite;
        reobject.pstg = lpStorage;
        hr = lpRichEditOle->InsertObject(&reobject);

        hr = lpAnimator->LoadFromFile(csFileName.AllocSysString());

        if(FAILED(hr)) {
            AfxThrowOleException(hr);
        }

        RedrawWindow();    // ˢ�´���

        lpOleClientSite->SaveObject();
        OleSetContainedObject(lpOleObject, TRUE);
    }
    catch(CException *e) {
        e->Delete();
    }
    lpAnimator->Release();
    lpStorage->Release();
    return TRUE;
}

void CSendDlg::OnCancel()
{
    CDialog::OnCancel();
    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();
    CPtrList *pDlgList = &pDlg->m_pSdDlgList;
    if(pDlgList != NULL) {
        POSITION pos = pDlgList->GetHeadPosition();
        while(pos) {
            POSITION ptTmp = pos;
            CSendDlg *pSendDlg = (CSendDlg *)pDlgList->GetNext(pos);
            if(pSendDlg != NULL) {
                if(pSendDlg->m_hWnd == m_hWnd) {
                    pDlgList->RemoveAt(ptTmp);
                    delete pSendDlg;
                    break;
                }
            }
        }
    }
}

// ����ͼ������
void CSendDlg::OnSendImage()
{
    // ���Ƚ��ļ����͵������������ɷ�����ת��

    // �ļ��ķ�����ͨ����������ʱͨ��ʵ�ֵ�

    // ���ļ�����ʱ��ʱ��������

    char chName[MAX_PATH] = {0};
    gethostname(chName, MAX_PATH);
    hostent *phost = gethostbyname(chName);
    char *chIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);

    CECSClientDlg *pDlg = (CECSClientDlg *)AfxGetMainWnd();

    CMessage Msg;

    m_csSock.ShutDown();
    m_csSock.Close();

    BOOL bRet = FALSE;
    int nPort = 0;
create:
    nPort = 4000 + rand() % 3000;
    bRet = m_csSock.Create(nPort);
    if(bRet == FALSE) {
        goto create;
    }
    char chDisable = 1;
    setsockopt(m_csSock.m_hSocket, IPPROTO_TCP, TCP_NODELAY, &chDisable, sizeof(chDisable));
    int nSize = PACKAGESIZE;
    m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
    m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));

    DWORD dwEnable = 1;
    m_csSock.IOCtl(FIONBIO, &dwEnable);
connect:
    bRet = m_csSock.Connect(pDlg->m_ServerIP, 601);
    if(bRet == FALSE) {
        goto connect;
    }
    // ���ȷ���������Ϣ

    CString csDesID, csSrcID;
    m_DesID.GetWindowText(csDesID);
    m_LocalID.GetWindowText(csSrcID);
    m_csSock.SetSockOpt(SO_SNDBUF, &nSize, sizeof(nSize));
    m_csSock.SetSockOpt(SO_RCVBUF, &nSize, sizeof(nSize));

    Msg.m_nMsgType = MT_SENDDATA;
    Msg.m_nSubType = ST_IMAGEREQUEST;
    Msg.m_nAccountID[0] = atoi(csDesID);    // ����Ŀ�����

    Msg.m_nSrcID = atoi(csSrcID);
//  strcpy(Msg.m_chContent,m_csFileName);   // �ļ�����

    m_csSock.Send(&Msg, sizeof(CMessage));  // ����������Ϣ

}

// ����ļ�����
CString CSendDlg::ExtractFilePath(CString csFullPath)
{
    int pos = csFullPath.ReverseFind('\\');
    int nLen = strlen(m_csFullName.GetBuffer(0));
    CString csTmp = csFullPath.Right(nLen - pos - 1);
    return csTmp;
}

void CSendDlg::OnTimer(UINT nIDEvent)
{
    m_ShowEdit.Invalidate();
    m_SendEdit.Invalidate();
    CDialog::OnTimer(nIDEvent);
}

// �༭���һ��¼�
void CSendDlg::OnSendEditRBtnUp(UINT nFlags, CPoint point)
{
    if(m_pShareEdit != NULL) {
        // ��ý���

        CPoint objPT = point;
        LPARAM lpParam = MAKELPARAM(point.x, point.y);
        // ʹ��ǰ�����µĶ���ѡ��

        m_pShareEdit->SendMessage(WM_LBUTTONDOWN, nFlags, lpParam);

        // ����һ������ʽ�˵�

        CMenu PopMenu;
        PopMenu.CreatePopupMenu();
        // ��Ӳ˵���Ŀ

        PopMenu.AppendMenu(0, ID_RICH_SAVEIMAGE, "�ղ�ͼƬ(&S)");
        PopMenu.AppendMenu(1, ID_RICH_CLEARALL, "���(&C)");
        int nSel = ((m_pShareEdit->GetSelectionType() == SEL_OBJECT) ? MF_ENABLED : MF_DISABLED | MF_GRAYED) ;
        PopMenu.EnableMenuItem(ID_RICH_SAVEIMAGE, MF_BYCOMMAND | nSel);
        CString csText;
        m_pShareEdit->GetWindowText(csText);
        nSel = csText.IsEmpty() ? MF_DISABLED | MF_GRAYED : MF_ENABLED;
        PopMenu.EnableMenuItem(ID_RICH_CLEARALL, MF_BYCOMMAND | nSel);

        CPoint pt;
        GetCursorPos(&pt);
        PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
        PopMenu.DestroyMenu();
    }
}

// ����ͼƬ
void CSendDlg::OnSaveImage()
{
    if(m_pShareEdit != NULL) {
        IRichEditOle *pRichOle = m_pShareEdit->GetIRichEditOle();
        if(pRichOle != NULL) {
            REOBJECT reObject;
            ZeroMemory(&reObject, sizeof(REOBJECT)); // ��ʼ��һ����

            reObject.cbStruct = sizeof(REOBJECT);
            pRichOle->GetObject(0, &reObject, REO_GETOBJ_ALL_INTERFACES);
            GIFLib::ICGifPtr lpAnimator = NULL;
            reObject.poleobj->QueryInterface(GIFLib::IID_ICGif, (void **)&lpAnimator);
            if(lpAnimator != NULL) {
                // ��ȡ����������ļ�����

                char chFullName[MAX_PATH] = {0};
                strcpy(chFullName, (char *)lpAnimator->GetFileName());
                CString csFullName = chFullName;
                // ��ȡ�ļ�����չ��

                CString csExtendedName = ExtractFileName(csFullName);
                // ����һ��Ψһ���ļ���

                GUID flGUID;
                CString csFileName;
                HRESULT hRet = CoCreateGuid(&flGUID);
                if(hRet == S_OK) {
                    LPOLESTR pOlestr = NULL;
                    StringFromCLSID(flGUID, &pOlestr);
                    csFileName =  CString(pOlestr);
                }
                else {
                    csFileName.Format("%d", 1000 + rand() % 2000);
                }
                csFileName += csExtendedName;
                CFileDialog flDlg(FALSE, csExtendedName, csFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
                if(flDlg.DoModal() == IDOK) {
                    // ����lpAnimator����Ӧ�ķ��������ļ�

                    CString csName = flDlg.GetPathName();
                    lpAnimator->SaveToFile(csName.AllocSysString());
                }
                // ��ֹ���ʹ����еĶ���ѡ��

                m_ShowEdit.SetSel(-1, 0);
                m_ShowEdit.Invalidate();
                lpAnimator->Release();
            }
            pRichOle->Release();
            pRichOle = NULL;
        }
    }
}

// ��ȡ�ļ���չ��
CString CSendDlg::ExtractFileName(CString csFullName)
{
    int pos = csFullName.ReverseFind('.');
    int nLen = csFullName.GetLength();
    CString csTmp = csFullName.Right(nLen - pos);
    return csTmp;
}

// �����Ϣ������
void CSendDlg::OnSendEditClear()
{
    if(m_pShareEdit != NULL) {
        m_pShareEdit->SetWindowText("");
        m_pShareEdit->Clear();
    }
}
