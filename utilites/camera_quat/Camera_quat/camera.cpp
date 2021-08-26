#include "camera.h"
#include <cmath>

// TODO
// devuelve los valores de distancia de los planos a partir del fov horizontal
FRUSTUM makeFrustum(double fovX, double aspectRatio, double nearValue, double farValue)
{
	const double DEG2RAD = 3.14159265 / 180;

	double tangent = tan(fovX / 2 * DEG2RAD); // tangent of half fovX
	double height = nearValue * tangent; // half height of near plane
	double width = height * aspectRatio; // half width of near plane

	FRUSTUM ret;
	ret.top = height;
	ret.left = -width;
	ret.bottom = -height;
	ret.right = width;
	ret.nearValue = nearValue;
	ret.farValue = farValue;
	// TODO : rellenar valores de ret
	return ret;
}

MATRIX4 lookAt(VECTOR3D eyePosition, VECTOR3D target, VECTOR3D upVector) {
	VECTOR3D z = Normalize(Substract(eyePosition, target));
	VECTOR3D x = CrossProduct(Normalize(upVector), z);
	VECTOR3D y = CrossProduct(z, x);

	MATRIX3 rot = { x, y, z };

	return InverseOrthogonalMatrix(rot, eyePosition);
}


void updateEulerOrientation(EULER &euler) {
    QUATERNION qYaw = QuaternionFromAngleAxis(euler.yaw, YAW_AXIS);
    QUATERNION qPitch = QuaternionFromAngleAxis(euler.pitch, PITCH_AXIS);
    QUATERNION qRoll = QuaternionFromAngleAxis(euler.roll, ROLL_AXIS);

    euler.orientation = Multiply(qYaw, Multiply(qPitch, qRoll));
}

VECTOR3D getForward(EULER euler) {
    return RotateWithQuaternion(FORWARD, euler.orientation);
}

VECTOR3D getUp(EULER euler) {
    return RotateWithQuaternion(UP, euler.orientation);
}