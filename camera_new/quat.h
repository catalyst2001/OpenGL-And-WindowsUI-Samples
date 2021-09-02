#pragma once
#ifndef __CAMERA_QUAT
#define __CAMERA_QUAT
#include "../utilites/glmath.h"

//#define PI 3.141592653589793238462643
#define DTOR            0.0174532925    
#define RTOD            57.2957795    

class Quat
{
public:
	Quat() {};
	Quat(double S, vec3 vec) {
		s = S;
		v = vec;
	};
	~Quat() {};
	double s;
	vec3 v;

private:
	
};

static vec3 MultiplyWithScalar(float scalar, vec3 a) {
	a.x = a.x * scalar;
	a.y = a.y * scalar;
	a.z = a.z * scalar;
	return a;
}

static Quat QuaternionFromAngleAxis(float angle, vec3 axis) {
	double theta = angle * DTOR;

	return {
		cos(theta / 2),
		MultiplyWithScalar(-sin(theta / 2), normalize(axis))
	};
}
static Quat QuaternionFromToVectors(vec3 from, vec3 to) {
	vec3 uFrom = normalize(from);
	vec3 uTo = normalize(to);

	return QuaternionFromAngleAxis(
		acos(dot(uFrom, uTo)), // Angle form dot product.
		cross(uFrom, uTo));    // Axis to rotate around.
}

static Quat Multiply(Quat a, Quat b) {
	return Quat(
		a.s*b.s - a.v.x*b.v.x - a.v.y*b.v.y - a.v.z*b.v.z,
		vec3(
		a.s*b.v.x + a.v.x*b.s + a.v.y*b.v.z - a.v.z*b.v.y,
		a.s*b.v.y - a.v.x*b.v.z + a.v.y*b.s + a.v.z*b.v.x,
		a.s*b.v.z + a.v.x*b.v.y - a.v.y*b.v.x + a.v.z*b.s ));
}

static Quat Conjugate(Quat a) {
	return Quat(
		a.s,
		vec3( -a.v.x, -a.v.y, -a.v.z ));
}

static vec3 RotateWithQuaternion(vec3 a, Quat q) {
	return Multiply(q, Multiply({ 0, a }, Conjugate(q))).v;
}

struct EULER {
	float yaw; // Rotation around the Y axis.
	float pitch; // Rotation around the X axis.
	float roll; // Rotation around the Z axis.
	Quat orientation; // Cached quaternion equivalent of this euler object.
};

#endif
