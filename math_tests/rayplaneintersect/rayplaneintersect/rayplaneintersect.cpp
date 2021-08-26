﻿#define OLD_CAMERA

#include "../../../utilites/glwnd.h"
#ifdef OLD_CAMERA
#include "../../../utilites/camera.h"
camera_t camera;
#else
#include "../../../utilites/camera/Camera.h"
CCamera camera;
#endif
#include "../../../utilites/utmath.h"
#include "../../../utilites/glmath.h"

bool mouseshow = false;
INT Width, Height;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024

#define PLANE_Y 0.f
#define PLANE_START 0.f
#define PLANE_WIDTH 100.f

#define CAMERA_START 0.f, 10.f, 0.f, PLANE_WIDTH, 0.f, PLANE_WIDTH

CPlane3 plane(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

CTriangle triangle(vec3(0.f, 0.f, 0.f), vec3(0.f, 10.f, 0.f), vec3(10.f, 10.f, 0.f));

CRay ray;

GLUquadric *sphere;

void DrawAxis()
{
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glColor3ub(0, 255, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3ub(0, 0, 255);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();
}

void Draw3DSGrid()
{
	// Turn the lines GREEN
	glColor3ub(0, 255, 0);

	// Draw a 1x1 grid along the X and Z axis'
	for (float i = -100; i <= 100; i += 1)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

		// Do the horizontal lines (along the X)
		glVertex3f(-100, 0, i);
		glVertex3f(100, 0, i);

		// Do the vertical lines (along the Z)
		glVertex3f(i, 0, -100);
		glVertex3f(i, 0, 100);

		// Stop drawing lines
		glEnd();
	}
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluPerspective(45.0, Width / (double)Height, 0.1, 1000.0);

	glEnableClientState(GL_VERTEX_ARRAY);

#ifdef OLD_CAMERA
	camera_look(&camera);
	ray.SetOrigin(camera.startX, camera.startY, camera.startZ);

	//vec3 end = vec3(camera.startX, camera.startY, camera.startZ) + vec3(camera.dirX, camera.dirY, camera.dirZ) * 1000.f;
	ray.SetDirection(camera.dirX, camera.dirY, camera.dirZ);
	//printf("(%f %f %f) (%f %f %f)\n", camera.startX, camera.startY, camera.startZ, /*end.x, end.y, end.z*/camera.dirX, camera.dirY, camera.dirZ);
#else
	if (mouseshow)
		camera.Update();

	camera.Look();
	ray.SetOrigin(camera.m_vPosition.x, camera.m_vPosition.y, camera.m_vPosition.z);
	ray.SetDirection(camera.m_vView.x, camera.m_vView.y, camera.m_vView.z);
	printf("(%f %f %f) (%f %f %f)\n", camera.m_vPosition.x, camera.m_vPosition.y, camera.m_vPosition.z, camera.m_vView.x, camera.m_vView.y, camera.m_vView.z);
#endif

	vec3 intersecttion;
	//if (ray.PlaneIntersection2(plane, intersecttion)) {
	ray.m_direction * 20.f;
	ray.m_direction.y / 12.f;
	if (ray.PlaneIntersection5(plane.m_origin, plane.m_normal, ray.m_origin, ray.m_direction, intersecttion)) {
		//float IntersectionPointRoundFactor = 1.0;
		//round_vector(intersecttion, intersecttion, IntersectionPointRoundFactor);
		glPushAttrib(GL_CURRENT_BIT);

		//printf("pos: %f %f %f\n", intersecttion.x, intersecttion.y, intersecttion.z);
		glColor3ub(0, 255, 0);
		glPushMatrix();
		glTranslatef(intersecttion.x, intersecttion.y, intersecttion.z);
		gluSphere(sphere, 0.1, 5, 5);
		glPopMatrix();
		glPopAttrib();
	}
	else
		glColor3ub(50, 50, 50);

	//glVertexPointer(3, GL_FLOAT, 0, &triangle);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//if (ray.TriangleIntersection(triangle.p1, triangle.p2, triangle.p3, intersecttion)) {
	//	printf("Triangle intersection point ( %f %f %f )\n", intersecttion.x, intersecttion.y, intersecttion.z);
	//	glPushAttrib(GL_CURRENT_BIT);

	//	//printf("pos: %f %f %f\n", intersecttion.x, intersecttion.y, intersecttion.z);
	//	glColor3ub(255, 0, 0);
	//	glPushMatrix();
	//	glTranslatef(intersecttion.x, intersecttion.y, intersecttion.z);
	//	gluSphere(sphere, 0.5, 5, 5);
	//	glPopMatrix();
	//	glPopAttrib();
	//}
	
	//float distance = ray.TriangleIntersection(triangle.p1, triangle.p2, triangle.p3);
	glPushAttrib(GL_CURRENT_BIT);
	DrawAxis();
	Draw3DSGrid();
	glLineWidth(2.0);
	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);

	glVertex3fv(&vec3(ray.m_origin + 1.f));
	glVertex3fv(&ray.m_direction);
	//printf("m_origin(%f %f %f) m_direction(%f %f %f)\n", ray.m_origin.x, ray.m_origin.y, ray.m_origin.z, ray.m_direction.x, ray.m_direction.y, ray.m_direction.z);
	glEnd();
	glPopAttrib();

	//check triangle intersection

	//if (rayIntersectsTriangle((float *)&ray.m_origin, (float *)&ray.m_direction, (float *)&triangle.p1, (float *)&triangle.p2, (float *)&triangle.p3))
	//	glColor3ub(255, 0, 0);
	//else glColor3ub(255, 255, 255);
	////printf("ray distance: %f\n", distance);
	//printf("origin( %f %f %f)  dir( %f %f %f )\n", ray.m_origin.x, ray.m_origin.y, ray.m_origin.z, ray.m_direction.x, ray.m_direction.y, ray.m_direction.z);
	//glVertexPointer(3, GL_FLOAT, 0, &triangle);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

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

#ifdef OLD_CAMERA
	camera_update_viewport(&camera, width, height);
#else
	camera.UpdateScreenResoluiton(width, height);
#endif
}

