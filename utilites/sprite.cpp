#include "sprite.h"

/*
	UINT	m_nTexture;
	float	m_fTextureCoordsArray[8];
	float	m_fOffset;
	int		m_nFrame;
	int		m_nFPS;
	int		m_nposx;
	int		m_nposy;
	int		m_nwidth;
	int		m_nheight;
*/

CSprite::CSprite()
{
}

CSprite::CSprite(int posx, int posy, int width, int height, int frame, int fps, UINT texture)
{
	m_nTexture = texture;
	m_nFrame = frame;
	m_nFPS = fps;
	m_nposx = posx;
	m_nposy = posy;
	m_nwidth = width;
	m_nheight = height;
	m_nloop = 0;
}

CSprite::~CSprite()
{
}


void CSprite::Update()
{
	if (m_nloop == (m_nFrame)) {
		m_nloop = 0;
	}
	if (AnimateNextFrame(m_nFPS)) {
		LogicAnimSprite();
		m_nloop++;
	}
	renderSprite();
	//DrawTexturedRect();
}

void CSprite::SetFps(int fps)
{
	m_nFPS = fps;
}

void CSprite::SetPos(int posx, int posy, int width, int height)
{
	m_nposx = posx;
	m_nposy = posy;
	m_nwidth = width;
	m_nheight = height;
}

void CSprite::LogicAnimSprite() {
	m_nNum = sqrt(m_nFrame);
	m_fOffset = 1.0f / m_nNum;
	float cx = float(m_nloop % m_nNum) / float(m_nNum);
	float cy = float(m_nloop / m_nNum) / float(m_nNum);

	//x							
	m_fTextureCoordsArray[0] = cx;
	m_fTextureCoordsArray[2] = cx;
	m_fTextureCoordsArray[4] = cx + m_fOffset;
	m_fTextureCoordsArray[6] = cx + m_fOffset;
	//y				  = 
	m_fTextureCoordsArray[3] = 1.0f - cy - m_fOffset;
	m_fTextureCoordsArray[5] = 1.0f - cy - m_fOffset;
	m_fTextureCoordsArray[1] = 1.0f - cy - 0.001f;
	m_fTextureCoordsArray[7] = 1.0f - cy - 0.001f;
}

bool CSprite::AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f; // Get the time (milliseconds = seconds * .001)
	float deltaTime = currentTime - lastTime; // Get the slice of time
	float desiredFPS = 1.0f / desiredFrameRate; // Store 1 / desiredFrameRate

	elapsedTime += deltaTime; // Add to the elapsed time
	lastTime = currentTime; // Update lastTime

	// Check if the time since we last checked is greater than our desiredFPS
	if (elapsedTime > desiredFPS)
	{
		elapsedTime -= desiredFPS; // Adjust the elapsed time

		// Return true, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}

void CSprite::renderSprite() {
	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	int coords[] = {
	m_nposx, m_nposy + m_nheight,
	m_nposx, m_nposy,
	m_nposx + m_nwidth, m_nposy,
	m_nposx + m_nwidth, m_nposy + m_nheight
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, m_fTextureCoordsArray);
	glDrawArrays(GL_QUADS, 0, 4);
}

void CSprite::DrawTexturedRect() {
	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	int coords[] = {
		m_nposx, m_nposy + m_nheight,
		m_nposx, m_nposy,
		m_nposx + m_nwidth, m_nposy,
		m_nposx + m_nwidth, m_nposy + m_nheight
	};
	float textureCoordsArray2[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray2);
	glDrawArrays(GL_QUADS, 0, 4);
}
