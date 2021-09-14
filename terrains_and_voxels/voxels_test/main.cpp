#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../../utilites/glwnd.h"
#include "../../utilites/glmath.h"
#include "../../utilites/utmath.h"

#include "camera.h"

#include "SimplexNoise.h"

#include "voxel.h"
#include "marchingcubes.h"
#include <vector>

INT Width, Height;

bool b_active_camera = true;
bool b_draw_voxels = false;

struct Triangle {
	vec3 p1, p2, p3;
};

CCamera cam(45.f, vec3(5.f, 5.f, 5.f));

//#define MC3

#ifdef MC3
#include "marching_cubes3.h"
marching_cubes3 mc3;
#else
//#define USE_INTERP
#include "marching_cubes4.h"
marching_cubes4 mc4;
#endif

chunk chnk;
//ChunkMeshRenderer chunk_renderer;
vec3 cubepos;


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
	//printf("pitch %f   yaw %f    roll %f\n", cam.GetPitch(), cam.GetYaw(), cam.GetRoll());

	//printf("( %f %f %f ) ( %f %f %f ) ( %f %f %f )\n", cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);

	//Draw3DSGrid();

	CRay ray;
	ray.SetOrigin(cam.m_vecOrigin);
	ray.SetDirection(cam.m_vecDirection);
	//for (int i = 0; i < mc3.index_index / 3; i++) {
	//	Triangle *tri = &((Triangle *)mc3.vertices)[i];

	//	vec3 intersection;
	//	if (ray.TriangleIntersection(tri->p1, tri->p2, tri->p3, intersection)) {
	//		glPushAttrib(GL_CURRENT_BIT);
	//		glColor3ub(0, 255, 0);
	//		glBegin(GL_TRIANGLES);
	//		glVertex3f(tri->p1.x, tri->p1.y, tri->p1.z);
	//		glVertex3f(tri->p2.x, tri->p2.y, tri->p2.z);
	//		glVertex3f(tri->p3.x, tri->p3.y, tri->p3.z);
	//		glEnd();
	//		glPopAttrib();
	//	}
	//}

	glBegin(GL_POINTS);
	glPushAttrib(GL_CURRENT_BIT);
	glVertex3f(0, 0, 0);
	if (b_draw_voxels) {
		for (int x = 0; x < chnk.chunk_width; x++) {
			for (int z = 0; z < chnk.chunk_width; z++) {
				for (int y = 0; y < chnk.chunk_height; y++) {
					char flag = chnk.voxels[COORD2OFFSET(&chnk, x, y, z)].flags;
					if (flag & VOXEL_FLAG_AIR)
						glColor3ub(20, 20, 20);
					else if (flag & VOXEL_FLAG_SOLID)
						glColor3ub(0, 100, 0);
					else if (flag & VOXEL_FLAG_LIQUID)
						glColor3ub(0, 0, 255);

					glVertex3f(x, y, z);
				}
			}
		}
	}
	glEnd();
	glPopAttrib();
	

	glEnableClientState(GL_VERTEX_ARRAY);
#ifdef MC3
	glVertexPointer(3, GL_FLOAT, 0, mc3.vertices);
	glDrawElements(GL_TRIANGLES, mc3.index_index, GL_UNSIGNED_INT, mc3.triangles);
#else
	mc4.DrawCubeCorners(cubepos);
	mc4.Draw();
#endif
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

void RebuildMesh()
{
	mc4.Clear();
	for (int y = 0; y < chnk.chunk_height; y++)
		for (int x = 0; x < chnk.chunk_width - 1; x++)
			for (int z = 0; z < chnk.chunk_width - 1; z++) {
					mc4.MarchCube(vec3(x, y, z));
			}
					
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

		//rebuild mesh
		case VK_F4:
			RebuildMesh();
			break;

		//cube pos
		case VK_LEFT:
			cubepos.z--;
			break;

		case VK_RIGHT:
			cubepos.z++;
			break;

		case VK_UP:
			cubepos.x++;
			break;

		case VK_DOWN:
			cubepos.x--;
			break;

		case VK_F10:
			cubepos.y++;
			break;

		case VK_F11:
			cubepos.y--;
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
	//chunk_renderer.Init();

	glPointSize(2);
	if (chunk_alloc_voxels(&chnk, 16, 100) != CHUNK_OK) {
		printf("Error allocate memory");
		assert(0);
	}
	chnk.pos = vec3(0.f, 0.f, 0.f);
	memset(chnk.voxels, VOXEL_FLAG_SOLID, CHUNK_SIZE(chnk.chunk_width, chnk.chunk_height));

#pragma region FILL_VOXELS
	//for (int x = 0; x < 4; x++)
	//	for (int y = 0; y < 4; y++)
	//		for (int z = 0; z < 4; z++) {
	//			int offset = COORD2OFFSET(&chnk, x, y, z);
	//			//printf("%d %d %d = %d\n", x, y, z, offset);
	//			chnk.voxels[offset].flags = VOXEL_FLAG_SOLID;
	//		}

	for (int y = chnk.chunk_height / 2; y < chnk.chunk_height; y++)
		for (int x = chnk.chunk_width / 2; x < chnk.chunk_width - 5; x++)
			for (int z = chnk.chunk_width / 2; z < chnk.chunk_width - 5; z++)
				chnk.voxels[COORD2OFFSET(&chnk, x, y, z)].flags = VOXEL_FLAG_AIR;

	//for (int y = chnk.chunk_height / 2; y < (chnk.chunk_height / 2) + 2; y++)
	//	for (int x = chnk.chunk_width / 2; x > 0; x--)
	//			chnk.voxels[COORD2OFFSET(&chnk, x, y, chnk.chunk_width / 2)].flags = VOXEL_FLAG_AIR;

		for (int x = 0 / 2; x < chnk.chunk_width - 5; x++)
			for (int z = 0 / 2; z < chnk.chunk_width - 5; z++)
				chnk.voxels[COORD2OFFSET(&chnk, x, 2, z)].flags = VOXEL_FLAG_AIR;

#ifdef MC3
	mc3.Start(); //generate map
#else
	mc4.vecMin(0.f, 0.f, 0.f);
	mc4.vecMax(chnk.chunk_width, chnk.chunk_height, chnk.chunk_width);
	RebuildMesh();

	cubepos(0.f, 0.f, 0.f);

#endif
	//chunk_renderer.BuildMesh(chnk, 0.5f);
#pragma endregion

#pragma region vertsprint
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
#pragma endregion
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