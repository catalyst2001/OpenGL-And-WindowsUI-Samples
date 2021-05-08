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

#include "SlerpTests.h"
#include "Resource.h"


//==================================================================================
//	Types
//==================================================================================
typedef float vec3_t[3];
typedef float texturecord[8];

//==================================================================================
// Structur
//==================================================================================

typedef struct												// Create A Structure
{
	GLubyte	*imageData;										// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel.
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
} TextureImage;												// Structure Name


//==================================================================================
// Глобальные переменные:
//==================================================================================
const char text[] = { "Possible Duplicate :/n Converting Char to int in C. I remember learning a long time ago in a course that converting from an ASCII character to an int by subtracting '0' is bad. For example: int converted; char ascii = '8'; converted = ascii - '0'; Why is this considered a bad practice? This is because..." };
char szTitle[] = "Linear interpolation & Record keyframes (2D)";
char szWindowClass[] = "WindowClass";
int i = 0, pxt = 15;
UINT g_Texture[100];
float offset;

texturecord	textureCoordsAnim;
texturecord textureCoords[sizeof(text)];
TextureImage textures[100];

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


UINT textureIDS[100];

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

bool LoadTGA(const char *filename, TextureImage *texture, int iogram)			// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if (file == NULL ||										// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||								// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||								// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp = header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize = texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData = (GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);											// Close The File 

	// Build A Texture From The Data
	glGenTextures(1, &g_Texture[iogram]);					// Generate OpenGL texture ID

	glBindTexture(GL_TEXTURE_2D, g_Texture[iogram]);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

	if (texture[0].bpp == 24)									// Was The TGA 24 Bits
	{
		type = GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return TRUE;											// Texture Building Went Ok, Return True
}

bool CreateTexture(UINT &textureID, const char *szFileName)                          // Creates Texture From A Bitmap File
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

void renderSprite(int mode, int posx, int posy, int width, int height, GLuint texid, int idtextcord) {
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
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoords[idtextcord]);
	glDrawArrays(mode, 0, 4);
}

void renderSpriteAnim(int mode, int posx, int posy, int width, int height, GLuint texid) {
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
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsAnim);
	glDrawArrays(mode, 0, 4);
}

