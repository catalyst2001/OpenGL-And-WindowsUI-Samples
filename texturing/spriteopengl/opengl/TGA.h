#pragma once
#ifndef _MAIN_H
#define _MAIN_H
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>									
#include <gl\glu.h>	





// This is our image structure for our targa data
struct tImageTGA
{
	int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
	int sizeX;				// The width of the image in pixels
	int sizeY;				// The height of the image in pixels
	unsigned char *data;	// The image pixel data
};

tImageTGA *LoadTGA(const char *filename);

void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);

#endif