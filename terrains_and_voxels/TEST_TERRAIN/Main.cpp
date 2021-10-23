#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// This is needed for timeGetTime() (also need to include <mmsystem.h>)
#pragma comment(lib, "winmm.lib")

#include "main.h"										// This includes our header file
#include "Camera.h"
#include "SimplexNoise.h"

#include "CTerrain.h"

CCamera g_Camera;										// This is our global camera object

bool  g_bFullScreen = TRUE;								// Set full screen as default
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
float g_DT = 0.0f;										// Global delta time

//keyboard focus
bool is_focus;

CTerrain* terrain_object = NULL;

/////	This function initializes the game window.
void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

						// Position        View		   Up Vector
	g_Camera.PositionCamera(1.5, 6.5f, -8.f,   0, 1.5f, 0,   0, 1, 0);
}


/////	This function clamps a specified frame rate for consistency
bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f; // Get the time (milliseconds = seconds * .001)
	float deltaTime = currentTime - lastTime; // Get the slice of time
	float desiredFPS = 1.0f / desiredFrameRate; // Store 1 / desiredFrameRate

	elapsedTime += deltaTime; // Add to the elapsed time
	lastTime = currentTime; // Update lastTime

	// Check if the time since we last checked is greater than our desiredFPS
	if( elapsedTime > desiredFPS )
	{
		g_DT = desiredFPS; // Set the delta time
		elapsedTime -= desiredFPS; // Adjust our elapsed time

		// Return true, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}

CVector3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return CVector3(posX, posY, posZ);
}

CVector3 current_point;

/////	This function handles the main loop
WPARAM MainLoop()
{
	MSG msg;

	while(1)											// Do our infinite loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			if(msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
			TranslateMessage(&msg);						// Find out what the message does
			DispatchMessage(&msg);						// Execute the message
		}
		else											// if there wasn't a message
		{
			if(AnimateNextFrame(4000))					// Make sure we only animate 60 FPS
			{
				if(is_focus)
					g_Camera.Update();						// Update the camera every frame

				RenderScene();							// Render the scene every frame
			}
			else
			{
				Sleep(1);								// Let other processes work
			}
		} 
	}

	DeInit();											// Clean up and free all allocated memory

	return(msg.wParam);									// Return from the program
}


#define GIRD_SIZE 100
float world[GIRD_SIZE][GIRD_SIZE];

struct vec3_t {
	float x, y, z;
};

#include "random.h"

GLfloat vertices[] = { 1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
					  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
					  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
					  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
					  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
					  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1 };   // v4-v7-v6-v5

float cube[] = {
//	X	 Y	  Z
	0.f, 0.f, 0.f,
	0.f, 0.f, 1.f,
	0.f, 1.f, 1.f,
	0.f, 1.f, 0.f,

	1.f, 0.f, 0.f,
	1.f, 0.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 1.f, 0.f, 

	0.f, 0.f, 1.f,
	0.f, 1.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 0.f, 1.f,

	0.f, 0.f, 0.f, 
	0.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 0.f, 0.f,

	0.f, 0.f, 0.f,
	0.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 0.f,

	0.f, 1.f, 0.f,
	0.f, 1.f, 1.f,
	1.f, 1.f, 1.f,
	1.f, 1.f, 0.f
};

void draw_cube()
{
	glVertexPointer(3, GL_FLOAT, 0, cube);
	glDrawArrays(GL_QUADS, 0, 24);
}

vec3_t* world_array = 0;
int size = 3000;


//float verts[] = {
//	0.f, 0.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//	1.f, 0.f, 1.f,
//	2.f, 0.f, 0.f,
//	2.f, 0.f, 1.f,
//	3.f, 0.f, 0.f,
//	3.f, 0.f, 1.f,
//	4.f, 0.f, 0.f,
//	4.f, 0.f, 1.f,
//	5.f, 0.f, 0.f,
//	5.f, 0.f, 1.f,
//	6.f, 0.f, 0.f,
//	6.f, 0.f, 1.f,
//	7.f, 0.f, 0.f,
//	7.f, 0.f, 1.f,

//	7.f, 0.f, 2.f,
//	7.f, 0.f, 1.f,
//	6.f, 0.f, 2.f,
//	6.f, 0.f, 1.f,
//	5.f, 0.f, 2.f,
//	5.f, 0.f, 1.f,
//	4.f, 0.f, 2.f,
//	4.f, 0.f, 1.f,
//	3.f, 0.f, 2.f,
//	3.f, 0.f, 1.f,
//	2.f, 0.f, 2.f,
//	2.f, 0.f, 1.f,
//	1.f, 0.f, 2.f,
//	1.f, 0.f, 1.f,
//	0.f, 0.f, 2.f,
//	0.f, 0.f, 1.f
//};

float scale = 0.001f;
float heightscale = 1.0f;

void Build()
{
	SimplexNoise snoise;
	for (int x = 0; x < GIRD_SIZE; x++) {
		for (int y = 0; y < GIRD_SIZE; y++) {
			world[y][x] = snoise.noise((float)x * scale, (float)y * scale) * heightscale;
		}
	}
}

