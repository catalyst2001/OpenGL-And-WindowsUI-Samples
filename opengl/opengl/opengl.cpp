#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <Windows.h>
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utility
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define WINDOW_CLASS "OpenGLWindow2021"

const char		g_Title[] = "OpenGL program";	//Заголовок окна
HINSTANCE		g_hInstance;					//
HWND			g_hWnd;							//Дескриптор главного созданного окна
HDC				g_handle_device_context;		//Дескриптор контекста устройства
HGLRC			g_rendering_context;			//Дескриптор контекста визуализации WGL

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 *	Вывод ошибки
 */
void error(const char *message, ...)
{
	char buffer[1024];
	va_list arg;
	va_start(arg, message);
	_vsprintf_p(buffer, sizeof(buffer), message, arg);
	va_end(arg);
	MessageBoxA(HWND_DESKTOP, buffer, "Critical error!", MB_OK | MB_ICONERROR);
}

/**
 *	WinMain
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = NULL;

	//Регистрируем класс окна
	if (!RegisterClassExA(&wcex)) {
		error("Error registering window class!\nGetLastError() = 0x%x", GetLastError());
		return -1;
	}

	//Создаем окно
	g_hWnd = CreateWindowExA(0, wcex.lpszClassName, g_Title, WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, NULL, hInstance, NULL);
	if (!g_hWnd) {
		error("Error create window!\nGetLastError() = 0x%x", GetLastError());
		return -2;
	}
	ShowWindow(g_hWnd, SW_SHOWNORMAL); //Отображаем окно в нормальном его виде
	UpdateWindow(g_hWnd); //Обновляем окно

	g_handle_device_context = GetDC(g_hWnd); //Получаем контекст устройства WinGDI
	if (!g_handle_device_context) {
		error("Error get device context!\nGetLastError() = 0x%x", GetLastError());
		return -3;
	}

	//Структура формата пикселей
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
	int iPixelFormat = ChoosePixelFormat(g_handle_device_context, &pfd); //Узнаем формат пикселей совместимый с контекстом устройства
	if (!iPixelFormat) {
		error("Error in ChoosePixelFormat!\nGetLastError() = 0x%x", GetLastError());
		return -5;
	}
	SetPixelFormat(g_handle_device_context, iPixelFormat, &pfd); //Устанавливаем совместимый с контекстом устройства формат пикселей

	g_rendering_context = wglCreateContext(g_handle_device_context); //Создаем контекст визуализации OpenGL
	if (!g_rendering_context) {
		error("Error create rendering context!\nGetLastError() = 0x%x", GetLastError());
		return -4;
	}

	//Устанавливаем текущим 
	if (!wglMakeCurrent(g_handle_device_context, g_rendering_context)) {
		error("Failed make current device context to rendering context!\nGetLastError() = 0x%x", GetLastError());
		return -6;
	}

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		//Если есть сообщения в очереди, обрабатываем сообщения и удаляем их из очереди
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Очищаем буфер цвета и буфер глубины
		glLoadIdentity(); //Выполняем сброс матриц
		glMatrixMode(GL_PROJECTION); //Выбираем матрицу проекции
		glPushMatrix(); //Записываем в стек состояние матрицы проекции
		glLoadIdentity(); //Выполняем сброс матрицы проекции
		glOrtho(0, 1024, 0, 768, -1, 1); //Формируем ортографическую проекцию (2D - вид)
		glRecti(50, 50, 50 + 50, 50 + 50); //Рисуем квадрат 50x50 пикселей в точке (50, 50)
		glPopMatrix(); //Восстанавливаем предыдущее состояние матрицы проекции (которое мы заранее сохранили с помощью glPushMatrix)
		SwapBuffers(g_handle_device_context); //Обмен буферов
	}
    return (int) msg.wParam;
}

/**
 *	Обработчик сообщений окна
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    //case WM_COMMAND:
    //    {
    //        //int wmId = LOWORD(wParam);
    //    }
    //    break;

    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;

	case WM_SIZE:
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluPerspective(45.0, w / (GLdouble)h, 0.1, 1000.0);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		break;
	}

	case WM_CLOSE:
	case WM_QUIT:
    case WM_DESTROY:
        PostQuitMessage(0);
		exit(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}