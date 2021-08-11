#include "Cgui.h"


bool Cgui::OnCreate(IElementBase * parent)
{
	return false;
}

void Cgui::OnDestroy()
{
}

void Cgui::OnMouseMove(int x, int y)
{
	OnMouseMoveRecurs(this, x, y);
}

void Cgui::OnMouseClick(int x, int y, int button, int state)
{
	OnMouseClickRecurs(this, x, y, button, state);
}

void Cgui::OnDraw()
{
	glDisable(GL_DEPTH_TEST); 
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, (double)m_width, (double)m_height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//render

	OnDrawRecurs(this);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Cgui::OnCharInput(char ch)
{
}

void Cgui::OnKeydown(int key, int flag)
{
}

void Cgui::OnResize(int width, int height)
{
}

void Cgui::AddElement(IElementBase * elem)
{
	childs.push_back(elem);
}
