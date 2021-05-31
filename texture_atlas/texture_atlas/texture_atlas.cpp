#include "../../utilites/glwnd.h"
#include "../../utilites/glutils.h"
#include "../../utilites/texture.h"

INT Width, Height;
HMENU hMenu;
HMENU hMenuFile;
HMENU hMenuSettings;
HWND hProperties;

CHAR szClassName[] = __FILE__ __TIME__;
bool buttons[1024];
bool mousebuttons[5];

BOOL in_process = FALSE;
POINT point_start = {-1, -1}, point_end;
POINT mouse;

BOOL texture_loaded = false;
Texture texture;
INT textureid = -1;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, Width, Height, 0.0);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (texture_loaded) {
		draw_textured_rect(50, 50, texture.width, texture.height, )
	}

	if (point_start.x != -1 && point_start.y != -1) {
		glColor3ub(0, 255, 0);
		draw_rect(GL_LINE_LOOP, point_start.x, point_start.y, point_end.x - point_start.x, point_end.y - point_start.y);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
}

void fn_window_resize(int width, int height)
{
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
}

void fn_mousemove(int x, int y)
{
	mouse.x = x;
	mouse.y = y;
	if (in_process) {
		point_end.x = x;
		point_end.y = y;
		printf("point_end = {%d %d}\n", x, y);
	}
}

void fn_mouseclick(int x, int y, int button, int state)
{
	if (button == LBUTTON) {
		mousebuttons[LBUTTON] = !state;
		if (state == KEY_DOWN) {
			if (!in_process) {
				point_start.x = x;
				point_start.y = y;
				point_end.x = x;
				point_end.y = y;
				in_process = TRUE;
				printf("in_process = true;\n");
			}
			else
			{
				printf("in_process = false;\n");
				in_process = FALSE;
			}
		}
	}

	if (button == RBUTTON) {
		mousebuttons[RBUTTON] = !state;
		if (in_process) {
			point_start.x = -1;
			point_start.y = -1;
			in_process = FALSE;
		}
	}
}

void fn_charinput(char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		switch (key)
		{

		//esc
		case 0x1B:
			if (in_process) {
				point_start.x = -1;
				point_start.y = -1;
				in_process = FALSE;
				break;
			}
			exit(0); //close program
			break;

		//enter
		case 0xD:

			break;


		}

		buttons[key] = true;
	}
	else
	{
		buttons[key] = false;
	}
}

#define IDC_OPENTEXTURE 1
#define IDC_CLOSE 2
void fn_windowcreate(HWND hWnd)
{
	hMenu = CreateMenu();
	hMenuFile = CreateMenu();
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hMenuFile, "File");
	AppendMenuA(hMenuFile, MF_STRING, IDC_OPENTEXTURE, "Open texture");
	AppendMenuA(hMenuFile, MF_STRING, IDC_CLOSE, "Close");

	hMenuSettings = CreateMenu();
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hMenuSettings, "Settings");
	SetMenu(hWnd, hMenu);

	//hProperties = CreateWindowExA(0, szClassName, "properties", WS_VISIBLE, 50, 50, 400, 400, hWnd, (HMENU)NULL, GetModuleHandleA(0), NULL);
}

void fn_menucallback(HMENU hMenu, INT id)
{
	switch (id)
	{
	case IDC_OPENTEXTURE: {
		char szFile[_MAX_PATH];
		char szFileTitle[_MAX_PATH];
		char szDefExt[32] = "tga";
		const char szFilter[] = "TrueVision TGA (*.tga)\0*.tga\0Bitmap Image (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0\0";
		szFile[0] = '\0';
		szFileTitle[0] = '\0';

		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrDefExt = szDefExt;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = _MAX_PATH;
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = "Load texture";
		if (!GetSaveFileNameA(&ofn))
			break;

		if (!LoadTexture(szFile, &texture)) {
			MessageBoxA(HWND_DESKTOP, "Unsuported image format", "Error", MB_OK);
		}
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
		break;
	}
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	set_menu_callback(fn_menucallback);
	create_window("Sample OpenGL Application", szClassName,
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
		0,					  //Window position X
		0,					  //Window position Y
		800,				  //Window width
		600);				  //Window height
	return 0;
}