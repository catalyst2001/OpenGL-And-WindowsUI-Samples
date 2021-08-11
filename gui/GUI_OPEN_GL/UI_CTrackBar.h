#pragma once
#include "Cgui.h"

//
//								CProgressBar
//
//

class CTrackBar : public IElementBase
{
	bool m_on;
	CRect rect_line;
	CRect rect_logic;
	CColor3 m_color_track;
	CColor3 m_color_line;

	void Set_Track_Pos(int x) {
		int center, center_difference;
		center_difference = x - (rect.p2.x - ((rect.p2.x - rect.p1.x) / 2));
		if (rect_logic.p1.x <= (center = (rect.p3.x + center_difference)) && rect_logic.p2.x >= center) {
			rect.MoveX(center_difference);
			percent = convert(center, rect_logic.p1.x, rect_logic.p2.x, 0, 100);
		}
	}
public:
	float percent;
	CTrackBar() {}
	CTrackBar(int x, int y, int width, int height, CColor3 color) {
		m_color_line = { 230, 230, 230 };
		m_color_track = color;
		rect = CRect(x, y, 8, height);
		rect_logic = CRect(x, y, width, height);
		rect_line = CRect(x, y - ((height / 2) - 2), width, 2);
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
		if (m_on) Set_Track_Pos(x);
	}
	void OnMouseClick(int x, int y, int button, int state)
	{
		if ((m_on = (rect_logic.PointInRect(x, y) && state == 0 && button == 0))) Set_Track_Pos(x);
	}
	void OnDraw()
	{

		glColor3ubv((uchar *)&m_color_line);
		glVertexPointer(2, GL_INT, 0, &rect_line);
		glDrawArrays(GL_QUADS, 0, 4);

		glColor3ubv((uchar *)&m_color_track);
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