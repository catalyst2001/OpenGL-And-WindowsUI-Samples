#pragma once
#include <stdint.h> //uint32_t
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <windows.h>
#include <gl/GLU.h>
#include <string.h>

#include <vector>

#include "IFont.h"

#define FONT_TOTAL_GLYPHS 128
#define FONT_RESOLUTION 96

class CColor4 {
public:
	CColor4(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t aa) : r(rr), g(gg), b(bb), a(aa) {}
	~CColor4() {}

	uint8_t r, g, b, a;
};

class ÑFont /*: public IFont*/
{
	FT_Face m_pFontFace;
	const char *m_pFontName;
	char_glyph_s *m_pFontGlyphs;
public:
	friend class CFontManager;

	char_glyph_s *GetCharGlyph(int glyph_index) {
		return nullptr;
	}
	void GetTextExtents(textextents_s *pextents) {

	}
	uint32_t GetTextWidth() {
		return 1;
	}
	uint32_t GetTextHeight() {
		return 1;
	}
	void SetColor(textcolor4_s color) {

	}
	void GetColor(textcolor4_s *pcolor) {
	}

	void RenderText(int x, int y, const char *p_text, ...) {
		glEnable(GL_TEXTURE_2D);
		float scale = 2.0;

		for (; *p_text; p_text++) {
			int idx = *p_text - 32;
			if (idx < 0)
				idx = 0;
			if (idx > 127)
				idx = 127;

			char_glyph_s *p_current_glyph = &m_pFontGlyphs[idx];
			int xpos = x + p_current_glyph->bearing.width * scale;
			int ypos = y - (p_current_glyph->size.height - p_current_glyph->bearing.height) * scale;
			int w = p_current_glyph->size.width * scale;
			int h = p_current_glyph->size.height * scale;
			//int vertices[] = {
			//	xpos, ypos + h,
			//	xpos, ypos,
			//	xpos + w, ypos,
			//	xpos + w, ypos + h
			//};

			float texcoords[] = {
				0.0, 0.0,
				0.0, 1.0,
				1.0, 1.0,

				0.0, 0.0,
				1.0, 1.0,
				1.0, 0.0
			};

			int vertices[] = {
				xpos,     ypos + h,
				xpos,     ypos,    
				xpos + w, ypos,    
				xpos,     ypos + h,
				xpos + w, ypos,    
				xpos + w, ypos + h,
			};

			//float uvs[] = {
			//	1.f, 0.f,
			//	0.f, 0.f,
			//	0.f, 1.f,
			//	1.f, 1.f
			//};

			glBindTexture(GL_TEXTURE_2D, p_current_glyph->texid);
			glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
			glVertexPointer(2, GL_INT, 0, vertices);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (p_current_glyph->advance >> 6) * scale + 5; //TODO: +5 - FOR TESTS!!!
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RenderText3D(float x, float y, float z, const char *p_text, ...) {

	}
};

class CFontManager/* : public IFontManager*/
{
	uint32_t m_nLastError;
	FT_Library m_pFreeTypeLibrary;
	std::vector<ÑFont *> m_pFonts;

	__forceinline void FontSetLastError(int error) { m_nLastError = error; }
public:
	CFontManager() {}
	~CFontManager() {}

	int Init() {
		if (FT_Init_FreeType(&m_pFreeTypeLibrary))
			return FONT_ERROR_INITIALIZING;
		
		return FONT_OK;
	}

	uint32_t LastError() { return m_nLastError; }

	ÑFont *LoadFont(const char *sz_font_name, int32_t type, int32_t size) {
		int error = FONT_OK;
		ÑFont *p_font = nullptr;
		p_font = (ÑFont *)FindFont(sz_font_name, type);
		if (p_font)
			return (ÑFont *)p_font;

		//if font not loaded
		p_font = (ÑFont*)malloc(sizeof(ÑFont));
		if (!p_font) {
			error = FONT_ERROR_ALLOCATE_MEMORY;
			goto __error;
		}

		p_font->m_pFontGlyphs = (char_glyph_s *)malloc(sizeof(char_glyph_s) * FONT_TOTAL_GLYPHS);
		if (!p_font->m_pFontGlyphs) {
			error = FONT_ERROR_ALLOCATE_MEMORY;
			goto __error;
		}
		
		if (FT_New_Face(m_pFreeTypeLibrary, sz_font_name, 0, &p_font->m_pFontFace)) {
			error = FONT_ERROR_LOAD;
			goto __error;
		}

		FT_Face this_font_face;
		this_font_face = p_font->m_pFontFace;
		FT_Set_Pixel_Sizes(this_font_face, 0, size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		for (uint8_t c = 0; c < 128; c++) {
			if (FT_Load_Char(p_font->m_pFontFace, c, FT_LOAD_RENDER)) //if char not loaded
				continue;

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this_font_face->glyph->bitmap.width, this_font_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, p_font->m_pFontFace->glyph->bitmap.buffer);
#define GL_CLAMP_TO_EDGE 0x812F //TODO: GLEW â ïîìîùü
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			char_glyph_s glyph;
			glyph.texid = texture;
			glyph.size = { this_font_face->glyph->bitmap.width, this_font_face->glyph->bitmap.rows };
			glyph.bearing = { this_font_face->glyph->bitmap_left, this_font_face->glyph->bitmap_top };
			glyph.advance = this_font_face->glyph->advance.x;
			p_font->m_pFontGlyphs[c] = glyph; //insert(std::pair<GLchar, Character>(c, character));
		}
		FT_Done_Face(this_font_face);
		m_pFonts.push_back(p_font); //add loaded font to font manager
		FontSetLastError(error); //set FONT_OK
		return (ÑFont *)p_font;

	__error:
		free(p_font);
		FontSetLastError(error); //set current error
		return nullptr;
	}

	ÑFont *FindFont(const char *sz_font_name, int32_t size) {
		for (int i = 0; i < m_pFonts.size(); i++) {
			if (!strcmp(m_pFonts[i]->m_pFontName, sz_font_name))
				return (ÑFont *)m_pFonts[i];
		}
		return nullptr;
	}

};