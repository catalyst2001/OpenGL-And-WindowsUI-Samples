#pragma once

#include <vector>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

//TESTS
#include "FTFont.h"
#include "ÑFont.h"

typedef unsigned long ulong_t;
typedef unsigned char uchar;
typedef int position[2];
typedef int difference[2];
#define UI_ELEMENT_ENABLED (1 << 0)
#define SR_LIGHTUP 0
#define SR_LIGHTDOWN 1


static double convert(double value, double From1, double From2, double To1, double To2)
{
	return (value - From1) / (From2 - From1)*(To2 - To1) + To1;
}

class CPointf{
public:
	CPointf() {}
	CPointf(float xx, float yy) : x(xx), y(yy) {}
	~CPointf() {}

	float x, y;
};

class CPoint {
public:
	CPoint() {}
	~CPoint() {}
	CPoint(int xx, int yy) : x(xx), y(yy) {}
	int x, y;
};

static class CColor3
{
	void SetCorrect(int colors[3])
	{
		if (colors[0] > 255)
			colors[0] = 255;
		if (colors[0] < 0)
			colors[0] = 0;
		if (colors[1] > 255)
			colors[1] = 255;
		if (colors[1] < 0)
			colors[1] = 0;
		if (colors[2] > 255)
			colors[2] = 255;
		if (colors[2] < 0)
			colors[2] = 0;
	}
public:
	CColor3() {}
	~CColor3() {}
	__forceinline CColor3(uchar rr, uchar gg, uchar bb) : r(rr), g(gg), b(bb) {}

	CColor3 operator+(int arg)
	{
		int colors[3] = { r, g, b };
		colors[0] += arg;
		colors[1] += arg;
		colors[2] += arg;
		SetCorrect(colors);
		r = (char)colors[0];
		g = (char)colors[1];
		b = (char)colors[2];
		return *this;
	}

	CColor3 operator-(int arg)
	{
		int colors[3] = { r, g, b };
		colors[0] -= arg;
		colors[1] -= arg;
		colors[2] -= arg;
		SetCorrect(colors);
		r = (char)colors[0];
		g = (char)colors[1];
		b = (char)colors[2];
		return *this;
	}

	uchar r, g, b;
};

static class CRect
{
#define POINT_IN_RECT(rectx, recty, rectright, recttop, pointx, pointy) (rectx < pointx && recty < pointy && rectright > pointx && recttop > pointy)
public:
	CRect() {}
	~CRect() {}
	CRect(int x, int y, int width, int height) {
		// (1)--------(2)
		//  |          |
		// (4)--------(3)
		p1 = CPoint(x, y);
		p2 = CPoint(x + width, y);
		p3 = CPoint(p2.x, y - height);
		p4 = CPoint(x, p3.y);
	}
	void MoveX(int x) { 
		p1.x += x;
		p2.x += x;
		p3.x += x;
		p4.x += x;
	}
	void MoveY(int y) { 
		p1.y += y;
		p2.y += y;
		p3.y += y;
		p4.y += y;
	}
	int GetWidth() { return p2.x - p1.x; }
	int GetHeight() { return p4.y - p1.y; }
	__forceinline bool PointInRect(int x, int y) { return POINT_IN_RECT(p1.x, p3.y, p3.x, p1.y, x, y); }
	CPoint p1, p2, p3, p4;
};

static void DrawShadowRect(CRect &rect, CColor3 *up, CColor3 *down, long flags)
{
	CColor3 *lightcolor, *darkcolor;
	if (flags == SR_LIGHTUP) {
		lightcolor = up;
		darkcolor = down;
	}
	else {
		lightcolor = down;
		darkcolor = up;
	}

	// (1)--------(2)
	//  |          |
	// (4)--------(3)

	//left top angle
	glPushAttrib(GL_CURRENT_BIT);
	CPoint verts[3];
	verts[0] = CPoint(rect.p1.x + 1, rect.p1.y);
	verts[1] = rect.p4;
	verts[2] = rect.p3;
	glVertexPointer(2, GL_INT, 0, verts);
	glColor3ubv((uchar*)lightcolor);
	glDrawArrays(GL_LINE_STRIP, 0, 3);

	//right down angle
	verts[0] = rect.p3;
	verts[1] = rect.p2;
	verts[2] = rect.p1;
	glVertexPointer(2, GL_INT, 0, verts);
	glColor3ubv((uchar*)darkcolor);
	glDrawArrays(GL_LINE_STRIP, 0, 3);
	glPopAttrib();
}

