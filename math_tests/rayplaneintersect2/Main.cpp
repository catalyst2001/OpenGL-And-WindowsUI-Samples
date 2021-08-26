//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		DigiBen@GameTutorials.com			 //
//																		 //
//		$Program:		TimeBasedMovement								 //
//																		 //
//		$Description:	Demonstrates camera movement in regards to time	 //

// This is a compiler directive that includes libraries (For Visual Studio).
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
// This is needed for timeGetTime() (also need to include <mmsystem.h>)
#pragma comment(lib, "winmm.lib")
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


#include "main.h"										// This includes our header file
#include "Camera.h"

CCamera g_Camera;										// This is our global camera object

bool  g_bFullScreen = TRUE;								// Set full screen as default
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
float g_DT = 0.0f;										// Global delta time

GLUquadric *sphere = NULL;
CPlane3 plane(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);


///////////////////////////////////////////////////////////////////////////////////////
//
// The code in this tutorial was taken from the Camera5 tutorial.  The point of this
// tutorial is to show how to make time based movement with the camera.  This means
// that the camera speed will be the same, no matter how fast the person's machine
// or video card is.  Granted, that the frames might render faster, but the player
// will not get from point A to B faster than the next guy.  Without doing time
// based movement, and doing just frame based movement, a person with a Geforce2
// will go incredibly faster than a person running a TNT2 in the level.  Some might
// try and lock their frame rate to compensate for this, but this doesn't always work.
// So spots might have more polygons that are being rendered and might make the player
// go slower on those parts, yet faster on low poly parts of the level.  
// 
// This feature is a MUST in any engine.  In the tutorial, we mixed the FPS
// and Camera5 tutorial together.  That way you don't have to sift through some new
// code to pick out the few lines that need to be added to make this feature happen.
// I will only mark these before mentioned lines as NEW so you can pick them out easier.
// Basically, all that's going on is that we are saving the time elapsed between each
// frame, then multiplying that interval by our desired speed.
//
// Notice that we did change the colors of the pyramids, just to give it a new look.
//
//


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

						// Position        View		   Up Vector
	g_Camera.PositionCamera(0, 1.5f, 6,   0, 1.5f, 0,   0, 1, 0);
}


/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function clamps a specified frame rate for consistency
/////
/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

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


///////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the main loop
/////
///////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
	MSG msg;

	sphere = gluNewQuadric();

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
			if(AnimateNextFrame(6000))					// Make sure we only animate 60 FPS
			{
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

void Draw3DSGrid()
{
	// Turn the lines GREEN
	glColor3ub(0, 255, 0);

	// Draw a 1x1 grid along the X and Z axis'
	for(float i = -50; i <= 50; i += 1)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

			// Do the horizontal lines (along the X)
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			// Do the vertical lines (along the Z)
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		// Stop drawing lines
		glEnd();
	}
}

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix
	g_Camera.Look();
	Draw3DSGrid();
	
	CRay ray;
	ray.SetOrigin(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z);
	ray.SetDirection(g_Camera.m_vView.x, g_Camera.m_vView.y, g_Camera.m_vView.z);
	ray.m_direction * 20.f;

	vec3 intersection;
	//ray.m_origin = normalize(ray.m_origin);
	//ray.m_direction = normalize(ray.m_direction);
	//plane.m_origin = normalize(plane.m_origin);
	//plane.m_normal = normalize(plane.m_normal);
	if (ray.PlaneIntersection5(plane.m_origin, plane.m_normal, ray.m_origin, ray.m_direction, intersection)) {
		glBegin(GL_LINES);
		glVertex3f(intersection.x, intersection.y, intersection.z);
		glVertex3f(intersection.x, intersection.y + 5, intersection.z);
		glEnd();
	}

	SwapBuffers(g_hDC);	
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 

	switch (uMsg)
	{ 
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