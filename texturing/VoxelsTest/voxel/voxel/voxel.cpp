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

struct vector3 {
	float x, y, z;
};

void Get3Dpos(int x, int y, vector3* pp) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble object_x, object_y, object_z;
	int mouse_x = x;
	int mouse_y = y;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)mouse_x;
	winY = (float)viewport[3] - (float)mouse_y;
	glReadBuffer(GL_BACK);
	glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);
	pp->x = object_x;
	pp->y = object_y;
	pp->z = object_z;
}

void MouseToWorld(int x, int y, float *ox, float *oy, float *oz)
{
	int vp[4];
	double modelMatrix[16];
	double projectMatrix[16];
	glGetIntegerv(GL_VIEWPORT, vp);
	y = vp[3] - y - 1;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectMatrix);

	float z;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	gluUnProject(x, y, z, modelMatrix, projectMatrix, vp, (double*)ox, (double*)oy, (double*)oz);
}

vector3 cubepos;
vector3 start;

void GetPos3(int x, int y)
{
	double matModelView[16], matProjection[16];
	int viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	double winX = (double)x;
	double winY = viewport[3] - (double)y;
	gluUnProject(winX, winY, 0.0, matModelView, matProjection, viewport, (double*)&start.x, (double*)&start.y, (double*)&start.z);
	gluUnProject(winX, winY, 1.0, matModelView, matProjection, viewport, (double*)&cubepos.x, (double*)&cubepos.y, (double*)&cubepos.z);
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, Width / (double)Height, 0.1, 1000.0);

	camera_look(&camera);

	glColor3ub(20, 20, 20);
	glBegin(GL_POLYGON);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(CHUNK_SIZE, 0.f, 0.f);
	glVertex3f(CHUNK_SIZE, 0.f, CHUNK_SIZE);
	glVertex3f(0.f, 0.f, CHUNK_SIZE);
	glEnd();

	//glPointSize(2);
	glColor3ub(200, 200, 200);
	glBegin(GL_POINTS);
	//for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				glVertex3f((float)x, /*(float)y*/0.f, (float)z);
			}
		}
	//}
	glEnd();

	//glBegin(GL_LINES);
	//glVertex3f(start.x, start.y, start.z);
	//glVertex3f(cubepos.x, cubepos.y, cubepos.z);
	//glEnd();

	glTranslatef(cubepos.x, cubepos.y, cubepos.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cube);
	glDrawArrays(GL_QUADS, 0, 24);
	//glDrawElements(GL_QUADS, (sizeof(cube) / sizeof(float)) / 3, GL_UNSIGNED_INT, indexes);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	if (state == KEY_DOWN) {
		//Get3Dpos(x, y, &cubepos);
		MouseToWorld(x, y, &cubepos.x, &cubepos.y, &cubepos.z);
		cubepos.x = roundf(cubepos.x);
		cubepos.y = roundf(cubepos.y);
		cubepos.z = roundf(cubepos.z);
		printf("cubepos: %f %f %f    mypos: %f %f %f\n", cubepos.x, cubepos.y, cubepos.z, camera.startX, camera.startY, camera.startZ);
	}

	//GetPos3(x, y);
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
	glEnable(GL_DEPTH_TEST);
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