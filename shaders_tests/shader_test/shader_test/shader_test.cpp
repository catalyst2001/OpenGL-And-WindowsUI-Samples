#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "../../../utilites/glwnd.h"
#include "../../../utilites/texture.h"
#include "../../../utilites/glmath.h"
#include "glextsloader.h"

INT Width, Height;

GLuint program;

Texture background;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Width, 0, Height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glUseProgram(program);
	//glUniform3f(glGetUniformLocation(program, "color"), 0.1, 0.1, 0.1);


	int coords[] = {
		0, 0,
		Width, 0,
		Width, Height,
		0, Height
	};

	float uvs[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glDrawArrays(GL_QUADS, 0, 4);

	//glUseProgram(0);
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
		switch (key) {
		case 27:
			exit(0); //close program
			break;
		}
	}
}

GLuint LoadShader(const char *path, int shadertype)
{
	FILE *fp = fopen("shader.fs", "rb");
	if (!fp) {
		printf("Failed to open shader file!\n");
		return 0;
	}

	int filesize = 0;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (filesize <= 0) {
		printf("Shader source file is empty!\n");
		return 0;
	}

	char *shader_source = (char *)calloc(filesize + 1, sizeof(char));
	if (!shader_source) {
		printf("Failed allocate memory for shader source!\n");
		return 0;
	}

	fread(shader_source, 1, filesize, fp);
	fclose(fp);

	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!shader) {
		printf("Failed to create shader!\n");
		return 0;
	}
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	free(shader_source);

	char log[1024];
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);
		printf("Error compile shader: %s\n", log);
		return 0;
	}
	return shader;
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

	if (!LoadGLExtensions()) {
		printf("Cannot load gl extensions!\n");
		exit(-2);
	}

	//loading shader
	GLuint fragment_shader = LoadShader("shader.fs", GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
		printf("Failed to load fragment shader!\n");
		exit(-1);
	}

	//shader program
	program = glCreateProgram();
	glAttachShader(program, fragment_shader);
	glBindAttribLocation(program, 0, "color");
	glLinkProgram(program);

	int status;
	char linklog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(program, sizeof(linklog), NULL, linklog);
		printf("Failed to link program: %s\n", linklog);
		exit(-1);
	}

	if (!LoadTexture("photo.bmp", &background)) {
		printf("Failed to load image!\n");
		exit(1);
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background.texID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
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
		fn_charinput,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0, 0,
		1280,				  //Window width
		1024);				  //Window height
	return 0;
}