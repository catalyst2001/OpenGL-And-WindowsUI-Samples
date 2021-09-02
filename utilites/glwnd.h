//------------------------------------------------------------------------------------
//	OpenGL Window Framework
//	
//	System:		Microsoft Windows Only!
//	Author:		Deryabin K.N & Tynin R
//	Created:	14.04.2021
//	Modified:	30.05.2021
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//	SAMPLE APPLICATION. COPY THIS!!!
//	1) COPY AND PASTE THIS CODE
//	2) UNCOMMENT CODE
//	3) USE :)
//------------------------------------------------------------------------------------
/*
	#include "glwnd.h"

	INT Width, Height;

	void fn_draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

	}

	void fn_window_resize(HWND hWnd, int width, int height)
	{
		if(!height)
			height = 1;

		Width = width;
		Height = height;
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void fn_mousemove(HWND hWnd, int x, int y)
	{
	}

	void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
	{
	}

	void fn_charinput(HWND hWnd, char symbol)
	{
	}

	//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
	void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
	{
		INT key = (INT)wparam;
		if (state == KEY_DOWN) {
			if (key == 27) //esc code
				exit(0); //close program
		}
	}

	//Add this GL functions
	void fn_windowcreate(HWND hWnd)
	{
		RECT rct;
		GetClientRect(hWnd, &rct);
		glViewport(0, 0, (GLsizei)rct.right, (GLsizei)rct.bottom);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, rct.right / (double)rct.bottom, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void fn_windowclose(HWND hWnd)
	{
		exit(0);
	}

	int main()
	{
		create_window("Sample OpenGL Application", __FILE__ __TIME__,
			24,					  //Colors bits
			32,					  //Depth bits
			fn_draw,			  //Draw function
			fn_window_resize,	  //Window resize function
			fn_mousemove,		  //Mouse move function
			fn_mouseclick,		  //Mouse click function
			fn_charinput,		  //Char input function
			fn_keydown,			  //Keydown function
			fn_windowcreate,	  //Window create function
			fn_windowclose,		  //Window close function
			0, 0,
			800,				  //Window width
			600);				  //Window height
		return 0;
	}
*/

// ---------------------------------------------- DON'T EDIT CODE BELLOW! ----------------------------------------------
#pragma once

#ifdef UNICODE
#undef UNICODE
#endif

#include <Windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma warning(disable:4838) //int to byte type conversion

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef void(*DRAWFN)();
typedef void(*RESIZEFN)(HWND window, INT width, INT height);
typedef void(*MOUSEMOVEFN)(HWND window, INT x, INT y);
typedef void(*CHARFN)(HWND window, CHAR sym);
typedef void(*KEYDOWNFN)(HWND window, INT state, WPARAM wParam, LPARAM lParam);
typedef void(*WINDOWCREATE)(HWND wnd);
typedef void(*WINDOWQUIT)(HWND wnd);

#define LBUTTON 0 //left mouse button
#define RBUTTON 1 //right mouse button
#define KEY_DOWN 0 //button dwown
#define KEY_UP 1 //button up
typedef void(*MOUSECLICKFN)(HWND wnd, INT x, INT y, INT button, INT state);

typedef void(*MENUCALLBACK)(HWND wnd, HMENU hMenu, INT id);

struct GLWINDOW {
	HMODULE			hModule;
	HWND			h_window;
	HDC				h_devicecontext;
	HGLRC			h_renderingcontext;
	INT				width;
	INT				height;
	MSG				msg;
	DRAWFN			p_drawfn;
	RESIZEFN		p_resizefn;
	MOUSEMOVEFN		p_mousemovefn;
	MOUSECLICKFN	p_mouseclickfn;
	CHARFN			p_keyboardfn;
	KEYDOWNFN		p_ketdownfn;
	WINDOWCREATE	p_windowcreate;
	WINDOWQUIT		p_windowquit;
	MENUCALLBACK	p_menucallback;
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
	case WM_COMMAND:
		if (global_window_data.p_menucallback)
			global_window_data.p_menucallback(hWnd, (HMENU)lParam, LOWORD(wParam));
		break;
	case WM_SIZE:
	{
		if (global_window_data.p_resizefn) {
			global_window_data.p_resizefn(hWnd, LOWORD(lParam), HIWORD(lParam));
			printf("W: %d  H: %d\n", LOWORD(lParam), HIWORD(lParam));
		}
		break;
	}

	case WM_LBUTTONDOWN:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, KEY_DOWN);
		break;

	case WM_LBUTTONUP:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, KEY_UP);
		break;

	case WM_RBUTTONDOWN:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, KEY_DOWN);
		break;

	case WM_RBUTTONUP:
		if(global_window_data.p_mouseclickfn)
			global_window_data.p_mouseclickfn(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, KEY_UP);
		break;

	case WM_CHAR:
		if (global_window_data.p_keyboardfn)
			global_window_data.p_keyboardfn(hWnd, (char)wParam);
		break;

	case WM_KEYDOWN:
		if (global_window_data.p_ketdownfn)
			global_window_data.p_ketdownfn(hWnd, KEY_DOWN, wParam, lParam);
		break;

	case WM_KEYUP:
		if (global_window_data.p_ketdownfn)
			global_window_data.p_ketdownfn(hWnd, KEY_UP, wParam, lParam);
		break;

	case WM_MOUSEMOVE:
		if (global_window_data.p_mousemovefn)
			global_window_data.p_mousemovefn(hWnd, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		if (global_window_data.p_windowquit)
			global_window_data.p_windowquit(hWnd);
		break;

		//case WM_NCHITTEST:
		//return HTCAPTION;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

GLWINDOW *GetStruct() { return &global_window_data; }

void create_window(const char *p_windowname,
	const char *p_classname,
	int colorbits,
	int depthbits,
	DRAWFN pDrawFn,
	RESIZEFN pResizeFn = NULL,
	MOUSEMOVEFN pMouseMoveFn = NULL,
	MOUSECLICKFN pMouseClickFn = NULL,
	CHARFN pKeyboardFn = NULL,
	KEYDOWNFN pKeydownFn = NULL,
	WINDOWCREATE pWindowCreateFn = NULL,
	WINDOWQUIT pWindowQuitFn = NULL,
	int x = 0, int y = 0,
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
	global_window_data.p_ketdownfn = pKeydownFn;

	int xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
	int ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);
	global_window_data.h_window = CreateWindowExA(NULL, p_classname, p_windowname, /*WS_VISIBLE|WS_POPUP*/WS_OVERLAPPEDWINDOW, xpos, ypos, width, height, NULL, (HMENU)NULL, global_window_data.hModule, NULL);
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

void set_menu_callback(MENUCALLBACK menufn)
{
	global_window_data.p_menucallback = menufn;
}