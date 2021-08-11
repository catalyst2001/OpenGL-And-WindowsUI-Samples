#pragma once
#include "Cgui.h"

//
//								CProgressBar
//
//

class CProgressBar : public IElementBase
{
	CColor3 m_color_one;
	CColor3 m_color_too;
	double m_percent;
	CRect rect_prog;
	void ProgressRect(CRect &rect, int x, int y, double proc, int width, int height) {
		int xx = convert(proc, 0.0, 100.0, x, width);
		rect.p1 = CPoint(x, y);
		rect.p2 = CPoint(xx, y);
		rect.p3 = CPoint(xx, height);
		rect.p4 = CPoint(x, height);
	}
public:
	CProgressBar() {}
	CProgressBar(int x, int y, int width, int height, float percent, CColor3 color_rect, CColor3 color_progres) {
		m_percent = percent;
		m_color_one = color_rect;
		m_color_too = color_progres;
		rect = CRect(x, y, width, height);
		flag = UI_ELEMENT_ENABLED;
	}
	~CProgressBar() {}
	__forceinline void SetPercent(float percent) { m_percent = percent; }
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
		ProgressRect(rect_prog, rect.p1.x, rect.p1.y, m_percent, rect.p2.x, rect.p3.y);
		glColor3ubv((uchar*)&m_color_one);
		glVertexPointer(2, GL_INT, 0, &rect);
		glDrawArrays(GL_QUADS, 0, 4);
		glColor3ubv((uchar*)&m_color_too);
		glVertexPointer(2, GL_INT, 0, &rect_prog);
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