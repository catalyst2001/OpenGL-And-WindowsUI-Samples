#pragma once
#include <Windows.h>
#include <gl/GL.h>

//*** Render primitives ***//
static void draw_rect(int mode, int posx, int posy, int right, int top)
{
	int coords[] = {
		posx, posy,
		right, posy,
		right, top,
		posx, top
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glDrawArrays(mode, 0, 4);
}

//*** Circle draw utils BEGIN ***//
typedef struct {
	int *verts;
	int stride;
	int mode;
} circleprimitive_t;

static void init_circle(circleprimitive_t *pCircle, int radius, int stride, int mode)
{
	pCircle->mode = mode;
	pCircle->stride = stride;
	pCircle->verts = (int*)malloc(pCircle->stride * sizeof(int) * 2);
	for (int i = 0, arridx = 0; i < stride; i++) {
		float theta = 2.0f * 3.1415926f * (float)i / (float)stride;//get the current angle
		int x = radius * cosf(theta);//calculate the x component
		int y = radius * sinf(theta);//calculate the y component
		pCircle->verts[arridx++] = x;
		pCircle->verts[arridx++] = y;
	}
}

static void draw_circle(circleprimitive_t *pCircle, int posx, int posy)
{
	glPushMatrix();
	glTranslatef(posx, posy, 0.f);
	glVertexPointer(2, GL_INT, 0, pCircle->verts);
	glDrawArrays(pCircle->mode, 0, pCircle->stride);
	glPopMatrix();
}

#define utl_free_circle(pCircle) free((pCircle)->verts);
//*** Circle draw utils END ***//

//*** POLYGON DRAW UTILS BEGIN ***//



