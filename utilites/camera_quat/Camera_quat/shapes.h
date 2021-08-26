#ifndef _GEOMETRIA_PROYECTIVA_SHAPES_H_
#define _GEOMETRIA_PROYECTIVA_SHAPES_H_

#include "GLInclude.h"
#include "math3d.h"
#include <vector>

typedef struct {
	std::vector<VECTOR3D> P;
} LINE;

typedef struct {
    VECTOR3D pos;
    float radius;
    COLOUR color;
    QUATERNION q;
} ROTATING_DOT;

void drawDot(VECTOR3D position, float sradius = 1, COLOUR color = grey);

void drawLine(LINE line, COLOUR color = grey, bool doDrawDots = false);

void drawAxis();

void drawGround();



#endif
