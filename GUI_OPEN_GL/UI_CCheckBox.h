#pragma once
#include "Cgui.h"

//
//								CProgressBar
//
//

class CCheckBox : public IElementBase
{
	bool m_check;
	CColor3 m_color;
	CColor3 m_color_on;
public:
	CCheckBox() {}
	CCheckBox(int x, int y, int width, int height, bool state, CColor3 color) {
		m_check = state;
		m_color = color;
		m_color_on = color + 100;
		rect = CRect(x, y, width, height);
		flag = UI_ELEMENT_ENABLED;
	}
	~CCheckBox() {}
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
		if (rect.PointInRect(x, y) && button == 0 && state == 0)
			m_check = !m_check;
		//m_check = rect.PointInRect(x, y);

	}
	void OnDraw()
	{
		m_check ? glColor3ubv((uchar *)&m_color_on) : glColor3ubv((uchar *)&m_color);

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