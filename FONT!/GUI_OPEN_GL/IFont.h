#pragma once
#include <stdint.h>

struct textsize_s {
	int32_t width, height;
};

struct textextents_s {
	int32_t x, y;
	int32_t width, height;
};

struct textcolor4_s {
	uint8_t r, g, b, a;
};

struct char_glyph_s {
	textsize_s size;
	textsize_s bearing;
	uint32_t texid;
	uint32_t advance;
};

class IFont;

enum fonttypes_e {
	FONTTYPE_TTF = 0,
	FONTTYPE_BITMAP
};

enum fontcreate_errors_e {
	FONT_OK,
	FONT_ERROR_INVALID_PARAMETER,
	FONT_ERROR_ALLOCATE_MEMORY,
	FONT_ERROR_INITIALIZING,
	FONT_ERROR_LOAD,
	FONT_ERROR_NOT_FOUND
};

class IFontManager
{
public:
	virtual IFont *LoadFont(const char *sz_font_name, int32_t type, int32_t size) = 0;
	virtual IFont *FindFont(const char *sz_font_name, int32_t size) = 0;
	virtual uint32_t LastError() = 0;
};

class IFont
{
public:
	/**
	* Get char glyph
	*/
	virtual char_glyph_s *GetCharGlyph(int glyph_index) = 0;

	/**
	* Calculate last text extents points
	*/
	virtual void GetTextExtents(textextents_s *pextents) = 0;

	/**
	* Get last text width
	*/
	virtual uint32_t GetTextWidth() = 0;

	/**
	* Get last text height
	*/
	virtual uint32_t GetTextHeight() = 0;

	/**
	* Set text current color
	*/
	virtual void SetColor(textcolor4_s color) = 0;

	/**
	* Get current text color
	*/
	virtual void GetColor(textcolor4_s *pcolor) = 0;

	/**
	* Render formatted text
	*/
	virtual void RenderText(int x, int y, const char *p_text, ...) = 0;

	/**
	* Render formatted text in 3D space
	*/
	virtual void RenderText3D(float x, float y, float z, const char *p_text, ...) = 0;
};