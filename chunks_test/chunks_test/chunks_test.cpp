#include "../../utilites/glwnd.h"

INT Width, Height;

enum {
	X=0, Y, Z
};

typedef float vec3_t[3];
typedef vec3_t cube[4];

#define NUM_OF_CUBES 16
struct chunk {
	cube cubes[NUM_OF_CUBES];
};

#define NUM_OF_CHUNKS 16
struct world {
	chunk chunks[NUM_OF_CHUNKS];
};

void init_chunks(world *pworld)
{
	int i, j, k;
	for (i = 0; i < NUM_OF_CHUNKS; i++) {
		for (j = 0; j < NUM_OF_CUBES; j++) {

		}
	}
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

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
}

void fn_mouseclick(int x, int y, int button, int state)
{
}

void fn_charinput(char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		if (key == 27) //esc code
			exit(0); //close program
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
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
		fn_charinput,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0,					  //Window position X
		0,					  //Window position Y
		1280,				  //Window width
		1024);				  //Window height
	return 0;
}