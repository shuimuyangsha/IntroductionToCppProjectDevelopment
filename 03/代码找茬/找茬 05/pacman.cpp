// pacman.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "pacman.h"
#include "GMap.h"
#include "GObject.h"

using namespace std;

#define WLENTH 700  // ��
#define WHIGHT 740  // ��
#define STAGE_COUNT 3 // һ������
HWND g_hwnd; // ��Ϸ���ھ��

#define MAX_LOADSTRING 100


// ȫ�ֱ���:
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������



// �˴���ģ���а����ĺ�����ǰ������:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    // ��������ʹ����
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��:
    if(!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

    // ��ǰ�Ĺؿ�
    int s_n = 0; // [0, 1, 2]
    // ��ͼ
    GMap *MapArray[STAGE_COUNT] = { new Stage_1(), new Stage_2(), new Stage_3() };

    // ��Ҷ���
    // �Լ�
    auto g_me = std::make_shared<PacMan>(P_ROW, P_ARRAY);
    // �趨�ĸ����˶���
    auto e1 = std::make_shared<RedOne>(E_ROW, E_ARRAY);
    auto e2 = std::make_shared<RedOne>(E_ROW, E_ARRAY);
    auto e3 = std::make_shared<BlueOne>(E_ROW, E_ARRAY);
    auto e4 = std::make_shared<YellowOne>(E_ROW, E_ARRAY);

    // �ؿ�
    GObject::pStage = MapArray[s_n]; //��ʼ��Ϊ��һ�ص�ͼ

    // �趨���
    Enermy::player = g_me;

    MSG msg;

    DWORD dwLastTime = 0;
    // ����Ϣѭ��:
    // ���û�б�ץ,���ҹؿ�<3
    while(!g_me->IsOver() && s_n < STAGE_COUNT) {
        // �ж��Ƿ�Ӯ�ñ���
        if(g_me->IsWin()) {
            s_n++;
            // �����Լ��͵���λ��
            g_me->SetPosition(P_ROW, P_ARRAY);
            e1->SetPosition(E_ROW, E_ARRAY);
            e2->SetPosition(E_ROW, E_ARRAY);
            e3->SetPosition(E_ROW, E_ARRAY);
            e4->SetPosition(E_ROW, E_ARRAY);
            // �ж��Ƿ������3��,������,�˳���Ϸ,���������һ��
            if(s_n < 3) {
                MessageBox(g_hwnd, _T("��ϲ����"), _T("�Զ�����ʾ"), MB_OK);
                GObject::pStage = MapArray[s_n];
                RECT screenRect;
                screenRect.top = 0;
                screenRect.left = 0;
                screenRect.right = WLENTH;
                screenRect.bottom = WHIGHT;

                HDC hdc = GetDC(g_hwnd);
                std::shared_ptr<HDC__> dc(hdc, [](HDC hdc) {
                    ::ReleaseDC(g_hwnd, hdc);
                });
                ::FillRect(dc.get(), &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
                GObject::pStage->DrawMap(hdc);
                // ��������ѭ��
                continue;
            }
            else {
                // ����ѭ��
                break;
            }
        }

        // ��ȡ��Ϣ
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        // �ж�ʱ��,���򻭵�̫��
        if(GetTickCount() - dwLastTime >= 40) {
            dwLastTime = GetTickCount();
        }
        else {
            // ʱ�䲻��,���β����л滭
            continue;
        }

        {
            // ����豸
            HDC hdc = ::GetDC(g_hwnd);
            // ��ʹ��ʱ�Զ��ͷ�
            std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {
                ::ReleaseDC(g_hwnd, hdc);
            });
            // ������
            MapArray[s_n]->DrawPeas(hdc);
            // ����ͼ
            MapArray[s_n]->DrawMap(hdc);

            // �����˼��Զ��˶�
            {
                e1->action();
                e1->DrawBlank(hdc);
                e1->Draw(hdc);

                e2->action();
                e2->DrawBlank(hdc);
                e2->Draw(hdc);

                e3->action();
                e3->DrawBlank(hdc);
                e3->Draw(hdc);

                e4->action();
                e4->DrawBlank(hdc);
                e4->Draw(hdc);
            }

            {
                // ���Լ�
                g_me->DrawBlank(hdc);
                g_me->Draw(hdc);
                // �Լ���ǰ�ƶ�
                //g_me->action();

                // ��ȡ���� : �����Լ��ķ���
                if(GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    g_me->SetTwCommand(DOWN);
                }
                if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    g_me->SetTwCommand(LEFT);
                }
                if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    g_me->SetTwCommand(RIGHT);
                }
                if(GetAsyncKeyState(VK_UP) & 0x8000) {
                    g_me->SetTwCommand(UP);
                }
            }
        }
    }

    // �����Ϸ����
    if(g_me->IsOver()) {
        MessageBoxA(NULL, "��ʦδ��", "�Զ�����ʾ", MB_OK);
    }
    // ����,��ʾӮ����Ϸ
    else {
        MessageBoxA(NULL, "��ϲ��Ӯ����ʤ��\r\nȷ������Ϸ�˳�", "�Զ�����ʾ", MB_OK);
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

    // ��������
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW
                              , 0 // ����Ļ�е�λ�� x
                              , 0 // ����Ļ�е�λ�� y
                              , WLENTH // ��
                              , WHIGHT // ��
                              , nullptr, nullptr, hInstance, nullptr);

    if(!hWnd) {
        return FALSE;
    }

    // ������Ϸ����,������õ�
    g_hwnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��:
            switch(wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch(message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
