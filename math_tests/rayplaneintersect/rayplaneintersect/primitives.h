#pragma once
#include <Windows.h>
#include <gl/GL.h>

class CCube3D
{
public:
	enum cubedrawmodes_e {
		CUBE_FILL = 0,
		CUBE_WIRE
	};

	CCube3D();
	~CCube3D();

	float SetSize(float size);
	void SelectSide(int side);
	void SetDrawMode(int mode);
	void SetPos(float x, float y, float z);
	void Draw();
};

