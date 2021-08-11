#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
using namespace std;

typedef unsigned char uchar;
#define POINT_IN_RECT(rectx, recty, rectright, recttop, pointx, pointy) (rectx < pointx && recty < pointy && rectright > pointx && recttop > pointy)

namespace glp
{
	static float defuvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	static void draw_rectv(void *coords)
	{
		glVertexPointer(2, GL_INT, 0, coords);
		glDrawArrays(GL_QUADS, 0, 4);
	}

	static void draw_textured_rectv(void *coords, float *uvs, unsigned int texid)
	{
		glBindTexture(GL_TEXTURE_2D, texid);
		glVertexPointer(2, GL_INT, 0, coords);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glDrawArrays(GL_QUADS, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static void draw_unfilled_rectv(void *verts)
	{
		glVertexPointer(2, GL_INT, 0, verts);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}

	static void draw_unfilled_rect(int x, int y, int width, int height)
	{
		int verts[] = {
			x - 1, y,
			x + width, y,
			x + width, y + height,
			x, y + height
		};
		glVertexPointer(2, GL_INT, 0, verts);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
};

class CColor3 {
public:
	CColor3() {}
	CColor3(uchar _r, uchar _g, uchar _b) : r(_r), g(_g), b(_b) {}

	unsigned char r, g, b;
};

class CPoint {
public:
	CPoint() {}
	~CPoint() {}
	CPoint(int _x, int _y) : x(_x), y(_y) {}
	int x, y;
};

class CRect {
public:
	CRect() {}
	~CRect() {}
	CRect(int x, int y, int width, int height) {
		m_p1 = CPoint(x - 1, y);
		m_p2 = CPoint(x + width, y);
		m_p3 = CPoint(m_p2.x, y + height);
		m_p4 = CPoint(x, m_p3.y);
	}
	CPoint m_p1;
	CPoint m_p2;
	CPoint m_p3;
	CPoint m_p4;
};

class IGuiElement {
public:
	virtual void Draw() = 0;
	virtual void MouseMove(int x, int y) = 0;
	virtual void MouseClick(int x, int y, int button) = 0;
	virtual void OnKeyboardButton(int sym, int action) = 0;
	virtual void OnCharInput(int sym) = 0;

	bool m_bActive = true;
	int m_nPosX, m_nPosY;
	int m_nWidth, m_nHeight;
	std::vector<IGuiElement *> m_pChilds;
	CRect m_Rect;
};

//TODO: user rect for optimize
class CCanvas : public IGuiElement {
public:
	CCanvas() {}
	CCanvas(int x, int y, int width, int height, CColor3 bkg, CColor3 edg) : m_BackgroundColor(bkg), m_EdgeColor(edg) {
		m_nPosX = x;
		m_nPosY = y;
		m_nWidth = width;
		m_nHeight = height;
		m_Rect = CRect(x, y, width, height);
		m_bActive = true;
	}
	~CCanvas() {}

	void Draw() {
		glColor3ub(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b);
		glp::draw_rectv(&m_Rect);
		glColor3ub(m_EdgeColor.r, m_EdgeColor.g, m_EdgeColor.b);
		glp::draw_unfilled_rectv(&m_Rect);
	}

	void AddElement(IGuiElement *pelement) {
		m_pChilds.push_back(pelement);
	}

	void MouseMove(int x, int y) {
	}

	void MouseClick(int x, int y, int button) {
	}

	int Create() {
		return 0;
	}

	void OnKeyboardButton(int sym, int action)
	{

	}

	void OnCharInput(int sym)
	{

	}

private:
	CColor3 m_BackgroundColor;
	CColor3 m_EdgeColor;
};

/**
* Buttons
*/
enum button_flags {
	BTF_STANDARD = (1 << 0),
	BTF_TOGGLE = (1 << 1),
	BTF_CHECK = (1 << 2),
	BTF_RADIO = (1 << 3),
	BTF_IMAGE = (1 << 4),
	BTF_SWITCH = (1 << 5),
	BTF_TEXTALIGNCENTER = (1 << 6)
};

/**
* IButtonHandler
* interface for handling button actions
*/
class IButtonHandler
{
public:
	virtual void OnMouseButtonDown(int id, int button, int state) = 0;
	virtual bool OnClick(int id, CPoint &point) = 0;
	virtual bool OnHoverSet(int id) = 0;
	virtual bool OnHoverLoss(int id) = 0;
};

struct UIColorStyles
{
	CColor3 canvas_background;
	CColor3 canvas_edge;

	CColor3 button_background;
	CColor3 button_background_hover;
	CColor3 button_edge;
	CColor3 button_edge_hover;
};

class CButton : public IGuiElement
{
#define BT_STANDARD_WIDTH 100
#define BT_STANDARD_HEIGHT 25
#define BT_CHECK_SIZE 20
#define BT_RADIO_SIZE 20
#define BT_SWITCH_WIDTH 45
#define BT_SWITCH_HEIGHT 20
	//friend class IButtonHandler; //TODO: for handler 'this' ptr
public:
	CButton() {}
	CButton(const char *name, int id, int x, int y, IButtonHandler *handler) {
		m_pName = name;
		m_nUniqueIdentifier = id;
		m_nFlags = BTF_STANDARD;
		m_Rect = CRect(x, y, BT_STANDARD_WIDTH, BT_STANDARD_HEIGHT);
		m_pHanlder = handler;
		//m_bActive = true;
		SetDefaultColor();
	}

	void SetDefaultColor()
	{
		m_BackgroundColor = CColor3(40, 40, 40);
		m_EdgeColor = CColor3(50, 50, 50);
		m_BackgroundColorHover = CColor3(60, 60, 60);
		m_EdgeColorHover = CColor3(70, 70, 70);
	}

	CButton(const char *name, int id, int x, int y, int width, int height, long flags, IButtonHandler *handler) {
		m_pName = name;
		m_nUniqueIdentifier = id;
		m_Rect = CRect(x, y, width, height);
		m_nFlags = flags;
		m_pHanlder = handler;
		//m_bActive = true;
		SetDefaultColor();
	}

	void Draw()
	{
		//TODO: copy colors 
		CColor3 background = m_bHover ? m_BackgroundColorHover : m_BackgroundColor;
		CColor3 edge = m_bHover ? m_EdgeColorHover : m_EdgeColor;
		glColor3ubv((unsigned char *)&background);
		glp::draw_rectv(&m_Rect);
		glColor3ubv((unsigned char *)&edge);
		glp::draw_unfilled_rectv(&m_Rect);
	}

	void MouseMove(int x, int y)
	{
		m_bHover = (bool)POINT_IN_RECT(m_Rect.m_p1.x, m_Rect.m_p1.y, m_Rect.m_p3.x, m_Rect.m_p3.y, x, y);
	}

	void MouseClick(int x, int y, int button)
	{

	}

	void OnKeyboardButton(int sym, int action)
	{

	}

	void OnCharInput(int sym)
	{

	}

private:
	CColor3 m_BackgroundColor;
	CColor3 m_EdgeColor;
	CColor3 m_BackgroundColorHover;
	CColor3 m_EdgeColorHover;
	IButtonHandler *m_pHanlder;
	const char *m_pName;
	int m_nUniqueIdentifier;
	bool m_bHover;
	long m_nFlags;
};

class CGUI : public IGuiElement
{
public:
	CGUI() {}
	CGUI(int width, int height) {
		m_nWidth = width;
		m_nHeight = height;
	}
	~CGUI() {}

	void OnResize(int width, int height)
	{
		m_nWidth = width;
		m_nHeight = height;
	}

	void AddElement(IGuiElement *pelem)
	{
		m_pChilds.push_back(pelem);
	}

	void DrawChild(std::vector<IGuiElement *> child)
	{
		for (int i = 0; i < child.size(); i++) {
			if(child[i]->m_bActive)
				child[i]->Draw();

			DrawChild(child[i]->m_pChilds);
		}
	}

	void Draw() { DrawChild(m_pChilds); }

	void MouseMoveChilds(std::vector<IGuiElement *> child, int x, int y)
	{
		for (int i = 0; i < child.size(); i++) {
			if (child[i]->m_bActive)
				child[i]->MouseMove(x, y);

			MouseMoveChilds(child[i]->m_pChilds, x, y);
		}
	}

	void MouseMove(int x, int y) {
		MouseMoveChilds(m_pChilds, x, y);
	}

	void MouseClickChilds(std::vector<IGuiElement *> child, int x, int y, int button)
	{
		for (int i = 0; i < child.size(); i++) {
			if (child[i]->m_bActive)
				child[i]->MouseClick(x, y, button);

			MouseClickChilds(child[i]->m_pChilds, x, y, button);
		}
	}

	void MouseClick(int x, int y, int button) {
		MouseClickChilds(m_pChilds, x, y, button);
	}

	int Create() {
		return 0;
	}

	void OnKeyboardButton(int sym, int action)
	{

	}

	void OnCharInput(int sym)
	{

	}

private:
	int m_nWidth, m_nHeight;
};

