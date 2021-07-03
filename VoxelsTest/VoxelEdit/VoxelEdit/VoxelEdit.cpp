#include "../../../utilites/glwin.h"
#include "Camera.h"

#define WORLD_MAX 1024
float world[WORLD_MAX][WORLD_MAX];

CCamera camera;

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera.Update();
	camera.Look();

	glPushAttrib(GL_CURRENT_BIT);
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
	glPopAttrib();

	glColor3ub(255, 255, 255);
	for (int x = 0; x < WORLD_MAX; x++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int z = 0; z < WORLD_MAX; z++) {
			glVertex3f((float)x, world[x][z], (float)z);
			glVertex3f((float)x + 1, world[x + 1][z], (float)z);
		}
		glEnd();
	}
}

void Resize(int width, int height)
{
	if (height == 0)
		height = 1;

	camera.m_width = width;
	camera.m_height = height;
	glViewport(0, 0, width, height);					// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what are our drawing boundaries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  the closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  the farthest distance before it stops drawing).
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 150.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void MouseMove(int x, int y)
{
	
}

//#define LBUTTON 0 //left mouse button
//#define RBUTTON 1 //right mouse button
//#define STATE_PRESSED 0 //button dwown
//#define STATE_RELEASED 1 //button up
void MouseClick(int x, int y, int button, int state)
{
}

void Keyboard(char ch)
{
	switch (ch) {
	case 27: //escape
		PostQuitMessage(0);
		exit(0);
		break;
	}
}

void Init(HWND wnd)
{
	memset(world, 0, WORLD_MAX*WORLD_MAX);
	camera.PositionCamera(0.f, 0.f, 0.f,   0, 1.5f, 0,   0, 1, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Shutdown(HWND wnd)
{
	PostQuitMessage(0);
	exit(0);
}

int main()
{
	create_window("Voxel Editor Test", "VE2021", 24, 32, Draw, Resize, MouseMove, MouseClick, Keyboard, Init, Shutdown, 0, 0, 1280, 1024);
	return 0;
}
