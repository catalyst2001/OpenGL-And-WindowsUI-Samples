#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef void(*DRAWFN)();
typedef void(*RESIZEFN)(int width, int height);
typedef void(*MOUSEMOVEFN)(int x, int y);
typedef void(*KEYBOARDFN)(char sym);
typedef void(*WINDOWCREATE)(HWND wnd);
typedef void(*WINDOWQUIT)(HWND wnd);

#define LBUTTON 0 //left mouse button
#define RBUTTON 1 //right mouse button
#define STATE_PRESSED 0 //button dwown
#define STATE_RELEASED 1 //button up
typedef void(*MOUSECLICKFN)(int x, int y, int button, int state);

struct GLWINDOW {
	HMODULE hModule;
	HWND	h_window;
	HDC		h_devicecontext;
	HGLRC	h_renderingcontext;
	INT		width;
	INT		height;
	MSG		msg;
	DRAWFN			p_drawfn;
	RESIZEFN		p_resizefn;
	MOUSEMOVEFN		p_mousemovefn;
	MOUSECLICKFN	p_mouseclickfn;
	KEYBOARDFN		p_keyboardfn;
	WINDOWCREATE	p_windowcreate;
	WINDOWQUIT		p_windowquit;
} global_window_data;

void error(const char *message, ...)
{
	char buffer[1024];
	va_list arg;
	va_start(arg, message);
	_vsprintf_p(buffer, sizeof(buffer), message, arg);
	va_end(arg);
	MessageBoxA(HWND_DESKTOP, buffer, "Critical error!", MB_OK | MB_ICONERROR);
}

//#define CREATE_WINDOW(name, width, height, drawfunc, resizefunc, mousemovefunc, mouseclickfunc, keyfunc, windowcreatefunc) \
//	create_window(name, "glwindow2021", 24, 32, drawfunc, resizefunc, mousemovefunc, mouseclickfunc, keyfunc, windowcreatefunc, 0, 0, width, height);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	//case WM_CREATE:
	//	if (global_window_data.p_windowcreate)
	//		global_window_data.p_windowcreate(hWnd);
	//	break;

	case WM_SIZE:
	{
		if (global_window_data.p_resizefn)
			global_window_data.p_resizefn(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_LBUTTONDOWN:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, STATE_PRESSED);
		break;

	case WM_LBUTTONUP:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, STATE_RELEASED);
		break;

	case WM_RBUTTONDOWN:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, STATE_PRESSED);
		break;

	case WM_RBUTTONUP:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, STATE_RELEASED);
		break;

	case WM_CHAR:
		if (global_window_data.p_keyboardfn)
			global_window_data.p_keyboardfn((char)wParam);
		break;

	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		if (global_window_data.p_windowquit)
			global_window_data.p_windowquit(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void create_window(const char *p_windowname,
	const char *p_classname,
	int colorbits,
	int depthbits,
	DRAWFN pDrawFn,
	RESIZEFN pResizeFn = NULL,
	MOUSEMOVEFN pMouseMoveFn = NULL,
	MOUSECLICKFN pMouseClickFn = NULL,
	KEYBOARDFN pKeyboardFn = NULL,
	WINDOWCREATE pWindowCreateFn = NULL,
	WINDOWQUIT pWindowQuitFn = NULL,
	int posx = 0,
	int posy = 0,
	int width = 1280,
	int height = 1024)
{
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandleA(NULL);
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = p_classname;
	wcex.hIconSm = NULL;
	if (!RegisterClassExA(&wcex)) {
		error("Error registering window class!\nGetLastError() = 0x%x", GetLastError());
		exit(-1);
	}
	global_window_data.hModule = wcex.hInstance;
	global_window_data.width = width;
	global_window_data.height = height;
	global_window_data.p_drawfn = pDrawFn;
	global_window_data.p_mousemovefn = pMouseMoveFn;
	global_window_data.p_mouseclickfn = pMouseClickFn;
	global_window_data.p_resizefn = pResizeFn;
	global_window_data.p_keyboardfn = pKeyboardFn;
	global_window_data.p_windowcreate = pWindowCreateFn;
	global_window_data.p_windowquit = pWindowQuitFn;
	global_window_data.h_window = CreateWindowExA(NULL, p_classname, p_windowname, WS_OVERLAPPEDWINDOW, posx, posy, width, height, NULL, (HMENU)NULL, global_window_data.hModule, NULL);
	if (!global_window_data.h_window) {
		error("Error create window!\nGetLastError() = 0x%x", GetLastError());
		exit(-2);
	}
	ShowWindow(global_window_data.h_window, SW_SHOWNORMAL);
	UpdateWindow(global_window_data.h_window);
	global_window_data.h_devicecontext = GetDC(global_window_data.h_window); //Получаем контекст устройства WinGDI
	if (!global_window_data.h_devicecontext) {
		error("Error get device context!\nGetLastError() = 0x%x", GetLastError());
		exit(-3);
	}

	//Структура формата пикселей
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		colorbits,             // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		depthbits,             // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	int iPixelFormat = ChoosePixelFormat(global_window_data.h_devicecontext, &pfd); //Узнаем формат пикселей совместимый с контекстом устройства
	if (!iPixelFormat) {
		error("Error in ChoosePixelFormat!\nGetLastError() = 0x%x", GetLastError());
		exit(-5);
	}
	SetPixelFormat(global_window_data.h_devicecontext, iPixelFormat, &pfd); //Устанавливаем совместимый с контекстом устройства формат пикселей

	global_window_data.h_renderingcontext = wglCreateContext(global_window_data.h_devicecontext); //Создаем контекст визуализации OpenGL
	if (!global_window_data.h_renderingcontext) {
		error("Error create rendering context!\nGetLastError() = 0x%x", GetLastError());
		exit(-4);
	}

	//Устанавливаем текущим 
	if (!wglMakeCurrent(global_window_data.h_devicecontext, global_window_data.h_renderingcontext)) {
		error("Failed make current device context to rendering context!\nGetLastError() = 0x%x", GetLastError());
		exit(-6);
	}

	if (global_window_data.p_windowcreate)
		global_window_data.p_windowcreate(global_window_data.h_window);
	
	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		global_window_data.p_drawfn();
		SwapBuffers(global_window_data.h_devicecontext);
	}
}

