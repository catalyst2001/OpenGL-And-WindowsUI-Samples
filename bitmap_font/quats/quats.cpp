#include "glwin.h"
#include <math.h>

bool CreateTextureBMP(unsigned int &textureID, const char *szFileName)                          // Creates Texture From A Bitmap File
{
	HBITMAP hBMP;                                                                 // Handle Of The Bitmap
	BITMAP  bitmap;																  // Bitmap Structure

	glGenTextures(1, &textureID);                                                 // Create The Texture
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP)                                                                    // Does The Bitmap Exist?
		return FALSE;                                                           // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap);                                     // Get The Object
																				  // hBMP:        Handle To Graphics Object
																				  // sizeof(bitmap): Size Of Buffer For Object Information
																				  // &bitmap:        Buffer For Object Information

	if (!(bool)bitmap.bmBits)
		MessageBoxA(0, "Error!", "Error loading image", 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureID);                                      // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	// MUST NOT BE INDEX BMP, BUT RGB
	DeleteObject(hBMP);                                                           // Delete The Object

	return TRUE;                                                                  // Loading Was Successful
}

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float quat_t[4];

struct glyph_t {
	vec2_t uv_left;
	vec2_t uv_top;
	vec2_t ub_right;
	vec2_t uv_bottom;
};

struct font_t {
	int textureid;
	int num_of_frames;
	int num_of_glyphs;
	glyph_t *glyphs;
};

bool init_font(font_t *pfont, int texid, int glyphs_per_line)
{
	pfont->num_of_frames = glyphs_per_line;
	pfont->textureid = texid;
	pfont->glyphs = (glyph_t *)malloc(sizeof(glyph_t) * 256);
	if (!pfont->glyphs) {
		//printf("Pizdec!\n");
		return false;
	}

	float glyph_width = 1.f / pfont->num_of_frames;
	for (int i = 0; i < pfont->num_of_frames*pfont->num_of_frames; i++) {
		float x = (i % pfont->num_of_frames) / (float)pfont->num_of_frames;
		float y = (i / pfont->num_of_frames) / (float)pfont->num_of_frames;
		//printf("%f %f\n", x, y);

		//int coords[] = {
		//	x, y,
		//	x + WIDTH, y,
		//	x + WIDTH, y + WIDTH,
		//	x, y + WIDTH,
		//};
		pfont->glyphs[i].uv_left[0] = x;
		pfont->glyphs[i].uv_left[1] = 1.f - y - glyph_width;
		pfont->glyphs[i].uv_top[0] = x + glyph_width;
		pfont->glyphs[i].uv_top[1] = 1.f - y - glyph_width;
		pfont->glyphs[i].ub_right[0] = x + glyph_width;
		pfont->glyphs[i].ub_right[1] = 1.f - y;
		pfont->glyphs[i].uv_bottom[0] = x;
		pfont->glyphs[i].uv_bottom[1] = 1.f - y;
	}
	//for (int index = 2; index < 2 + 3; index++) {
	//	printf("glyph: %c  coords (%f %f) (%f %f) (%f %f) (%f %f)\n", 32 + index,
	//		pfont->glyphs[index].uv_left[0], pfont->glyphs[index].uv_left[1],
	//		pfont->glyphs[index].uv_top[0], pfont->glyphs[index].uv_top[1],
	//		pfont->glyphs[index].ub_right[0], pfont->glyphs[index].ub_right[1],
	//		pfont->glyphs[index].uv_bottom[0], pfont->glyphs[index].uv_bottom[1]
	//	);
	//}
	return true;
}

//0.0625f - one glyph
#define WIDTH 15
#define offset 1.f
void Print(font_t *pfont, int posx, int posy, const char *text)
{
	int x = posx, y = posy;
	for (int i = 0; text[i] != 0x0; i++) {
		if (text[i] == '\n') {
			y -= WIDTH;
			x = posx;
		}
		int idx = text[i] - 32;

		//check char bounds
		if (idx < 0)
			idx = 0;
		if (idx > 255)
			idx = 255;

		int coords[] = {
			x, y,
			x + WIDTH, y,
			x + WIDTH, y + WIDTH,
			x, y + WIDTH,
		};
		glBindTexture(GL_TEXTURE_2D, pfont->textureid);
		glVertexPointer(2, GL_INT, 0, coords);
		glTexCoordPointer(2, GL_FLOAT, 0, &pfont->glyphs[idx]);
		glDrawArrays(GL_QUADS, 0, 4);
		x += (int)WIDTH - WIDTH / 9.f;
	}
}

//C++ 11 standard requiried
int g_width = 800;
int g_height = 600;
unsigned int texid;

font_t font;

void OnCreate(HWND wnd)
{
	if (!CreateTextureBMP(texid, "font.bmp")) {
		printf("Error loading texture!\n");
	}
	init_font(&font, texid, 16);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
}

void OnClose(HWND wnd)
{
	PostQuitMessage(0);
	exit(0);
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_width, 0, g_height, -1, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	Print(&font, 20, 400, "privet kavo\n1)Roma Krasavchik :O\n2)Kirill math ychit nado :)\n3)Spokoinoi nochi");
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	printf("Window size:  (%dx%d)\n", width, height);
}

void OnMouseMove(int x, int y)
{
}

void OnMouseClick(int x, int y, int button, int state)
{
}

void OnChar(char ch)
{
	if (ch == 27) //esc code
		exit(0);
}

int main()
{
	create_window("tests", "glwindow2021", 24, 32, Draw, OnResize, OnMouseMove, OnMouseClick, OnChar, OnCreate, OnClose, 0, 0, g_width, g_height);
	return 0;
}
