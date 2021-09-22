#pragma once
#ifndef __CAMERA_INCLUIDED
#define __CAMERA_INCLUIDED
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define FLT_PI 3.141592653
#define SPEED 0.01

struct camera_t {
	float startX, startY, startZ;
	double angleX;
	double angleY;
	float dirX, dirY, dirZ;

	POINT mousexy;
	int width, height;
	bool active;
};

inline void camera_init(camera_t *pcamera, float stx, float sty, float stz, float dirx, float diry, float dirz)
{
	pcamera->startX = stx;
	pcamera->startY = sty;
	pcamera->startZ = stz;
	pcamera->dirX = dirx;
	pcamera->dirY = diry;
	pcamera->dirZ = dirz;
}

inline void camera_update_viewport(camera_t *pcamera, int _width, int _height)
{
	pcamera->width = _width;
	pcamera->height = _height;
}

inline void camera_set_active(camera_t *pcamera, bool state)
{
	pcamera->active = state;
}

void camera_look(camera_t *pcamera)
{
	//update view by mouse
	if (pcamera->active) {
		GetCursorPos(&pcamera->mousexy);
		pcamera->angleX += (pcamera->width / 2 - pcamera->mousexy.x) / 80.5f; //2 — чувствительность
		pcamera->angleY += (pcamera->height / 2 - pcamera->mousexy.y) / 80.5f;
		SetCursorPos(pcamera->width >> 1, pcamera->height >> 1);
	
		if(pcamera->angleY < -89.0)
			pcamera->angleY = -89.0;
		if(pcamera->angleY > 89.0)
			pcamera->angleY = 89.0;

		if (GetAsyncKeyState(VK_SHIFT)) {
			pcamera->startY += (float)SPEED / 2.f;
		}
		if (GetAsyncKeyState(VK_CONTROL)) {
			pcamera->startY -= (float)SPEED / 2.f;
		}
		if (GetAsyncKeyState(0x57)) //w
		{
			pcamera->startX -= (float)sinf(pcamera->angleX / 180.f * FLT_PI) * SPEED;
			pcamera->startZ -= (float)cosf(pcamera->angleX / 180.f * FLT_PI) * SPEED;
		}
		if (GetAsyncKeyState(0x53))//s
		{
			pcamera->startX += (float)sinf(pcamera->angleX / 180.f * FLT_PI) * SPEED;
			pcamera->startZ += (float)cosf(pcamera->angleX / 180.f * FLT_PI) * SPEED;
		}
		if (GetAsyncKeyState(0x44)) //right
		{
			pcamera->startX += (float)sinf((pcamera->angleX + 90) / 180.f * FLT_PI) * SPEED;
			pcamera->startZ += (float)cosf((pcamera->angleX + 90) / 180.f * FLT_PI) * SPEED;
		}
		if (GetAsyncKeyState(0x41))//left
		{
			pcamera->startX += (float)sinf((pcamera->angleX - 90) / 180.f * FLT_PI) * SPEED;
			pcamera->startZ += (float)cosf((pcamera->angleX - 90) / 180.f * FLT_PI) * SPEED;
		}
	}
	
	pcamera->dirX = pcamera->startX - sinf(pcamera->angleX / 180.f * FLT_PI);
	pcamera->dirY = pcamera->startY + (tanf(pcamera->angleY / 180.f * FLT_PI));
	pcamera->dirZ = pcamera->startZ - cosf(pcamera->angleX / 180.f * FLT_PI);
	//printf("Start: %f %f %f   dir: %f %f %f\n", x, y, z, dir[X], dir[Y], dir[Z]);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	gluLookAt(pcamera->startX, pcamera->startY, pcamera->startZ, pcamera->dirX, pcamera->dirY, pcamera->dirZ, 0.f, 1.f, 0.f);
}

#endif