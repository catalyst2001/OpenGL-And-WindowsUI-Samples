#include "../../../utilites/glwnd.h"
#include "../../../utilites/camera.h"
#include "../../../utilites/utmath.h"

INT Width, Height;
camera_t camera;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024

#define PLANE_Y 0.f
#define PLANE_START 0.f
#define PLANE_WIDTH 100.f

#define CAMERA_START 0.f, 10.f, 0.f, PLANE_WIDTH, 0.f, PLANE_WIDTH

CRect3 plane;

CTriangle triangle(CVector3(0.f, 0.f, 0.f), CVector3(0.f, 1.f, 0.f), CVector3(1.f, 1.f, 0.f));

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluPerspective(45.0, Width / (double)Height, 0.1, 1000.0);

	camera_look(&camera);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &plane);
	glDrawArrays(GL_LINE_LOOP, 0, 4);


	//check triangle intersection
	CRay ray(camera.startX, camera.startY, camera.startZ, camera.endX, camera.endY, camera.endZ);
	//float distance = ray.TriangleIntersection(triangle.p1, triangle.p2, triangle.p3);

	ray.m_direction.x *= 100;
	ray.m_direction.y *= 100;
	ray.m_direction.z *= 100;

	glVertexPointer(3, GL_FLOAT, 0, &ray);
	glDrawArrays(GL_LINES, 0, 2);


	if (rayIntersectsTriangle((float *)&ray.m_origin, (float *)&ray.m_direction, (float *)&triangle.p1, (float *)&triangle.p2, (float *)&triangle.p3))
		glColor3ub(255, 0, 0);
	else glColor3ub(255, 255, 255);
	//printf("ray distance: %f\n", distance);
	printf("origin( %f %f %f)  dir( %f %f %f )\n", ray.m_origin.x, ray.m_origin.y, ray.m_origin.z, ray.m_direction.x, ray.m_direction.y, ray.m_direction.z);

	glVertexPointer(3, GL_FLOAT, 0, &triangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableClientState(GL_VERTEX_ARRAY);
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

	camera_update_viewport(&camera, width, height);
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
	static bool mouseshow = false;
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		if (key == 27) //esc code
			exit(0); //close program

		switch (key) {
		case VK_F1:
			camera_set_active(&camera, mouseshow);
			ShowCursor(!mouseshow);
			mouseshow = !mouseshow;
			break;
		}
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	camera_update_viewport(&camera, Width, Height);
	camera_init(&camera, CAMERA_START);
	camera_set_active(&camera, true);
	plane.InitByStartPoint({ 0.f, PLANE_Y, 0.f }, PLANE_WIDTH);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("ray plane intersect test", __FILE__ __TIME__,
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
		200,					 //Window position X
		200,					 //Window position Y
		SCREEN_WIDTH,				  //Window width
		SCREEN_HEIGHT);				  //Window height
	return 0;
}