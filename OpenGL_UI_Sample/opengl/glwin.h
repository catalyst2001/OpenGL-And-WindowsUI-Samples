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

#define LBUTTON 0 //left mouse button
#define RBUTTON 1 //right mouse button
#define STATE_PRESSED 0 //button dwown
#define STATE_RELEASED 1 //button up
typedef void(*MOUSECLICKFN)(int x, int y, int button, int state);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GLWINDOW *glwindow = (GLWINDOW *)GetWindowLongA(hWnd, GWL_USERDATA);
	switch (message)
	{
	case WM_SIZE:
	{
		if (glwindow->p_resizefn)
			glwindow->p_resizefn(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_LBUTTONDOWN:
		if(glwindow->p_mouseclickfn)
			glwindow->p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, STATE_PRESSED);
		break;

	case WM_LBUTTONUP:
		if(glwindow->p_mouseclickfn)
			glwindow->p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), LBUTTON, STATE_RELEASED);
		break;

	case WM_RBUTTONDOWN:
		if(glwindow->p_mouseclickfn)
			glwindow->p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, STATE_PRESSED);
		break;

	case WM_RBUTTONUP:
		if(glwindow->p_mouseclickfn)
			glwindow->p_mouseclickfn(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), RBUTTON, STATE_RELEASED);
		break;

	case WM_CHAR:
		if (glwindow->p_keyboardfn)
			glwindow->p_keyboardfn((char)wParam);
		break;

	//case WM_CLOSE:
	//case WM_QUIT:
	//case WM_DESTROY:
	//	PostQuitMessage(0);
	//	exit(0);
	//	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void error(const char *message, ...)
{
	char buffer[1024];
	va_list arg;
	va_start(arg, message);
	_vsprintf_p(buffer, sizeof(buffer), message, arg);
	va_end(arg);
	MessageBoxA(HWND_DESKTOP, buffer, "Critical error!", MB_OK | MB_ICONERROR);
}

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
};

#define CREATE_WINDOW(name, width, height, drawfunc, resizefunc, mousemovefunc, mouseclickfunc, keyfunc) \
do { \
	GLWINDOW gw; \
	create_window(&gw, name, "glwindow2021", 24, 32, drawfunc, resizefunc, mousemovefunc, mouseclickfunc, keyfunc, 0, 0, width, height);\
} while (0.0);

void create_window(GLWINDOW *p_window,
	const char *p_windowname,
	const char *p_classname,
	int colorbits,
	int depthbits,
	DRAWFN pDrawFn,
	RESIZEFN pResizeFn = NULL,
	MOUSEMOVEFN pMouseMoveFn = NULL,
	MOUSECLICKFN pMouseClickFn = NULL,
	KEYBOARDFN pKeyboardFn = NULL,
	int posx = 0,
	int posy = 0,
	int width = 800,
	int height = 600)
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
	p_window->hModule = wcex.hInstance;
	p_window->width = width;
	p_window->height = height;
	p_window->p_drawfn = pDrawFn;
	p_window->p_mousemovefn = pMouseMoveFn;
	p_window->p_mouseclickfn = pMouseClickFn;
	p_window->p_resizefn = pResizeFn;
	p_window->p_keyboardfn = pKeyboardFn;
	p_window->h_window = CreateWindowExA(NULL, p_classname, p_windowname, WS_VISIBLE | WS_OVERLAPPED, posx, posy, width, height, NULL, (HMENU)NULL, p_window->hModule, NULL);
	if (!p_window->h_window) {
		error("Error create window!\nGetLastError() = 0x%x", GetLastError());
		exit(-2);
	}
	ShowWindow(p_window->h_window, SW_SHOWNORMAL);
	UpdateWindow(p_window->h_window);

	p_window->h_devicecontext = GetDC(p_window->h_window); //Получаем контекст устройства WinGDI
	if (!p_window->h_devicecontext) {
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
	int iPixelFormat = ChoosePixelFormat(p_window->h_devicecontext, &pfd); //Узнаем формат пикселей совместимый с контекстом устройства
	if (!iPixelFormat) {
		error("Error in ChoosePixelFormat!\nGetLastError() = 0x%x", GetLastError());
		exit(-5);
	}
	SetPixelFormat(p_window->h_devicecontext, iPixelFormat, &pfd); //Устанавливаем совместимый с контекстом устройства формат пикселей

	p_window->h_renderingcontext = wglCreateContext(p_window->h_devicecontext); //Создаем контекст визуализации OpenGL
	if (!p_window->h_renderingcontext) {
		error("Error create rendering context!\nGetLastError() = 0x%x", GetLastError());
		exit(-4);
	}

	//Устанавливаем текущим 
	if (!wglMakeCurrent(p_window->h_devicecontext, p_window->h_renderingcontext)) {
		error("Failed make current device context to rendering context!\nGetLastError() = 0x%x", GetLastError());
		exit(-6);
	}
	SetWindowLongA(p_window->h_window, GWL_USERDATA, (LONG)p_window);

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		p_window->p_drawfn();
		SwapBuffers(p_window->h_devicecontext);
	}
}

