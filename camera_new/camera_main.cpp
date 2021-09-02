#include "include.h"

#define GLUT_BUILDING_LIB
#include "glut.h"

#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

camera cam(45.0, { 0.0f,0.0f,0.0f });

int gird = 200;

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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	cam.UpdateCameraState();

	Draw3DSGrid();

	vec3 direction = cam.position + cam.direction;
	gluLookAt(cam.position.x, cam.position.y, cam.position.z, direction.x, direction.y, direction.z, cam.up.x, cam.up.y, cam.up.z);

	glutSwapBuffers();
}

void fn_window_resize(int width, int height)
{
	if (height == 0)
		height = 1;

	cam.screenwidth = width;
	cam.screenheight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void HandleIdle()
{
	glutPostRedisplay();
}

void HandleKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
	case 'Q':
	case 'q':
		exit(0);
		break;

	case 'w':
	case 'W':
		movement.z = 1;
		break;
	case 'a':
	case 'A':
		movement.x = 1;
		break;
	case 's':
	case 'S':
		movement.z = -1;
		break;
	case 'd':
	case 'D':
		movement.x = -1;
		break;
	case ' ':
		movement.y = 1;
		break;
	case 'm':
	case 'M':
		movement.y = -1;
		break;
	case 'z':
	case 'Z':
		movement.y = 1;
		break;
	}

}

void fn_mousemove(int x, int y)
{
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("New camera test");
	glutDisplayFunc(fn_draw);
	glutReshapeFunc(fn_window_resize);
	glutReshapeWindow(1920, 1080);
	cam.screenwidth = 1920;
	cam.screenheight = 1080;
	glutIdleFunc(HandleIdle);
	glutKeyboardFunc(HandleKeyboard);
	glutMotionFunc(fn_mousemove);
	glutPassiveMotionFunc(fn_mousemove);
	glutMainLoop();
	return 0;
}