void fn_mousemove(HWND hWnd, int x, int y)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	Width = rect.right;
	Height = rect.bottom;
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

		switch (key) {
		case VK_F1:
#ifdef OLD_CAMERA
			camera_set_active(&camera, mouseshow);
#endif
			ShowCursor(!mouseshow);
			mouseshow = !mouseshow;
			break;

		case VK_F2:
			ray.m_origin.y += 1.0f;
			ray.m_direction.y += 1.0f;
			break;

		case VK_F3:
			ray.m_origin.y -= 1.0f;
			ray.m_direction.y -= 1.0f;
			break;

		//ray direction control
		case VK_LEFT:
			//ray.m_direction.x += 
			break;

		case VK_RIGHT:
			break;

		case VK_UP:
			ray.m_direction.y += 1.5f;
			break;

		case VK_DOWN:
			ray.m_direction.y -= 1.5f;
			break;


		}
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
#ifdef OLD_CAMERA
	camera_update_viewport(&camera, Width, Height);
	camera_init(&camera, CAMERA_START);
	camera_set_active(&camera, true);
#else
	camera.PositionCamera(0, 0, 0, 0, 0, 1, 0, 1, 0);
#endif

	ray.SetLength(100000.0f);
	//ray.SetOrigin(10, -5, 10);
	//ray.SetDirection(10, 0, 10);
	ray.SetOrigin(10, 0, 10);
	ray.SetDirection(10, -5, 10);

	sphere = gluNewQuadric();
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	//int window_posx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (SCREEN_WIDTH / 2);
	//int window_posy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (SCREEN_HEIGHT / 2);
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
		0,		//Window position X
		0, //Window position Y
		SCREEN_WIDTH, //Window width
		SCREEN_HEIGHT); //Window height
	return 0;
}