#ifndef _GEOMETRIA_PROYECTIVA_CAMERA_H_
#define _GEOMETRIA_PROYECTIVA_CAMERA_H_

#include "math3d.h"


typedef struct {
    float yaw; // Rotation around the Y axis.
    float pitch; // Rotation around the X axis.
    float roll; // Rotation around the Z axis.
    QUATERNION orientation; // Cached quaternion equivalent of this euler object.
} EULER;

typedef struct {
    VECTOR3D position;
    VECTOR3D direction;
    VECTOR3D up;
    
    double aperture;    // in degrees
    int screenwidth,screenheight;
    EULER euler;
} CAMERA;

typedef struct {
    double left;
    double right;
    double bottom;
    double top;
    double nearValue;
    double farValue;
} FRUSTUM;

FRUSTUM makeFrustum(double fovX, double aspectRatio, double nearValue, double farValue);

// TODO
MATRIX4 lookAt(VECTOR3D eyePosition, VECTOR3D target, VECTOR3D upVector);

void updateEulerOrientation(EULER &euler);

VECTOR3D getForward(EULER euler);
VECTOR3D getUp(EULER euler);

#endif
