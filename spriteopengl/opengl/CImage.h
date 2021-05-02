#pragma once
#include "Opengl.h"

class CImage
{
	uint8_t	*m_pPixelsData;
public:
	uint32_t m_TexID;
	uint32_t m_TexWidth;
	uint32_t m_TexHeight;
	int32_t	m_Channels;

	CImage() {}
	~CImage() {}

	bool CreateTexture(const char *pFileName);
	bool LoadBMP(const char *pFileName);
	bool LoadTGA(const char *pFileName);
	//bool LoadPNG(const char *pFileName);
	//bool LoadPCX(const char *pFileName);
};

