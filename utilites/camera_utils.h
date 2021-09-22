#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glmath.h"
#include "camera_quat/camera.h"

static void CameraUtils_Move(CCamera &cam, int key, int state, float scale)
{
	switch (key) {
	case 'w':
	case 'W':
		cam.m_vecMovement.z = state ? scale : 0.f;
		break;
	case 'a':
	case 'A':
		cam.m_vecMovement.x = state ? scale : 0.f;
		break;
	case 's':
	case 'S':
		cam.m_vecMovement.z = state ? -scale : 0.f;
		break;
	case 'd':
	case 'D':
		cam.m_vecMovement.x = state ? -scale : 0.f;
		break;
	case ' ':
		cam.m_vecMovement.y = state ? scale : 0.f;
		break;
	case 'm':
	case 'M':
		cam.m_vecMovement.y = state ? -scale : 0.f;
		break;
	case 'z':
	case 'Z':
		cam.m_vecMovement.y = state ? scale : 0.f;
		break;
	}
}

static void CameraUtils_LookAt(CCamera &cam)
{
	vec3 dir = cam.m_vecOrigin + cam.m_vecDirection;
	gluLookAt(cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, dir.x, dir.y, dir.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);
}

//Добавляет возможность отключения привязки камеры на клавишу и показывает курсор
//CameraUtils_CheckActive(camera, NULL, NULL) - сделать камеру активной и скрыть курсор
static void CameraUtils_CheckActive(CCamera &cam, int key, int checkkey)
{
	if (!key) {
		cam.SetActive(true);
		ShowCursor(FALSE);
		return;
	}

	if (key == checkkey) {
		cam.SetActive(!cam.IsActive());
		ShowCursor(!cam.IsActive());
	}
}

static void CameraUtils_DrawCrosshair(int width, int height)
{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3ub(120, 255, 255);
	glDisable(GL_DEPTH);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2i(width >> 1, height >> 1);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPointSize(1);
	glEnable(GL_DEPTH);
	glPopAttrib();
}