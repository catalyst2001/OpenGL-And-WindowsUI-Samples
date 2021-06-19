#pragma once
#include <Windows.h>
#include <gl/gl.h>

static void draw_rect(int mode, int x, int y, int width, int height)
{
	int verts[] = {
		x, y,
		x + width, y,
		x + width, y + height,
		x, y + height
	};
	glVertexPointer(2, GL_INT, 0, verts);
	glDrawArrays(mode, 0, 4);
}

static void draw_textured_rect(int x, int y, int width, int height, float lu, float lv, float ru, float rv, int texid)
{
	int verts[] = {
		x, y,
		x + width, y,
		x + width, y + height,
		x, y + height
	};
	float texcoords[] = { lu, lv, ru, rv };
	glBindTexture(GL_TEXTURE_2D, texid);
	glVertexPointer(2, GL_INT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glDrawArrays(GL_QUADS, 0, 4);
}

static void draw_textured_rectv(int *verts, float *texcoords, int texid)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glVertexPointer(2, GL_INT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glDrawArrays(GL_QUADS, 0, 4);
}