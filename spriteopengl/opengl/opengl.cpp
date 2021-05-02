#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <intrin.h>
#include <math.h>

#include <Windows.h>
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utility
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma warning(disable:4996)

#define WINDOW_CLASS "OpenGLWindow2021"

//
// Global 
//

int cordx = 0, cordy = 0, hueta=0;
float ytex = 1.0f, xtex = 0.0f, ytex2 = 0.0f, offset = 0.0f;
float textureCoordsArray[8] = {
			0.0f, offset,           // x, y + h
			0.0f, 0.0f,           // x, y
			offset, 0.0f,           // x + w, y
			offset, offset            // x + w, y + h
};

const char		g_Title[] = "OpenGL program";	//Заголовок окна
HINSTANCE		g_hInstance;					//
HWND			g_hWnd;							//Дескриптор главного созданного окна
HDC				g_handle_device_context;		//Дескриптор контекста устройства
HGLRC			g_rendering_context;			//Дескриптор контекста визуализации WGL

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

extern UINT g_Texture[100];
UINT g_Texture[100];

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

/*
* AnimashUny'
*/

bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f; // Get the time (milliseconds = seconds * .001)
	float deltaTime = currentTime - lastTime; // Get the slice of time
	float desiredFPS = 1.0f / desiredFrameRate; // Store 1 / desiredFrameRate

	elapsedTime += deltaTime; // Add to the elapsed time
	lastTime = currentTime; // Update lastTime

	// Check if the time since we last checked is greater than our desiredFPS
	if (elapsedTime > desiredFPS)
	{
		elapsedTime -= desiredFPS; // Adjust the elapsed time

		// Return true, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}

/*
* GIF
*/

void renderSprite(int mode, int posx, int posy, int width, int height, unsigned int texid, float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) {
		float textureCoordsArray[] = {
			x, y,           // x, y + h  0 1
			x1, y1,           // x, y 0 0
			x2, y2,           // x + w, y 1 0
			x3, y3            // x + w, y + h 1 1
		};
		int coords[] = {
		posx, posy + height,
		posx, posy,
		posx + width, posy,
		posx + width, posy + height
		};
		/*printf("%f %f %f %f %f %f %f %f\n",
			textureCoordsArray[0], textureCoordsArray[1],
			textureCoordsArray[2], textureCoordsArray[3],
			textureCoordsArray[4], textureCoordsArray[5],
			textureCoordsArray[6], textureCoordsArray[7])*/
		glVertexPointer(2, GL_INT, 0, coords);
		glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray);
		glDrawArrays(mode, 0, 4);
}



void logickSprite(int mode, int posx, int posy, int width, int height, unsigned int texid, unsigned int numbs) {
	int numbs_sqrt = sqrt(numbs);
	offset = 1.0f / numbs_sqrt;
	glBindTexture(GL_TEXTURE_2D, texid);
	
	if (AnimateNextFrame(36)) {
		if (cordx < numbs_sqrt+1) { /*y++)*/
			cordx++;
			textureCoordsArray[0] = xtex;
			textureCoordsArray[2] = xtex;
			xtex += offset;
			textureCoordsArray[4] = xtex;
			textureCoordsArray[6] = xtex;
			hueta++;
		}
		if (hueta == numbs_sqrt + 1) {
			cordy++;
			textureCoordsArray[1] = ytex;
			textureCoordsArray[7] = ytex;
			ytex -= offset;
			textureCoordsArray[3] = ytex;
			textureCoordsArray[5] = ytex;
			textureCoordsArray[0] = 0;
			textureCoordsArray[2] = 0;
			textureCoordsArray[4] = offset;
			textureCoordsArray[6] = offset;
			xtex = 0;
			cordx = 0;
			hueta = 0;
		}
		if (cordy == numbs_sqrt) {
			ytex = 1.0f;
			cordy = 0;
		}
		
	}
	renderSprite(mode, posx, posy, width, height, texid,
		textureCoordsArray[0], textureCoordsArray[1],
		textureCoordsArray[2], textureCoordsArray[3],
		textureCoordsArray[4], textureCoordsArray[5],
		textureCoordsArray[6], textureCoordsArray[7]);
}

/*
* Отрисовка квадратика
*/
void DrawTexturedRect(int mode, int posx, int posy, int width, int height, unsigned int texid)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	int coords[] = {
		posx, posy + height,
		posx, posy,
		posx + width, posy,
		posx + width, posy + height
	};
	float textureCoordsArray[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray);
	glDrawArrays(mode, 0, 4);
}

/*
* CreateTexture
*/

bool CreateTexture(GLuint &textureID, const char * szFileName)                          // Creates Texture From A Bitmap File
{
	HBITMAP hBMP;                                                                 // Handle Of The Bitmap
	BITMAP  bitmap;																  // Bitmap Structure

	glGenTextures(1, &textureID);                                                 // Create The Texture
	hBMP = (HBITMAP)LoadImageA(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP)                                                                    // Does The Bitmap Exist?
		return FALSE;                                                           // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap);                                     // Get The Object
																				  // hBMP:        Handle To Graphics Object
																				  // sizeof(bitmap): Size Of Buffer For Object Information
																				  // &bitmap:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureID);                                      // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	// MUST NOT BE INDEX BMP, BUT RGB
	DeleteObject(hBMP);                                                           // Delete The Object

	return TRUE;                                                                  // Loading Was Successful
}

/**
 *	WinMain
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);
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

	CreateTexture(g_Texture[0], "bitmap.bmp");
	CreateTexture(g_Texture[1], "fire.bmp");
	CreateTexture(g_Texture[2], "vec.bmp");

	MSG msg = { NULL };
	glEnable(GL_TEXTURE_2D);
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
		//glRecti(50, 50, 50 + 50, 50 + 50); //Рисуем квадрат 50x50 пикселей в точке (50, 50)
		/*glBindTexture(GL_TEXTURE_2D, g_Texture[0]);*/
		//int xy[2] = { 300, 600 };
		//int vertexArray[] = {xy[1], xy[2], xy[1], xy[1], xy[2], xy[1], xy[2], xy[2] };
		//float textureCoordsArray[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//DrawTexturedRect(GL_QUADS, 100, 100, 300, 300, g_Texture[0]);
		logickSprite(GL_QUADS, 300, 256, 300, 300, g_Texture[2], 16);
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		//glVertexPointer(2, GL_INT, 0, vertexArray);
		//glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray);
		//glDrawArrays(GL_QUADS, 0, 4);

		//glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 1.0f);
		//glVertex2i(500, 600);
		//glTexCoord2f(0.0f, 0.0f);
		//glVertex2i(500, 500);
		//glTexCoord2f(1.0f, 0.0f);
		//glVertex2i(600, 500);
		//glTexCoord2f(1.0f, 1.0f);
		//glVertex2i(600, 600);
		//glEnd();


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