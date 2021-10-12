#include "glmath.h"

// ----------------------------------------------------------------------------------------------------------------------------

float dot(const vec2 &u, const vec2 &v)
{
	return u.x * v.x + u.y * v.y;
}

float length(const vec2 &u)
{
	return (float)sqrt(u.x * u.x + u.y * u.y);
}

float length2(const vec2 &u)
{
	return u.x * u.x + u.y * u.y;
}

//vec2 mix(const vec2 &u, const vec2 &b, float a)
//{
//	return u * (1.0f - a) + b * a;
//}

vec2 normalize(const vec2 &u)
{
	return u / sqrt(u.x * u.x + u.y * u.y);
}

vec2 reflect(const vec2 &i, const vec2 &n)
{
	return i - 2.0f * dot(n, i) * n;
}

vec2 refract(const vec2 &i, const vec2 &n, float eta)
{
	vec2 r;
	float ndoti = dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);
	if(k >= 0.0f) {
		r = eta * i - n * (eta * ndoti + sqrt(k));
	}
	return r;
}

vec2 rotate(const vec2 &u, float angle)
{
	angle = angle / 180.0f * (float)M_PI;

	float c = cos(angle), s = sin(angle);

	return vec2(u.x * c - u.y * s, u.x * s + u.y * c);
}

// ----------------------------------------------------------------------------------------------------------------------------

vec3 add(vec3 a, vec3 b)
{
	vec3 res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res;
}

vec3 sub(vec3 a, vec3 b)
{
	vec3 res;
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res;
}

vec3 mul(vec3 a, vec3 b)
{
	vec3 res;
	res.x = a.x * b.x;
	res.y = a.y * b.y;
	res.z = a.z * b.z;
	return res;
}

vec3 mulsc(vec3 a, float scalar)
{
	vec3 res;
	res.x = a.x * scalar;
	res.y = a.y * scalar;
	res.z = a.z * scalar;
	return res;
}

