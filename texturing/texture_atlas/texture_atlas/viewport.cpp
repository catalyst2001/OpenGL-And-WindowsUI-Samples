#include "viewport.h"

int create_viewport(viewport_t *pviewport, const char *classname, HWND parent,
	int colorbits,
	int depthbits,
	const char *label,
	int style,
	int posx, int posy,
	int width, int height)
{
	POINT pp;
	DWORD dwStyle = 0, dwStyleEx = WS_EX_CLIENTEDGE;
	if (parent)
	{
		dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | /*WS_GROUP |*/ WS_TABSTOP;
		//dwStyleEx = WS_EX_CONTROLPARENT;
	}
	else
	{
		pp.x = pp.y = CW_USEDEFAULT;
	}

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// size of this pfd
		 1,		// version number
		 PFD_DRAW_TO_WINDOW |	// support window
		 PFD_SUPPORT_OPENGL |	// support OpenGL
		 PFD_DOUBLEBUFFER,	// double buffered
		 PFD_TYPE_RGBA,	// RGBA type
		 32,		// 24-bit color depth
		 0, 0, 0, 0, 0, 0,	// color bits ignored
		 0,		// no alpha buffer
		 0,		// shift bit ignored
		 0,		// no accumulation buffer
		 0, 0, 0, 0,	// accum bits ignored
		 24,		// 32-bit z-buffer      
		 0,		// no stencil buffer
		 0,		// no auxiliary buffer
		 PFD_MAIN_PLANE,	// main layer
		 0,		// reserved
		 0, 0, 0	// layer masks ignored
	};

	pfd.cColorBits = colorbits;
	pfd.cDepthBits = depthbits;

	pviewport->hWindow = CreateWindowExA(dwStyleEx, classname, label,
		dwStyle,
		posx, posy,
		width, height,
		parent,
		(HMENU)0,
		GetModuleHandle(0),
		0);

	if (!pviewport->hWindow)
		return FAILED_CREATE_WINDOW;

	pviewport->hDevCtx = GetDC(pviewport->hWindow);
	if (!pviewport->hDevCtx)
		return FAILED_GET_DEVICE_CONTEXT;

	int pxfmt = ChoosePixelFormat(pviewport->hDevCtx, &pfd);
	if (!pxfmt)
		return FAILED_CHOOSE_PIXELFORMAT;

	if (!SetPixelFormat(pviewport->hDevCtx, pxfmt, &pfd))
		return FAILED_SET_PIXELFORMAT;

	//pxfmt = GetPixelFormat(pviewport->hDevCtx);
	//DescribePixelFormat(pviewport->hDevCtx, pxfmt, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	pviewport->hRndCtx = wglCreateContext(pviewport->hDevCtx);
	if (!pviewport->hRndCtx)
		return FAILED_CREATE_RENDERING_CONTEXT;

	if (!wglMakeCurrent(pviewport->hDevCtx, pviewport->hRndCtx))
		return FAILED_MAKE_CURRENT;

	return SUCCESS;
}

#define viewport_makecurrent(pviewport) wglMakeCurrent((pviewport)->hDevCtx, (pviewport)->hRndCtx)
#define viewport_swap_buffers(pviewport) SwapBuffers((pviewport)->hDevCtx)

void viewport_shutdown(viewport_t *pviewport)
{
	if (pviewport->hRndCtx) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(pviewport->hRndCtx);
	}
	if (pviewport->hDevCtx)
		ReleaseDC(pviewport->hWindow, pviewport->hDevCtx);
}