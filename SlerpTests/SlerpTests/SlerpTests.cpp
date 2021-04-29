#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "SlerpTests.h"
#include "Resource.h"

// Глобальные переменные:
char szTitle[] = "Window";
char szWindowClass[] = "WindowClass";

HINSTANCE	hInst;
HWND		hWnd;
HWND		hViewportWnd;
HDC			hDevCtx;
HGLRC		hRndCtx;

// Отправить объявления функций, включенных в этот модуль кода:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SLERPTESTS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEA(IDC_SLERPTESTS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(IDI_SMALL));
	if (!RegisterClassExA(&wcex)) {
		MessageBoxA(0, "Cannot register window class!", "Error", MB_OK);
		return 1;
	}

	hInst = hInstance;
	hWnd = CreateWindowExA(NULL, szWindowClass, wcex.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd) {
		MessageBoxA(0, "Cannot create window!", "Error", MB_OK);
		return -1;
	}
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	hViewportWnd = CreateWindowExA(WS_EX_CLIENTEDGE, "static", "", WS_VISIBLE | WS_CHILD, 1, 1, 800, 600, hWnd, (HMENU)NULL, hInstance, NULL);
	if (!hViewportWnd) {
		MessageBoxA(0, "Cannot create viewport window!", "Error", MB_OK);
		return -11;
	}

	hDevCtx = GetDC(hViewportWnd);
	if (!hDevCtx) {
		MessageBoxA(0, "Cannot get device context!", "Error", MB_OK);
		return -2;
	}

	//https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-choosepixelformat
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	int pixelformat = ChoosePixelFormat(hDevCtx, &pfd);
	if (!pixelformat) {
		MessageBoxA(0, "Cannot choose pixel format!", "Error", MB_OK);
		return -3;
	}

	if (!SetPixelFormat(hDevCtx, pixelformat, &pfd)) {
		MessageBoxA(0, "Cannot set pixel format!", "Error", MB_OK);
		return -4;
	}

	hRndCtx = wglCreateContext(hDevCtx);
	if (!hRndCtx) {
		MessageBoxA(0, "Cannot create rendering context!", "Error", MB_OK);
		return -4;
	}

	if (!wglMakeCurrent(hDevCtx, hRndCtx)) {
		MessageBoxA(0, "Cannot make current rendering context to device context!", "Error", MB_OK);
		return -5;
	}

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glRectf(0.f, 0.f, 1.f, 1.f);

		SwapBuffers(hDevCtx);
	}
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			exit(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
