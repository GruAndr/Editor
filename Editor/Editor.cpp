// Editor.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Editor.h"
#include <vector>
#include <windowsx.h>
#include <string>
#include <codecvt>

#define MAX_LOADSTRING 100


int actionIndex = 0; 
bool isMouseButtonDown = false;
std::vector<int> clickPosition(2);
RECT drawRect;
std::vector<std::unique_ptr<figure>> figures;
HINSTANCE hInst;      
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING]; 


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void addMainWidgets(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EDITOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            addMainWidgets(hWnd);

            
            break;
        }
    case WM_LBUTTONDOWN:
    {

        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam); 


        if (xPos >= drawRect.left + 10 && xPos <= drawRect.right - 10 && yPos >= drawRect.top + 10 && yPos <= drawRect.bottom - 150)
        {
            isMouseButtonDown = true;
            if (actionIndex == 2)
            {
                clickPosition[0] = xPos;
                clickPosition[1] = yPos;
                figures.push_back(std::make_unique<circle>(xPos, yPos, 0, 0));
                InvalidateRect(hWnd, NULL, TRUE);
                

            }
            if (actionIndex == 1)
            {
                clickPosition[0] = xPos;
                clickPosition[1] = yPos;
                figures.push_back(std::make_unique<rectangle>(xPos, yPos, 0, 0));
                InvalidateRect(hWnd, NULL, TRUE);
            }
            if (actionIndex == 3)
            {
                clickPosition[0] = xPos;
                clickPosition[1] = yPos;
                figures.push_back(std::make_unique<triangle>(xPos, yPos, 0, 0));
                InvalidateRect(hWnd, NULL, TRUE);

            }


        }
        break;
       
    }
  
    case WM_MOUSEMOVE:
    {
        if (isMouseButtonDown && figures.size()>0)
        {
            int xPos = GET_X_LPARAM(lParam); 
            int yPos = GET_Y_LPARAM(lParam);
            
            short shiftState = GetKeyState(VK_SHIFT);

            bool shiftPressed = shiftState & 0x8000;
            figures[figures.size() - 1]->setCenterX(int(((xPos - clickPosition[0]) / 2)+ clickPosition[0]));
            figures[figures.size() - 1]->setCenterY(int(((yPos - clickPosition[1]) / 2)+ clickPosition[1]));
            if (shiftPressed == true)
            {
                figures[figures.size() - 1]->setRadX(int((xPos - clickPosition[0]) / 2));
                figures[figures.size() - 1]->setRadY(int((xPos - clickPosition[0]) / 2));


           
            }
            else
            {


                figures[figures.size() - 1]->setRadX(int((xPos - clickPosition[0]) / 2));
                figures[figures.size() - 1]->setRadY(int((yPos - clickPosition[1]) / 2));


            }
            InvalidateRect(hWnd, NULL, TRUE);

        }
        break;
    } 
    case WM_LBUTTONUP:
    {
        isMouseButtonDown = false;

        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case SQUARE_BUTTON:
                actionIndex = 1;

                break;
            case CURSOR_BUTTON:
                actionIndex = 0;
                break;
            case CIRCLE_BUTTON:
                actionIndex = 2;
                break;
            case TRIANGLE_BUTTON:
                actionIndex = 3;
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            GetClientRect(hWnd, &drawRect);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HDC backBufferDC = CreateCompatibleDC(hdc);
            HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
            HBITMAP oldBackBufferBitmap = (HBITMAP)SelectObject(backBufferDC, backBufferBitmap);


            Rectangle(backBufferDC, drawRect.left + 10, drawRect.top + 10, drawRect.right - 10, drawRect.bottom - 150);
            RECT rectangle{ drawRect.left + 10, drawRect.top + 10, drawRect.right - 10, drawRect.bottom - 150 };
            HRGN hRgn = CreateRectRgnIndirect(&rectangle);

            
            SelectClipRgn(hdc,hRgn);

            for (const auto& figure : figures) {
                figure->drawFigure(backBufferDC);
            }

            BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
                backBufferDC, 0, 0, SRCCOPY);

            SelectObject(backBufferDC, oldBackBufferBitmap);
            DeleteObject(backBufferBitmap);
            DeleteDC(backBufferDC);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_ERASEBKGND:
        return TRUE;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void addMainWidgets(HWND hWnd)
{
    CreateWindowA("button", "Square", WS_VISIBLE | WS_CHILD, 50, 400, 50, 50, hWnd, (HMENU)SQUARE_BUTTON, NULL,NULL);
    CreateWindowA("button", "Circle", WS_VISIBLE | WS_CHILD, 100, 400, 50, 50, hWnd, (HMENU)CIRCLE_BUTTON, NULL, NULL);
    CreateWindowA("button", "Triangle", WS_VISIBLE | WS_CHILD, 150, 400, 50, 50, hWnd, (HMENU)TRIANGLE_BUTTON, NULL, NULL);
    CreateWindowA("button", "Cursor", WS_VISIBLE | WS_CHILD, 200, 400, 50, 50, hWnd, (HMENU)CURSOR_BUTTON,NULL, NULL);


}