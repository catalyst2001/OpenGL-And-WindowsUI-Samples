//Written by Kristian-Angel Carazo free to use and distribute as you see fit.
//Questions, comments, using this for something interesting? rebelcoder@gmail.com
//

#include "ftfont.h"
#include "FreetypeUtils.h"

CFTFont::CFTFont(void) : m_glTextureIds(NULL), m_uiMaxPixelHeight(0), m_uiMaxPixelWidth(0)
{
	memset(&m_ftSizes, 0, sizeof(m_ftSizes));
}

CFTFont::~CFTFont(void)
{
}

void RenderText(CFTFont* pFont, const char* strText, Vector2f vtPosition, float32 fScale, const sRGBColor& rgbColor)
{
	GLuint font = pFont->m_uiBase;
	// We Make The Height A Little Bigger.  There Will Be Some Space Between Lines.
	float h = pFont->m_uiFontHeight / .63f;

	// Here Is Some Code To Split The Text That We Have Been
	// Given Into A Set Of Lines.  
	// This Could Be Made Much Neater By Using
	// A Regular Expression Library Such As The One Available From
	// boost.org (I've Only Done It Out By Hand To Avoid Complicating
	// This Tutorial With Unnecessary Library Dependencies).
	const char *start_line = strText;

	std::vector<std::string> lines;

	const char *c = strText;
	for (; *c; c++)
	{
		if (*c == '\n')
		{
			std::string line;

			for (const char *n = start_line; n < c; n++)
				line.append(1, *n);

			lines.push_back(line);

			start_line = c + 1;
		}
	}

	if (start_line)
	{
		std::string line;

		for (const char *n = start_line; n < c; n++)
			line.append(1, *n);

		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(font);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	// This Is Where The Text Display Actually Happens.
	// For Each Line Of Text We Reset The Modelview Matrix
	// So That The Line's Text Will Start In The Correct Position.
	// Notice That We Need To Reset The Matrix, Rather Than Just Translating
	// Down By h. This Is Because When Each Character Is
	// Drawn It Modifies The Current Matrix So That The Next Character
	// Will Be Drawn Immediately After It.  

	//set the current color to white.
	glColor4f(rgbColor.fRed, rgbColor.fGreen, rgbColor.fBlue, rgbColor.fAlpha);

	for (uint32 i = 0; i < lines.size(); i++) {
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(vtPosition.x, vtPosition.y - h * i, 0);
		glScalef(fScale, fScale, 0.0f);
		glMultMatrixf(modelview_matrix);

		// The Commented Out Raster Position Stuff Can Be Useful If You Need To
		// Know The Length Of The Text That You Are Creating.
		// If You Decide To Use It Make Sure To Also Uncomment The glBitmap Command
		// In make_dlist().
		// glRasterPos2f(0,0);
		glCallLists(static_cast<GLsizei>(lines[i].length()), GL_UNSIGNED_BYTE, lines[i].c_str());
		// float rpos[4];
		// glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
		// float len=x-rpos[0]; (Assuming No Rotations Have Happend)

		glPopMatrix();
	}

	glPopAttrib();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void CFTFont::Init(const std::string& strFileName, uint32 uiHeight)
{
	m_uiFontHeight = uiHeight;
	FreetypeUtils::InitializeFont(strFileName, uiHeight, &m_glTextureIds, &m_uiBase, &m_uiMaxPixelWidth, &m_uiMaxPixelHeight, m_ftSizes);
}

void CFTFont::Clean()
{
	glDeleteLists(m_uiBase,128);
	glDeleteTextures(128, m_glTextureIds);
	delete [] m_glTextureIds;
}

uint32 CFTFont::CalcTextWidth(const std::string& strText)
{
	uint32 uiPixelLength = 0;
	for(uint32 i = 0; i < strText.size(); i++)
	{
		uiPixelLength += m_ftSizes[static_cast<uint32>(strText[i])].uiWidth;
	}

	return uiPixelLength;
}

uint32 CFTFont::CalcTextHeight(const std::string& strText)
{
	return m_uiMaxPixelHeight;
}

void CFTFont::CalcExtents(const std::string& strText, uint32& uiWidth, uint32& uiHeight)
{
	uiWidth = CalcTextWidth(strText);
	uiHeight = CalcTextHeight(strText);
}