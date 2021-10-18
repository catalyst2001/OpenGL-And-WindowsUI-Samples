#include <windows.h>
#include "../../../utilites/glwnd.h"
#include "../../../utilites/texture.h"
#include "../../../utilites/glmath.h"
#include "../../../utilites/scene_rotation.h"
#include "glextsloader.h"
#include "../../../utilites/utmath.h"

#include "CVertexBufferObject.h"
#include "CShader.h"

//#include "cube.h"
vec2 uvs[] = {
	// Front Face
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),

	// Back Face
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),

	// Top Face
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),

	// Bottom Face
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),

	// Right face
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),

	// Left Face
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
};

vec3 normals[] = {
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f,-1.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f,-1.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(-1.0f, 0.0f, 0.0f)
};

vec3 vertices[] = {
	// Front Face
	vec3(-1.0f, -1.0f,  1.0f),
	vec3(1.0f, -1.0f,  1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(-1.0f,  1.0f,  1.0f),

	// Back Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(-1.0f,  1.0f, -1.0f),
	vec3(1.0f,  1.0f, -1.0f),
	vec3(1.0f, -1.0f, -1.0f),

	// Top Face
	vec3(-1.0f,  1.0f, -1.0f),
	vec3(-1.0f,  1.0f,  1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(1.0f,  1.0f, -1.0f),

	// Bottom Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(1.0f, -1.0f, -1.0f),
	vec3(1.0f, -1.0f,  1.0f),
	vec3(-1.0f, -1.0f,  1.0f),

	// Right face
	vec3(1.0f, -1.0f, -1.0f),
	vec3(1.0f,  1.0f, -1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(1.0f, -1.0f,  1.0f),

	// Left Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(-1.0f, -1.0f,  1.0f),
	vec3(-1.0f,  1.0f,  1.0f),
	vec3(-1.0f,  1.0f, -1.0f)
};


INT Width, Height;

Texture diffuse;
Texture height;
Texture normal;
CSceneRotation rot(0, 0, -10);

CVBO<GL_ARRAY_BUFFER> vertex_buffer;
CVBO<GL_ARRAY_BUFFER> normals_buffer;
CVBO<GL_ARRAY_BUFFER> uvs_buffer;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	rot.Look();

	glBindTexture(GL_TEXTURE_2D, diffuse.texID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	vertex_buffer.Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	normals_buffer.Bind();
	glNormalPointer(GL_FLOAT, 0, NULL);
	uvs_buffer.Bind();
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_QUADS, 0, 24);
	vertex_buffer.Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
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
	rot.MouseMove(x, y);
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	switch (button) {
	case LBUTTON:
		rot.Rotating((bool)state);
		break;
	case RBUTTON:
		rot.Scaling((bool)state);
		break;
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	if (!LoadGLExtensions()) {
		printf("Failed to load gl extensions!\n");
		exit(-1);
	}

	printf("glMultiDrawArrays: 0x%x\n", glMultiDrawArrays);

	if (!LoadTexture("rockwall.tga", &diffuse))
		printf("Failed load texture rockwall.tga\n");
	
	if (!LoadTexture("rockwall_height.tga", &height))
		printf("Failed load texture rockwall_height.tga\n");

	if (!LoadTexture("rockwall_normal.tga", &normal))
		printf("Failed load texture rockwall_normal.tga\n");

	rot.SetRotationSpeed(0.09);
	rot.SetScaleSpeed(0.01);

	vertex_buffer.Create();
	vertex_buffer.Bind();
	vertex_buffer.BufferData(sizeof(vertices), &vertices);
	vertex_buffer.Unbind();

	normals_buffer.Create();
	normals_buffer.Bind();
	normals_buffer.BufferData(sizeof(normals), &normals);
	normals_buffer.Unbind();

	uvs_buffer.Create();
	uvs_buffer.Bind();
	uvs_buffer.BufferData(sizeof(uvs), &uvs);
	uvs_buffer.Unbind();
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Sample OpenGL Application", __FILE__ __TIME__,
		24,					  //Colors bits
		32,					  //Depth bits
		fn_draw,			  //Draw function
		fn_window_resize,	  //Window resize function
		fn_mousemove,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		NULL,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0, 0,
#ifdef ROMA
		800, //Window width
		600 //Window height
#else
		1280,
		1024
#endif
	);				 
	return 0;
}