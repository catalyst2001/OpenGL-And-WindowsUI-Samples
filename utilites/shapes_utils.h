#pragma once
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "glmath.h"

class CShapes
{
public:
	CShapes() { m_pQuadric = gluNewQuadric(); }
	~CShapes() { gluDeleteQuadric(m_pQuadric); }

	void DrawCylinder(vec3 pos, float baseRadius = 5.0f, float topRadius = 5.0f, float height = 10.0f, int slices = 20, int stacks = 20) {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		gluCylinder(m_pQuadric, baseRadius, topRadius, height, slices, stacks);
		glPopMatrix();
	}

	void DrawDisk(vec3 pos, float innerRadius = 5.0f, float outerRadius = 10.f, int slices = 30, int loops = 30) {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		gluDisk(m_pQuadric, innerRadius, outerRadius, slices, loops);
		glPopMatrix();
	}

	void DrawSphere(vec3 pos, float radius = 0.3f, int slices = 10, int stacks = 10) {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		gluSphere(m_pQuadric, radius, slices, stacks);
		glPopMatrix();
	}
	
	GLUquadric *m_pQuadric;
};