#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "camera.h"
#include "vector3d.h"

//#include "marching_cubes.h"
//#include "marching_cubes2.h"
#include "marching_cubes3.h"

#define WINDOW_CLASS "MarchingCubes2021Kirill&Roma";

HINSTANCE h_instance;
camera_t camera;

struct {
	HWND handle;
	HDC handle_device_context;
	HGLRC handle_rendering_context;
	INT width, height;
} window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//marching_cubes mc;
marching_cubes3 mc3;

int main()
{
	WNDCLASSEXA wcex;
	memset(&wcex, NULL, sizeof(WNDCLASSEXA));
	wcex.cbSize = sizeof WNDCLASSEXA;
	wcex.hInstance = h_instance = GetModuleHandleA(NULL);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wcex.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW));
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.lpfnWndProc = WndProc;
	if (!RegisterClassExA(&wcex)) {
		printf("Error register window class!\n");
		MessageBoxA(NULL, "Error register window class!", "Critical error!", MB_OK | MB_ICONERROR);
		return -1;
	}

	window.handle = CreateWindowExA(NULL, wcex.lpszClassName, "Marching cubes", WS_VISIBLE|WS_OVERLAPPEDWINDOW, 0, 0, 1680, 1050, (HWND)NULL, (HMENU)NULL, h_instance, (LPVOID)NULL);
	window.width = 1680;
	window.height = 1050;
	if (!window.handle) {
		printf("Error create window!\n");
		MessageBoxA(NULL, "Error create window!", "Critical error!", MB_OK | MB_ICONERROR);
		return -2;
	}

	window.handle_device_context = GetDC(window.handle);
	if (!window.handle_device_context) {
		printf("Failed get device context!\n");
		MessageBoxA(NULL, "Failed get device context", "Critical error!", MB_OK | MB_ICONERROR);
		return -3;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof PIXELFORMATDESCRIPTOR;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	int pixelformat = ChoosePixelFormat(window.handle_device_context, &pfd);
	if (!pixelformat) {
		printf("Failed choose pixel formaat!\n");
		MessageBoxA(NULL, "Failed choose pixel format", "Critical error!", MB_OK | MB_ICONERROR);
		return -4;
	}

	if (!SetPixelFormat(window.handle_device_context, pixelformat, &pfd)) {
		printf("Failed set pixel formaat!\n");
		MessageBoxA(NULL, "Failed set pixel format", "Critical error!", MB_OK | MB_ICONERROR);
		return -5;
	}

	window.handle_rendering_context = wglCreateContext(window.handle_device_context);
	if (!window.handle_rendering_context) {
		printf("Failed create device context!\n");
		MessageBoxA(NULL, "Failed create device context", "Critical error!", MB_OK | MB_ICONERROR);
		return -6;
	}
	wglMakeCurrent(window.handle_device_context, window.handle_rendering_context);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
	glClearDepth(1.0f); // Depth Buffer Setup
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST); // Enable Depth Testing


	camera_init(&camera, 0.f, 0.f, 10.f, 0.f, 0.f, 0.f);
	camera_update_viewport(&camera, window.width, window.height);
	camera_set_active(&camera, true);
	//for (int y = 0; y < 50; y++) {
	//	for (int x = 0; x < 50; x++) {
	//		for (int z = 0; z < 50; z++) {
	//			vb_test.AddVertex({ (float)x, (float)y, (float)z });
	//			vb_test.AddVertex({ (float)x + 1, (float)y, (float)z + 1 });
	//		}
	//	}
	//}

	mc3.Start();
	printf("Verts: %d\n", mc3.verts_index);
	printf("Indices: %d\n", mc3.index_index);

	//mc.MarchCube({ 10.f, 2.f, 10.f });
	//printf("Verts size: %d\n", mc.vertices.size());
	//for (auto elem : mc.vertices)
	//	printf("vert %f %f %f\n", elem.x, elem.y, elem.z);

	//std::vector<Vector3> vertices;

	//InputType input;
	//input.position = {1.f, 1.f, 1.f};

	//ParamBuffer bufp;
	//bufp.isoValue = 1.f;
	//bufp.meshScaleFactor = 1.f;
	//bufp.meshSize = 1.f;
	//bufp.padding = 1.f;
	//Marching(input, bufp, vertices);

	MSG msg = {NULL};
	while (msg.message != WM_CLOSE) {
		if (PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, window.width / (double)window.height, 1.0, 1000.0);

		camera_look(&camera);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, mc3.vertices);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, mc3.index_index, GL_UNSIGNED_INT, mc3.triangles);
		glDisableClientState(GL_VERTEX_ARRAY);

		SwapBuffers(window.handle_device_context);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_QUIT:
	case WM_CLOSE: {
		exit(0);
		break;
	}

	case WM_KEYDOWN:
		switch (wParam) {
		case 27:
			exit(0);
			break;

		case VK_F1:
			static bool bpoly = false;
			glPolygonMode(GL_FRONT_AND_BACK, bpoly ? GL_FILL : GL_LINE);
			bpoly = !bpoly;
		}
		break;

	case WM_SIZE: {
		window.width = LOWORD(lParam);
		window.height = HIWORD(lParam);
		if (window.height < 1)
			window.height = 1;

		printf("resize %dx%d\n", window.width, window.height);
		glViewport(0, 0, window.width, window.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, ((double)window.width / (double)window.height), 1.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}


	}
	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}