#pragma once

#include <stdio.h> 
#include <stdlib.h> //malloc, calloc, realloc, free
#include <Windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma warning(disable:4996)

class CSprite
{
public:
	CSprite(int posx, int posy, int width, int height, int frame, int fps, UINT texture);
	CSprite();
	~CSprite();
	void Update();
	void SetFps(int fps);
	void SetPos(int posx, int posy, int width, int height);
	//setfps
private:
	void LogicAnimSprite();
	void renderSprite();
	void DrawTexturedRect();
	bool AnimateNextFrame(int desiredFrameRate);
	UINT m_nTexture;
	float m_fTextureCoordsArray[8];
	float m_fOffset;
	int m_nFrame;
	int m_nFPS;
	int m_nNum;
	int m_nloop;
	int m_nposx;
	int m_nposy;
	int m_nwidth;
	int m_nheight;
};

