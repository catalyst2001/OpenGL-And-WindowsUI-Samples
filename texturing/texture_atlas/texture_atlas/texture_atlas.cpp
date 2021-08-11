#include <Windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../../utilites/glutils.h"
#include "../../utilites/texture.h"
#include "controls.h"
#include "viewport.h"

#pragma comment(lib, "glu32.lib")

enum {
	X = 0, Y
};

INT Width, Height;
HMENU hMenu;
HMENU hMenuFile;
HMENU hMenuZoom;

bool buttons[1024];
bool mousebuttons[5];

BOOL in_process = FALSE;
POINT point_start = {-1, -1}, point_end;
POINT mouse;

BOOL texture_loaded = false;
Texture texture;

#define PADDING 50

viewport_t viewport_main, viewport_preview;

#define IDC_OPENTEXTURE 1
#define IDC_CLOSE 2

#define IDC_ZOOM10 3
#define IDC_ZOOM50 4
#define IDC_ZOOM100 5
#define IDC_ZOOM200 6
#define IDC_ZOOM300 7
#define IDC_ZOOM400 8
#define IDC_ZOOM500 9
#define IDC_ZOOMRESET 10

#define ATLAS_EDITOR_CLASS "AtEdit2021"
HINSTANCE hInstance;
HWND main_window;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {


	case WM_SIZE: {
		INT width = LOWORD(lParam);
		INT height = HIWORD(lParam);
		if (!height)
			height = 1;

		Width = width;
		Height = height;
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}
	

	case WM_COMMAND: {
		switch (wParam)
		{
		case IDC_OPENTEXTURE: {
			char szFile[_MAX_PATH];
			char szFileTitle[_MAX_PATH];
			char szDefExt[32] = "tga";
			const char szFilter[] = "TrueVision TGA (*.tga)\0*.tga\0Bitmap Image (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0\0";
			szFile[0] = '\0';
			szFileTitle[0] = '\0';

			OPENFILENAMEA ofn;
			memset(&ofn, 0, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrDefExt = szDefExt;
			ofn.lpstrFilter = szFilter;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = _MAX_PATH;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = _MAX_PATH;
			ofn.Flags = OFN_HIDEREADONLY | /*OFN_OVERWRITEPROMPT | */OFN_PATHMUSTEXIST;
			ofn.lpstrTitle = "Load texture";
			if (!GetSaveFileNameA(&ofn))
				break;

			if (!LoadTexture(szFile, &texture)) {
				MessageBoxA(HWND_DESKTOP, "Unsuported image format", "Error", MB_OK);
			}
			printf("TEXTURE LOADED!  texid: %d  w: %d  h: %d\n", texture.texID, texture.width, texture.height);
			glEnable(GL_TEXTURE_2D);
			texture_loaded = TRUE;
			break;
		}


		case IDC_CLOSE:
			printf("CLOSE TEXTURE\n");
			if (texture.imageData != NULL && texture.texID != -1) {
				glDeleteTextures(1, &texture.texID);
				memset(&texture, NULL, sizeof(Texture));
				texture_loaded = FALSE;
			}
			glDisable(GL_TEXTURE_2D);
			break;
		}
		break;
	}

	
	case WM_KEYDOWN: {
		switch (wParam)
		{
		//esc
		case 0x1B:
			if (point_start.x != -1 || point_start.y != -1) {
				point_start.x = -1;
				point_start.y = -1;
				in_process = FALSE;
				break;
			}

			if (MessageBoxA(0, "Program will be closed", "Continue?", MB_YESNO) != IDNO)
				exit(0); //close program

			break;


		//enter
		case 0xD:
			break;
		}
		buttons[wParam] = true;
		break;
	}


	case WM_KEYUP: {
		buttons[wParam] = false;
		break;
	}


	case WM_LBUTTONDOWN: {
		mousebuttons[0] = true;
		if (!in_process) {
			point_start.x = GET_X_LPARAM(lParam);
			point_start.y = GET_Y_LPARAM(lParam);
			point_end.x = point_start.x;
			point_end.y = point_start.y;
			in_process = TRUE;
			printf("in_process = true;\n");
		}
		break;
	}


	case WM_RBUTTONDOWN: {
		mousebuttons[1] = false;
		if (in_process) {
			point_start.x = -1;
			point_start.y = -1;
			in_process = FALSE;
		}
		printf("in_process = false;\n");
		in_process = FALSE;
		break;
	}

	
	case WM_MOUSEMOVE: {
		mouse.x = GET_X_LPARAM(lParam);
		mouse.y = GET_Y_LPARAM(lParam);
		if (in_process) {
			point_end.x = mouse.x;
			point_end.y = mouse.y;
			printf("point_end = {%d %d}\n", point_end.x, point_end.y);
		}
		break;
	}

		
	case WM_CLOSE:
	case WM_QUIT:
		exit(0);
		break;

	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

int main()
{
	WNDCLASSEXA wcex;
	memset(&wcex, NULL, sizeof(wcex));
	wcex.cbSize = sizeof WNDCLASSEXA;
	wcex.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW));
	wcex.hInstance = hInstance = GetModuleHandleA(NULL);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = ATLAS_EDITOR_CLASS;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	if (!RegisterClassExA(&wcex)) {
		printf("Cannot register window class!\n");
		return -1;
	}

	main_window = CreateWindowExA(NULL, wcex.lpszClassName, "Texture atlas", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 10, 800, 600, HWND_DESKTOP, (HMENU)NULL, hInstance, (LPVOID)NULL);
	if (!main_window) {
		printf("Cannot create window!\n");
		return -2;
	}
	ShowWindow(main_window, SW_SHOWNORMAL);
	UpdateWindow(main_window);
	
	/**
	*	Initializing menu
	*/
	hMenu = CreateMenu();
	hMenuFile = CreateMenu();
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hMenuFile, "File");
	AppendMenuA(hMenuFile, MF_STRING, IDC_OPENTEXTURE, "Open texture");
	AppendMenuA(hMenuFile, MF_STRING, IDC_CLOSE, "Close");

	hMenuZoom = CreateMenu();
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hMenuZoom, "Zoom");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM10, "10%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM50, "50%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM100, "100%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM200, "200%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM300, "300%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM400, "400%");
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOM500, "500%");
	AppendMenuA(hMenuZoom, MF_SEPARATOR, NULL, NULL);
	AppendMenuA(hMenuZoom, MF_STRING, IDC_ZOOMRESET, "Reset");
	SetMenu(main_window, hMenu);

	create_viewport(&viewport_main, wcex.lpszClassName, main_window, 24, 32, "", NULL, 0, 0, 600, 600);
	create_viewport(&viewport_preview, wcex.lpszClassName, main_window, 24, 32, "", NULL, 650, 10, 100, 100);

	ControlsSetFont((HFONT)GetStockObject(ANSI_VAR_FONT));
	AddButton(main_window, "Test", 100, 10, 10, 80, 30);

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		
		/**
		*	Rendering main viewport
		*/
		{
			wglMakeCurrent(viewport_main.hDevCtx, viewport_main.hRndCtx);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0.0, Width, Height, 0.0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (texture_loaded) {
				int x = 100;
				int y = 100;
				int verts[] = {
					x, y,
					x + texture.width, y,
					x + texture.width, y + texture.height,
					x, y + texture.height,
				};

				float texcoords[] = {
					0.f, 1.f,
					1.f, 1.f,
					1.f, 0.f,
					0.f, 0.f,
				};

				/**
				* draw full image
				*/
				draw_textured_rectv(verts, texcoords, texture.texID);

				/**
				* draw preview image region
				*/

				//compute quad position
				verts[0] = x + texture.width + PADDING;
				verts[1] = y;
				verts[2] = x + texture.width + texture.width + PADDING;
				verts[3] = y;
				verts[4] = verts[2];
				verts[5] = y + texture.height;
				verts[6] = verts[0];
				verts[7] = verts[5];

				//compute texture coordinates

				//x, y,											0.f, 1.f,
				//x + texture.width, y,							1.f, 1.f,
				//x + texture.width, y + texture.height,		1.f, 0.f,
				//x, y + texture.height,						0.f, 0.f,

				POINT start;
				start.x = point_start.x - 100;
				start.y = point_end.y - 100;
				if (start.x < 0)
					start.x = 0;
				if (start.x > texture.width)
					start.x = texture.width;
				if (start.y < 0)
					start.y = 0;
				if (start.y > texture.height)
					start.y = texture.height;

				POINT end;
				end.x = point_end.x - 100;
				end.y = point_start.y - 100;
				if (end.x < 0)
					end.x = 0;
				if (end.x > texture.width)
					end.x = texture.width;
				if (end.y < 0)
					end.y = 0;
				if (end.y > texture.height)
					end.y = texture.height;


				if (start.x != -1 && start.y != -1) {
					glPushAttrib(GL_CURRENT_BIT);
					glColor3ub(0, 255, 0);
					glDisable(GL_TEXTURE_2D);
					draw_rect(GL_LINE_LOOP, start.x + x, start.y + y, (end.x - start.x), (end.y - start.y));
					glEnable(GL_TEXTURE_2D);
					glPopAttrib();
				}
				//printf("1 = %d start %d\n", end.y, start.y);

				end.y = texture.height - end.y;
				start.y = texture.height - start.y;

				//point_start.x, point_start.y, point_end.x - point_start.x, point_end.y - point_start.y
				texcoords[0] = start.x / (float)texture.height;
				texcoords[1] = end.y / (float)texture.height;
				texcoords[2] = end.x / (float)texture.width;
				texcoords[3] = end.y / (float)texture.height;
				texcoords[4] = end.x / (float)texture.width;
				texcoords[5] = start.y / (float)texture.height;
				texcoords[6] = start.x / (float)texture.width;
				texcoords[7] = start.y / (float)texture.height;

				//texcoords[6] = start.x / (float)texture.width;
				//texcoords[7] = end.y / (float)texture.height;
				//texcoords[2] = end.x / (float)texture.width;
				//texcoords[3] = end.y / (float)texture.height;
				//texcoords[0] = end.x / (float)texture.width;
				//texcoords[1] = start.y / (float)texture.height;
				//texcoords[4] = start.x / (float)texture.width;
				//texcoords[5] = start.y / (float)texture.height;
				draw_textured_rectv(verts, texcoords, texture.texID);
			}
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		SwapBuffers(viewport_main.hDevCtx);


		/**
		*	Rendering preview viewport
		*/
		wglMakeCurrent(viewport_preview.hDevCtx, viewport_preview.hRndCtx);
		{
			
		}
		SwapBuffers(viewport_preview.hDevCtx);
	}

	UnregisterClassA(wcex.lpszClassName, wcex.hInstance);
	return 0;
}