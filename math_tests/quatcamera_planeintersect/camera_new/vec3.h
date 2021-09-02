#pragma once
#ifndef __CAMERA_VEC3D
#define __CAMERA_VEC3D
#include "include.h"

class vec3D
{
public:
	union {
		struct { float x, y, z; };
		struct { float s, t, p; };
		struct { float r, g, b; };
	};
	vec3D() : x(0.0f), y(0.0f), z(0.0f) {}
	~vec3D() {}
	vec3D(float num) : x(num), y(num), z(num) {}
	vec3D(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3D(const vec2 &u, float z) : x(u.x), y(u.y), z(z) {}
	vec3D(const vec3D &u) : x(u.x), y(u.y), z(u.z) {}
	vec3D& operator = (const vec3D &u) { x = u.x; y = u.y; z = u.z; return *this; }
	vec3D operator - () { return vec3D(-x, -y, -z); }
	float* operator & () { return (float*)this; }
	vec3D& operator += (float num) { x += num; y += num; z += num; return *this; }
	vec3D& operator += (const vec3D &u) { x += u.x; y += u.y; z += u.z; return *this; }
	vec3D& operator -= (float num) { x -= num; y -= num; z -= num; return *this; }
	vec3D& operator -= (const vec3D &u) { x -= u.x; y -= u.y; z -= u.z; return *this; }
	vec3D& operator *= (float num) { x *= num; y *= num; z *= num; return *this; }
	vec3D& operator *= (const vec3D &u) { x *= u.x; y *= u.y; z *= u.z; return *this; }
	vec3D& operator /= (float num) { x /= num; y /= num; z /= num; return *this; }
	vec3D& operator /= (const vec3D &u) { x /= u.x; y /= u.y; z /= u.z; return *this; }
	friend vec3D operator + (const vec3D &u, float num) { return vec3D(u.x + num, u.y + num, u.z + num); }
	friend vec3D operator + (float num, const vec3D &u) { return vec3D(num + u.x, num + u.y, num + u.z); }
	friend vec3D operator + (const vec3D &u, const vec3D &v) { return vec3D(u.x + v.x, u.y + v.y, u.z + v.z); }
	friend vec3D operator - (const vec3D &u, float num) { return vec3D(u.x - num, u.y - num, u.z - num); }
	friend vec3D operator - (float num, const vec3D &u) { return vec3D(num - u.x, num - u.y, num - u.z); }
	friend vec3D operator - (const vec3D &u, const vec3D &v) { return vec3D(u.x - v.x, u.y - v.y, u.z - v.z); }
	friend vec3D operator * (const vec3D &u, float num) { return vec3D(u.x * num, u.y * num, u.z * num); }
	friend vec3D operator * (float num, const vec3D &u) { return vec3D(num * u.x, num * u.y, num * u.z); }
	friend vec3D operator * (const vec3D &u, const vec3D &v) { return vec3D(u.x * v.x, u.y * v.y, u.z * v.z); }
	friend vec3D operator / (const vec3D &u, float num) { return vec3D(u.x / num, u.y / num, u.z / num); }
	friend vec3D operator / (float num, const vec3D &u) { return vec3D(num / u.x, num / u.y, num / u.z); }
	friend vec3D operator / (const vec3D &u, const vec3D &v) { return vec3D(u.x / v.x, u.y / v.y, u.z / v.z); }

};

#endif