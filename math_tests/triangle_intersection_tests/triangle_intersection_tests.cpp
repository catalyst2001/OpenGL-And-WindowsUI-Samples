#include "../../utilites/glwnd.h"
#include "../../utilites/camera_quat/camera.h"
#include "../../utilites/utmath.h"
#include "../../utilites/camera_utils.h"
#include "../../utilites/shapes_utils.h"

INT Width, Height;

CShapes shape;
CCamera camera(45.f, vec3(0.f, 0.f, 0.f));
vec3 triangle[3] = {
	vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f), vec3(1.f, 0.f, 0.f)
};

vec3 TriangleNearPoint1(vec3 &p0, vec3 &p1, vec3 &p2, vec3 &point)
{

	float d0 = distance(p0, point);
	float d1 = distance(p1, point);
	float d2 = distance(p2, point);
	return vec3();
}

vec3 TriangleNearPoint2(vec3 &p0, vec3 &p1, vec3 &p2, vec3 &point)
{
	vec3 verts[] = { p0 , p1 , p2 };
	float dist[3];
	dist[0] = distance(p0, point);
	dist[1] = distance(p1, point);
	dist[2] = distance(p2, point);
	if (dist[0] < dist[1]) {
		if (dist[0] < dist[2]) {
			return verts[0];
		}
		else {
			return verts[2];
		}
	}
	else if (dist[1] < dist[2]) {
		return verts[1];
	}
	else {
		return verts[2];
	}
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	CameraUtils_DrawCrosshair(Width, Height);

	if(camera.IsActive())
		camera.UpdateCameraState();

	CameraUtils_LookAt(camera);

	//Draw3DSGrid();
	DrawAxis();
	glColor3ub(255, 255, 255);

	vec3 intersect;
	CRay ray(camera.m_vecOrigin, camera.m_vecDirection);
	if (ray.TriangleIntersection(triangle[0], triangle[1], triangle[2], intersect)) {
		glColor3ub(255, 0, 0);
		shape.DrawSphere(intersect, 0.01f);
		vec3 pnear = TriangleNearPoint2(triangle[0], triangle[1], triangle[2], intersect);
		shape.DrawSphere(pnear, 0.01f);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &triangle);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
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
	camera.UpdateViewport(width, height);
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
	CameraUtils_Move(camera, key, state, 0.01f);
	if (state == KEY_DOWN) {
		CameraUtils_CheckActive(camera, key, VK_F1);
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

	camera.UpdateViewport(rct.right, rct.bottom);
	CameraUtils_CheckActive(camera, NULL, NULL); //params NULL, NULL == camera active, cursor hide
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Выбор ближайшей точки на грани треугольнике по точке пересечения", __FILE__ __TIME__,
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
		0, 0,
		1280,				  //Window width
		1024);				  //Window height
	return 0;
}