class IElementBase;
class CGUIElementBase
{
public:
	int id;
	ulong_t flag;
	CRect rect;
	std::vector<IElementBase*> childs;
};


class IElementBase : public CGUIElementBase
{
public:
	virtual bool OnCreate(IElementBase * parent) = 0;
	virtual void OnDestroy() = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseClick(int x, int y, int button, int state) = 0;
	virtual void OnDraw() = 0;
	virtual void OnCharInput(char ch) = 0;
	virtual void OnKeydown(int key, int flag) = 0;
	virtual void OnResize(int width, int height) = 0;
	virtual void AddElement(IElementBase * elem) = 0;
};

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

class Cgui : public IElementBase
{
	int m_width, m_height;
	void OnMouseMoveRecurs(IElementBase * self, int X, int Y)
	{
		for (int i = 0; i < self->childs.size(); i++) {
			IElementBase * child = self->childs[i];
			if (child && (child->flag & UI_ELEMENT_ENABLED)) {
				child->OnMouseMove(X, Y);
				OnMouseMoveRecurs(child, X, Y);
			}
		}
	}

	void OnMouseClickRecurs(IElementBase * self, int x, int y, int button, int state)
	{
		for (int i = 0; i < self->childs.size(); i++) {
			IElementBase * child = self->childs[i];
			if (child && (child->flag & UI_ELEMENT_ENABLED)) {
				child->OnMouseClick(x, y , button, state);
				OnMouseClickRecurs(child, x, y, button, state);
			}
		}
	}

	void OnDrawRecurs(IElementBase * self)
	{
		for (int i = 0; i < self->childs.size(); i++) {
			IElementBase * child = self->childs[i];
			if (child && (child->flag & UI_ELEMENT_ENABLED)) {
				child->OnDraw();
				OnDrawRecurs(child);
			}
		}
	}

public:
	__forceinline void OnResizewindow(int width, int height) { m_width = width; m_height = height; }
	bool OnCreate(IElementBase * parent);
	void OnDestroy();
	void OnMouseMove(int x, int y);
	void OnMouseClick(int x, int y, int button, int state);
	void OnDraw();
	void OnCharInput(char ch);
	void OnKeydown(int key, int flag);
	void OnResize(int width, int height);
	void AddElement(IElementBase * elem);

};


#include "UI_CButton.h"
#include "UI_CCheckBox.h"
#include "UI_CEditbox.h"
#include "UI_CProgressBar.h"
#include "UI_CTrackBar.h"





/*
class CTrackBar : public IElementBase
{
	CColor3 m_color;
public:
	CTrackBar() {}
	CTrackBar(int x, int y, int width, int height, CColor3 color) {
		m_color = color;
		rect = CRect(x, y, width, height);
		flag = UI_ELEMENT_ENABLED;
	}
	~CTrackBar() {}
	bool OnCreate(IElementBase * parent)
	{
		return TRUE;
	}
	void OnDestroy()
	{

	}
	void OnMouseMove(int x, int y)
	{

	}
	void OnMouseClick(int x, int y, int button, int state)
	{

	}
	void OnDraw()
	{
	}
	void OnCharInput(char ch)
	{

	}
	void OnKeydown(int key, int flag)
	{

	}
	void OnResize(int width, int height)
	{

	}
	void AddElement(IElementBase * elem)
	{

	}
};
*/