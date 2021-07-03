#include <stdio.h>
#include "../../../utilites/glwnd.h"
#include "../../../utilites/camera.h"
#include "../../../utilites/texture.h"

INT Width, Height;
camera_t camera;


#define CHUNK_SIZE 40

float cube[] = {
	0.f, 0.f, 0.f,
	0.f, 0.f, 1.f,
	0.f, 1.f, 1.f,
	0.f, 1.f, 0.f,

	1.f, 0.f, 0.f,
	1.f, 0.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 1.f, 0.f,

	0.f, 0.f, 1.f,
	0.f, 1.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 0.f, 1.f,

	0.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 0.f, 0.f,

	0.f, 0.f, 0.f,
	0.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 0.f,

	0.f, 1.f, 0.f,
	0.f, 1.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 1.f, 0.f
};

int indexes[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23
};

//float texcoords[] = {
//
//};

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, Width / (double)Height, 0.1, 1000.0);

	camera_look(&camera);

	//glPointSize(2);
	glBegin(GL_POINTS);
	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				glVertex3f((float)x, (float)y, (float)z);
			}
		}
	}
	glEnd();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cube);
	//glDrawArrays(GL_QUADS, 0, 24);
	glDrawElements(GL_QUADS, (sizeof(cube) / sizeof(float)) / 3, GL_UNSIGNED_INT, indexes);
	glDisableClientState(GL_VERTEX_ARRAY);


	glEnd();
}

void fn_window_resize(HWND window, int width, int height)
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
	
	camera_update_viewport(&camera, width, height);
}

void fn_mousemove(HWND window, int x, int y)
{
}

void fn_mouseclick(HWND window, int x, int y, int button, int state)
{
}

void fn_charinput(HWND window, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND window, INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		if (key == 27) //esc code
			exit(0); //close program

		switch (key) {
		case VK_F1:
			static bool bpolygon = true;
			glPolygonMode(GL_FRONT_AND_BACK, bpolygon ? GL_LINE : GL_FILL);
			bpolygon = !bpolygon;
			break;
		}
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	camera_init(&camera, 0.f, 0.f, 5.f, 0.f, 0.f, 0.f);
	camera_set_active(&camera, true);
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
		1680,				  //Window width
		1050);				  //Window height
	return 0;
}