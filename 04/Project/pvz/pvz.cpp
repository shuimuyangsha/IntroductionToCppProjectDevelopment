// pvz.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "pvz.h"
#include "DialogMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//      ��Ӵ� DLL �������κε���
//      MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//      �ú�������ǰ�档
//
//      ����:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // �˴�Ϊ��ͨ������
//      }
//
//      �˺������κ� MFC ����
//      ������ÿ��������ʮ����Ҫ��  ����ζ��
//      ��������Ϊ�����еĵ�һ�����
//      ���֣������������ж������������
//      ������Ϊ���ǵĹ��캯���������� MFC
//      DLL ���á�
//
//      �й�������ϸ��Ϣ��
//      ����� MFC ����˵�� 33 �� 58��
//

// CpvzApp

BEGIN_MESSAGE_MAP(CpvzApp, CWinApp)
END_MESSAGE_MAP()


// CpvzApp ����

CpvzApp::CpvzApp()
{
    // TODO:  �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CpvzApp ����

CpvzApp theApp;


// ��Ϸ���ڱ���
#define GAME_WIN_NAME _T("ֲ���ս��ʬ���İ�")

// ȫ�ֱ���
// ���Ӿ��
HHOOK g_hHook;
// �����Ի���
CDialogMain *g_pDlg{ nullptr };

// CpvzApp ��ʼ��

BOOL CpvzApp::InitInstance()
{
    CWinApp::InitInstance();

    if(!AfxSocketInit()) {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    return TRUE;
}

static bool IsFullScreenWindow(HWND hWnd)
{
    bool bFullScreen = false;
    RECT rcApp, rcDesk;
    GetWindowRect(GetDesktopWindow(), &rcDesk);
    if(hWnd != GetDesktopWindow() && hWnd != GetShellWindow()) {
        GetWindowRect(hWnd, &rcApp);
        if(rcApp.left <= rcDesk.left
           && rcApp.top <= rcDesk.top
           && rcApp.right >= rcDesk.right
           && rcApp.bottom >= rcDesk.bottom) {
            bFullScreen = true;
        }
    }
    return bFullScreen;
}

LRESULT WINAPI KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    // �� "HOME"��̧��ʱ����������
    if(code == HC_ACTION && wParam == VK_HOME && GetKeyState(VK_HOME) < 0) {
        if(!g_pDlg) {
            // �½���������
            g_pDlg = new CDialogMain;
            g_pDlg->Create(IDD_DIALOG1);

            // �ж���Ϸ�Ƿ�ȫ�����������ȫ�����Ƶ���Ҵ��ڵ���Ϸ�����Ҳ�
            // ������ʾ�û�ȥ����ȫ��������
            HWND hWndGame = ::FindWindow(NULL, GAME_WIN_NAME);
            if(!IsFullScreenWindow(hWndGame)) {
                // �ƶ���Ϸ���ڵ����Ͻ�
                ::SetWindowPos(hWndGame, HWND_TOP, 0+10, 0+10, -1, -1, SWP_NOSIZE);
                RECT rcGame;
                ::GetWindowRect(hWndGame, &rcGame);
                // �ƶ��������ڵ���Ϸ�����Ҳ�
                ::SetWindowPos(g_pDlg->GetSafeHwnd(), HWND_TOP, rcGame.right + 0, 10,
                               -1, -1, SWP_NOSIZE);
            }
            else {
                AfxMessageBox(_T("��ȥ�� ��Ϸ ��ȫ���� ����"));
            }
        }
        g_pDlg->ShowWindow(SW_SHOW);
        return 0;
    }
    else {
        return CallNextHookEx(NULL, code, wParam, lParam);
    }
}

void __stdcall SetHook()
{
    // ��Ϸ���ھ��
    HWND hWndGame;
    hWndGame = ::FindWindow(NULL, GAME_WIN_NAME);

    // ��ȡ ��Ϸ����ID�ʹ����߳�ID
    DWORD dwProcessId, dwThreadId;
    dwThreadId = ::GetWindowThreadProcessId(hWndGame, &dwProcessId);

    // ��װ����
    g_hHook = ::SetWindowsHookEx(WH_KEYBOARD,  // ���̹���
                                 KeyboardProc,  // ���ӹ���
                                 GetModuleHandle(_T("pvz.dll")), // ���Ӻ������ڵ�ģ��
                                 dwThreadId); // ��Ϸ�����߳�ID
}
