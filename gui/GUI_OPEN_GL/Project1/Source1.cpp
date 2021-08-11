//==================================================================================
// include:
//==================================================================================
#include "glwin.h" 
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//==================================================================================
// Types:
//==================================================================================

typedef float vec3_t[3];
typedef float texturecord[8];

//==================================================================================
// Structur:
//==================================================================================

typedef struct														// Create A Structure
{
	GLubyte	*imageData;												// Image Data (Up To 32 Bits)
	GLuint	bpp;													// Image Color Depth In Bits Per Pixel.
	GLuint	width;													// Image Width
	GLuint	height;													// Image Height
	GLuint	texID;													// Texture ID Used To Select A Texture
} TextureImage;														// Structure Name


//==================================================================================
// Глобальные переменные:
//==================================================================================

CONCHAR			text[] = { "Possible Duplicate :\n Converting Char to int in C. I remember learning a long time ago in a course that converting from an ASCII character to an int by subtracting '0' is bad. For example: int converted; char ascii = '8'; converted = ascii - '0'; Why is this considered a bad practice? This is because..." };
INT				g_i = 0, pxt = 15;
UINT			g_Texture[100];
FLOAT			offset=1.f/16.0f;

texturecord		textureCoordsAnim;
texturecord		textureCoords[sizeof(text)];
TextureImage	textures[100];


BOOL			INITSICKAYBLAYTb = TRUE;
INT				g_width = 800; 
INT				g_height = 600;
INT nums = 16, pxx = 0, pxy = 250, v = 0, g_stop = -1; /*snos[5] = { -1,-1,-1,-1,-1 }, mix[2] = { 0,0 }, mix2 = 0, , space[1024] = { -1 }*/
FLOAT cx = 0.062500f, cy = 0.0f;

//==================================================================================|
// Прототипы:																		|
// AnimateNextFrame = fps;															|
// LoadTGA;																			|
// LoadBMP;																			|
// LogicAnimSprite															    	|
// LogicAnimSpriteAnimRenderSprite													|
// renderSprite																		|
// renderSpriteAnimDrawTexturedRect													|
// OnResize =																		|
// OnMouseMove 																		|
// OnMouseClick 																	|
// OnChar = нажатие клавиш 															|
// OnCreate = Создание текстур 														|
// OnClose					 														|
// Draw = Отрисовка;																|
//==================================================================================|

bool AnimateNextFrame(int desiredFrameRate);
bool LoadTGA(const char *filename, TextureImage *texture, int iogram);
bool LoadBMP(UINT &textureID, const char *szFileName);
void LogicAnimSprite(int num);
void LogicAnimSpriteAnim(int loop, int num);
void renderSprite(int mode, int posx, int posy, int width, int height, GLuint texid, int idtextcord);
void renderSpriteAnim(int mode, int posx, int posy, int width, int height, GLuint texid);
void DrawTexturedRect(int mode, int posx, int posy, int width, int height, GLuint texid);
void OnResize(int width, int height);
void OnMouseMove(int x, int y);
void OnMouseClick(int x, int y, int button, int state);
void OnChar(char ch);
void OnCreate(HWND wnd);
void OnClose(HWND wnd);
void Draw();


//=================================================================================
//-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=Main=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=================================================================================

