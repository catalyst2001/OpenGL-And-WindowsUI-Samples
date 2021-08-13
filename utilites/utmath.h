#pragma once
#include <math.h>
#include <float.h> //FLT_EPSILON, FLT_PI

enum {
	X, Y, Z, W
};

#ifndef min
#define min(a, b) (a < b) ? a : b
#endif

#ifndef max
#define max(a, b) (a > b) ? a : b
#endif

#ifndef abs
#define abs(f) (f < 0.f) ? -f : f
#endif

//https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
float clampf(float d, float min, float max)
{
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

class CVector2
{
public:
	CVector2() {}
	CVector2(float xx, float yy, float zz) : x(xx), y(yy) {}

	CVector2 &operator+(const CVector2 &vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	float x, y;
};

class CVector3
{
public:
	CVector3() {}
	CVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	CVector3(float vv[3]) : x(vv[0]), y(vv[1]), z(vv[2]) {}
	CVector3(float y, float p) : x( -sin(y) * cos(p) ), y( cos(y) * cos(p) ), z( sin(p) ) {}
	~CVector3() {}
	__forceinline bool IsZero() { return x == 0.f && y == 0.f && z == 0.f; }
	__forceinline void SetZero() { x = 0.f, y = 0.f, z = 0.f; }
	__forceinline float SquaredLen() const { return x * x + y * y + z * z; }
	__forceinline float DotProduct(CVector3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; } //скалярное произведение векторов
	
	//перекрестное произведение векторов
	//TODO: проверить, возможно косячно работает
	__forceinline CVector3 &CrossProduct(CVector3 &vec) {
		x = (y * vec.z - z * vec.y);
		y = -(x * vec.z - z * vec.x);
		z = (x * vec.y - y * vec.x);
		return *this;
	}
	CVector3 &Min(CVector3 &v) { x = min(x, v.x); y = min(y, v.y); z = min(y, v.y); return *this; }
	CVector3 &Max(CVector3 &v) { x = max(x, v.x); y = max(y, v.y); z = max(y, v.y); return *this; }
	CVector3 &Min(float f) { x = min(x, f); y = min(y, f); z = min(y, f); return *this; }
	CVector3 &Max(float f) { x = max(x, f); y = max(y, f); z = max(y, f); return *this; }
	CVector3 &Clamp(float min, float max) { x = clampf(x, min, max); y = clampf(y, min, max); z = clampf(z, min, max); return *this; }
	void Negative() { x = -x; y = -y; z = -z; }
	float Magnitude() const { return sqrtf(SquaredLen()); }
	CVector3 &Scale(float k) { x *= k; y *= k; z *= k; return *this; }

	//TODO: check epsilon in normalization
	CVector3 &Normalize() {
		float mag = Magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
		return *this;
	}
	bool IsNormalized() { float l = SquaredLen(); return (l > 0.99f && l < 1.01f); }
	float Distance(CVector3 &v, CVector3 &dest) { dest = *this; dest - v; return dest.Magnitude(); }

	//TODO: check linear interpolation
	CVector3 &Lerp(CVector3 &v, float t) {
		x += (v.x - x) * t;
		y += (v.y - y) * t;
		z += (v.z - z) * t;
		return *this;
	}

	CVector3 &operator+(const CVector3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; } //add vector
	CVector3 &operator+=(const CVector3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; } //add vector
	CVector3 &operator-(const CVector3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; } //sub vector
	CVector3 &operator-=(const CVector3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; } //sub vector
	CVector3 &operator*(const CVector3 &vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; } //mul vector
	CVector3 &operator/(const CVector3 &vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this; } //div vector
	bool operator==(CVector3 vec) { return x == vec.x && y == vec.y && z == vec.z; }
	bool operator!=(CVector3 vec) { return x != vec.x && y != vec.y && z != vec.z; }

	union {
		struct { float x, y, z; };
		float v[3];
	};
};


//поверхность
//3d plane at 4 points
class CRect3
{
public:
	CRect3() {}
	CRect3(CVector3 start, float width) { InitByStartPoint(start, width); }
	CRect3(CVector3 start, CVector3 end) { InitByVectors(start, end); }

	void InitByStartPoint(CVector3 start, float width)
	{
		p1 = start;
		p2.x = start.x + width;
		p2.y = start.y;
		p2.z = start.z;
		p3.x = start.x + width;
		p3.y = start.y;
		p3.z = start.z + width;
		p4.x = start.x;
		p4.y = start.y;
		p4.z = start.z + width;
	}

	void InitByVectors(CVector3 &start, CVector3 &end) {
		/*
				 (2)-----------------(3)
				   \                   \
					\                   \
					 \                   \
					 (1)-----------------(4)
		*/
		float width = fabs(end.z - start.z);
		float length = fabs(end.x - start.x);
		p1 = start;
		p2.x = start.x + width;
		p2.y = start.y;
		p2.z = start.z;

		p3.x = start.x + width;
		p3.y = start.y;
		p3.z = start.z + length;

		p4.x = start.x;
		p4.y = start.y;
		p4.z = start.z + length;
	}
	~CRect3() {}

	CVector3 p1, p2, p3, p4;
};

class CTriangle
{
public:
	CTriangle() {}
	CTriangle(CVector3 pp1, CVector3 pp2, CVector3 pp3) : p1(pp1), p2(pp2), p3(pp3) {};
	~CTriangle() {}

	CVector3 p1, p2, p3;
};

//ray
enum rayintersecttypes {
	RAY_INFINITY, //луч направлен в пустое пространство и ни с чем не пересекается
	RAY_INTERSECT //луч что то пересек
};

#define innerProduct(v,q) \
	((v)[0] * (q)[0] + \
	(v)[1] * (q)[1] + \
	(v)[2] * (q)[2])

#define crossProduct(a,b,c) \
	(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
	(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
	(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

/* a = b - c */
#define vector(a,b,c) \
	(a)[0] = (b)[0] - (c)[0];	\
	(a)[1] = (b)[1] - (c)[1];	\
	(a)[2] = (b)[2] - (c)[2];

int rayIntersectsTriangle(float *p, float *d, float *v0, float *v1, float *v2) {

	float e1[3], e2[3], h[3], s[3], q[3];
	float a, f, u, v;
	vector(e1, v1, v0);
	vector(e2, v2, v0);

	crossProduct(h, d, e2);
	a = innerProduct(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1 / a;
	vector(s, p, v0);
	u = f * (innerProduct(s, h));

	if (u < 0.0 || u > 1.0)
		return(false);

	crossProduct(q, s, e1);
	v = f * innerProduct(d, q);

	if (v < 0.0 || u + v > 1.0)
		return(false);

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float t = f * innerProduct(e2, q);

	if (t > 0.00001) // ray intersection
		return(true);

	else // this means that there is a line intersection
		 // but not a ray intersection
		return (false);

}

class CRay
{
public:
	CRay() {}
	CRay(CVector3 sstart, CVector3 ddir) : m_origin(sstart), m_direction(ddir) {}
	CRay(float xorigin, float yorigin, float zorigin, float xdir, float ydir, float zdir) : m_origin(CVector3(xorigin, yorigin, zorigin)), m_direction(CVector3(xdir, ydir, zdir)) {}
	~CRay() {}

	void SetOrigin(CVector3 origin) { m_origin = origin; }
	void SetOrigin(float x, float y, float z) { m_origin = CVector3(x, y, z); }
	void SetDirection(CVector3 dir) { m_direction = dir; }
	void SetDirection(float x, float y, float z) { m_direction = CVector3(x, y, z); }

	float TriangleIntersection(CVector3 &v0, CVector3 &v1, CVector3 &v2) {
		CVector3 e1 = v1 - v0;
		CVector3 e2 = v2 - v0;
		// Вычисление вектора нормали к плоскости
		CVector3 pvec = m_direction.CrossProduct(e2);
		float det = e1.DotProduct(pvec);

		// Луч параллелен плоскости
		if (det < 1e-8 && det > -1e-8) {
			return 0.f;
		}

		float inv_det = 1 / det;
		CVector3 tvec = m_origin - v0;
		float u = tvec.DotProduct(pvec) * inv_det;
		if (u < 0 || u > 1) {
			return 0.f;
		}

		CVector3 qvec = tvec.CrossProduct(e1);
		float v = m_direction.DotProduct(qvec) * inv_det;
		if (v < 0 || u + v > 1) {
			return 0.f;
		}
		return e2.DotProduct(qvec) * inv_det;
	}

	int PlaneIntersection(CRect3 &plane) {
		float denominator = 0;
		//https://stackoverflow.com/questions/23975555/how-to-do-ray-plane-intersection
	}

	CVector3 m_origin;
	CVector3 m_direction;
};