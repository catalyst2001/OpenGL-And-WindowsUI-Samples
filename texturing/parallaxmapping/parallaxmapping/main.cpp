#include "../../../utilites/glwnd.h"
#include "../../../utilites/texture.h"
#include "glextsloader.h"

INT Width, Height;

Texture diffuse;
Texture height;
Texture normal;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


}

void fn_window_resize(HWND hWnd, int width, int height)
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

void fn_mousemove(HWND hWnd, int x, int y)
{
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		switch (key) {
		case 27:
			exit(0); //close program
			break;
		}
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	RECT rct;
	GetClientRect(hWnd, &rct);
	glViewport(0, 0, (GLsizei)rct.right, (GLsizei)rct.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, rct.right / (double)rct.bottom, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!LoadGLExtensions()) {
		printf("Failed to load gl extensions!\n");
		exit(-1);
	}

	printf("glMultiDrawArrays: 0x%x\n", glMultiDrawArrays);

	if (!LoadTexture("rockwall.tga", &diffuse))
		printf("Failed load texture rockwall.tga\n");
	
	if (!LoadTexture("rockwall_height.tga", &height))
		printf("Failed load texture rockwall_height.tga\n");

	if (!LoadTexture("rockwall_normal.tga", &normal))
		printf("Failed load texture rockwall_normal.tga\n");
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Sample OpenGL Application", __FILE__ __TIME__,
		24,					  //Colors bits
		32,					  //Depth bits
		fn_draw,			  //Draw function
		fn_window_resize,	  //Window resize function
		fn_mousemove,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		NULL,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0, 0,
#ifdef ROMA
		800, //Window width
		600 //Window height
#else
		1280,
		1024
#endif
	);				 
	return 0;
}