#pragma once
#include "Opengl.h"

class CAnimatedSprite
{

public:
	CAnimatedSprite() {}
	~CAnimatedSprite() {}

	int cordx = 0, cordy = 0, hueta = 0;
	float ytex = 1.0f, xtex = 0.0f, offset;
	float textureCoordsArray[8] = {
				0.0f, offset,           // x, y + h
				0.0f, 0.0f,           // x, y
				offset, 0.0f,           // x + w, y
				offset, offset            // x + w, y + h
	};

	/*
	* AnimashUny'
	*/

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

	/*
	* GIF
	*/

	void renderSprite(int mode, int posx, int posy, int width, int height, unsigned int texid, float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) {
		float textureCoordsArray[] = {
			x, y,           // x, y + h  0 1
			x1, y1,           // x, y 0 0
			x2, y2,           // x + w, y 1 0
			x3, y3            // x + w, y + h 1 1
		};
		int coords[] = {
		posx, posy + height,
		posx, posy,
		posx + width, posy,
		posx + width, posy + height
		};
		/*printf("%f %f %f %f %f %f %f %f\n",
			textureCoordsArray[0], textureCoordsArray[1],
			textureCoordsArray[2], textureCoordsArray[3],
			textureCoordsArray[4], textureCoordsArray[5],
			textureCoordsArray[6], textureCoordsArray[7])*/
		glVertexPointer(2, GL_INT, 0, coords);
		glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsArray);
		glDrawArrays(mode, 0, 4);
	}

	void logickSprite(int mode, int posx, int posy, int width, int height, unsigned int texid, unsigned int numbs) {
		int numbs_sqrt = sqrt(numbs);
		offset = 1.0f / numbs_sqrt;
		glBindTexture(GL_TEXTURE_2D, texid);

		if (AnimateNextFrame(36)) {
			if (cordx < numbs_sqrt + 1) { /*y++)*/
				cordx++;
				textureCoordsArray[0] = xtex;
				textureCoordsArray[2] = xtex;
				xtex += offset;
				textureCoordsArray[4] = xtex;
				textureCoordsArray[6] = xtex;
				hueta++;
			}
			if (hueta == numbs_sqrt + 1) {
				cordy++;
				textureCoordsArray[1] = ytex;
				textureCoordsArray[7] = ytex;
				ytex -= offset;
				textureCoordsArray[3] = ytex;
				textureCoordsArray[5] = ytex;
				textureCoordsArray[0] = 0;
				textureCoordsArray[2] = 0;
				textureCoordsArray[4] = offset;
				textureCoordsArray[6] = offset;
				xtex = 0;
				cordx = 0;
				hueta = 0;
			}
			if (cordy == numbs_sqrt) {
				ytex = 1.0f;
				cordy = 0;
			}

		}
		renderSprite(mode, posx, posy, width, height, texid,
			textureCoordsArray[0], textureCoordsArray[1],
			textureCoordsArray[2], textureCoordsArray[3],
			textureCoordsArray[4], textureCoordsArray[5],
			textureCoordsArray[6], textureCoordsArray[7]);
	}
};

