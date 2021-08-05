#pragma once
#include "Cgui.h"

//
//								CEditbox
//
//

class CEditbox : public IElementBase
{
	CColor3 m_color;
public:
	CEditbox() {}
	CEditbox(int x, int y, int width, int height, CColor3 color) {
		m_color = color;
		rect = CRect(x, y, width, height);
		flag = UI_ELEMENT_ENABLED;
	}
	~CEditbox() {}
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
		glColor3ubv((uchar *)&m_color);
		glVertexPointer(2, GL_INT, 0, &rect);
		glDrawArrays(GL_QUADS, 0, 4);
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