int main() {
	for (int i = 0; i < sizeof(textureCoords) / sizeof(texturecord); i++) {
		textureCoords[i][0] = 0.0f;
		textureCoords[i][1] = offset;
		textureCoords[i][2] = 0.0f;
		textureCoords[i][3] = 0.0f;
		textureCoords[i][4] = offset;
		textureCoords[i][5] = 0.0f;
		textureCoords[i][6] = offset;
		textureCoords[i][7] = offset;
	}

	for (int i = 0; i < 255; ++i) {
		LogicAnimSprite(nums);
	}

	glEnable(GL_TEXTURE_2D);
	create_window("Text Draw Program", "glwindow2021", 24, 32, Draw, OnResize, OnMouseMove, OnMouseClick, OnChar, OnCreate, OnClose, 0, 0, g_width, g_height);

	return 0;
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void Draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_width, 0, g_height, -1, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // В общем создание альфа канала
	glEnable(GL_BLEND);								  // Активацыя альфа

	while (v <= sizeof(text) || pxy >= 0) {
		for (int d = 0; d < ((g_width - (pxt-(pxt/7))) / (pxt - 5)); d++) {
			if (text[v] - 32 == -32) {
				pxy = -1;
				d = ((g_width - (pxt - (pxt / 7))) / (pxt - 5)+1);
			}
			if (text[v] - 32 == -22) {
				d = ((g_width - (pxt - (pxt / 7))) / (pxt - 5) + 1);
			}
			renderSprite(GL_QUADS, pxx, pxy, pxt, pxt, g_Texture[0], text[v]-32);
	
			pxx += pxt;
			pxx -= 5;
			v++;
		}
		pxx = 0;
		pxy -= pxt + 5;
	} 
	pxy = 250;
	pxx = 0;
	v = 0;

	glDisable(GL_BLEND);														//Дизоктивацыя альфа

	DrawTexturedRect(GL_QUADS, 0, 295, 305, 305, -1);
	DrawTexturedRect(GL_QUADS, 0, 300, 300, 300, g_Texture[0]);

	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

bool AnimateNextFrame(int desiredFrameRate)
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

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

bool LoadTGA(const char *filename, TextureImage *texture, int iogram)			// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if (file == NULL ||										// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||								// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||								// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp = header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize = texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData = (GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);											// Close The File 

	// Build A Texture From The Data
	glGenTextures(1, &g_Texture[iogram]);					// Generate OpenGL texture ID
	

	glBindTexture(GL_TEXTURE_2D, g_Texture[iogram]);			// Bind Our Texture     // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter

	if (texture[0].bpp == 24)									// Was The TGA 24 Bits
	{
		type = GL_RGB;										// If So Set The 'type' To GL_RGB
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return TRUE;											// Texture Building Went Ok, Return True
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

bool LoadBMP(UINT &textureID, const char *szFileName)                          // Creates Texture From A Bitmap File
{

	HBITMAP hBMP;                                                                 // Handle Of The Bitmap
	BITMAP  bitmap;																  // Bitmap Structure

	glGenTextures(1, &textureID);                                                 // Create The Texture
	hBMP = (HBITMAP)LoadImageA(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP)                                                                    // Does The Bitmap Exist?
		return FALSE;                                                           // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap);                                     // Get The Object
																				  // hBMP:        Handle To Graphics Object
																				  // sizeof(bitmap): Size Of Buffer For Object Information
																				  // &bitmap:        Buffer For Object Information

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

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void LogicAnimSprite(int num) {
	for (int loop = 0; loop < 255; ++loop) {
		float cx = (float(loop % num) / float(num));
		float cy = (float(loop / num) / float(num));

		//x							
		textureCoords[loop][0] = cx;
		textureCoords[loop][2] = cx;
		textureCoords[loop][4] = cx + offset;
		textureCoords[loop][6] = cx + offset;
		//y		 [idtextcord] = 
		textureCoords[loop][3] = 1.0f - cy - offset;
		textureCoords[loop][5] = 1.0f - cy - offset;
		textureCoords[loop][1] = 1.0f - cy - 0.001f;
		textureCoords[loop][7] = 1.0f - cy - 0.001f;
	}
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void LogicAnimSpriteAnim(int loop, int num) {
	offset = (1.0f / num);
	float cx = (float(loop % num) / float(num));
	float cy = (float(loop / num) / float(num));

	textureCoordsAnim[0] = cx;
	textureCoordsAnim[2] = cx;
	textureCoordsAnim[4] = cx + offset;
	textureCoordsAnim[6] = cx + offset;

	textureCoordsAnim[3] = 1.0f - cy - offset;
	textureCoordsAnim[5] = 1.0f - cy - offset;
	textureCoordsAnim[1] = 1.0f - cy - 0.001f;
	textureCoordsAnim[7] = 1.0f - cy - 0.001f;
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void renderSprite(int mode, int posx, int posy, int width, int height, GLuint texid, int idtextcord) {
	glBindTexture(GL_TEXTURE_2D, texid);
	int coords[] = {
	posx, posy + height,
	posx, posy,
	posx + width, posy,
	posx + width, posy + height
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoords[idtextcord]);
	glDrawArrays(mode, 0, 4);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void renderSpriteAnim(int mode, int posx, int posy, int width, int height, GLuint texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
	int coords[] = {
	posx, posy + height,
	posx, posy,
	posx + width, posy,
	posx + width, posy + height
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsAnim);
	glDrawArrays(mode, 0, 4);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void DrawTexturedRect(int mode, int posx, int posy, int width, int height, GLuint texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
	int coords[] = {
		posx, posy + height,
		posx, posy,
		posx + width, posy,
		posx + width, posy + height
	};
	float textureCoordsArray2[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	glVertexPointer(2, GL_INT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray2);
	glDrawArrays(mode, 0, 4);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnResize(int width, int height){
	glViewport(0, 0, width, height);
	g_width = width; 
	g_height = height;
	printf("Window size:  (%dx%d)\n", width, height);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnMouseMove(int x, int y) {
	
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnMouseClick(int x, int y, int button, int state) {
	printf("x = %d, y = %d, button %d, %d\n", x, y, button, state);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnChar(char ch)
{
	if (ch == 27) //esc code
		exit(0);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnCreate(HWND wnd) {
	if (LoadTGA(/*g_Texture[0],*/ "Font.tga", &textures[0], 0)) {
		printf("font.tga %d\n", g_Texture[0]);
	}
	else {
		printf("Textur_Not\n");
	}
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
}

//==================================================================================
//=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//==================================================================================

void OnClose(HWND wnd) {
	PostQuitMessage(0);
	exit(0);
}

/*
  ==================================================================================
  =-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=NEW=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  ==================================================================================

	SwapBuffers(hDevCtx);
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRndCtx);
	DeleteDC(hDevCtx);
	UnregisterClassA(szWindowClass, hInstance);
	return (int)msg.wParam;
	if (i == (nums*nums)) {
		i = 0;
	}
	if (AnimateNextFrame(30)) {
		//LogicAnimSpriteAnim(i, 6);
		i++;
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawTexturedRect(GL_QUADS, 495, 295, 305, 305, -1);
	renderSpriteAnim(GL_QUADS, 500, 300, 300, 300, g_Texture[1]);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	do {
		for (int d = 0; d < (780 / (pxt - 5)); d++) {
			for (int i = 0; i < 5; i++) {
				if (v == snos[i]) {
					d = (780 / (pxt - 3) - 5) + 1;
				}
			}
			if (v == stop) {
				pxy = -1;
			}
	
			renderSprite(GL_QUADS, pxx, pxy, pxt, pxt, textures[0].texID, v);
	
			pxx += pxt;
			pxx -= 5;
			v++;
		}
		pxx = 0;
		pxy -= pxt + 5;
	} while (v <= sizeof(text) || pxy >= 0);
	pxy = 250;
	pxx = 0;
	v = 0;
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glScissor(1, int(0.135416f*300), 300 - 2, int(0.597916f*600));			// Define Scissor Region
	glEnable(GL_SCISSOR_TEST);												// Enable Scissor Testing
	
	
	
	DrawTexturedRect(GL_QUADS, 0, 295, 305, 305, -1);
	DrawTexturedRect(GL_QUADS, 0, 300, 300, 300, textures[0].texID);
	
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();

	int nums = 16, pxx = 0, pxy = 250, v = 0, snos[5] = { -1,-1,-1,-1,-1 }, mix[2] = { 0,0 }, mix2 = 0, stop = -1, space[1024] = { -1 };
	for (int l = 0; l < sizeof(text); l++) {
		if (text[l] - 32 != -32 && text[l] - 32 != -22) {
			LogicAnimSprite((text[l] - 32), nums, l);
		}
		else if (text[l] - 32 == -22) {
			snos[mix[1]] = l;
			mix[1]++;
		}
		else if (text[l] - 32 == -32) {
			stop = l;
			printf("%d\n", stop);
		}
	}
*/
