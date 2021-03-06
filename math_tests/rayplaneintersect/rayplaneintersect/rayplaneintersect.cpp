//#define OLD_CAMERA
//#include "../../../utilites/glwnd.h"
#include <stdio.h>

#define GLUT_BUILDING_LIB
#include "glut.h"
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "utmath.h"
#include "glmath.h"

//#define OLD_CAMERA

#ifdef OLD_CAMERA
#include "../../../utilites/camera.h"
camera_t camera;
#else
#include "CCamera.h"
CCamera2 camera;
#endif

bool mouseshow = true;
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

	int size = 500;

	// Draw a 1x1 grid along the X and Z axis'
	for (float i = -size; i <= size; i++)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

		// Do the horizontal lines (along the X)
		glVertex3f(-size, 0, i);
		glVertex3f(size, 0, i);

		// Do the vertical lines (along the Z)
		glVertex3f(i, 0, -size);
		glVertex3f(i, 0, size);

		// Stop drawing lines
		glEnd();
	}
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//gluPerspective(45.0, Width / (double)Height, 0.1, 1000.0);

	glEnableClientState(GL_VERTEX_ARRAY);

#ifdef OLD_CAMERA
	camera_look(&camera);

	vec3 normorigin = vec3(camera.startX, camera.startY, camera.startZ);
	normorigin = normalize(normorigin);
	ray.SetOrigin(normorigin);

	vec3 normdir = vec3(camera.dirX, camera.dirY, camera.dirZ);
	normdir = normalize(normdir);
	ray.SetDirection(normdir);
	printf("(%f %f %f) (%f %f %f)\n", camera.startX, camera.startY, camera.startZ, /*end.x, end.y, end.z*/camera.dirX, camera.dirY, camera.dirZ);
#else
	if (mouseshow)
		camera.Update();

	camera.Look();
	ray.SetOrigin(camera.m_vecOrigin.x, camera.m_vecOrigin.y, camera.m_vecOrigin.z);
	ray.SetDirection(camera.m_vecDirection.x, camera.m_vecDirection.y, camera.m_vecDirection.z);
	//printf("(%f %f %f) (%f %f %f)\n", camera.m_vecOrigin.x, camera.m_vecOrigin.y, camera.m_vecOrigin.z, camera.m_vecDirection.x, camera.m_vecDirection.y, camera.m_vecDirection.z);
	//printf("p %f   y %f   r %f\n", camera.GetPitch(), camera.GetYaw(), camera.GetRoll());
#endif

	vec3 intersecttion;
	//if (ray.PlaneIntersection2(plane, intersecttion)) {
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
	//glLineWidth(2.0);
	//glColor3ub(255, 255, 255);
	//glBegin(GL_LINES);

	//glVertex3fv(&vec3(ray.m_origin + 1.f));
	//glVertex3fv(&ray.m_direction);
	////printf("m_origin(%f %f %f) m_direction(%f %f %f)\n", ray.m_origin.x, ray.m_origin.y, ray.m_origin.z, ray.m_direction.x, ray.m_direction.y, ray.m_direction.z);
	//glEnd();
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

	glutSwapBuffers();
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

#ifdef OLD_CAMERA
	camera_update_viewport(&camera, width, height);
#else
	camera.UpdateViewport(width, height);
#endif
}

void fn_mousemove(int x, int y)
{
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
}

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void HandleKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;

	case 'P':
	case 'p':
#ifdef OLD_CAMERA
		camera_set_active(&camera, mouseshow);
#endif
		ShowCursor(!mouseshow);
		mouseshow = !mouseshow;
		break;

#ifndef OLD_CAMERA
	case 'A':
	case 'a':
		camera.MoveStrafe(0.1f);
		break;

	case 'D':
	case 'd':
		camera.MoveStrafe(-0.1f);
		break;

	case 'W':
	case 'w':
		camera.MoveForward(0.1f);
		break;

	case 'S':
	case 's':
		camera.MoveForward(-0.1f);
#endif
		break;
	}
}

void HandleIdle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("New camera test");
	//glutFullScreen();
	glutDisplayFunc(fn_draw);
	glutReshapeFunc(fn_window_resize);
	glutReshapeWindow(1920, 1080);
	glutIdleFunc(HandleIdle);
	glutKeyboardFunc(HandleKeyboard);
	glutMotionFunc(fn_mousemove);
	glutPassiveMotionFunc(fn_mousemove);

#ifdef OLD_CAMERA
	camera_update_viewport(&camera, Width, Height);
	camera_init(&camera, CAMERA_START);
	camera_set_active(&camera, true);
#else
	//camera.PositionCamera(0, 0, 0, 0, 0, 1, 0, 1, 0);
#endif

	ray.SetLength(100000.0f);
	//ray.SetOrigin(10, -5, 10);
	//ray.SetDirection(10, 0, 10);
	ray.SetOrigin(10, 0, 10);
	ray.SetDirection(10, -5, 10);

	sphere = gluNewQuadric();
#ifndef OLD_CAMERA
	camera.Init();
#endif

	glutMainLoop();
	return 0;
}