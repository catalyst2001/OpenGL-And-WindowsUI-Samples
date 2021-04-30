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
	typedef std::vector<IBaseGUIElement> BaseElemsArray;
	int m_iID;
	bool m_bTouched;
	Vec2 m_Position;
	Rect m_Rect;
	const char *m_pName;
	BaseElemsArray m_ChildElems;

public:
	IBaseGUIElement() {}
	~IBaseGUIElement() {}

	virtual void Create() = 0;
	virtual void Shutdown() = 0;
	virtual void Render() = 0;
};

//================================================================
// GUI Canvas
//================================================================
class CCanvas : public IBaseGUIElement {
public:
	CCanvas() {}
	~CCanvas() {}

	void Create()
	{

	}
};

class SampleGUI
{



public:
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

