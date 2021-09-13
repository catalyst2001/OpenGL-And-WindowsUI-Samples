#include <vector>
#include "../../../utilites/glwnd.h"
#include "../../../utilites/glmath.h"
#include "../../../utilites/utmath.h"
#include "../../../utilites/scene_rotation.h"
#include "tritable.h"
using namespace std;

vector<vec3> vertices;
vector<int> indices;

vec3 position(0.f, 0.f, 0.f);

static vec3int CornerTable[] = {
	{ 0, 0, 0 },
	{ 1, 0, 0 },
	{ 1, 1, 0 },
	{ 0, 1, 0 },
	{ 0, 0, 1 },
	{ 1, 0, 1 },
	{ 1, 1, 1 },
	{ 0, 1, 1 }
};

static vec3 EdgeTable[12][2] = {

	{ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
	{ {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f} },
	{ {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} },
	{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
	{ {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f} },
	{ {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f} },
	{ {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} },
	{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f} },
	{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
	{ {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f} },
	{ {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} },
	{ {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f} }
};

INT Width, Height;

CSceneRotation rotation(0.f, 0.f, 2.f);
int cubeconfig = 1;

void Clear()
{
	vertices.clear();
	indices.clear();
}

void print_info()
{
	char buffer[512];
	sprintf_s(buffer, sizeof(buffer), "Pos %f %f %f   Rotation %f %f %f   Verts: %d   Config: %d",
		rotation.m_position.x, rotation.m_position.y, rotation.m_position.z,
		rotation.m_rotation.x, rotation.m_rotation.y, rotation.m_rotation.z,
		vertices.size(),
		cubeconfig);

	SetWindowTextA(GetStruct()->h_window, buffer);
}

bool march_cube(vec3 pos, int configindex)
{
	if (!configindex || configindex == 255)
		return false;

	int edgeIndex = 0;
	for (int i = 0; i < 5; i++) {
		for (int p = 0; p < 3; p++) {
			int indice = tritable[configindex][edgeIndex];
			if (indice == -1)
				return false;

			vec3 vert1 = pos + EdgeTable[indice][0];
			vec3 vert2 = pos + EdgeTable[indice][1];
			vec3 vertpos = (vert1 + vert2) / 2.f;
			vertices.push_back(vertpos);
			indices.push_back(vertices.size() - 1);
			edgeIndex++;
		}
	}

	return true;
}

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	rotation.Look();

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

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
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
}

void fn_mousemove(HWND hWnd, int x, int y)
{
	print_info();
	rotation.MouseMove(x, y);
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	print_info();
	switch (button) {
	case LBUTTON:
		rotation.Rotating((bool)state);
		break;
	case RBUTTON:
		rotation.Scaling((bool)state);
		break;
	}
}

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
{
	static bool b_wire = false;
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		switch (key) {
		case 27:
			exit(0); //close program
			break;

		case VK_F1:
			glPolygonMode(GL_FRONT_AND_BACK, b_wire ? GL_LINE : GL_FILL);
			b_wire = !b_wire;
			break;

		case VK_RIGHT:
			if(cubeconfig < 254)
				cubeconfig++;

			Clear();
			march_cube(position, cubeconfig);
			print_info();
			break;

		case VK_LEFT:
			if (cubeconfig > 1)
				cubeconfig--;

			Clear();
			march_cube(position, cubeconfig);
			print_info();
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

	rotation.SetRotationSpeed(1.f);
	rotation.SetScaleSpeed(0.1f);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Marching Cubes configurations view", __FILE__ __TIME__,
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
		800,				  //Window width
		600);				  //Window height
	return 0;
}