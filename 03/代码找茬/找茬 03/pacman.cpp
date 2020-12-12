// pacman.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "pacman.h"

using namespace std;

#define WLENTH 700  // ��
#define WHIGHT 740  // ��
#define STAGE_COUNT 3 // һ������
static HWND g_hwnd; // ��Ϸ���ھ��

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
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��:
    if(!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

    MSG msg;

    // ����Ϣѭ��:
    bool bRunning = true;
    while(bRunning) {
        // ��ȡ��Ϣ
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        /*// �������
        {
            HDC hdc = ::GetDC(g_hwnd);
            SetPixel(hdc, rand() % WLENTH , rand()%WHIGHT, RGB(rand()%256, rand()%256, rand()%256));
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        /*// ���߲���
        {
            HDC hdc = ::GetDC(g_hwnd);
            // ��������
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
            // ѡ�񻭱�
            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
            MoveToEx(hdc, rand() % WLENTH, rand() % WHIGHT, NULL);
            LineTo(hdc, rand() % WLENTH, rand() % WHIGHT);

            // �ָ�����
            SelectObject(hdc, oldPen);
            ::ReleaseDC(g_hwnd, hdc);
            // ��ͣ1����,��Ȼ����̫��,����
            Sleep(1);
        }*/

        /*// �����Ͳ���
        {
            HDC hdc = ::GetDC(g_hwnd);
            {
                // ��������
                HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
                // ѡ�񻭱�
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                // ������(����)
                Rectangle(hdc, 100, 200, 300, 500);
                // �ָ�����
                SelectObject(hdc, oldPen);
                DeleteObject(pen);
            }

            {
                // ��������
                HBRUSH bBrush = CreateSolidBrush(RGB(0, 0, 255));
                // ��� ����
                RECT rect;
                rect.left = 50;
                rect.top = 270;
                rect.right = 150;
                rect.bottom = 370;
                FillRect(hdc, &rect, bBrush);
                DeleteObject(bBrush);
            }

            ::ReleaseDC(g_hwnd, hdc);
            // ��ͣ1����,��Ȼ����̫��,����
            Sleep(1);
        }
        */

        /*// ��Բ����
        {
            HDC hdc = ::GetDC(g_hwnd);
            //��Բ
            Ellipse(hdc, 200, 150, 300, 250);
            //����Բ
            Ellipse(hdc, 200, 270, 340, 370);
            //����Բ
            Ellipse(hdc, 100, 100, 200, 150);
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        /*// �����Ͳ���
        {
            HDC hdc = ::GetDC(g_hwnd);
            Arc(hdc, 100, 100, 200, 300  // ���� ���ϵ�,���µ�
                , 150, 200 // ���
                , 100, 200 // �յ� (�������ʱ������)
               );

            Arc(hdc, 0,0, 100, 100
                , 50, 100
                , 50, 0
               );
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        // �ۺ�Ӧ��, ��һ���������
        {
            static DWORD dwTime = GetTickCount();
            // �������ϻ�ͼ��ʱ�����50����ʱ,�Ž��б��λ���
            if(GetTickCount() - dwTime >= 50) {
                dwTime = GetTickCount();
            }
            else {
                continue;
            }
            /* ģ�⵱ǰ��֡
                ������һ��5֡,ÿһ֡����ͬ��ͼ��
            */
            static int iFrame = 0;
            //++iFrame;
            if(iFrame >= 5) {
                iFrame = 0;
            }

            // ������������λ��
            int x = 300, y = 300;

            // ����İ뾶
            int r = 100;

            // dc ������
            HDC hdc = ::GetDC(g_hwnd);
            std::shared_ptr<HDC__> dc(::GetDC(g_hwnd), [](HDC hdc) {
                ::ReleaseDC(g_hwnd, hdc);
            });
            // ��ȡ���ڿͻ�����С
            RECT rc;
            GetClientRect(g_hwnd, &rc);

            // ������ˢ
            std::shared_ptr<HBRUSH__> br(
                ::CreateSolidBrush(RGB(255, 255, 255)),
            [](HBRUSH hbr) {
                ::DeleteObject(hbr);
            });

            // ������(�����һ֡��������
            FillRect(dc.get(), &rc, br.get());

#define PI (3.1415926f) // ���� Բ���ʵ�ֵ
            switch(iFrame) {
                case 0: {
                    // ��һ��Բ
                    Ellipse(dc.get(), x - r, y - r, x + r, y + r);
                    // ��һ������
                    MoveToEx(dc.get(), x - r, y, NULL);
                    LineTo(dc.get(), x, y);
                    break;
                }
                case 1: {
                    // ����(������������ƫ�� PI/4
                    int x0, y0; // ���Ͻǵĵ�
                    int x1, y1; // ���½ǵĵ�
                    x0 = x - static_cast<int>(r * sin(PI * 0.75f));
                    y0 = y + static_cast<int>(r * cos(PI * 0.75f));

                    x1 = x + static_cast<int>(r * sin(PI * 1.25f));
                    y1 = y - static_cast<int>(r * cos(PI * 1.25f));

                    SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
                    SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
                    SetPixel(dc.get(), x, y, RGB(0, 0, 0));
                    // ��һ����Բ + һ������
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x1, y1
                        , x0, y0);

                    // ������
                    MoveToEx(dc.get(), x0, y0, NULL);
                    LineTo(dc.get(), x, y);

                    MoveToEx(dc.get(), x1, y1, NULL);
                    LineTo(dc.get(), x, y);
                    break;

                }
                case 2: {
                    // ��һ����Բ + һ������
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x, y + r
                        , x, y - r
                       );
                    // ������
                    // Բ������ĵ�
                    MoveToEx(dc.get(), x, y - r, NULL);
                    // Բ������ĵ�
                    LineTo(dc.get(), x, y + r);
                    break;
                }
                case 3: {
                    // ����(������������ƫ�� PI/4
                    int x0, y0; // ���Ͻǵĵ�
                    int x1, y1; // ���½ǵĵ�
                    x0 = x - static_cast<int>(r * sin(PI * 0.75f));
                    y0 = y + static_cast<int>(r * cos(PI * 0.75f));

                    x1 = x + static_cast<int>(r * sin(PI * 1.25f));
                    y1 = y - static_cast<int>(r * cos(PI * 1.25f));

                    SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
                    SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
                    SetPixel(dc.get(), x, y, RGB(0, 0, 0));
                    // ��һ����Բ + һ������
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x1, y1
                        , x0, y0);

                    // ������
                    MoveToEx(dc.get(), x0, y0, NULL);
                    LineTo(dc.get(), x, y);

                    MoveToEx(dc.get(), x1, y1, NULL);
                    LineTo(dc.get(), x, y);
                    break;

                }
                case 4: {
                    // ��һ��Բ
                    Ellipse(dc.get(), x - r, y - r, x + r, y + r);
                    // ��һ������
                    MoveToEx(dc.get(), x - r, y, NULL);
                    LineTo(dc.get(), x, y);
                    break;
                }
                default:
                    break;
            }
        }
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
