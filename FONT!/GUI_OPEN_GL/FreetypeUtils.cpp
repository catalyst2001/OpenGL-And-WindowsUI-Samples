//Written by Kristian-Angel Carazo free to use and distribute as you see fit.
//Questions, comments, using this for something interesting? rebelcoder@gmail.com
//

#include "FreetypeUtils.h"

namespace FreetypeUtils {

void CreateDisplayList(FT_Face ftFace, char ch, GLuint uiListBase, GLuint* puiTexBase, uint32* puiMaxPixelWidth, uint32* puiMaxPixelHeight, sSizei* pSizes)
{
	FT_Error ftError = FT_Load_Glyph(ftFace, FT_Get_Char_Index(ftFace, 240), FT_LOAD_DEFAULT);

	FT_Glyph ftGlyph;
	ftError = FT_Get_Glyph( ftFace->glyph, &ftGlyph);

	// Convert The Glyph To A Bitmap.
	FT_Glyph_To_Bitmap(&ftGlyph, ft_render_mode_normal, 0, 1 );
	
	FT_BitmapGlyph ftBitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(ftGlyph);

	// This Reference Will Make Accessing The Bitmap Easier.
	FT_Bitmap& ftBitmap = ftBitmapGlyph->bitmap;


	// Use Our Helper Function To Get The Widths Of
	// The Bitmap Data That We Will Need In Order To Create
	// Our Texture.
	int iWidth  = NextP2(ftBitmap.width);
	int iHeight = NextP2(ftBitmap.rows);

	// Allocate Memory For The Texture Data.
	GLubyte* pExpandedData = new GLubyte[ 2 * iWidth * iHeight];

	// Here We Fill In The Data For The Expanded Bitmap.
	// Notice That We Are Using A Two Channel Bitmap (One For
	// Channel Luminosity And One For Alpha), But We Assign
	// Both Luminosity And Alpha To The Value That We
	// Find In The FreeType Bitmap. 
	// We Use The ?: Operator To Say That Value Which We Use
	// Will Be 0 If We Are In The Padding Zone, And Whatever
	// Is The FreeType Bitmap Otherwise.
	for(int j = 0; j < iHeight;j++) {
		for(int i = 0; i < iWidth; i++){
			pExpandedData[2*(i+j*iWidth)]= pExpandedData[2*(i+j*iWidth)+1] = (i >= ftBitmap.width || j >= ftBitmap.rows) ? 0 : ftBitmap.buffer[i + ftBitmap.width*j];
		}
	}

	// Now We Just Setup Some Texture Parameters.
	glBindTexture( GL_TEXTURE_2D, puiTexBase[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	// Here We Actually Create The Texture Itself, Notice
	// That We Are Using GL_LUMINANCE_ALPHA To Indicate That
	// We Are Using 2 Channel Data.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pExpandedData);

	// With The Texture Created, We Don't Need The Expanded Data Anymore.
	delete [] pExpandedData;

	// Now We Create The Display List
	glNewList(uiListBase+ch, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, puiTexBase[ch]);

	glPushMatrix();

	// First We Need To Move Over A Little So That
	// The Character Has The Right Amount Of Space
	// Between It And The One Before It.
	glTranslatef(static_cast<GLfloat>(ftBitmapGlyph->left), 0, 0);

	// Now We Move Down A Little In The Case That The
	// Bitmap Extends Past The Bottom Of The Line 
	// This Is Only True For Characters Like 'g' Or 'y'.

	// KC - Changed to work with the code change listed below.
	// 
	glTranslatef(0, static_cast<GLfloat>(-(ftBitmapGlyph->top - ftBitmap.rows)), 0);

	// Now We Need To Account For The Fact That Many Of
	// Our Textures Are Filled With Empty Padding Space.
	// We Figure What Portion Of The Texture Is Used By 
	// The Actual Character And Store That Information In
	// The x And y Variables, Then When We Draw The
	// Quad, We Will Only Reference The Parts Of The Texture
	// That Contains The Character Itself.
	float x = (float)ftBitmap.width / (float)iWidth;
	float y = (float)ftBitmap.rows / (float)iHeight;


	pSizes[ch].uiWidth = static_cast<uint32>(ftBitmap.width);
	pSizes[ch].uiHeight = static_cast<uint32>(ftBitmap.rows);

	if(static_cast<uint32>(ftBitmap.width) > *puiMaxPixelWidth)
	{
		*puiMaxPixelWidth = ftBitmap.width;
	}

	if(static_cast<uint32>(ftBitmap.rows) > *puiMaxPixelHeight)
	{
		*puiMaxPixelHeight = ftBitmap.rows;
	}

	// Here We Draw The Texturemapped Quads.
	// The Bitmap That We Got From FreeType Was Not 
	// Oriented Quite Like We Would Like It To Be,
	// But We Link The Texture To The Quad
	// In Such A Way That The Result Will Be Properly Aligned.

	// KC - I changed the following code so that it creates
	// the quads as if the coordinate system is beggning at 0,0 being the
	// top left hand corner and moving down the window is increasing the the positive
	// y axis. For this reason the extens of the quad are created with negative Y components.
	//
	//
	glBegin(GL_QUADS);
		glTexCoord2d(0, 0);				//Bottom left hand corner.
		glVertex2f(0, static_cast<GLfloat>(-ftBitmap.rows));

		glTexCoord2d(0, y);				//Top left
		glVertex2f(0, 0);

		glTexCoord2d(x, y);				//Right  Top
		glVertex2f(static_cast<GLfloat>(ftBitmap.width), 0);

		glTexCoord2d(x, 0);				//Right bottom
		glVertex2f(static_cast<GLfloat>(ftBitmap.width), static_cast<GLfloat>(-ftBitmap.rows));		
	glEnd();

	glPopMatrix();
	
	glTranslatef(static_cast<GLfloat>(ftFace->glyph->advance.x >> 6), 0, 0);

	pSizes[ch].uiWidth = static_cast<uint32>(ftFace->glyph->advance.x >> 6);
	// Increment The Raster Position As If We Were A Bitmap Font.
	// (Only Needed If You Want To Calculate Text Length)
	// glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	// Finish The Display List
	glEndList();
}

void InitializeFont(const std::string& strFontName, unsigned int uiFontHeight, unsigned int** uiFontTextureIds, unsigned int* uiListIds, uint32* puiMaxPixelWidth, uint32* puiMaxPixelHeight, sSizei* pSizes)
{
	*puiMaxPixelWidth = 0;
	*puiMaxPixelHeight = 0;

	// Allocate Some Memory To Store The Texture Ids.
	*uiFontTextureIds = new GLuint[128];

	// Create And Initilize A FreeType Font Library.
	FT_Library library;
	
	FT_Error ftError = FT_Init_FreeType( &library );

	if(ftError != 0)
	{
		MessageBox(NULL, NULL, NULL, MB_OK);
	}
	// The Object In Which FreeType Holds Information On A Given
	// Font Is Called A "face".
	FT_Face face;

	// This Is Where We Load In The Font Information From The File.
	// Of All The Places Where The Code Might Die, This Is The Most Likely,
	// As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
	ftError = FT_New_Face(library, strFontName.c_str(), 0, &face ); 
	
	if(ftError != 0)
	{
		MessageBox(NULL, strFontName.c_str(), strFontName.c_str(), MB_OK);
	}
	// For Some Twisted Reason, FreeType Measures Font Size
	// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
	// h Pixels High, We Need To Request A Size Of h*64.
	// (h << 6 Is Just A Prettier Way Of Writing h*64)
	FT_Set_Char_Size(face, uiFontHeight << 6, uiFontHeight << 6, 96, 96);

	// Here We Ask OpenGL To Allocate Resources For
	// All The Textures And Display Lists Which We
	// Are About To Create.  
	*uiListIds =glGenLists(128);
	glGenTextures(128, *uiFontTextureIds);

	// This Is Where We Actually Create Each Of The Fonts Display Lists.
	for(unsigned char i=0;i<128;i++)
		CreateDisplayList(face, i, *uiListIds, *uiFontTextureIds, puiMaxPixelWidth, puiMaxPixelHeight, pSizes);

	// We Don't Need The Face Information Now That The Display
	// Lists Have Been Created, So We Free The Associated Resources.
	FT_Done_Face(face);

	// Ditto For The Font Library.
	FT_Done_FreeType(library);
}
};