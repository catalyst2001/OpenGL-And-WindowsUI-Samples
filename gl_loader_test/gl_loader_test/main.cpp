#include "../../utilites/glwnd.h"
#include <vector>

INT Width, Height;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

}

void fn_window_resize(HWND hWnd, int width, int height)
{
	if(!height)
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

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
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
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("Exts:\n%s\n", glGetString(GL_EXTENSIONS));

	printf("\n\n\n\n\n\n");
	int num_of_exts = 0;

#define GL_NUM_EXTENSIONS 0x821D
	glGetIntegerv(GL_NUM_EXTENSIONS, &num_of_exts);
	printf("Num of extensions: %d\n", num_of_exts);


}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Test OpenGL Loader", __FILE__ __TIME__,
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
