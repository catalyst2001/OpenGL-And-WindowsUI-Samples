#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../../utilites/glwnd.h"
#include "../../utilites/glmath.h"
#include "../../utilites/utmath.h"

#include "camera.h"

#include "marching_cubes3.h"
#include "SimplexNoise.h"

INT Width, Height;

bool b_active_camera = true;


struct Triangle {
	vec3 p1, p2, p3;
};

CCamera cam(45.f, vec3(5.f, 5.f, 5.f));
marching_cubes3 mc3;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glBegin(GL_POINTS);
	//for (int x = -10; x < 10; x++)
	//	for (int z = -10; z < 10; z++)
	//		glVertex3f(x, 0, z);
	//glEnd();

	//в треугольнике 3 вертекса
	//узнаем количество треугольников, поделив количество вертексов на 3

	if(b_active_camera)
		cam.UpdateCameraState();

	vec3 dir = cam.m_vecOrigin + cam.m_vecDirection;
	gluLookAt(cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);
	printf("pitch %f   yaw %f    roll %f\n", cam.GetPitch(), cam.GetYaw(), cam.GetRoll());

	//printf("( %f %f %f ) ( %f %f %f ) ( %f %f %f )\n", cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);

	//Draw3DSGrid();

	//CRay ray;
	//ray.SetOrigin(cam.position);
	//ray.SetDirection(cam.direction);
	//for (int i = 0; i < mc3.index_index / 3; i++) {
	//	Triangle *tri = &((Triangle *)mc3.vertices)[i];

	//	vec3 intersection;
	//	if (ray.TriangleIntersection(tri->p1, tri->p2, tri->p3, intersection)) {
	//		glBegin(GL_TRIANGLES);
	//		glVertex3f(tri->p1.x, tri->p1.y, tri->p1.z);
	//		glVertex3f(tri->p2.x, tri->p2.y, tri->p2.z);
	//		glVertex3f(tri->p3.x, tri->p3.y, tri->p3.z);
	//		glEnd();
	//	}
	//}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mc3.vertices);
	glDrawElements(GL_TRIANGLES, mc3.index_index, GL_UNSIGNED_INT, mc3.triangles);
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

	cam.m_iScreenWidth = width;
	cam.m_iScreenHeight = height;
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

		if (key == VK_F1) {
			b_active_camera = !b_active_camera;
			ShowCursor(!b_active_camera);
		}

		if (key == VK_F2) {
			static bool b_wire = false;
			b_wire = !b_wire;
			glPolygonMode(GL_FRONT_AND_BACK, b_wire ? GL_LINE : GL_FILL);
		}
	}

	switch (key) {
	case 'w':
	case 'W':
		cam.m_vecMovement.z = state ? 1 : 0;
		break;
	case 'a':
	case 'A':
		cam.m_vecMovement.x = state ? 1 : 0;
		break;
	case 's':
	case 'S':
		cam.m_vecMovement.z = state ? -1 : 0;
		break;
	case 'd':
	case 'D':
		cam.m_vecMovement.x = state ? -1 : 0;
		break;
	case ' ':
		cam.m_vecMovement.y = state ? 1 : 0;
		break;
	case 'm':
	case 'M':
		cam.m_vecMovement.y = state ? -1 : 0;
		break;
	case 'z':
	case 'Z':
		cam.m_vecMovement.y = state ? 1 : 0;
		break;
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

	glEnable(GL_DEPTH_TEST);

	ShowCursor(!b_active_camera);

	mc3.Start(); //generate map

	//сохраняю все вертексы в файл текстовый (для теста сделал)
	//FILE *fp = fopen("vertlog.txt", "wt");
	//if (fp) {
	//	fprintf(fp, "Verts: %d\n", mc3.verts_index);
	//	fprintf(fp, "Indices: %d\n", mc3.index_index);
	//	fprintf(fp, "Vertex indexes {\n");
	//	for (int i = 0; i < mc3.index_index - 2; i++)
	//		fprintf(fp, "\t%d %d %d\n", mc3.triangles[i], mc3.triangles[i + 1], mc3.triangles[i + 2]);
	//	fprintf(fp, "}\n\n");

	//	fprintf(fp, "Vertices {\n");
	//	for (int i = 0; i < mc3.verts_index; i++)
	//		fprintf(fp, "\t( %f %f %f )\n", mc3.vertices[i].x, mc3.vertices[i].y, mc3.vertices[i].z);
	//	fprintf(fp, "}\n\n");

	//	fprintf(fp, "Triangles {\n");
	//	for (int i = 0; i < mc3.verts_index / 3; i++) { //sizeof(Triangle) == sizeof(Vertex) * 3
	//		Triangle *ptri = &((Triangle *)mc3.vertices)[i];
	//		fprintf(fp, "\t( %f %f %f ) ( %f %f %f ) ( %f %f %f )\n",
	//			ptri->p1.x, ptri->p1.y, ptri->p1.z,
	//			ptri->p2.x, ptri->p2.y, ptri->p2.z,
	//			ptri->p3.x, ptri->p3.y, ptri->p3.z);
	//	}
	//	fprintf(fp, "}\n\n");
	//	fclose(fp);
	//}
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Voxels Test", __FILE__ __TIME__,
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