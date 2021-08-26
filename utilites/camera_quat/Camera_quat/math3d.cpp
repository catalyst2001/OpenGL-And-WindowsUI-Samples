#include <cmath>
#include "math3d.h"

VECTOR3D Add(VECTOR3D a, VECTOR3D b) //Ya implementado como ejemplo.
{
	VECTOR3D ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

VECTOR3D Substract(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

VECTOR3D Multiply(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;
}

VECTOR3D MultiplyWithScalar(float scalar, VECTOR3D a) {
	VECTOR3D ret;
	ret.x = a.x * scalar;
	ret.y = a.y * scalar;
	ret.z = a.z * scalar;
	return ret;
}

double Magnitude(VECTOR3D a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

VECTOR3D Normalize(VECTOR3D a) {
    if (a.x == 0 && a.y == 0 && a.z == 0) return {0, 0, 0};
	return MultiplyWithScalar(1 / Magnitude(a), a);
}

VECTOR3D CrossProduct(VECTOR3D a, VECTOR3D b) {
	return { a.y*b.z - a.z*b.y, // X
			 a.z*b.x - a.x*b.z, // Y
			 a.x*b.y - a.y*b.x};// Z
}

double DotProduct(VECTOR3D a, VECTOR3D b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

MATRIX3 Transpose(MATRIX3 m) {
	MATRIX3 ret;
	ret.column0 = { m.column0.x, m.column1.x, m.column2.x };
	ret.column1 = { m.column0.y, m.column1.y, m.column2.y };
	ret.column2 = { m.column0.z, m.column1.z, m.column2.z };
	return ret;
}

VECTOR3D Transform(MATRIX3 m, VECTOR3D a) {
	MATRIX3 mt = Transpose(m);

	return { DotProduct(m.column0, a), DotProduct(m.column1, a), DotProduct(m.column2, a) };
}

MATRIX4 InverseOrthogonalMatrix(MATRIX3 A, VECTOR3D t) {
	MATRIX4 R;
	
	MATRIX3 At = Transpose(A);

	R.m[0] = At.column0.x;
	R.m[1] = At.column0.y;
	R.m[2] = At.column0.z;
	R.m[3] = 0;

	R.m[4] = At.column1.x;
	R.m[5] = At.column1.y;
	R.m[6] = At.column1.z;
	R.m[7] = 0;

	R.m[8] = At.column2.x;
	R.m[9] = At.column2.y;
	R.m[10] = At.column2.z;
	R.m[11] = 0;

	R.m[12] = -DotProduct(t, A.column0);
	R.m[13] = -DotProduct(t, A.column1);
	R.m[14] = -DotProduct(t, A.column2);
	R.m[15] = 1;

	return R;
}

QUATERNION QuaternionFromAngleAxis(float angle, VECTOR3D axis) {
    double theta = angle * DTOR;

    return {
        cos(theta / 2),
        MultiplyWithScalar(-sin(theta / 2), Normalize(axis))
    };
}
QUATERNION QuaternionFromToVectors(VECTOR3D from, VECTOR3D to) {
    VECTOR3D uFrom = Normalize(from);
    VECTOR3D uTo = Normalize(to);

    return QuaternionFromAngleAxis(
                acos(DotProduct(uFrom, uTo)), // Angle form dot product.
                CrossProduct(uFrom, uTo));    // Axis to rotate around.
}

QUATERNION Multiply(QUATERNION a, QUATERNION b) {
    return {
        a.s*b.s   - a.v.x*b.v.x - a.v.y*b.v.y - a.v.z*b.v.z,
        {
        a.s*b.v.x + a.v.x*b.s   + a.v.y*b.v.z - a.v.z*b.v.y,
        a.s*b.v.y - a.v.x*b.v.z + a.v.y*b.s   + a.v.z*b.v.x,
        a.s*b.v.z + a.v.x*b.v.y - a.v.y*b.v.x + a.v.z*b.s   }
    };
}

QUATERNION Conjugate(QUATERNION a) {
    return {
        a.s,
        { -a.v.x, -a.v.y, -a.v.z }
    };
}
VECTOR3D RotateWithQuaternion(VECTOR3D a, QUATERNION q) {
    return Multiply(q, Multiply({ 0, a }, Conjugate(q))).v;
}