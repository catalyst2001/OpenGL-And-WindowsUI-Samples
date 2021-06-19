#include "main.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

char szTitle[] = "gui test";
char szWindowClass[] = "gui test";
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

BOOL bclose = FALSE;
HWND hwnd_main;
HWND hwnd_viewport;
HDC viewport_device_context;
HGLRC viewport_rendering_context;
RECT window_rect;

CGui gui;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBJVIEW));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(IDI_SMALL));
	if (!RegisterClassExA(&wcex))
		return EXIT_FAILURE;

	AllocConsole();
	freopen("conout$", "w", stdout);

	DynamicHeap<int> heap;
	for (int i = 0; i < 1000; i++)
		heap.Push(i);

	for (int i = 0; i < heap.Size(); i++) {
		printf("%d\n", heap[i]);
	}

#pragma region WINDOW
	hInst = hInstance;
	hwnd_main = CreateWindowExA(NULL, szWindowClass, wcex.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd_main)
		return EXIT_FAILURE;

	hwnd_viewport = hwnd_main;

	GetClientRect(hwnd_main, &window_rect);
	//hwnd_viewport = CreateWindowExA(WS_EX_CLIENTEDGE, wcex.lpszClassName, "", WS_VISIBLE | WS_CHILD, 1, 1, window_rect.right - 100, window_rect.bottom - 50, hwnd_main, (HMENU)NULL, hInst, (LPVOID)NULL);
	//if (!hwnd_viewport)
	//	return EXIT_FAILURE;

	viewport_device_context = GetDC(hwnd_viewport);
	if (!viewport_device_context)
		return EXIT_FAILURE;

	int pixelformat;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,             // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,             // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	pixelformat = ChoosePixelFormat(viewport_device_context, &pfd);
	if (!pixelformat) {
		return EXIT_FAILURE;
	}

	if (!SetPixelFormat(viewport_device_context, pixelformat, &pfd)) {
		return EXIT_FAILURE;
	}
	
	viewport_rendering_context = wglCreateContext(viewport_device_context);
	if (!viewport_rendering_context)
		return EXIT_FAILURE;

	if (!wglMakeCurrent(viewport_device_context, viewport_rendering_context))
		return EXIT_FAILURE;

	ShowWindow(hwnd_main, SW_SHOWNORMAL);
	UpdateWindow(hwnd_main);
#pragma endregion

	CCanvas ss;
	gui.AddCanvas(&ss, NULL, 1, 50, 50, 500, 500, { 80, 80, 80, 255 }, { 255, 255, 255, 255 });

	MSG msg = {NULL};
	while (!bclose) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
			DispatchMessageA(&msg);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, 0, window_rect.right, window_rect.bottom);

		glColor3ub(255, 255, 255);
		glEnableClientState(GL_VERTEX_ARRAY);
		gui.Render();
		glDisableClientState(GL_VERTEX_ARRAY);

		SwapBuffers(viewport_device_context);
	}
	wglMakeCurrent(viewport_device_context, NULL);
	wglDeleteContext(viewport_rendering_context);
	UnregisterClassA(wcex.lpszClassName, hInst);
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

	case WM_SIZE: {
		INT width = LOWORD(lParam);
		INT height = HIWORD(lParam);
		if (height < 1)
			height = 1;

		//MoveWindow(hwnd_viewport, 1, 1, width - 100, height - 50, TRUE);
		glViewport(0.0, 0.0, (double)width, (double)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, width / (double)height, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_QUIT:
	case WM_CLOSE:
	{
		bclose = true;
		break;
	}

    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}