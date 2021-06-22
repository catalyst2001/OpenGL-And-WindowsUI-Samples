﻿#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <time.h>
#include "texture.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "gui.h"
#include "gui_elements.h"

#define WINDOWCLASS "OpenGLUI"
#define WIDTH 800
#define HEIGHT 600
#define DEPTH_BUFFER_BITS 24

HDC hDevCtx;
HGLRC hRndCtx;

bool bSetupPixelFormat(HDC hdc);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

circleprimitive_t circle;

void buttonclick_callback(int id, ui_handle_t *this)
{
	printf("button pressed: %d\n", id);
}

Texture button_texture_test;

bool checkbox_callback(int id, ui_handle_t *handle, bool *pvalue)
{
	if(pvalue)
	printf("checkbox %s\n", *pvalue ? "enabled" : "disabled");
	return true;
}

int main(int argc, char *argv[])
{
	WNDCLASSA wndclass;
	memset(&wndclass, 0, sizeof(WNDCLASS)); // Init the size of the class
	wndclass.style = CS_HREDRAW | CS_VREDRAW; // Regular drawing capabilities
	wndclass.lpfnWndProc = WndProc; // Pass our function pointer as the window procedure
	wndclass.hInstance = GetModuleHandleA(0); // Assign our hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // An arrow for the cursor
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // A white window
	wndclass.lpszClassName = WINDOWCLASS; // Assign the class name
	RegisterClassA(&wndclass); // Register the class

	HWND wnd = CreateWindowExA(0, wndclass.lpszClassName, "OpenGL User Interface",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE,
		0, 0,
		WIDTH, HEIGHT,
		NULL,
		NULL,
		NULL,
		NULL);
	UIEvent_WindowResize(WIDTH, HEIGHT);
	hDevCtx = GetDC(wnd);
	SetForegroundWindow(wnd);
	ShowWindow(wnd, SW_NORMAL);
	ShowCursor(TRUE);
	if (!hDevCtx) {
		MessageBoxA(0, "Error get device context!", "Critical error!", 0);
		return -1;
	}

	if (!bSetupPixelFormat(hDevCtx))
		return -11;

	hRndCtx = wglCreateContext(hDevCtx);
	wglMakeCurrent(hDevCtx, hRndCtx);

	UI_Init(1, 1);
	ui_register_elements();

	if (!LoadTexture("dbutton.tga", &button_texture_test))
		printf("Error loading texture!\n");

	ui_handle_t *helem = ui_create(UI_CANVAS, 1, "Canvas", NULL, NULL, NULL, NULL, NULL, 40, 100, 800, 300, 0);

	//helem = NULL;
	int posy = 60;
	int posx = 45;

	//bool bTest = false;
	//bool bTest2 = false;
	
	ui_create(UI_BUTTON, 1, "Test button", helem, buttonclick_callback, (void*)1, NULL, NULL, posx, posy += 45, 80, 40, BF_DEFAULT);
	ui_create(UI_BUTTON, 2, "Test button", helem, buttonclick_callback, (void*)1, NULL, NULL, posx, posy += 45, 80, 40, BF_DEFAULT);
	ui_create(UI_BUTTON, 3, "Test button", helem, buttonclick_callback, (void*)1, NULL, NULL, posx, posy += 45, 80, 40, BF_DEFAULT);
	ui_handle_t *texbutton = ui_create(UI_BUTTON, 4, "Test button", helem, buttonclick_callback, (void*)1, NULL, NULL, 45, posy += 45, 50, 50, BF_DRAWTEXTURE);
	texbutton->texid = button_texture_test.texID;

	posy = 60;
	posx = 130;
	bool value = false;
	ui_create(UI_CHECKBOX, 1, "Test checkbox", helem, checkbox_callback, &value, NULL, NULL, posx, posy += 45, 20, 20, CF_SWITCH);

	//ui_create(UI_BUTTON, 2, "Test check button", helem, buttonclick_callback, (void*)2, &bTest, NULL, 40, posy += 40 + 2, 200, 40, FL_BTNCHECK);
	//ui_create(UI_BUTTON, 3, "Test check button", helem, buttonclick_callback, (void*)3, &bTest2, NULL, 40, posy += 40 + 2, 200, 40, FL_BTNRADIO);
	//ui_create(UIELEM_BUTTON, "Test button", helem, buttonclick_callback, (void*)1, NULL, NULL, 40, posy += 40 + 2, 200, 40, NULL);
	
	glEnable(GL_TEXTURE_2D);
	while (true) {
		MSG msg;
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			DispatchMessageA(&msg);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		//gluOrtho2D(0, WIDTH, 0, HEIGHT);
		gluOrtho2D(0, input.window_width, input.window_height, 0);
		//glViewport(0, 0, input.window_width, input.window_height);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		UIEvent_Render();


//#define XX 500
//#define YY 500
//#define WW 300
//#define HH 300
//		int verts[] = {
//			XX, YY,
//			XX + WW, YY,
//			XX + WW, YY + HH,
//			XX, YY + HH
//		};
//
//		float tcoords[] = {
//			0.f, 1.f,
//			1.f, 1.f,
//			1.f, 0.f,
//			0.f, 0.f,
//		};
//		glBindTexture(GL_TEXTURE_2D, button_texture_test.texID);
//		glVertexPointer(2, GL_INT, 0, verts);
//		glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
//		glDrawArrays(GL_QUADS, 0, 4);
//		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		SwapBuffers(hDevCtx);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//ShowCursor(TRUE);
		//utl_init_circle(&circle, 50, 100);
		break;

	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		glViewport(0, 0, width, height);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluPerspective(45.0, width / (double)height, -1.0, 1.0);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		UIEvent_WindowResize(width, height);
		break;
	}

	case WM_MOUSEMOVE: {
		//LOWORD - x
		//HIWORD - y
		UIEvent_MouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_LBUTTONDOWN: {
		UINT x = LOWORD(lParam);
		UINT y = HIWORD(lParam);
		UIEvent_MouseClick(x, y, MLEFTBUTTON);
		break;
	}

	case WM_RBUTTONDOWN: {
		UINT x = LOWORD(lParam);
		UINT y = HIWORD(lParam);
		UIEvent_MouseClick(x, y, MRIGHTBUTTON);
		break;
	}

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			exit(0);
			break;
		}
		break;

	case WM_CLOSE:
		exit(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	int pixelformat;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
	pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
	pfd.cColorBits = DEPTH_BUFFER_BITS;						// Here we use our #define for the color bits
	pfd.cDepthBits = DEPTH_BUFFER_BITS;						// Depthbits is ignored for RGBA, but we do it anyway
	pfd.cAccumBits = 0;									// No special bitplanes needed
	pfd.cStencilBits = 0;								// We desire no stencil bits

	// This gets us a pixel format that best matches the one passed in from the device
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE)
	{
		MessageBoxA(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	// This sets the pixel format that we extracted from above
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		MessageBoxA(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;										// Return a success!
}