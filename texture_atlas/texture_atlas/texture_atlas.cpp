#include "../../utilites/glwnd.h"
#include "../../utilites/glutils.h"
#include "../../utilites/texture.h"

enum {
	X = 0, Y
};

INT Width, Height;
HMENU hMenu;
HMENU hMenuFile;
HMENU hMenuZoom;
HWND hProperties;

CHAR szClassName[] = __FILE__ __TIME__;
bool buttons[1024];
bool mousebuttons[5];

BOOL in_process = FALSE;
POINT point_start = {-1, -1}, point_end;
POINT mouse;

BOOL texture_loaded = false;
Texture texture;

#define PADDING 50

void fn_draw()
{
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
		start.y = point_start.y - 100;
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
		end.y = point_end.y - 100;
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
	
		//point_start.x, point_start.y, point_end.x - point_start.x, point_end.y - point_start.y
		//texcoords[0] = start.x / (float)texture.width;
		//texcoords[1] = end.y / (float)texture.height;
		//texcoords[2] = end.x / (float)texture.width;
		//texcoords[3] = end.y / (float)texture.height;
		//texcoords[4] = end.x / (float)texture.width;
		//texcoords[5] = start.y / (float)texture.height;
		//texcoords[6] = start.x / (float)texture.width;
		//texcoords[7] = start.y / (float)texture.height;

		texcoords[6] = start.x / (float)texture.width;
		texcoords[7] = end.y / (float)texture.height;
		texcoords[2] = end.x / (float)texture.width;
		texcoords[3] = end.y / (float)texture.height;
		texcoords[0] = end.x / (float)texture.width;
		texcoords[1] = start.y / (float)texture.height;
		texcoords[4] = start.x / (float)texture.width;
		texcoords[5] = start.y / (float)texture.height;
		draw_textured_rectv(verts, texcoords, texture.texID);
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
			if (point_start.x != -1 || point_start.y != -1) {
				point_start.x = -1;
				point_start.y = -1;
				in_process = FALSE;
				break;
			}

			if(MessageBoxA(0, "Program will be closed", "Continue?", MB_YESNO) != IDNO)
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

#define IDC_ZOOM10 3
#define IDC_ZOOM50 4
#define IDC_ZOOM100 5
#define IDC_ZOOM200 6
#define IDC_ZOOM300 7
#define IDC_ZOOM400 8
#define IDC_ZOOM500 9
#define IDC_ZOOMRESET 10

void fn_windowcreate(HWND hWnd)
{
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
		ofn.Flags = OFN_HIDEREADONLY | /*OFN_OVERWRITEPROMPT | */OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = "Load texture";
		if (!GetSaveFileNameA(&ofn))
			break;

		if (!LoadTexture(szFile, &texture)) {
			MessageBoxA(HWND_DESKTOP, "Unsuported image format", "Error", MB_OK);
		}
		printf("TEXTURE LOADED  BLYAT!  texid: %d  w: %d  h: %d\n", texture.texID, texture.width, texture.height);
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
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	set_menu_callback(fn_menucallback);
	create_window("Texture Atlas Editor", szClassName,
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