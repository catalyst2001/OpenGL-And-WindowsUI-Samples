#pragma warning(disable:4996)
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <vector>
#include "../../utilites/glwnd.h"
#include "../../utilites/texture.h"
#include "../../utilites/glmath.h"
#include "../../utilites/utmath.h"
#include "camera.h"
#include "SimplexNoise.h"
#include "voxel.h"
#include "CChunksManager.h"

GLUquadric *quadric;
INT Width, Height;
extern Texture textures[15];
Texture textures[15];

CChunksManager chunksmgr;

float g_LighPos[] = { 40.f, 40.f, -20.0f, 1.0f };       // This is the position of the 'lamp' joint in the test mesh in object-local space
float g_LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
float g_LightDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
float g_LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float g_LighAttenuation0 = 1.0f;
float g_LighAttenuation1 = 0;
float g_LighAttenuation2 = 0;

bool b_active_camera = true;
bool b_draw_voxels = false;
bool b_wire = true;
int voxvis_mode = 0;

struct Triangle {
	vec3 p1, p2, p3;
};

CCamera cam(45.f, vec3(5.f, 5.f, 5.f));
CRay ray;

double prev_time;

void DrawCrosshair()
{
	//прицел -->
	glDisable(GL_DEPTH);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, Width, 0, Height);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2i(Width >> 1, Height >> 1);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPointSize(1);
	glEnable(GL_DEPTH);
	//<---
}

double TimeGetSeconds()
{
	LARGE_INTEGER frequency, time;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&time);
	return time.QuadPart / (double)frequency.QuadPart;
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	DrawCrosshair();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if(b_active_camera)
		cam.UpdateCameraState();

	vec3 dir = cam.m_vecOrigin + cam.m_vecDirection;
	gluLookAt(cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);

	DrawAxis();

	ray.SetOrigin(cam.m_vecOrigin);
	ray.SetDirection(cam.m_vecDirection);

	glLightfv(GL_LIGHT0, GL_POSITION, g_LighPos);
	glTranslatef(g_LighPos[0], g_LighPos[1], g_LighPos[2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 0.0f);
	gluSphere(quadric, 0.3f, 8, 8);
	glEnable(GL_LIGHTING);
	glTranslatef(-g_LighPos[0], -g_LighPos[1], -g_LighPos[2]);

	for (int i = 0; i < chunksmgr.m_nNumberOfChunks; i++)
		chunksmgr.m_pChunks[i].DrawMesh();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	//prev_time = current_time;
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
	prev_time = TimeGetSeconds();
	glEnable(GL_TEXTURE_2D);
	LoadTexture("green.bmp", &textures[1]);

	quadric = gluNewQuadric();
	glPointSize(2);

	//LIGHT
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_LightSpecular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, g_LighAttenuation0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, g_LighAttenuation1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, g_LighAttenuation2);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#define CHUNK_WIDTH 20
	chunksmgr.Init(CHUNK_WIDTH, CHUNK_WIDTH, 5, 5);

	#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Renderer: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//DWORD WINAPI Kopat(LPVOID param)
//{
//	chunk.RebuildMesh();
//	printf("Thread is dead\n");
//	return 0;
//}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	if (state == KEY_DOWN) {
		if (button == LBUTTON) {
			//if (chunksmgr.GetVoxelByRay(ray, 10.f, VOXEL_FLAG_AIR)) {
			//	//CreateThread(0, 0, Kopat, 0, 0, 0);
			//}
			chunksmgr.RemoveSolidVoxel(ray, 100.f);
			return;
		}

		if (button == RBUTTON) {
			//if (chunk.PlaceVoxel(ray, 10.f, VOXEL_FLAG_SOLID)) {
			//	//CreateThread(0, 0, Kopat, 0, 0, 0);
			//}
			chunksmgr.PlaceSolidVoxel(ray, 100.f);
			return;
		}
	}
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

		case VK_F1:
			b_active_camera = !b_active_camera;
			ShowCursor(!b_active_camera);
			break;

		case VK_F2:
			glPolygonMode(GL_FRONT_AND_BACK, b_wire ? GL_LINE : GL_FILL);
			b_wire = !b_wire;
			break;

		case VK_F3:
			b_draw_voxels = !b_draw_voxels;
			break;
		}
	}

	switch (key) {
	case 'w':
	case 'W':
		cam.m_vecMovement.z = state ? 0.1f : 0.f;
		break;
	case 'a':
	case 'A':
		cam.m_vecMovement.x = state ? 0.1f : 0.f;
		break;
	case 's':
	case 'S':
		cam.m_vecMovement.z = state ? -0.1f : 0.f;
		break;
	case 'd':
	case 'D':
		cam.m_vecMovement.x = state ? -0.1f : 0.f;
		break;
	case ' ':
		cam.m_vecMovement.y = state ? 0.1f : 0.f;
		break;
	case 'm':
	case 'M':
		cam.m_vecMovement.y = state ? -0.1f : 0.f;
		break;
	case 'z':
	case 'Z':
		cam.m_vecMovement.y = state ? 0.1f : 0.f;
		break;
	}
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
		NULL,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		NULL,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0, 0,
		1280,				  //Window width
		1024);				  //Window height
	return 0;
}