vec3 cross(const vec3 &u, const vec3 &v)
{
	return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float dot(const vec3 &u, const vec3 &v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float length(const vec3 &u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

float length_squared(const vec3 &u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

vec3 mix(const vec3 &u, const vec3 &v, float a)
{
	return u * (1.0f - a) + v * a;
}

vec3 normalize(const vec3 &u)
{
	float t = sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
	if (t <= 0.f)
		return { 0.f, 0.f, 0.f };

	return u / t;
}

vec3 reflect(const vec3 &i, const vec3 &n)
{
	return i - 2.0f * dot(n, i) * n;
}

vec3 refract(const vec3 &i, const vec3 &n, float eta)
{
	vec3 r;

	float ndoti = dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);

	if(k >= 0.0f)
	{
		r = eta * i - n * (eta * ndoti + sqrt(k));
	}

	return r;
}

vec3 rotate(const vec3 &u, float angle, const vec3 &v)
{
	return *(vec3*)&(rotate(angle, v) * vec4(u, 1.0f));
}

vec3 vecabs(vec3 &v)
{
	vec3 res;
	res.x = (float)fabs(v.x);
	res.y = (float)fabs(v.y);
	res.z = (float)fabs(v.z);
	return res;
}

float distance(float ax, float ay, float az, float bx, float by, float bz)
{
	float diffX = ax - bx;
	float diffY = ay - by;
	float diffZ = az - bz;
	return sqrt((diffY * diffY) + (diffX * diffX) + (diffZ * diffZ));
}

float distance(vec3 &a, vec3 &b)
{
	float diffX = a.x - b.x;
	float diffY = a.y - b.y;
	float diffZ = a.z - b.z;
	return sqrt((diffY * diffY) + (diffX * diffX) + (diffZ * diffZ));
}

vec3 Min(const vec3 &a, const vec3 &b)
{
	vec3 res;
	res.x = __mmin(a.x, b.x);
	res.y = __mmin(a.y, b.y);
	res.z = __mmin(a.z, b.z);
	return res;
}

vec3 Max(const vec3 &a, const vec3 &b)
{
	vec3 res;
	res.x = __mmax(a.x, b.x);
	res.y = __mmax(a.y, b.y);
	res.z = __mmax(a.z, b.z);
	return res;
}

void round_vector(vec3 &dest, const vec3 vec, float roundfactor)
{
	dest.x = round(vec.x / roundfactor) * roundfactor;
	dest.y = round(vec.y / roundfactor) * roundfactor;
	dest.z = round(vec.z / roundfactor) * roundfactor;
}

// ----------------------------------------------------------------------------------------------------------------------------

mat2x2::mat2x2()
{
	M[0] = 1.0f; M[2] = 0.0f;
	M[1] = 0.0f; M[3] = 1.0f;
}

mat2x2::~mat2x2()
{
}

mat2x2::mat2x2(const mat2x2 &Matrix)
{
	for(int i = 0; i < 4; i++)
	{
		M[i] = Matrix.M[i];
	}
}

mat2x2::mat2x2(const vec2 &col1, const vec2 &col2)
{
	M[0] = col1.x; M[2] = col2.x;
	M[1] = col1.y; M[3] = col2.y;
}

mat2x2::mat2x2(float c1r1, float c1r2, float c2r1, float c2r2)
{
	M[0] = c1r1; M[2] = c2r1;
	M[1] = c1r2; M[3] = c2r2;
}

mat2x2::mat2x2(const mat3x3 &Matrix)
{
	M[0] = Matrix.M[0]; M[2] = Matrix.M[3];
	M[1] = Matrix.M[1]; M[3] = Matrix.M[4];
}

mat2x2::mat2x2(const mat4x4 &Matrix)
{
	M[0] = Matrix.M[0]; M[2] = Matrix.M[4];
	M[1] = Matrix.M[1]; M[3] = Matrix.M[5];
}

mat2x2& mat2x2::operator = (const mat2x2 &Matrix)
{
	for(int i = 0; i < 4; i++)
	{
		M[i] = Matrix.M[i];
	}

	return *this;
}

float& mat2x2::operator [] (int i)
{
	return M[i];
}

float* mat2x2::operator & ()
{
	return (float*)this;
}

quat computew(quat q)
{

	return quat();
}

float length_squared(quat q)
{
	return (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
}

float length(quat q)
{
	return sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
}

float dot(quat qa, quat qb)
{
	return ((qa.x * qb.x) + (qa.y * qb.y) + (qa.z * qb.z) + (qa.w * qb.w));
}

quat normalize(quat q)
{
	quat res;
	float mag = length(q); //compute magnitude of the quaternion
	if (mag > 0.0f) // check for bogus length, to protect against divide by zer
	{
		float oneOverMag = 1.0f / mag; //normalize it
		res.x *= oneOverMag;
		res.y *= oneOverMag;
		res.z *= oneOverMag;
		res.w *= oneOverMag;
		return res;
	}
	return res;
}

// out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z])
// out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y])
// out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z])
// out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X])
quat mul(quat qa, quat qb)
{
	quat res;
	res.w = (qa.w * qb.w) - (qa.x * qb.x) - (qa.y * qb.y) - (qa.z * qb.z);
	res.x = (qa.x * qb.w) + (qa.w * qb.x) + (qa.y * qb.z) - (qa.z * qb.y);
	res.y = (qa.y * qb.w) + (qa.w * qb.y) + (qa.z * qb.x) - (qa.x * qb.y);
	res.z = (qa.z * qb.w) + (qa.w * qb.z) + (qa.x * qb.y) - (qa.y * qb.x);
	return res;
}

// out[W] = -(q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z])
// out[X] = (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y])
// out[Y] = (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z])
// out[Z] = (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X])
quat mul(quat qa, vec3 vb)
{
	quat res;
	res.w = -(qa.x * vb.x) - (qa.y * vb.y) - (qa.z * vb.z);
	res.x = (qa.w * vb.x) + (qa.y * vb.z) - (qa.z * vb.y);
	res.y = (qa.w * vb.y) + (qa.z * vb.x) - (qa.x * vb.z);
	res.z = (qa.w * vb.z) + (qa.x * vb.y) - (qa.y * vb.x);
	return res;
}

quat slerp(quat &qa, quat &qb, float t)
{
	quat res;
	/* Check for out-of range parameter and return edge points if so */
	if (t <= 0.0)
		return qa;

	if (t >= 1.0)
		return qb;

	/* Compute "cosine of angle between quaternions" using dot product */
	float cosOmega = dot(qa, qb);

	/* If negative dot, use -q1.	Two quaternions q and -q
		 represent the same rotation, but may produce
		 different slerp.	We chose q or -q to rotate using
		 the acute angle. */
	float q1w = qb.w;
	float q1x = qb.x;
	float q1y = qb.y;
	float q1z = qb.z;
	if (cosOmega < 0.0f) {
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	/* We should have two unit quaternions, so dot should be <= 1.0 */
	assert(cosOmega < 1.1f);

	/* Compute interpolation fraction, checking for quaternions
		 almost exactly the same */
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		/* Very close - just use linear interpolation,
		which will protect againt a divide by zero */

		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		/* Compute the sin of the angle using the
		trig identity sin^2(omega) + cos^2(omega) = 1 */
		float sinOmega = sqrt(1.0f - (cosOmega * cosOmega));

		/* Compute the angle from its sin and cosine */
		float omega = atan2(sinOmega, cosOmega);

		/* Compute inverse of denominator, so we only have
		to divide once */
		float oneOverSinOmega = 1.0f / sinOmega;

		/* Compute interpolation parameters */
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	/* Interpolate and return new quaternion */
	res.w = (k0 * qa.w) + (k1 * q1w);
	res.x = (k0 * qa.x) + (k1 * q1x);
	res.y = (k0 * qa.y) + (k1 * q1y);
	res.z = (k0 * qa.z) + (k1 * q1z);
	return res;
}

quat conjugate(quat &q)
{
	quat res;
	res.w = q.w;
	res.x = -q.x;
	res.y = -q.y;
	res.z = -q.z;
	return res;
}

vec3 rotate_with_quat(vec3 a, quat q)
{
	return mul(q, mul(quat(a, 0.f), conjugate(q))).vec();
}

quat quat_from_angle_axis(float angle, vec3 axis)
{
	float theta = angle * DTOR;
	return quat(normalize(axis) * -sin(theta / 2.f), cos(theta / 2));
}

mat2x2 operator * (const mat2x2 &Matrix1, const mat2x2 &Matrix2)
{
	mat2x2 Matrix3;
	Matrix3.M[0] = Matrix1.M[0] * Matrix2.M[0] + Matrix1.M[2] * Matrix2.M[1];
	Matrix3.M[1] = Matrix1.M[1] * Matrix2.M[0] + Matrix1.M[3] * Matrix2.M[1];
	Matrix3.M[2] = Matrix1.M[0] * Matrix2.M[2] + Matrix1.M[2] * Matrix2.M[3];
	Matrix3.M[3] = Matrix1.M[1] * Matrix2.M[2] + Matrix1.M[3] * Matrix2.M[3];
	return Matrix3;
}

vec2 operator * (const mat2x2 &Matrix, const vec2 &u)
{
	vec2 v;

	v.x = Matrix.M[0] * u.x + Matrix.M[2] * u.y;
	v.y = Matrix.M[1] * u.x + Matrix.M[3] * u.y;
	
	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 2
// 1 3
//
// ----------------------------------------------------------------------------------------------------------------------------

mat2x2 inverse(const mat2x2 &Matrix)
{
	const float *m = Matrix.M;

	float det = m[0] * m[3] - m[2] * m[1];

	mat2x2 Inverse;

	Inverse.M[0] = m[3] / det;
	Inverse.M[1] = -m[1] / det;
	Inverse.M[2] = -m[2] / det;
	Inverse.M[3] = m[0] / det;

	return Inverse;
}

mat2x2 transpose(const mat2x2 &Matrix)
{
	mat2x2 Transpose;

	Transpose.M[0] = Matrix.M[0];
	Transpose.M[1] = Matrix.M[2];
	Transpose.M[2] = Matrix.M[1];
	Transpose.M[3] = Matrix.M[3];

	return Transpose;
}

// ----------------------------------------------------------------------------------------------------------------------------

mat3x3::mat3x3()
{
	M[0] = 1.0f; M[3] = 0.0f; M[6] = 0.0f;
	M[1] = 0.0f; M[4] = 1.0f; M[7] = 0.0f;
	M[2] = 0.0f; M[5] = 0.0f; M[8] = 1.0f;
}

mat3x3::~mat3x3()
{
}

mat3x3::mat3x3(const mat3x3 &Matrix)
{
	for(int i = 0; i < 9; i++)
	{
		M[i] = Matrix.M[i];
	}
}

mat3x3::mat3x3(const vec3 &col1, const vec3 &col2, const vec3 &col3)
{
	M[0] = col1.x; M[3] = col2.x; M[6] = col3.x;
	M[1] = col1.y; M[4] = col2.y; M[7] = col3.y;
	M[2] = col1.z; M[5] = col2.z; M[8] = col3.z;
}

mat3x3::mat3x3(float c1r1, float c1r2, float c1r3, float c2r1, float c2r2, float c2r3, float c3r1, float c3r2, float c3r3)
{
	M[0] = c1r1; M[3] = c2r1; M[6] = c3r1;
	M[1] = c1r2; M[4] = c2r2; M[7] = c3r2;
	M[2] = c1r3; M[5] = c2r3; M[8] = c3r3;
}

mat3x3::mat3x3(const mat2x2 &Matrix)
{
	M[0] = Matrix.M[0]; M[3] = Matrix.M[2]; M[6] = 0.0f;
	M[1] = Matrix.M[1]; M[4] = Matrix.M[3]; M[7] = 0.0f;
	M[2] = 0.0f; M[5] = 0.0f; M[8] = 1.0f;
}

mat3x3::mat3x3(const mat4x4 &Matrix)
{
	M[0] = Matrix.M[0]; M[3] = Matrix.M[4]; M[6] = Matrix.M[8];
	M[1] = Matrix.M[1]; M[4] = Matrix.M[5]; M[7] = Matrix.M[9];
	M[2] = Matrix.M[2]; M[5] = Matrix.M[6]; M[8] = Matrix.M[10];
}

mat3x3& mat3x3::operator = (const mat3x3 &Matrix)
{
	for(int i = 0; i < 9; i++)
	{
		M[i] = Matrix.M[i];
	}

	return *this;
}

float& mat3x3::operator [] (int i)
{
	return M[i];
}

float* mat3x3::operator & ()
{
	return (float*)this;
}

mat3x3 operator * (const mat3x3 &Matrix1, const mat3x3 &Matrix2)
{
	mat3x3 Matrix3;

	Matrix3.M[0] = Matrix1.M[0] * Matrix2.M[0] + Matrix1.M[3] * Matrix2.M[1] + Matrix1.M[6] * Matrix2.M[2];
	Matrix3.M[1] = Matrix1.M[1] * Matrix2.M[0] + Matrix1.M[4] * Matrix2.M[1] + Matrix1.M[7] * Matrix2.M[2];
	Matrix3.M[2] = Matrix1.M[2] * Matrix2.M[0] + Matrix1.M[5] * Matrix2.M[1] + Matrix1.M[8] * Matrix2.M[2];
	Matrix3.M[3] = Matrix1.M[0] * Matrix2.M[3] + Matrix1.M[3] * Matrix2.M[4] + Matrix1.M[6] * Matrix2.M[5];
	Matrix3.M[4] = Matrix1.M[1] * Matrix2.M[3] + Matrix1.M[4] * Matrix2.M[4] + Matrix1.M[7] * Matrix2.M[5];
	Matrix3.M[5] = Matrix1.M[2] * Matrix2.M[3] + Matrix1.M[5] * Matrix2.M[4] + Matrix1.M[8] * Matrix2.M[5];
	Matrix3.M[6] = Matrix1.M[0] * Matrix2.M[6] + Matrix1.M[3] * Matrix2.M[7] + Matrix1.M[6] * Matrix2.M[8];
	Matrix3.M[7] = Matrix1.M[1] * Matrix2.M[6] + Matrix1.M[4] * Matrix2.M[7] + Matrix1.M[7] * Matrix2.M[8];
	Matrix3.M[8] = Matrix1.M[2] * Matrix2.M[6] + Matrix1.M[5] * Matrix2.M[7] + Matrix1.M[8] * Matrix2.M[8];

	return Matrix3;
}

vec3 operator * (const mat3x3 &Matrix, const vec3 &u)
{
	vec3 v;

	v.x = Matrix.M[0] * u.x + Matrix.M[3] * u.y + Matrix.M[6] * u.z;
	v.y = Matrix.M[1] * u.x + Matrix.M[4] * u.y + Matrix.M[7] * u.z;
	v.z = Matrix.M[2] * u.x + Matrix.M[5] * u.y + Matrix.M[8] * u.z;
	
	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 3 6 | + - +
// 1 4 7 | - + -
// 2 5 8 | + - +
//
// ----------------------------------------------------------------------------------------------------------------------------

float det2x2sub(const float *m, int i0, int i1, int i2, int i3)
{
	return m[i0] * m[i3] - m[i2] * m[i1];
}

mat3x3 inverse(const mat3x3 &Matrix)
{
	const float *m = Matrix.M;

	float det = 0.0f;

	det += m[0] * det2x2sub(m, 4, 5, 7, 8);
	det -= m[3] * det2x2sub(m, 1, 2, 7, 8);
	det += m[6] * det2x2sub(m, 1, 2, 4, 5);

	mat3x3 Inverse;

	Inverse.M[0] = det2x2sub(m, 4, 5, 7, 8) / det;
	Inverse.M[1] = -det2x2sub(m, 1, 2, 7, 8) / det;
	Inverse.M[2] = det2x2sub(m, 1, 2, 4, 5) / det;
	Inverse.M[3] = -det2x2sub(m, 3, 5, 6, 8) / det;
	Inverse.M[4] = det2x2sub(m, 0, 2, 6, 8) / det;
	Inverse.M[5] = -det2x2sub(m, 0, 2, 3, 5) / det;
	Inverse.M[6] = det2x2sub(m, 3, 4, 6, 7) / det;
	Inverse.M[7] = -det2x2sub(m, 0, 1, 6, 7) / det;
	Inverse.M[8] = det2x2sub(m, 0, 1, 3, 4) / det;

	return Inverse;
}

mat3x3 transpose(const mat3x3 &Matrix)
{
	mat3x3 Transpose;

	Transpose.M[0] = Matrix.M[0];
	Transpose.M[1] = Matrix.M[3];
	Transpose.M[2] = Matrix.M[6];
	Transpose.M[3] = Matrix.M[1];
	Transpose.M[4] = Matrix.M[4];
	Transpose.M[5] = Matrix.M[7];
	Transpose.M[6] = Matrix.M[2];
	Transpose.M[7] = Matrix.M[5];
	Transpose.M[8] = Matrix.M[8];

	return Transpose;
}

// ----------------------------------------------------------------------------------------------------------------------------

mat4x4::mat4x4()
{
	M[0] = 1.0f; M[4] = 0.0f; M[8] = 0.0f; M[12] = 0.0f;
	M[1] = 0.0f; M[5] = 1.0f; M[9] = 0.0f; M[13] = 0.0f;
	M[2] = 0.0f; M[6] = 0.0f; M[10] = 1.0f; M[14] = 0.0f;
	M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f;
}

mat4x4::~mat4x4()
{
}

mat4x4::mat4x4(const mat4x4 &Matrix)
{
	for(int i = 0; i < 16; i++)
	{
		M[i] = Matrix.M[i];
	}
}

mat4x4::mat4x4(const vec4 &col1, const vec4 &col2, const vec4 &col3, const vec4 &col4)
{
	M[0] = col1.x; M[4] = col2.x; M[8] = col3.x; M[12] = col4.x;
	M[1] = col1.y; M[5] = col2.y; M[9] = col3.y; M[13] = col4.y;
	M[2] = col1.z; M[6] = col2.z; M[10] = col3.z; M[14] = col4.z;
	M[3] = col1.w; M[7] = col2.w; M[11] = col3.w; M[15] = col4.w;
}

mat4x4::mat4x4(float c1r1, float c1r2, float c1r3, float c1r4, float c2r1, float c2r2, float c2r3, float c2r4, float c3r1, float c3r2, float c3r3, float c3r4, float c4r1, float c4r2, float c4r3, float c4r4)
{
	M[0] = c1r1; M[4] = c2r1; M[8] = c3r1; M[12] = c4r1;
	M[1] = c1r2; M[5] = c2r2; M[9] = c3r2; M[13] = c4r2;
	M[2] = c1r3; M[6] = c2r3; M[10] = c3r3; M[14] = c4r3;
	M[3] = c1r4; M[7] = c2r4; M[11] = c3r4; M[15] = c4r4;
}

mat4x4::mat4x4(const mat2x2 &Matrix)
{
	M[0] = Matrix.M[0]; M[4] = Matrix.M[2]; M[8] = 0.0f; M[12] = 0.0f;
	M[1] = Matrix.M[1]; M[5] = Matrix.M[3]; M[9] = 0.0f; M[13] = 0.0f;
	M[2] = 0.0f; M[6] = 0.0f; M[10] = 1.0f; M[14] = 0.0f;
	M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f;
}

mat4x4::mat4x4(const mat3x3 &Matrix)
{
	M[0] = Matrix.M[0]; M[4] = Matrix.M[3]; M[8] = Matrix.M[6]; M[12] = 0.0f;
	M[1] = Matrix.M[1]; M[5] = Matrix.M[4]; M[9] = Matrix.M[7]; M[13] = 0.0f;
	M[2] = Matrix.M[2]; M[6] = Matrix.M[5]; M[10] = Matrix.M[8]; M[14] = 0.0f;
	M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f;
}

mat4x4& mat4x4::operator = (const mat4x4 &Matrix)
{
	for(int i = 0; i < 16; i++)
	{
		M[i] = Matrix.M[i];
	}

	return *this;
}

float& mat4x4::operator [] (int i)
{
	return M[i];
}

float* mat4x4::operator & ()
{
	return (float*)this;
}

mat4x4 operator * (const mat4x4 &Matrix1, const mat4x4 &Matrix2)
{
	mat4x4 Matrix3;

	Matrix3.M[0] = Matrix1.M[0] * Matrix2.M[0] + Matrix1.M[4] * Matrix2.M[1] + Matrix1.M[8] * Matrix2.M[2] + Matrix1.M[12] * Matrix2.M[3];
	Matrix3.M[1] = Matrix1.M[1] * Matrix2.M[0] + Matrix1.M[5] * Matrix2.M[1] + Matrix1.M[9] * Matrix2.M[2] + Matrix1.M[13] * Matrix2.M[3];
	Matrix3.M[2] = Matrix1.M[2] * Matrix2.M[0] + Matrix1.M[6] * Matrix2.M[1] + Matrix1.M[10] * Matrix2.M[2] + Matrix1.M[14] * Matrix2.M[3];
	Matrix3.M[3] = Matrix1.M[3] * Matrix2.M[0] + Matrix1.M[7] * Matrix2.M[1] + Matrix1.M[11] * Matrix2.M[2] + Matrix1.M[15] * Matrix2.M[3];
	Matrix3.M[4] = Matrix1.M[0] * Matrix2.M[4] + Matrix1.M[4] * Matrix2.M[5] + Matrix1.M[8] * Matrix2.M[6] + Matrix1.M[12] * Matrix2.M[7];
	Matrix3.M[5] = Matrix1.M[1] * Matrix2.M[4] + Matrix1.M[5] * Matrix2.M[5] + Matrix1.M[9] * Matrix2.M[6] + Matrix1.M[13] * Matrix2.M[7];
	Matrix3.M[6] = Matrix1.M[2] * Matrix2.M[4] + Matrix1.M[6] * Matrix2.M[5] + Matrix1.M[10] * Matrix2.M[6] + Matrix1.M[14] * Matrix2.M[7];
	Matrix3.M[7] = Matrix1.M[3] * Matrix2.M[4] + Matrix1.M[7] * Matrix2.M[5] + Matrix1.M[11] * Matrix2.M[6] + Matrix1.M[15] * Matrix2.M[7];
	Matrix3.M[8] = Matrix1.M[0] * Matrix2.M[8] + Matrix1.M[4] * Matrix2.M[9] + Matrix1.M[8] * Matrix2.M[10] + Matrix1.M[12] * Matrix2.M[11];
	Matrix3.M[9] = Matrix1.M[1] * Matrix2.M[8] + Matrix1.M[5] * Matrix2.M[9] + Matrix1.M[9] * Matrix2.M[10] + Matrix1.M[13] * Matrix2.M[11];
	Matrix3.M[10] = Matrix1.M[2] * Matrix2.M[8] + Matrix1.M[6] * Matrix2.M[9] + Matrix1.M[10] * Matrix2.M[10] + Matrix1.M[14] * Matrix2.M[11];
	Matrix3.M[11] = Matrix1.M[3] * Matrix2.M[8] + Matrix1.M[7] * Matrix2.M[9] + Matrix1.M[11] * Matrix2.M[10] + Matrix1.M[15] * Matrix2.M[11];
	Matrix3.M[12] = Matrix1.M[0] * Matrix2.M[12] + Matrix1.M[4] * Matrix2.M[13] + Matrix1.M[8] * Matrix2.M[14] + Matrix1.M[12] * Matrix2.M[15];
	Matrix3.M[13] = Matrix1.M[1] * Matrix2.M[12] + Matrix1.M[5] * Matrix2.M[13] + Matrix1.M[9] * Matrix2.M[14] + Matrix1.M[13] * Matrix2.M[15];
	Matrix3.M[14] = Matrix1.M[2] * Matrix2.M[12] + Matrix1.M[6] * Matrix2.M[13] + Matrix1.M[10] * Matrix2.M[14] + Matrix1.M[14] * Matrix2.M[15];
	Matrix3.M[15] = Matrix1.M[3] * Matrix2.M[12] + Matrix1.M[7] * Matrix2.M[13] + Matrix1.M[11] * Matrix2.M[14] + Matrix1.M[15] * Matrix2.M[15];

	return Matrix3;
}

vec4 operator * (const mat4x4 &Matrix, const vec4 &u)
{
	vec4 v;

	v.x = Matrix.M[0] * u.x + Matrix.M[4] * u.y + Matrix.M[8] * u.z + Matrix.M[12] * u.w;
	v.y = Matrix.M[1] * u.x + Matrix.M[5] * u.y + Matrix.M[9] * u.z + Matrix.M[13] * u.w;
	v.z = Matrix.M[2] * u.x + Matrix.M[6] * u.y + Matrix.M[10] * u.z + Matrix.M[14] * u.w;
	v.w = Matrix.M[3] * u.x + Matrix.M[7] * u.y + Matrix.M[11] * u.z + Matrix.M[15] * u.w;
	
	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------

mat4x4 BiasMatrix = mat4x4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
mat4x4 BiasMatrixInverse = mat4x4(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, -1.0f, -1.0f, -1.0f, 1.0f);

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 4  8 12 | + - + -
// 1 5  9 13 | - + - +
// 2 6 10 14 | + - + -
// 3 7 11 15 | - + - +
//
// ----------------------------------------------------------------------------------------------------------------------------

float det3x3sub(const float *m, int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	float det = 0.0f;

	det += m[i0] * det2x2sub(m, i4, i5, i7, i8);
	det -= m[i3] * det2x2sub(m, i1, i2, i7, i8);
	det += m[i6] * det2x2sub(m, i1, i2, i4, i5);

	return det;
}

mat4x4 inverse(const mat4x4 &Matrix)
{
	const float *m = Matrix.M;

	float det = 0.0f;
	
	det += m[0] * det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	det -= m[4] * det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	det += m[8] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	det -= m[12] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11);

	mat4x4 Inverse;

	Inverse.M[0] = det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15) / det;
	Inverse.M[1] = -det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15) / det;
	Inverse.M[2] = det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15) / det;
	Inverse.M[3] = -det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11) / det;
	Inverse.M[4] = -det3x3sub(m, 4, 6, 7, 8, 10, 11, 12, 14, 15) / det;
	Inverse.M[5] = det3x3sub(m, 0, 2, 3, 8, 10, 11, 12, 14, 15) / det;
	Inverse.M[6] = -det3x3sub(m, 0, 2, 3, 4, 6, 7, 12, 14, 15) / det;
	Inverse.M[7] = det3x3sub(m, 0, 2, 3, 4, 6, 7, 8, 10, 11) / det;
	Inverse.M[8] = det3x3sub(m, 4, 5, 7, 8, 9, 11, 12, 13, 15) / det;
	Inverse.M[9] = -det3x3sub(m, 0, 1, 3, 8, 9, 11, 12, 13, 15) / det;
	Inverse.M[10] = det3x3sub(m, 0, 1, 3, 4, 5, 7, 12, 13, 15) / det;
	Inverse.M[11] = -det3x3sub(m, 0, 1, 3, 4, 5, 7, 8, 9, 11) / det;
	Inverse.M[12] = -det3x3sub(m, 4, 5, 6, 8, 9, 10, 12, 13, 14) / det;
	Inverse.M[13] = det3x3sub(m, 0, 1, 2, 8, 9, 10, 12, 13, 14) / det;
	Inverse.M[14] = -det3x3sub(m, 0, 1, 2, 4, 5, 6, 12, 13, 14) / det;
	Inverse.M[15] = det3x3sub(m, 0, 1, 2, 4, 5, 6, 8, 9, 10) / det;

	return Inverse;
}

mat4x4 look(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 Z = normalize(eye - center);
	vec3 X = normalize(cross(up, Z));
	vec3 Y = cross(Z, X);

	mat4x4 View;

	View.M[0] = X.x;
	View.M[1] = Y.x;
	View.M[2] = Z.x;
	View.M[4] = X.y;
	View.M[5] = Y.y;
	View.M[6] = Z.y;
	View.M[8] = X.z;
	View.M[9] = Y.z;
	View.M[10] = Z.z;
	View.M[12] = -dot(X, eye);
	View.M[13] = -dot(Y, eye);
	View.M[14] = -dot(Z, eye);

	return View;
}

mat4x4 ortho(float left, float right, float bottom, float top, float n, float f)
{
	mat4x4 Ortho;

	Ortho.M[0] = 2.0f / (right - left);
	Ortho.M[5] = 2.0f / (top - bottom);
	Ortho.M[10] = -2.0f / (f - n);
	Ortho.M[12] = -(right + left) / (right - left);
	Ortho.M[13] = -(top + bottom) / (top - bottom);
	Ortho.M[14] = -(f + n) / (f - n);

	return Ortho;
}

mat4x4 perspective(float fovy, float aspect, float n, float f)
{
	mat4x4 Perspective;

	float coty = 1.0f / tan(fovy * (float)M_PI / 360.0f);

	Perspective.M[0] = coty / aspect;
	Perspective.M[5] = coty;
	Perspective.M[10] = (n + f) / (n - f);
	Perspective.M[11] = -1.0f;
	Perspective.M[14] = 2.0f * n * f / (n - f);
	Perspective.M[15] = 0.0f;

	return Perspective;
}

mat4x4 rotate(float angle, const vec3 &u)
{
	mat4x4 Rotate;

	angle = angle / 180.0f * (float)M_PI;

	vec3 v = normalize(u);

	float c = 1.0f - cos(angle), s = sin(angle);

	Rotate.M[0] = 1.0f + c * (v.x * v.x - 1.0f);
	Rotate.M[1] = c * v.x * v.y + v.z * s;
	Rotate.M[2] = c * v.x * v.z - v.y * s;
	Rotate.M[4] = c * v.x * v.y - v.z * s;
	Rotate.M[5] = 1.0f + c * (v.y * v.y - 1.0f);
	Rotate.M[6] = c * v.y * v.z + v.x * s;
	Rotate.M[8] = c * v.x * v.z + v.y * s;
	Rotate.M[9] = c * v.y * v.z - v.x * s;
	Rotate.M[10] = 1.0f + c * (v.z * v.z - 1.0f);

	return Rotate;
}

mat4x4 scale(float x, float y, float z)
{
	mat4x4 Scale;

	Scale.M[0] = x;
	Scale.M[5] = y;
	Scale.M[10] = z;

	return Scale;
}

mat4x4 translate(float x, float y, float z)
{
	mat4x4 Translate;

	Translate.M[12] = x;
	Translate.M[13] = y;
	Translate.M[14] = z;

	return Translate;
}

mat4x4 transpose(const mat4x4 &Matrix)
{
	mat4x4 Transpose;

	Transpose.M[0] = Matrix.M[0];
	Transpose.M[1] = Matrix.M[4];
	Transpose.M[2] = Matrix.M[8];
	Transpose.M[3] = Matrix.M[12];
	Transpose.M[4] = Matrix.M[1];
	Transpose.M[5] = Matrix.M[5];
	Transpose.M[6] = Matrix.M[9];
	Transpose.M[7] = Matrix.M[13];
	Transpose.M[8] = Matrix.M[2];
	Transpose.M[9] = Matrix.M[6];
	Transpose.M[10] = Matrix.M[10];
	Transpose.M[11] = Matrix.M[14];
	Transpose.M[12] = Matrix.M[3];
	Transpose.M[13] = Matrix.M[7];
	Transpose.M[14] = Matrix.M[11];
	Transpose.M[15] = Matrix.M[15];

	return Transpose;
}

void ComputeNormalWithTriangle(vec3 &destnormal, vec3 &a, vec3 &b, vec3 &c)
{
	vec3 d = (b - a) * (c - a);
	destnormal = d / length(d);
}
