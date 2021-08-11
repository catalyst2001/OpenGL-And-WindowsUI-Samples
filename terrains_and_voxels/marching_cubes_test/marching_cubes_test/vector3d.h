#pragma once
#include <math.h>
#include "SimplexNoise.h"

struct Vector3 {
	float x, y, z;
};

struct Vector3Int {
	int x, y, z;
};

static float VectorDistance(Vector3 p1, Vector3 p2)
{
	float diffX = p1.x - p2.x;
	float diffY = p1.y - p2.y;
	float diffZ = p1.z - p2.z;
	return sqrt((diffY * diffY) + (diffX * diffX) + (diffZ * diffZ));
}

static Vector3 AddVectors(Vector3 v1, Vector3 v2)
{
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

static Vector3 SubVectors(Vector3 v1, Vector3 v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

static Vector3 DivVector(Vector3 v1, float k)
{
	Vector3 result;
	result.x = v1.x / k;
	result.y = v1.y / k;
	result.z = v1.z / k;
	return result;
}

static Vector3 MulVector(Vector3 v1, float k)
{
	Vector3 result;
	result.x = v1.x * k;
	result.y = v1.y * k;
	result.z = v1.z * k;
	return result;
}