#pragma once
#include <math.h>
#include "vec.h"

/**
 * Basic quaternion operations.
 */

/* Quaternion (x, y, z, w) */
typedef float quat4_t[4];

enum {
	X, Y, Z, W
};

void Quat_computeW(quat4_t q);
void Quat_normalize(quat4_t q);
void Quat_multQuat(const quat4_t qa, const quat4_t qb, quat4_t out);
void Quat_multVec(const quat4_t q, const vec3_t v, quat4_t out);
void Quat_rotatePoint(const quat4_t q, const vec3_t in, vec3_t out);