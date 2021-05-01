//TODO: C++ 14 standard requiried
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//C++ libraries
#include <vector>
using namespace std;

#define POINT_IN_RECT(rectx, recty, rectright, recttop, pointx, pointy) (rectx < pointx && recty < pointy && rectright > pointx && recttop > pointy)

//================================================================
// Base Types
//================================================================
struct Vec2 { int x, y; };
struct Rect { Vec2 x1, y1, x2, y2; };
struct Color3 { unsigned char r, g, b; };
struct Color4 { unsigned char r, g, b, a; };

//================================================================
// Main interface
//================================================================
class IBaseGUIElement {
public:
	typedef std::vector<IBaseGUIElement> BaseElemsArray;
	int m_iID;
	bool m_bTouched;
	Vec2 m_Position;
	Rect m_Rect;
	const char *m_pName;
	BaseElemsArray m_ChildElems;

	IBaseGUIElement() {}
	~IBaseGUIElement() {}

	virtual void Create(int x, int y, int width, int height) = 0;
	virtual void MouseMove(int x, int y) = 0;
	virtual void MouseClick(int x, int y, int button) = 0;
	virtual void Shutdown() = 0;
	virtual void Render() = 0;
};

//================================================================
// GUI Canvas
//================================================================
class CCanvas : public IBaseGUIElement {
public:
	CCanvas() {}
	CCanvas(IBaseGUIElement &elem) {
		m_ChildElems.push_back(elem);
	}
	~CCanvas() {}

	void Create(int x, int y, int width, int height)
	{
		m_Rect.x1.x = x;
		m_Rect.x1.y = y;
		m_Rect.x2.x = x + width;
		m_Rect.x2.y = y + height;
	}
};

class CSampleGUI : IBaseGUIElement
{
	enum mousebuttons_e {
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_RIGHT_BUTTON,
		MOUSE_CENTER_BUTTON,
		MOUSE_NUM_BUTTONS
	};

	CSampleGUI() {}
	~CSampleGUI() {}

	void AddControl(IBaseGUIElement *canvas) { m_ChildElems.push_back(*canvas); }
	void Create(int x, int y, int width, int height) {}
	void MouseMove(int x, int y) {}
	void MouseClick(int x, int y, int button) {}


	void Shutdown() {}
	void Render() {}

	void Event_MouseMove(int x, int y)
	{

	}

	void Event_MoveClick(int x, int y, int button)
	{

	}

	void Event_Keyboard(char ch)
	{

	}

public:
	class CInput {
	public:
		//CInput() {}
		//~CInput() {}
		bool m_Buttons[1024];
		bool m_Mousebuttons[MOUSE_NUM_BUTTONS];
	} m_Input;

	//canvas colors
	Color4 m_CanvasColorBackground;
	Color4 m_CanvasColorEdge;
	Color4 m_CanvasColorShadow;
	Color3 m_CanvasColorTitleText;

	//button color
	Color4 m_ButtonColorText;
	Color4 m_ButtonColorBackground;
	Color4 m_ButtonColorBackgroundNavigated;
	Color4 m_ButtonColorEdge;
	Color4 m_ButtonColorShadow;
	
	//checkbox color

};

