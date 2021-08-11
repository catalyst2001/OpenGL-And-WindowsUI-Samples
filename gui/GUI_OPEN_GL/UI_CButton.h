#pragma once
#include "Cgui.h"

//
//                         CButton               flags (1 << 1) = текстура, (1 << 2) = текстура с альфа
//                          120 +
//

#define UI_BTN_TEXTURED (1 << 1)
#define UI_BTN_TEXTURE_WITH_ALPHA (1 << 2)

class CButton : public IElementBase
{
	CColor3 m_color;
	CColor3 m_color_press;
	CColor3 m_color_hover;
	bool m_StatusButton;
	bool m_StatusButtonCry;
	unsigned int m_texid;
	CColor3 gg = { 244,244,244 }, bb = { 10,10,10 };
public:
	CButton() {}
	CButton(int x, int y, int wight, int height, CColor3 color, long flag_s, unsigned int texture) {
		m_StatusButton = false;
		m_StatusButtonCry = false;
		m_color = color;
		m_color_hover = color - 20;
		m_color_press = color - 60;
		m_texid = texture;
		rect = CRect(x, y, wight, height);
		flag = UI_ELEMENT_ENABLED | flag_s;
	}
	~CButton() {}
	bool OnCreate(IElementBase * parent)
	{
		return TRUE;
	}
	void OnDestroy()
	{

	}
	void OnMouseMove(int x, int y)
	{
		m_StatusButton = rect.PointInRect(x, y);
	}
	void OnMouseClick(int x, int y, int button, int state)
	{
		if (rect.PointInRect(x, y) && button == 0 && state == 0) {
			m_StatusButtonCry = true;
		}
		else if (button == 0 && state == 1) {
			m_StatusButtonCry = false;
		}
		printf("x = %d, y = %d, button = %d, state = %d,  m_StatusButton = %d, m_StatusButtonCry = %d \n", x, y, button, state, m_StatusButton, m_StatusButtonCry);
	}
	void OnDraw()
	{
		if (flag & UI_BTN_TEXTURED) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_texid);
			//glColor3f(1.f, 1.f, 1.f);
		}
		if (m_StatusButton) {
			glColor3ubv((uchar *)&m_color_hover);
		}
		else if (!m_StatusButton) {
			glColor3ubv((uchar *)&m_color);
		}
		if (m_StatusButtonCry) {
			glColor3ubv((uchar *)&m_color_press);
		}
		else if (!m_StatusButton) {
			glColor3ubv((uchar *)&m_color);
		}
		glVertexPointer(2, GL_INT, 0, &rect);
		glDrawArrays(GL_QUADS, 0, 4);
		//DrawShadowRect(rect, &gg, &bb, 1);
		if (flag & UI_BTN_TEXTURED) {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
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