//Written by Kristian-Angel Carazo free to use and distribute as you see fit.
//Questions, comments, using this for something interesting? rebelcoder@gmail.com
//
//The code in this file was taken from someone elses open source project, I dont remember where I found it,
//if you know who this belongs to feel free to contact me so I can give them appropriate credit.
//
#pragma once
#ifndef _FREETYPE_UTILS_H_
#define _FREETYPE_UTILS_H_

#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <windows.h>
#include <gl/GLU.h>
#include <string>

//Unsigned types.
typedef unsigned __int64 uint64;
typedef unsigned int    uint32;
typedef unsigned short    uint16;
typedef unsigned char    ubyte;
typedef unsigned char    uint8;

//Signed types
typedef __int64 int64;
typedef int                int32;
typedef short            int16;
typedef unsigned char    byte;
typedef char            int8;

//floats
typedef float    float32;
typedef double    float64;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#define MegsToBytes(n) (n * 1024) * 1024
#define KiloToByte(n) (n * 1024)

struct sSizei
{
	uint32 uiWidth;
	uint32 uiHeight;

	void Scale(uint32 uiScale) { uiWidth *= uiScale; uiHeight *= uiScale; }
	void GrowBy(uint32 uiAmt) { uiWidth += uiAmt; uiHeight += uiAmt; }
};

inline int NextP2(int iNext)
{
	int iRVal = 1;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2; 
	while (iRVal < iNext) iRVal <<= 1;
	return iRVal;
}

struct Vector2f
{
	float x;
	float y;

	Vector2f(float _x, float _y) : x(_x), y(_y) { }
	Vector2f() : x(0.0f), y(0.0f) { }
};

struct sRGBColor
{
	float32 fRed;
	float32 fGreen;
	float32 fBlue;
	float32 fAlpha;

	sRGBColor(float32 fR = 1.0f, float32 fG = 1.0f, float32 fB = 1.0f, float32 fA = 1.0f) : fRed(fR), fGreen(fG), fBlue(fB), fAlpha(fA) {}
};


struct sSizei;

namespace FreetypeUtils 
{
	//Creates an opengl display list for this font type.
	void CreateDisplayList(FT_Face ftFace, char ch, GLuint uiListBase, GLuint* uiTexBase, uint32* puiMaxPixelWidth, uint32* puiMaxPixelHeight, sSizei* pSizes);

	void InitializeFont(const std::string& strFontName, uint32 uiFontHeight, uint32** uiFontTextureIds, uint32* uiListIds, uint32* puiMaxPixelWidth, uint32* puiMaxPixelHeight, sSizei* pSizes);
};

#endif