void Draw3DSGrid(CCamera camera)
{
	if (GetKeyState(VK_F3) & 0x80) {
		scale -= 0.001;
		Build();
	}

	if (GetKeyState(VK_F4) & 0x80) {
		scale += 0.001;
		Build();
	}

	if (GetKeyState(VK_F5) & 0x80) {
		heightscale -= 0.1;
		Build();
	}
	
	if (GetKeyState(VK_F6) & 0x80) {
		heightscale += 0.1;
		Build();
	}

	//Draw axises
	glLineWidth(5);
	glBegin(GL_LINES);

	//Y
	glColor3ub(0, 255, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.5f, 0.f);

	//X
	glColor3ub(255, 0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.5f, 0.f, 0.f);

	//Z
	glColor3ub(0, 0, 255);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.5f);
	glEnd();
	glLineWidth(1);

	//glColor3ub(0, 255, 0);
	for(int z = 0; z < GIRD_SIZE; z++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < GIRD_SIZE; x++) {
			glVertex3f((float)x, world[x][z], (float)z);
			glVertex3f((float)x, world[x][z+1], (float)z+1);
		}
		glEnd();
	}

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, world_array);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, size*size);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//prev_coord = pos;
}

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix
	
		// Tell OpenGL where to point the camera
	g_Camera.Look();

	// Draw a grid so we can get a good idea of movement around the world		
	Draw3DSGrid(g_Camera);

	// Draw the pyramids that spiral in to the center
	//DrawSpiralTowers();

	// Swap the backbuffers to the foreground
	SwapBuffers(g_hDC);	
}

 void gen_vertexes(int world_size)
 {
	 /////////////////////////////////////////////////////////////////////////////////////
	 int world_x = world_size * 2;
	 int ssize = sizeof(vec3_t) * (world_x * world_x) * 2;
	 world_array = (vec3_t*)malloc(ssize);

	 //set start pos
	 //world_array[0].x = 0.f;
	 //world_array[0].y = 0.f;
	 //world_array[0].z = 0.f;
	 int c = 0;
	 bool is_right = true;
	 for (int z = 0; z < world_x; z++) {
		 int j = z * world_x;
		 if (is_right) {
			 for (int x = 0, i = 0; x < world_x; x += 2, i++) {
				 world_array[c].z = (float)z;
				 world_array[c].x = (float)i;
				 world_array[c].y = 0.f;
				
				 world_array[c + 1].z = (float)z + 1.f;
				 world_array[c + 1].x = (float)i;
				 world_array[c + 1].y = 0.f;

				 //printf("1 right  (x:%f ; z: %f)  index = %d\n",
				 //	world_array[c].x,
				 //	world_array[c].z,
				 //	c);
				 //printf("2 right  (x:%f ; z: %f)  index = %d\n",
				 //	world_array[c + 1].x,
				 //	world_array[c + 1].z,
				 //	c + 1);
				 c += 2;
			 }

			 world_array[c].z = (float)z;
			 world_array[c].x = (float)world_x / 2;
			 world_array[c].y = 0.f;
			 //printf("\nend right: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
			 c++;

			 world_array[c].z = (float)z + 1;
			 world_array[c].x = (float)world_x / 2;
			 world_array[c].y = 0.f;
			 //printf("\nend right: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
			 c++;

			 if (z <= world_size-3)
			 {
				 world_array[c].z = (float)z + 2;
				 world_array[c].x = (float)world_x / 2;
				 world_array[c].y = 0.f;
				 //printf("\nend right: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
				 c++;

				 world_array[c].z = (float)z + 1;
				 world_array[c].x = (float)world_x / 2;
				 world_array[c].y = 0.f;
				 //printf("\nend right: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
				 c++;
			 }
		 }
		 else
		 {
			 for (int x = world_x, i = world_x / 2; x >= 2; x -= 2, i--) {
				 world_array[c].z = (float)z;
				 world_array[c].x = (float)i;
				 world_array[c].y = 0.f;

				 world_array[c + 1].z = (float)z + 1.f;
				 world_array[c + 1].x = (float)i - 1;
				 world_array[c + 1].y = 0.f;

				 //printf("1 left  (x:%f ; z: %f)  index = %d\n",
				 //	world_array[c].x,
				 //	world_array[c].z,
				 //	c);
				 //printf("2 left  (x:%f ; z: %f)  index = %d\n",
				 //	world_array[c + 1].x,
				 //	world_array[c + 1].z,
				 //	c + 1);
				 c += 2;
			 }
			 world_array[c].z = (float)z + 1;
			 world_array[c].x = 0.f;
			 world_array[c].y = 0.f;
			 //printf("\nend left: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
			 c++;

			 world_array[c].z = (float)z - 1;
			 world_array[c].x = 0.f;
			 world_array[c].y = 0.f;
			 //printf("\nend left: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
			 c++;

			 world_array[c].z = (float)z + 1;
			 world_array[c].x = 0.f;
			 world_array[c].y = 0.f;
			 //printf("\nend left: x:%f z:%f  index: %d\n\n", world_array[c].x, world_array[c].z, c);
			 c++;
		 }
		 is_right = !is_right;
	 }
	 /////////////////////////////////////////////////////////////////////////////////////
 }


LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
	switch (uMsg)
	{
	case WM_CREATE:
		AllocConsole();
		freopen("conout$", "w", stdout);

		gen_vertexes(size); //!!!!!!!!!!!

		break;

	case WM_LBUTTONDOWN:
		current_point = GetOGLPos(LOWORD(lParam), HIWORD(lParam));
		printf("Point: ( %f %f %f )\n", current_point.x, current_point.y, current_point.z);
		break;

	case WM_SETFOCUS:
		is_focus = true;
		break;

	case WM_KILLFOCUS:
		is_focus = false;
		break;

    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Do this only if we are in window mode
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);				// Get the window rectangle
		}
        break; 

	case WM_KEYDOWN:

		switch(wParam) {								// Check if we hit a key
			case VK_ESCAPE:								// If we hit the escape key
				PostQuitMessage(0);						// Send a QUIT message to the window
				break;

			case VK_F1:
				is_focus = !is_focus;
				break;
		}
		break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}