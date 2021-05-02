#include <stdio.h> 
#include <string.h>
#include <stdlib.h> //malloc, calloc, realloc, free

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma warning(disable:4996)

#include <CommCtrl.h> //common controls
#pragma comment(lib, "comctl32.lib")

#include "Resource.h"

// Глобальные переменные:
char szTitle[] = "Linear interpolation & Record keyframes (2D)";
char szWindowClass[] = "WindowClass";
int i = 0;
UINT g_Texture[100];
float offset;
float textureCoordsArray[8] = {
				0.0f, offset,           // x, y + h
				0.0f, 0.0f,           // x, y
				offset, 0.0f,           // x + w, y
				offset, offset            // x + w, y + h
};

//==================================================================================
//	Types
//==================================================================================
typedef float vec3_t[3];

//==================================================================================
//	Window
//==================================================================================
HINSTANCE	hInst;
HWND		hWnd;
HWND		hViewportWnd;
HDC			hDevCtx;
HGLRC		hRndCtx;
INT			width = 800;
INT			height = 600;
HFONT		hFont;

//==================================================================================
//	Controls
//==================================================================================
HWND		hButtonKeyframesRecord;
HWND		hButtonKeyframesStoprecord;
HWND		hButtonPlay;
HWND		hButtonPause;
HWND		hButtonStop;

HWND		hCheckInterp;
HWND		hButtonSetKeyFrames;
HWND		hTextboxNumKeyFrames;
HWND		hTrackKeyFrames;

HWND		hStaticCurrentFrame;
HWND		hStaticNumberOfFrames;

//==================================================================================
//	Keyframes data
//==================================================================================
INT			 NumberOfFramesRecorded;
INT			 NumberOfTotalFrames;
vec3_t		*pKeyframesData;

enum IDCS {
	IDC_BUTTON_RECORDKEYFRAMES = 1,
	IDC_BUTTON_STOPRECORDKEYFRAMES,
	IDC_BUTTON_PLAY,
	IDC_BUTTON_PAUSE,
	IDC_BUTTON_STOP,
	IDC_BUTTON_SETTOTALKEYFRAMES,
	IDC_EDIT_NUM_KEYFRAMES,
	IDC_TRACK_SWITCH_KEYFRAMES,
	IDC_CHECK_INTERP,
	IDC_TRACK_FRAMES
};


UINT textureID[100];

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

bool CreateTexture(GLuint &textureID, const char *szFileName)                          // Creates Texture From A Bitmap File
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

