//Written by Kristian-Angel Carazo free to use and distribute as you see fit.
//Questions, comments, using this for something interesting? rebelcoder@gmail.com
//

#ifndef _FTFONT_H_
#define _FTFONT_H_

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <windows.h>
#include <vector>
#include "FreetypeUtils.h"
#include <string>

const uint32 uiAlphabetSize = 256;


class CFTFont
{
public:

	uint32 m_uiMaxPixelHeight;
	uint32 m_uiMaxPixelWidth;

	int  m_uiFontHeight;
	GLuint* m_glTextureIds;
	GLuint  m_uiBase;

	sSizei m_ftSizes[uiAlphabetSize];

public:
	CFTFont(void);
	~CFTFont(void);

	void Init(const std::string& strFileName, uint32 uiHeight);
	void Clean();

	uint32 CalcTextWidth(const std::string& strText);
	uint32 CalcTextHeight(const std::string& strText);
	void CalcExtents(const std::string& strText, uint32& uiWidth, uint32& uiHeight);
};

void RenderText(CFTFont* pFont, const char* strText, Vector2f vtPosition, float32 fScale = 1.0f, const sRGBColor& rgbColor = sRGBColor(1.0f, 1.0f, 1.0f));

#endif