#pragma once
#include <windows.h>
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

typedef struct viewport {
	HDC hDevCtx;
	HGLRC hRndCtx;
	HWND hWindow;
} viewport_t;

enum viewport_errors_e {
	FAILED_CREATE_WINDOW = 0,
	FAILED_GET_DEVICE_CONTEXT,
	FAILED_CHOOSE_PIXELFORMAT,
	FAILED_SET_PIXELFORMAT,
	FAILED_CREATE_RENDERING_CONTEXT,
	FAILED_MAKE_CURRENT,
	SUCCESS
};

#define viewport_makecurrent(pviewport) wglMakeCurrent((pviewport)->hDevCtx, (pviewport)->hRndCtx)
#define viewport_swap_buffers(pviewport) SwapBuffers((pviewport)->hDevCtx)
int create_viewport(viewport_t *pviewport, const char *classname, HWND parent, int colorbits, int depthbits, const char *label, int style, int posx, int posy, int width, int height);
void viewport_shutdown(viewport_t *pviewport);