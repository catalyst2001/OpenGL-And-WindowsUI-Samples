#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include "../../utilites/glwnd.h"
#include "../../utilites/glmath.h"
#include "../../utilites/utmath.h"

#include "camera.h"

#include "SimplexNoise.h"

#include "voxel.h"

INT Width, Height;

bool b_active_camera = true;
bool b_draw_voxels = false;

struct Triangle {
	vec3 p1, p2, p3;
};

CCamera cam(45.f, vec3(5.f, 5.f, 5.f));
CChunk chunk;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(b_active_camera)
		cam.UpdateCameraState();

	vec3 dir = cam.m_vecOrigin + cam.m_vecDirection;
	gluLookAt(cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);

	DrawAxis();

	//CRay ray;
	//ray.SetOrigin(cam.m_vecOrigin);
	//ray.SetDirection(cam.m_vecDirection);

	glEnableClientState(GL_VERTEX_ARRAY);
	chunk.DrawMesh();
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
	static bool b_wire = true;
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
			b_wire = !b_wire;
			glPolygonMode(GL_FRONT_AND_BACK, b_wire ? GL_LINE : GL_FILL);
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

	glPointSize(2);
	chunk.Init(vec3int(0, 0, 0), 32, 250, VOXEL_FLAG_SOLID);
	chunk.DebugDraw_ChunkBounds(true);
	CVoxel *voxels = chunk.GetVoxels();

	//прорыл вертикальную дыру от центра до самого верха
	for (int y = chunk.GetChunkHeight() / 2; y < chunk.GetChunkHeight(); y++)
		for (int x = chunk.GetChunkWidth() / 2; x < chunk.GetChunkWidth() - 5; x++)
			for (int z = chunk.GetChunkWidth() / 2; z < chunk.GetChunkWidth() - 5; z++)
				voxels[COORD2OFFSET2(chunk.GetChunkWidth(), chunk.GetChunkHeight(), x, y, z)].SetFlag(VOXEL_FLAG_AIR);

	//прорыл внизу дыру до обоих граней чанка
	for (int x = 0 / 2; x < chunk.GetChunkWidth() - 5; x++)
		for (int z = 0 / 2; z < chunk.GetChunkWidth() - 5; z++)
			voxels[COORD2OFFSET2(chunk.GetChunkWidth(), chunk.GetChunkHeight(), x, 2, z)].SetFlag(VOXEL_FLAG_AIR);

	//одна дырка внутри чанка
	voxels[COORD2OFFSET2(chunk.GetChunkWidth(), chunk.GetChunkHeight(), 2, 5, 2)].SetFlag(VOXEL_FLAG_AIR);

	chunk.RebuildMesh();
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