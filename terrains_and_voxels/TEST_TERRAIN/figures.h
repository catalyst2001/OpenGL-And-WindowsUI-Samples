#pragma once


///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This draws a spiral of pyramids to maneuver around
/////
///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawSpiralTowers()
{
	const float PI = 3.14159f;							// Create a constant for PI
	const float kIncrease = PI / 16.0f;					// This is the angle we increase by in radians
	const float kMaxRotation = PI * 6;					// This is 1080 degrees of rotation in radians (3 circles)
	float radius = 40;									// We start with a radius of 40 and decrease towards the center

	// Keep looping until we go past the max degrees of rotation (which is 3 full rotations)
	for(float degree = 0; degree < kMaxRotation; degree += kIncrease)
	{
		// Here we use polar coordinates for the rotations, but we swap the y with the z
		float x = float(radius * cos(degree));			// Get the x position for the current rotation and radius
		float z = float(radius * sin(degree));			// Get the z position for the current rotation and radius

		// Create a pyramid for every degree in our spiral with a width of 1 and height of 3 
		CreatePyramid(x, 3, z, 1, 3);
	
		// Decrease the radius by the constant amount so the pyramids spirals towards the center
		radius -= 40 / (kMaxRotation / kIncrease);
	}	
}


/////	This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
void CreatePyramid(float x, float y, float z, int width, int height)
{
	// Start rendering the 4 triangles for the sides
	glBegin(GL_TRIANGLES);
	// These vertices create the Back Side
	glColor3ub(255, 255, 0);
	glVertex3f(x, y + height, z);					// Top point
	glColor3ub(255, 255, 255);
	glVertex3f(x - width, y - height, z - width);	// Bottom left point
	glColor3ub(255, 0, 0);
	glVertex3f(x + width, y - height, z - width);		// Bottom right point

	// These vertices create the Front Side
	glColor3ub(255, 255, 0);
	glVertex3f(x, y + height, z);					// Top point
	glColor3ub(255, 255, 255);
	glVertex3f(x + width, y - height, z + width);	// Bottom right point
	glColor3ub(255, 0, 0);
	glVertex3f(x - width, y - height, z + width);		// Bottom left point

	// These vertices create the Front Left Side
	glColor3ub(255, 255, 0);
	glVertex3f(x, y + height, z);					// Top point
	glColor3ub(255, 0, 0);
	glVertex3f(x - width, y - height, z + width);		// Front bottom point
	glColor3ub(255, 255, 255);
	glVertex3f(x - width, y - height, z - width);	// Bottom back point

	// These vertices create the Front Right Side
	glColor3ub(255, 255, 0);
	glVertex3f(x, y + height, z);					// Top point
	glColor3ub(255, 0, 0);
	glVertex3f(x + width, y - height, z - width);		// Bottom back point
	glColor3ub(255, 255, 255);
	glVertex3f(x + width, y - height, z + width);	// Front bottom point
	glEnd();

	// Now render the bottom of our pyramid

	glBegin(GL_QUADS);
	// These vertices create the bottom of the pyramid
	glColor3ub(0, 0, 255);
	glVertex3f(x - width, y - height, z + width);	// Front left point
	glColor3ub(0, 0, 255);
	glVertex3f(x + width, y - height, z + width);    // Front right point
	glColor3ub(0, 0, 255);
	glVertex3f(x + width, y - height, z - width);    // Back right point
	glColor3ub(0, 0, 255);
	glVertex3f(x - width, y - height, z - width);    // Back left point
	glEnd();
}
