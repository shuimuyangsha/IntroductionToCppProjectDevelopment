// pacman.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "pacman.h"

using namespace std;

#define WLENTH 700  // 高
#define WHIGHT 740  // 宽
#define STAGE_COUNT 3 // 一共三关
static HWND g_hwnd; // 游戏窗口句柄

#define MAX_LOADSTRING 100


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名



// 此代码模块中包含的函数的前向声明:
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if(!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

    MSG msg;

    // 主消息循环:
    bool bRunning = true;
    while(bRunning) {
        // 获取消息
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        /*// 画点测试
        {
            HDC hdc = ::GetDC(g_hwnd);
            SetPixel(hdc, rand() % WLENTH , rand()%WHIGHT, RGB(rand()%256, rand()%256, rand()%256));
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        /*// 画线测试
        {
            HDC hdc = ::GetDC(g_hwnd);
            // 创建画笔
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
            // 选择画笔
            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
            MoveToEx(hdc, rand() % WLENTH, rand() % WHIGHT, NULL);
            LineTo(hdc, rand() % WLENTH, rand() % WHIGHT);

            // 恢复画笔
            SelectObject(hdc, oldPen);
            ::ReleaseDC(g_hwnd, hdc);
            // 暂停1豪秒,不然画得太快,看清
            Sleep(1);
        }*/

        /*// 画矩型测试
        {
            HDC hdc = ::GetDC(g_hwnd);
            {
                // 创建画笔
                HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
                // 选择画笔
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                // 画矩型(空心)
                Rectangle(hdc, 100, 200, 300, 500);
                // 恢复画笔
                SelectObject(hdc, oldPen);
                DeleteObject(pen);
            }

            {
                // 创建画笔
                HBRUSH bBrush = CreateSolidBrush(RGB(0, 0, 255));
                // 填充 矩型
                RECT rect;
                rect.left = 50;
                rect.top = 270;
                rect.right = 150;
                rect.bottom = 370;
                FillRect(hdc, &rect, bBrush);
                DeleteObject(bBrush);
            }

            ::ReleaseDC(g_hwnd, hdc);
            // 暂停1豪秒,不然画得太快,看清
            Sleep(1);
        }
        */

        /*// 画圆测试
        {
            HDC hdc = ::GetDC(g_hwnd);
            //画圆
            Ellipse(hdc, 200, 150, 300, 250);
            //画椭圆
            Ellipse(hdc, 200, 270, 340, 370);
            //画椭圆
            Ellipse(hdc, 100, 100, 200, 150);
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        /*// 画弧型测试
        {
            HDC hdc = ::GetDC(g_hwnd);
            Arc(hdc, 100, 100, 200, 300  // 矩型 左上点,右下点
                , 150, 200 // 起点
                , 100, 200 // 终点 (与起点逆时针连接)
               );

            Arc(hdc, 0,0, 100, 100
                , 50, 100
                , 50, 0
               );
            ::ReleaseDC(g_hwnd, hdc);
        }*/

        // 综合应用, 画一个大嘴对象
        {
            static DWORD dwTime = GetTickCount();
            // 当距离上绘图的时间大于50豪秒时,才进行本次绘制
            if(GetTickCount() - dwTime >= 50) {
                dwTime = GetTickCount();
            }
            else {
                continue;
            }
            /* 模拟当前的帧
                本对象一共5帧,每一帧画不同的图形
            */
            static int iFrame = 0;
            //++iFrame;
            if(iFrame >= 5) {
                iFrame = 0;
            }

            // 代表对象的中心位置
            int x = 300, y = 300;

            // 对象的半径
            int r = 100;

            // dc 对象句柄
            HDC hdc = ::GetDC(g_hwnd);
            std::shared_ptr<HDC__> dc(::GetDC(g_hwnd), [](HDC hdc) {
                ::ReleaseDC(g_hwnd, hdc);
            });
            // 获取窗口客户区大小
            RECT rc;
            GetClientRect(g_hwnd, &rc);

            // 创建画刷
            std::shared_ptr<HBRUSH__> br(
                ::CreateSolidBrush(RGB(255, 255, 255)),
            [](HBRUSH hbr) {
                ::DeleteObject(hbr);
            });

            // 画背景(清除上一帧所画内容
            FillRect(dc.get(), &rc, br.get());

#define PI (3.1415926f) // 定义 圆周率的值
            switch(iFrame) {
                case 0: {
                    // 画一个圆
                    Ellipse(dc.get(), x - r, y - r, x + r, y + r);
                    // 画一个横线
                    MoveToEx(dc.get(), x - r, y, NULL);
                    LineTo(dc.get(), x, y);
                    break;
                }
                case 1: {
                    // 画嘴(两条线与纵轴偏离 PI/4
                    int x0, y0; // 左上角的点
                    int x1, y1; // 左下角的点
                    x0 = x - static_cast<int>(r * sin(PI * 0.75f));
                    y0 = y + static_cast<int>(r * cos(PI * 0.75f));

                    x1 = x + static_cast<int>(r * sin(PI * 1.25f));
                    y1 = y - static_cast<int>(r * cos(PI * 1.25f));

                    SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
                    SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
                    SetPixel(dc.get(), x, y, RGB(0, 0, 0));
                    // 画一个半圆 + 一条坚线
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x1, y1
                        , x0, y0);

                    // 画坚线
                    MoveToEx(dc.get(), x0, y0, NULL);
                    LineTo(dc.get(), x, y);

                    MoveToEx(dc.get(), x1, y1, NULL);
                    LineTo(dc.get(), x, y);
                    break;

                }
                case 2: {
                    // 画一个半圆 + 一条坚线
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x, y + r
                        , x, y - r
                       );
                    // 画坚线
                    // 圆弧上面的点
                    MoveToEx(dc.get(), x, y - r, NULL);
                    // 圆弧下面的点
                    LineTo(dc.get(), x, y + r);
                    break;
                }
                case 3: {
                    // 画嘴(两条线与纵轴偏离 PI/4
                    int x0, y0; // 左上角的点
                    int x1, y1; // 左下角的点
                    x0 = x - static_cast<int>(r * sin(PI * 0.75f));
                    y0 = y + static_cast<int>(r * cos(PI * 0.75f));

                    x1 = x + static_cast<int>(r * sin(PI * 1.25f));
                    y1 = y - static_cast<int>(r * cos(PI * 1.25f));

                    SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
                    SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
                    SetPixel(dc.get(), x, y, RGB(0, 0, 0));
                    // 画一个半圆 + 一条坚线
                    Arc(dc.get(), x - r, y - r, x + r, y + r
                        , x1, y1
                        , x0, y0);

                    // 画坚线
                    MoveToEx(dc.get(), x0, y0, NULL);
                    LineTo(dc.get(), x, y);

                    MoveToEx(dc.get(), x1, y1, NULL);
                    LineTo(dc.get(), x, y);
                    break;

                }
                case 4: {
                    // 画一个圆
                    Ellipse(dc.get(), x - r, y - r, x + r, y + r);
                    // 画一个横线
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    // 创建窗口
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW
                              , 0 // 在屏幕中的位置 x
                              , 0 // 在屏幕中的位置 y
                              , WLENTH // 宽
                              , WHIGHT // 高
                              , nullptr, nullptr, hInstance, nullptr);

    if(!hWnd) {
        return FALSE;
    }

    // 保存游戏窗口,后面会用到
    g_hwnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
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

// “关于”框的消息处理程序。
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