void renderSprite(int mode, int posx, int posy, int width, int height, unsigned int texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
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

void DrawTexturedRect(int mode, int posx, int posy, int width, int height, unsigned int texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
	int coords[] = {
		posx, posy + height,
		posx, posy,
		posx + width, posy,
		posx + width, posy + height
	};
	float textureCoordsArray2[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray2);
	glDrawArrays(mode, 0, 4);
}

void LogicAnimSprite(int loop, int num) {
	offset = 1.0f / num;
	float cx = float(loop % num) / float(num);
	float cy = float(loop / num) / float(num);
	//x							
	textureCoordsArray[0] = cx;
	textureCoordsArray[2] = cx;
	textureCoordsArray[4] = cx + offset;
	textureCoordsArray[6] = cx + offset;
	//y				  = 
	textureCoordsArray[3] = 1.0f - cy - offset;
	textureCoordsArray[5] = 1.0f - cy - offset;
	textureCoordsArray[1] = 1.0f - cy - 0.001f;
	textureCoordsArray[7] = 1.0f - cy - 0.001f;
}

HWND CreateButton(HWND hParentWindow, const char *buttonname, int id, int posx, int posy, int width = 80, int height = 30)
{
	HWND hButton = CreateWindowExA(NULL, WC_BUTTON, buttonname, WS_VISIBLE | WS_CHILD, posx, posy, width, height, hParentWindow, (HMENU)id, NULL, NULL);
	SendMessageA(hButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	return hButton;
}

HWND CreateCheckbox(HWND hParentWindow, const char *checkname, int id, int posx, int posy, int width, int height)
{
	HWND hButton = CreateWindowExA(NULL, WC_BUTTON, checkname, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, posx, posy, width, height, hParentWindow, (HMENU)id, NULL, NULL);
	SendMessageA(hButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	return hButton;
}

HWND CreateTrackBar(HWND hParentWindow, const char *checkname, int id, int posx, int posy, int width, int height)
{
	HWND hButton = CreateWindowExA(NULL, TRACKBAR_CLASSA, checkname, WS_VISIBLE | WS_CHILD, posx, posy, width, height, hParentWindow, (HMENU)id, NULL, NULL);
	SendMessageA(hButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	return hButton;
}

HWND CreateEditInput(HWND hParentWindow, int id, int posx, int posy, int width, int height)
{
	HWND hButton = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDIT, "", WS_VISIBLE | WS_CHILD, posx, posy, width, height, hParentWindow, (HMENU)id, NULL, NULL);
	SendMessageA(hButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	return hButton;
}

#define ISALPHA(ch) (ch >= 'a' && ch <= 'z' && ch >= 'A' && ch <= 'Z')
#define ISNUMER(ch) (ch >= '0' && ch <= '9')
#define ISSPACE(ch) (ch == ' ' || ch == '\t')
bool GetCorrectTotalFrames(int *pVal, const char *pArg)
{
	for (; *pArg != '\0'; pArg++) {
		if (!ISNUMER(*pArg))
			return false;
	}
	*pVal = atoi(pArg);
	return true;
}

// Отправить объявления функций, включенных в этот модуль кода:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conout$", "w", stdout);

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITEVIEWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEA(IDC_SPRITEVIEWER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(IDI_SMALL));
	if (!RegisterClassExA(&wcex)) {
		MessageBoxA(0, "Cannot register window class!", "Error", MB_OK);
		return 1;
	}

	hInst = hInstance;
	hWnd = CreateWindowExA(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW/* ^ WS_THICKFRAME*/, 0, 0, 800, 700, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd) {
		MessageBoxA(0, "Cannot create window!", "Error", MB_OK);
		return -1;
	}
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	int posy = 10;

	//Record/Play controls
	HWND hText = CreateWindowExA(NULL, WC_STATIC, "Record/Play", WS_VISIBLE | WS_CHILD, 815, posy, 150, 20, hWnd, (HMENU)NULL, NULL, NULL);
	SendMessageA(hText, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	hButtonKeyframesRecord = CreateButton(hWnd, "Record keyframes", IDC_BUTTON_RECORDKEYFRAMES, 815, posy += 30, 150, 30);
	hButtonKeyframesStoprecord = CreateButton(hWnd, "Stop keyframes", IDC_BUTTON_STOPRECORDKEYFRAMES, 815, posy += 33, 150, 30);

	
	hStaticCurrentFrame = CreateWindowExA(NULL, WC_STATIC, "Current frame: 0", WS_VISIBLE | WS_CHILD, 815, posy += 30, 100, 20, hWnd, (HMENU)NULL, NULL, NULL);
	SendMessageA(hStaticCurrentFrame, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	hStaticNumberOfFrames = CreateWindowExA(NULL, WC_STATIC, "Recorded frames: 0", WS_VISIBLE | WS_CHILD, 815, posy += 20, 100, 20, hWnd, (HMENU)NULL, NULL, NULL);
	SendMessageA(hStaticNumberOfFrames, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);

	//Create OpenGL viewport
	hViewportWnd = CreateWindowExA(WS_EX_CLIENTEDGE, "static", "", WS_VISIBLE | WS_CHILD, 1, 1, width, height, hWnd, (HMENU)NULL, hInstance, NULL);
	if (!hViewportWnd) {
		MessageBoxA(0, "Cannot create viewport window!", "Error", MB_OK);
		return -11;
	}

	//Frames controls
	HBITMAP hImage = LoadBitmapA(GetModuleHandleA(0), MAKEINTRESOURCEA(IDB_BITMAP1)); //pause
	//hButtonPlay = CreateButton(hWnd, "play", IDC_BUTTON_PLAY, 815, posy += 33, 30, 30);
	hButtonPlay = CreateWindowExA(NULL, WC_BUTTON, "play", WS_VISIBLE|WS_CHILD|BS_BITMAP, 815, posy += 33, 30, 30, hWnd, (HMENU)IDC_BUTTON_PLAY, NULL, NULL);
	SendMessageA(hButtonPlay, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImage);

	hButtonPause = CreateButton(hWnd, "pause", IDC_BUTTON_PAUSE, 815 + 30, posy, 30, 30);
	hButtonStop = CreateButton(hWnd, "stop", IDC_BUTTON_STOP, 815 + 30 + 30, posy, 30, 30);
	hTrackKeyFrames = CreateWindowExA(NULL, TRACKBAR_CLASSA, "", WS_VISIBLE | WS_CHILD, 0, posy += 500, 800, 40, hWnd, (HMENU)IDC_TRACK_FRAMES, NULL, NULL);
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
	//ShowWindow(hViewportWnd, SW_HIDE);

	MSG msg = { NULL };
	int nums = 8;
	if (!CreateTexture(g_Texture[1], "explode03.bmp")) {
		printf("Not texture");
	}

	// TrackBar
	SendMessageA(hTrackKeyFrames, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, nums*nums));  // min. & max. positions
	SendMessageA(hTrackKeyFrames, TBM_SETPAGESIZE, 0, (LPARAM)1);                 
	SendMessageA(hTrackKeyFrames, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(0, nums*nums));
	SendMessageA(hTrackKeyFrames, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0); //

	glEnable(GL_TEXTURE_2D);
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (i == (nums*nums)) {
			i = 0;
		}
		if (AnimateNextFrame(30)) {
			LogicAnimSprite(i, nums);
			i++;
			SendMessage(hTrackKeyFrames, TBM_SETPOS,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)i);
		}
		renderSprite(GL_QUADS, 100, 100, 300, 300, g_Texture[1]);
		DrawTexturedRect(GL_QUADS, 400, 100, 300, 300, g_Texture[1]);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
		SwapBuffers(hDevCtx);
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRndCtx);
	DeleteDC(hDevCtx);
	UnregisterClassA(szWindowClass, hInstance);
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

	case WM_SIZE: {
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		printf("Window size:  (w: %d  | h: %d)\n", width, height);
		break;
	}

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
