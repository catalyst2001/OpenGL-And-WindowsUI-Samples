#include "quat.h"


void Quat_computeW(quat4_t q)
{
	float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);
	if (t < 0.0f)
		q[W] = 0.0f;
	else
		q[W] = -sqrt(t);
}

void Quat_normalize(quat4_t q)
{
	/* Compute magnitude of the quaternion */
	float mag = sqrt((q[X] * q[X]) + (q[Y] * q[Y]) + (q[Z] * q[Z]) + (q[W] * q[W]));
	/* Check for bogus length, to protect against divide by zero */
	if (mag > 0.0f)
	{
		/* Normalize it */
		float oneOverMag = 1.0f / mag;

		q[W] *= oneOverMag;
		q[X] *= oneOverMag;
		q[Y] *= oneOverMag;
		q[Z] *= oneOverMag;
	}
}

void Quat_multQuat(const quat4_t qa, const quat4_t qb, quat4_t out)
{
	out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z]);
	out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y]);
	out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z]);
	out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X]);
}

void Quat_multVec(const quat4_t q, const vec3_t v, quat4_t out)
{
	out[W] = -(q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z]);
	out[X] = (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y]);
	out[Y] = (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z]);
	out[Z] = (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X]);
}

void Quat_rotatePoint(const quat4_t q, const vec3_t in, vec3_t out)
{
	quat4_t tmp, inv, final;
	inv[X] = -q[X];
	inv[Y] = -q[Y];
	inv[Z] = -q[Z];
	inv[W] = q[W];
	Quat_normalize(inv);
	Quat_multVec(q, in, tmp);
	Quat_multQuat(tmp, inv, final);
	out[X] = final[X];
	out[Y] = final[Y];
	out[Z] = final[Z];
}