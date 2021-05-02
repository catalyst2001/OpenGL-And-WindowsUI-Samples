#include "CImage.h"

// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

bool CImage::CreateTexture(const char *pFileName)
{
	if (!pFileName) // Return from the function if no file name was passed in
		return false;

	if (strstr(pFileName, ".bmp")) {
		return LoadBMP(pFileName);
	}

	if (strstr(pFileName, ".tga")) {
		if (!LoadTGA(pFileName))			// Load the image and store the data
			return false;

		glGenTextures(1, &m_TexID); // Generate a texture with the associative texture ID stored in the array
		glBindTexture(GL_TEXTURE_2D, m_TexID); // Bind the texture to the texture arrays index and init the texture
		int textureType = GL_RGB; // Assume that the texture is a 24 bit RGB texture (We convert 16-bit ones to 24-bit)
		if (m_Channels == 4) // If the image is 32-bit (4 channels), then we need to specify GL_RGBA for an alpha
			textureType = GL_RGBA;

		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		gluBuild2DMipmaps(GL_TEXTURE_2D, m_Channels, m_TexWidth, m_TexHeight, textureType, GL_UNSIGNED_BYTE, m_pPixelsData);

		// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
		// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
		// but looks blochy and pixilated.  Good for slower computers though. 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now we need to free the image data that we loaded since OpenGL stored it as a texture
		if (m_pPixelsData) // If we loaded the image
			free(m_pPixelsData); // Free the image data

		return true;
	}
	return false;
}

bool CImage::LoadBMP(const char *pFileName)
{
	HBITMAP hBMP; // Handle Of The Bitmap
	BITMAP  bitmap; // Bitmap Structure

	glGenTextures(1, &m_TexID); // Create The Texture
	hBMP = (HBITMAP)LoadImageA(GetModuleHandle(NULL), pFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP) // Does The Bitmap Exist?
		return false; // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap); // Get The Object
	// hBMP:        Handle To Graphics Object
	// sizeof(bitmap): Size Of Buffer For Object Information
	// &bitmap:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, m_TexID); // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	// MUST NOT BE INDEX BMP, BUT RGB
	DeleteObject(hBMP); // Delete The Object
	return true;
}

