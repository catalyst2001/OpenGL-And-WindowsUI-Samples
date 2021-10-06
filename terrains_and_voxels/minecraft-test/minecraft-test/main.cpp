#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "../../../utilites/glwnd.h"
#include "../../../utilites/glmath.h"
#include "../../../utilites/camera_utils.h"
#include "../../../utilites/utmath.h"
#include "../../../utilites/camera_quat/camera.h"
#include "../../../utilites/shapes_utils.h"

#include <vector>
using namespace std;

#define COORD2OFFSET(w, h, x, y, z) ((x * w + z) * h + y)

#define BLOCK_HALF 0.5f

CShapes *shapes = NULL;

struct voxel_t {
	bool is_solid;
	CAABB bbox;
};

struct chunk_t {
	int width, height;
	voxel_t *pvoxels;

	vector<vec3> vertices;
	vec3 min;
	vec3 max;
};

INT Width, Height;
chunk_t chunk;
CCamera camera(45, vec3(0, 5, 0));
CRay ray;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera.UpdateCameraState();
	CameraUtils_LookAt(camera);

	ray.SetOrigin(camera.m_vecOrigin);
	ray.SetDirection(normalize(camera.m_vecDirection));


	//float distance = 60.f;
	//vec3 curr = ray.m_origin + normalize(ray.m_direction) * distance;
	//shapes->DrawSphere(curr);

	float d = 0.f;
	while (d < 100.f) {
		vec3 curr = ray.m_origin + normalize(ray.m_direction) * d;
		round_vector(curr, curr, 1.0);
		int offs = COORD2OFFSET(chunk.width, chunk.height, curr.x, curr.y, curr.z);
		if (offs < 0 || offs >= (chunk.width*chunk.width*chunk.height)) {
			d++;
			continue;
		}

		if (chunk.pvoxels[offs].is_solid) {
			shapes->DrawSphere(curr, 0.01);
			break;
		}
		d++;
	}


	DrawAxis();
	CameraUtils_DrawCrosshair(Width, Height);
	glPointSize(2.f);

	glPushAttrib(GL_CURRENT_BIT);

	//draw chunk bounds
	//glColor3ub(255, 0, 0);
	//DrawBBox(chunk.min, chunk.max);

	voxel_t *pvox = NULL;
	
	for (int y = 0; y < chunk.height; y++) {
		for (int x = 0; x < chunk.width; x++) {
			for (int z = 0; z < chunk.width; z++) {

				//draw voxel
				//glColor3ub(0, 0, 0);
				//glBegin(GL_POINTS);
				//glVertex3f(x, y, z);
				//glEnd();

				//draw blocks
				pvox = &chunk.pvoxels[COORD2OFFSET(chunk.width, chunk.height, x, y, z)];
				if (pvox->is_solid) {
					glColor3ub(0, 0, 255);
					pvox->bbox.Draw();
				}
			}
		}
	}
	glPopAttrib();
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

	camera.UpdateViewport(Width, Height);
}

void fn_mousemove(HWND hWnd, int x, int y)
{
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	if (state == KEY_DOWN) {
		if (button == LBUTTON) {
			printf("LBUTTON\n");

			//destroy block
			//voxel_t *pvox = NULL;
			//float tmin, tmax;
			//for (int x = 0; x < chunk.width; x++) {
			//	for (int y = 0; y < chunk.height; y++) {
			//		for (int z = 0; z < chunk.width; z++) {
			//			pvox = &chunk.pvoxels[COORD2OFFSET(chunk.width, chunk.height, x, y, z)];
			//			//printf("( %f %f %f ) ( %f %f %f )\n", pvox->bbox.Min.x, pvox->bbox.Min.y, pvox->bbox.Min.z, pvox->bbox.Max.x, pvox->bbox.Max.y, pvox->bbox.Max.z);
			//			if (pvox->bbox.RayIntersect(ray, &tmin, &tmax)) {
			//				pvox->is_solid = false;
			//				printf("Voxel destoyed\n");
			//				continue;
			//			}
			//		}
			//	}
			//}

			float d = 1.f;
			while (d < 10.f) {
				vec3 curr = ray.m_origin + normalize(ray.m_direction) * d;
				round_vector(curr, curr, 1.0);
				int offs = COORD2OFFSET(chunk.width, chunk.height, curr.x, curr.y, curr.z);
				if (offs < 0 || offs >= (chunk.width*chunk.width*chunk.height)) {
					d++;
					continue;
				}

				if (chunk.pvoxels[offs].is_solid) {
					chunk.pvoxels[offs].is_solid = false;
					shapes->DrawSphere(curr, 0.01);
					break;
				}
				d++;
			}
		}
	}
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

	///////////////////////////////////////
	shapes = new CShapes();
	CameraUtils_CheckActive(camera, NULL, NULL);

	chunk.width = 10;
	chunk.height = 10;
	chunk.min = vec3(0, 0, 0);
	chunk.max = vec3(chunk.width, chunk.height, chunk.width);

	int num_of_voxels = chunk.width * chunk.width * chunk.height;

	//8 углов у куба
	//chunk.vertices.reserve(num_of_voxels * 8 * sizeof(vec3));

	chunk.pvoxels = (voxel_t *)malloc(sizeof(voxel_t) * num_of_voxels);
	if (!chunk.pvoxels) {
		printf("Cananot allocate memory for vxoels!\n");
		exit(1);
	}
	memset(chunk.pvoxels, NULL, sizeof(voxel_t) * num_of_voxels);

	vec3 corners[8];
	voxel_t *pvox = NULL;
	for (int y = 0; y < chunk.height; y++) {
		for (int x = 0; x < chunk.width; x++) {
			for (int z = 0; z < chunk.width; z++) {
				pvox = &chunk.pvoxels[COORD2OFFSET(chunk.width, chunk.height, x, y, z)];

				//расчет углов куба вокруг вокселя
				pvox->bbox.Min.x = x - BLOCK_HALF;
				pvox->bbox.Min.y = y - BLOCK_HALF;
				pvox->bbox.Min.z = z - BLOCK_HALF;
				pvox->bbox.Max.x = x + BLOCK_HALF;
				pvox->bbox.Max.y = y + BLOCK_HALF;
				pvox->bbox.Max.z = z + BLOCK_HALF;

				if(!x || x >= chunk.width || !y || y >= chunk.width || !z || z >= chunk.width)
					pvox->is_solid = true;
			}
		}
	}
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Cube voxels world test", __FILE__ __TIME__,
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