void DrawTexturedRect(int mode, int posx, int posy, int width, int height, GLuint texid) {
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

void LogicAnimSprite(int loop, int num, int idtextcord) {
	offset = (1.0f / num);
	float cx = (float(loop % num) / float(num));
	float cy = (float(loop / num) / float(num));

	//x							
	textureCoords[idtextcord][0] = cx;
	textureCoords[idtextcord][2] = cx;
	textureCoords[idtextcord][4] = cx + offset;
	textureCoords[idtextcord][6] = cx + offset;
	//y		 [idtextcord] = 
	textureCoords[idtextcord][3] = 1.0f - cy - offset;
	textureCoords[idtextcord][5] = 1.0f - cy - offset;
	textureCoords[idtextcord][1] = 1.0f - cy - 0.001f;
	textureCoords[idtextcord][7] = 1.0f - cy - 0.001f;

	
}

void LogicAnimSpriteAnim(int loop, int num) {
	offset = (1.0f / num);
	float cx = (float(loop % num) / float(num));
	float cy = (float(loop / num) / float(num));

	textureCoordsAnim[0] = cx;
	textureCoordsAnim[2] = cx;
	textureCoordsAnim[4] = cx + offset;
	textureCoordsAnim[6] = cx + offset;
	//
	textureCoordsAnim[3] = 1.0f - cy - offset;
	textureCoordsAnim[5] = 1.0f - cy - offset;
	textureCoordsAnim[1] = 1.0f - cy - 0.001f;
	textureCoordsAnim[7] = 1.0f - cy - 0.001f;
}

int TexturUP(const char * szFileName, UINT g_Texture) {
	if (strstr(szFileName, ".tga")) {
		printf("%s \n", szFileName);
		if (LoadTGA(szFileName, &textures[0], 0)) {
			printf("%d", g_Texture);
		}
	}
	else if (strstr(szFileName, ".bmp")) {
		printf("%s \n", szFileName);
		g_Texture = CreateTexture(g_Texture, szFileName);
		printf("%d", g_Texture);
	}
	return 1;
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

	for (int i = 0; i < sizeof(textureCoords) / sizeof(texturecord); i++) {
		textureCoords[i][0] = 0.0f;
		textureCoords[i][1] = offset;
		textureCoords[i][2] = 0.0f;
		textureCoords[i][3] = 0.0f;
		textureCoords[i][4] = offset;
		textureCoords[i][5] = 0.0f;
		textureCoords[i][6] = offset;
		textureCoords[i][7] = offset;
	}

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
	hWnd = CreateWindowExA(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW/* ^ WS_THICKFRAME*/, 0, 0, 818, 661, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd) {
		MessageBoxA(0, "Cannot create window!", "Error", MB_OK);
		return -1;
	}
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	//int posy = 10;
	//int iMin, iMax, iSelMin, iSelMax;

	//Record/Play controls
	//HWND hText = CreateWindowExA(NULL, WC_STATIC, "Record/Play", WS_VISIBLE | WS_CHILD, 815, posy, 150, 20, hWnd, (HMENU)NULL, NULL, NULL);
	//SendMessageA(hText, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	//hButtonKeyframesRecord = CreateButton(hWnd, "Record keyframes", IDC_BUTTON_RECORDKEYFRAMES, 815, posy += 30, 150, 30);
	//hButtonKeyframesStoprecord = CreateButton(hWnd, "Stop keyframes", IDC_BUTTON_STOPRECORDKEYFRAMES, 815, posy += 33, 150, 30);
	//hButtonPlay = CreateButton(hWnd, "|>", IDC_BUTTON_PLAY, 815, posy += 33, 30, 30);
	//hButtonPause = CreateButton(hWnd, "[][]", IDC_BUTTON_PAUSE, 815 + 30, posy, 30, 30);
	//hButtonStop = CreateButton(hWnd, "[_]", IDC_BUTTON_STOP, 815 + 30 + 30, posy, 30, 30);

	////Settings
	//hText = CreateWindowExA(NULL, WC_STATIC, "Keyframes settings", WS_VISIBLE | WS_CHILD, 815, posy += 60, 150, 20, hWnd, (HMENU)NULL, NULL, NULL);
	//SendMessageA(hText, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	//hTextboxNumKeyFrames = CreateEditInput(hWnd, NULL, 815, posy += 20, 50, 20);
	//hText = CreateWindowExA(NULL, WC_STATIC, "Total keyframes", WS_VISIBLE | WS_CHILD, 815 + 54, posy, 100, 20, hWnd, (HMENU)NULL, NULL, NULL);
	//SendMessageA(hText, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	//hButtonSetKeyFrames = CreateButton(hWnd, "Set total keyframes", IDC_BUTTON_STOP, 815, posy += 30, 150, 30);
	//hCheckInterp = CreateCheckbox(hWnd, "Use interploation", IDC_CHECK_INTERP, 815, posy += 30, 150, 30);
	//hStaticCurrentFrame = CreateWindowExA(NULL, WC_STATIC, "Current frame: 0", WS_VISIBLE | WS_CHILD, 815, posy += 30, 100, 20, hWnd, (HMENU)NULL, NULL, NULL);
	//SendMessageA(hStaticCurrentFrame, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	//hStaticNumberOfFrames = CreateWindowExA(NULL, WC_STATIC, "Recorded frames: 0", WS_VISIBLE | WS_CHILD, 815, posy += 20, 100, 20, hWnd, (HMENU)NULL, NULL, NULL);
	//SendMessageA(hStaticNumberOfFrames, WM_SETFONT, (WPARAM)hFont, (LPARAM)NULL);
	//hTrackKeyFrames = CreateWindowExA(NULL, TRACKBAR_CLASSA, "", WS_VISIBLE | WS_CHILD, 0, posy += 310, 800, 40, hWnd, (HMENU)IDC_TRACK_FRAMES, NULL, NULL);

	//Create OpenGL viewport
	hViewportWnd = CreateWindowExA(WS_EX_CLIENTEDGE, "static", "", WS_VISIBLE | WS_CHILD, 1, 1, width, height, hWnd, (HMENU)NULL, hInstance, NULL);
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
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd  
		1,								// version number  
		PFD_DRAW_TO_WINDOW |			// support window  
		PFD_SUPPORT_OPENGL |			// support OpenGL  
		PFD_DOUBLEBUFFER,				// double buffered  
		PFD_TYPE_RGBA,					// RGBA type  
		24,								// 24-bit color depth  
		0, 0, 0, 0, 0, 0,				// color bits ignored  
		0,								// no alpha buffer  
		0,								// shift bit ignored  
		0,								// no accumulation buffer  
		0, 0, 0, 0,						// accum bits ignored  
		32,								// 32-bit z-buffer      
		0,								// no stencil buffer  
		0,								// no auxiliary buffer  
		PFD_MAIN_PLANE,					// main layer  
		0,								// reserved  
		0, 0, 0							// layer masks ignored  
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
	
	int nums = 16, pxx = 0, pxy = 250, v = 0, snos[5] = { -1,-1,-1,-1,-1 }, mix[2] = { 0,0 }, mix2 = 0, stop = -1, space[1024] = { -1 };

	if (LoadTGA(/*g_Texture[0],*/ "font.tga", &textures[0], 0)) {
		printf("font.tga %d\n", g_Texture[0]);
	}
	if (CreateTexture(g_Texture[1], "fire.bmp")) {
		printf("fire.bmp %d\n", g_Texture[1]);
	}

	/*if (TexturUP("fire.bmp", g_Texture[1])) {
		printf("fire %d \n",g_Texture[1]);
	}
	if (TexturUP("font.tga", g_Texture[0])) {
		printf("font %d \n", g_Texture[0]);
	}*/
	for (int l = 0; l < sizeof(text); l++) {
		if (text[l] - 32 != -32 && text[l] - 32 != -22) {
			LogicAnimSprite((text[l] - 32), nums, l);
		}
		else if (text[l] - 32 == -22) {
			snos[mix[1]] = l;
			mix[1]++;
		}
		else if (text[l] - 32 == -32) {
			stop = l;
			printf("%d\n", stop);
		}
	}
	// TrackBar
	//SendMessage(hTrackKeyFrames, TBM_SETRANGE,
	//	(WPARAM)TRUE,									// redraw flag 
	//	(LPARAM)MAKELONG(0, nums*nums));				// min. & max. positions

	//SendMessage(hTrackKeyFrames, TBM_SETPAGESIZE,
	//	0, (LPARAM)1);									// new page size 

	//SendMessage(hTrackKeyFrames, TBM_SETSEL,
	//	(WPARAM)FALSE,									// redraw flag 
	//	(LPARAM)MAKELONG(0, nums*nums));

	//SendMessage(hTrackKeyFrames, TBM_SETPOS,
	//	(WPARAM)TRUE,									// redraw flag 
	//	(LPARAM)0);

	float cx = 0.062500f, cy = 0.0f;
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
			LogicAnimSpriteAnim(i, 6);
			i++;
		}
		//	SendMessage(hTrackKeyFrames, TBM_SETPOS,
		//		(WPARAM)TRUE,                   // redraw flag 
		//		(LPARAM)i);
		//									glColor3f(1.0f, 1.0f, 1.0f);
		DrawTexturedRect(GL_QUADS, 495, 295, 305, 305, -1);
		renderSpriteAnim(GL_QUADS, 500, 300, 300, 300, g_Texture[1]);
		//									glColor3f(0.5f, 0.5f, 1.0f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// В общем создание альфа канала
		glEnable(GL_BLEND);// Активацыя альфа
		
		do {
			for (int d = 0; d < (780 / (pxt - 5)); d++) {
				for (int i = 0; i < 5; i++) {
					if (v == snos[i]) {
						d = (780 / (pxt - 3) - 5) + 1;
					}
				}
				if (v == stop) {
					pxy = -1;
				}

				renderSprite(GL_QUADS, pxx, pxy, pxt, pxt, g_Texture[0], v);

				pxx += pxt;
				pxx-=5;
				v++;
			}
			pxx = 0;
			pxy -= pxt+5;
		}
		while (v <= sizeof(text) || pxy >= 0);
		pxy = 250;
		pxx = 0;
		v = 0;

		//									glColor3f(1.0f, 1.0f, 1.0f);
		//glScissor(1, int(0.135416f*300), 300 - 2, int(0.597916f*600));			// Define Scissor Region
		//glEnable(GL_SCISSOR_TEST);												// Enable Scissor Testing

		glDisable(GL_BLEND);//Дизоктивацыя альфа

		DrawTexturedRect(GL_QUADS, 0, 295, 305, 305, -1);
		DrawTexturedRect(GL_QUADS, 0, 300, 300, 300, g_Texture[0]);

		
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