bool CImage::LoadTGA(const char *pFileName)
{
	WORD width = 0, height = 0; // The dimensions of the image
	uint8_t length = 0; // The length in bytes to the pixels
	uint8_t imageType = 0; // The image type (RLE, RGB, Alpha...)
	uint8_t bits = 0; // The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL; // The file pointer
	int channels = 0; // The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0; // The stride (channels * width)
	int i = 0; // A counter

	// This function loads in a TARGA (.TGA) file and returns its data to be
	// used as a texture or what have you.  This currently loads in a 16, 24
	// and 32-bit targa file, along with RLE compressed files.  Eventually you
	// will want to do more error checking to make it more robust.  This is
	// also a perfect start to go into a modular class for an engine.
	// Basically, how it works is, you read in the header information, then
	// move your file pointer to the pixel data.  Before reading in the pixel
	// data, we check to see the if it's an RLE compressed image.  This is because
	// we will handle it different.  If it isn't compressed, then we need another
	// check to see if we need to convert it from 16-bit to 24 bit.  24-bit and
	// 32-bit textures are very similar, so there's no need to do anything special.
	// We do, however, read in an extra bit for each color.

	// Open a file pointer to the targa file and check if it was found and opened 
	if ((pFile = fopen(pFileName, "rb")) == NULL)
	{
		// Display an error message saying the file was not found, then return NULL
		MessageBox(0, "Unable to load TGA File!", "Error", MB_OK);
		return NULL;
	}
	fread(&length, sizeof(uint8_t), 1, pFile); // Read in the length in bytes from the header to the pixel data
	fseek(pFile, 1, SEEK_CUR); // Jump over one uint8_t
	fread(&imageType, sizeof(uint8_t), 1, pFile); // Read in the imageType (RLE, RGB, etc...)
	fseek(pFile, 9, SEEK_CUR); // Skip past general information we don't care about

	// Read the width, height and bits per pixel (16, 24 or 32)
	fread(&width, sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits, sizeof(uint8_t), 1, pFile);
	fseek(pFile, length + 1, SEEK_CUR); // Now we move the file pointer to the pixel data
	if (imageType != TGA_RLE) // Check if the image is RLE compressed or not
	{
		// Check if the image is a 24 or 32-bit image
		if (bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			m_pPixelsData = (unsigned char*)malloc(stride * height);

			// Load in all the pixel data line by line
			for (int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(m_pPixelsData[stride * y]);

				// Read in the current line of pixels
				fread(pLine, stride, 1, pFile);

				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for (i = 0; i < stride; i += channels)
				{
					int temp = pLine[i];
					pLine[i] = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if (bits == 16)
		{
			unsigned short pixels = 0;
			int r = 0, g = 0, b = 0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			m_pPixelsData = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for (int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, pFile);

				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;

				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				m_pPixelsData[i * 3 + 0] = r;
				m_pPixelsData[i * 3 + 1] = g;
				m_pPixelsData[i * 3 + 2] = b;
			}
		}
		
		else
			return false; // Else return a false for a bad or unsupported pixel format
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		// First, let me explain real quickly what RLE is.  
		// For further information, check out Paul Bourke's intro article at: 
		// http://astronomy.swin.edu.au/~pbourke/dataformats/rle/
		// 
		// Anyway, we know that RLE is a basic type compression.  It takes
		// colors that are next to each other and then shrinks that info down
		// into the color and a integer that tells how much of that color is used.
		// For instance:
		// aaaaabbcccccccc would turn into a5b2c8
		// Well, that's fine and dandy and all, but how is it down with RGB colors?
		// Simple, you read in an color count (rleID), and if that number is less than 128,
		// it does NOT have any optimization for those colors, so we just read the next
		// pixels normally.  Say, the color count was 28, we read in 28 colors like normal.
		// If the color count is over 128, that means that the next color is optimized and
		// we want to read in the same pixel color for a count of (colorCount - 127).
		// It's 127 because we add 1 to the color count, as you'll notice in the code.

		// Create some variables to hold the rleID, current colors read, channels, & stride.
		uint8_t rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		m_pPixelsData = new unsigned char[stride * height];
		uint8_t *pColors = (uint8_t *)malloc(channels);
		while (i < width*height) // Load in all the pixel data
		{
			fread(&rleID, sizeof(uint8_t), 1, pFile); // Read in the current color count + 1
			if (rleID < 128) // Check if we don't have an encoded string of colors
			{
				rleID++; // Increase the count by 1
				while (rleID) // Go through and read all the unique colors found
				{
					// Read in the current color
					fread(pColors, sizeof(uint8_t) * channels, 1, pFile);

					// Store the current pixel in our image array
					m_pPixelsData[colorsRead + 0] = pColors[2];
					m_pPixelsData[colorsRead + 1] = pColors[1];
					m_pPixelsData[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						m_pPixelsData[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else // Else, let's read in a string of the same character
			{
				
				rleID -= 127; // Minus the 128 ID + 1 (127) to get the color count that needs to be read
				fread(pColors, sizeof(uint8_t) * channels, 1, pFile); // Read in the current color, which is the same for a while
				while (rleID) // Go and read as many pixels as are the same
				{
					// Assign the current pixel to the current index in our pixel array
					m_pPixelsData[colorsRead + 0] = pColors[2];
					m_pPixelsData[colorsRead + 1] = pColors[1];
					m_pPixelsData[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						m_pPixelsData[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
		free(pColors); // Free up pColors
	}
	fclose(pFile); // Close the file pointer that opened the file
	m_Channels = channels;
	m_TexWidth = width;
	m_TexHeight =  height;
	return true;
}
