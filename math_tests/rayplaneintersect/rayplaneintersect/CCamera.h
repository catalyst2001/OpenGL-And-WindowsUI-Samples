#pragma once
#include "glmath.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>

#include <stdio.h>

/**
* Quaternion Camera Class
* Created: 27.08.2021
* Modified: 27.08.2021
* 
* Camera initializing:
* NOTE: 1) Call constructor and set position and rotation values. 
*       2) Next, call the UpdateViewport function, passing there the screen resolution at the moment.
*          Always update it in the camera, otherwise the cursor will be positioned incorrectly!
*/
static const vec3 __YAW_AXIS	= { 0.f, 1.f, 0.f };
static const vec3 __PITCH_AXIS	= { 1.f, 0.f, 0.f };
static const vec3 __ROLL_AXIS	= { 0.f, 0.f, 1.f };
static const vec3 __FORWARD		= { 0.f, 0.f, -1.f };
static const vec3 __UP			= { 0.f, 1.f, 0.f };

class CCamera
{
public:
	CCamera() {}

	CCamera(vec3 pos, vec3 rotation) {
		m_vecOrigin = pos;
		euler.m_quatOrient = vec4(0.f, 0.f, 0.f, 0.f);
		euler.m_fPitch = rotation.x;
		euler.m_fYaw = rotation.y;
		euler.m_fRoll = rotation.z;
		m_fFOV = 45.0f;
		UpdateEulerOrientation();
	}

	CCamera(vec3 pos, vec3 rotation, float fov) {
		m_vecOrigin = pos;
		euler.m_quatOrient = vec4(0.f, 0.f, 0.f, 0.f);
		euler.m_fPitch = rotation.x;
		euler.m_fYaw = rotation.y;
		euler.m_fRoll = rotation.z;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}

	CCamera(float posx, float posy, float posz, float pitch, float yaw, float roll, float fov) {
		m_vecOrigin = vec3(posx, posy, posz);
		euler.m_quatOrient = vec4(0.f, 0.f, 0.f, 0.f);
		euler.m_fPitch = pitch;
		euler.m_fYaw = yaw;
		euler.m_fRoll = roll;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}

	CCamera(float posx, float posy, float posz, float pitch, float yaw, float roll, float fov, vec4 quatdir) {
		m_vecOrigin = vec3(posx, posy, posz);
		euler.m_quatOrient = quatdir;
		euler.m_fPitch = pitch;
		euler.m_fYaw = yaw;
		euler.m_fRoll = roll;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}
	~CCamera() {}

	void UpdateViewport(int width, int height) {
		m_nScreenWidth = width;
		m_nScreenHeight = height;
	}

	void Update() {
		UpdateEulerOrientation();
		m_vecDirection = GetForward();
		m_vecUp = GetUp();
		m_vecMovement = Normalize(m_vecMovement);
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.z, m_vecDirection)); // updateForwardMovement
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.y, m_vecUp)); // updateUpMovement
		vec3 sideVector = Normalize(cross(m_vecUp, m_vecDirection)); // updateSideMovement
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.x, sideVector));
		m_vecMovement = { 0, 0, 0 };

		printf("(%f %f %f) (%f %f %f)\n", m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z, m_vecDirection.x, m_vecDirection.y, m_vecDirection.z);

		if (GetKeyState('W') & 0x80)
			m_vecMovement.z = 1.f;
		if(GetKeyState('S') & 0x80)
			m_vecMovement.z = -1.f;
		if (GetKeyState('A') & 0x80)
			m_vecMovement.x = 1.f;
		if (GetKeyState('D') & 0x80)
			m_vecMovement.x = -1.f;

		POINT pp;
		int centerX = m_nScreenWidth >> 1;
		int centerY = m_nScreenHeight >> 1;
		GetCursorPos(&pp);

		//If the mouse does not move, there is no point in performing calculations. Û :) 
		if (pp.x == centerX && pp.y == centerY)
			return;

		euler.m_fYaw += (centerX - pp.x) * -0.1f;
		euler.m_fPitch += (centerY - pp.y) * -0.1f;
		SetCursorPos(centerX, centerY);
	}

	//TODO: replace this to Matrix4x4
	void Look() {
		vec3 target = m_vecOrigin + m_vecDirection;/*m_vecDirection;*/
		gluLookAt(m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z, target.x, target.y, target.z, m_vecUp.x, m_vecUp.y, m_vecUp.z);
	}

	inline float &GetFOV() { return m_fFOV; } //field of view
	inline void SetFOV(float fov) { m_fFOV = fov; }
	inline vec3 &GetUpVector() { return m_vecUp; }
	inline void SetUpVector(vec3 upVec) { m_vecUp = upVec; }
	inline vec3 &GetOrigin() { return m_vecOrigin; }
	inline void SetOrigin(vec3 originVec) { m_vecOrigin = originVec; }
	inline vec3 &GetDirection() { return m_vecDirection; }
	inline void SetDirection(vec3 dirVec) { m_vecDirection = dirVec; }
	inline vec3 &GetMovement() { return m_vecMovement; }
	inline void SetMovement(vec3 movementVec) { m_vecMovement = movementVec; }
	//W = 1, S = -1
	inline void MoveForward(float val) { m_vecMovement.z = val; }

	//A = 1, D = -1
	inline void MoveStrafe(float val) { m_vecMovement.x = val; }

	//rotation
	inline float GetPitch() { return euler.m_fPitch; }
	inline float GetYaw() { return euler.m_fYaw; }
	inline float GetRoll() { return euler.m_fRoll; }
	inline vec3 GetRotation() { return vec3(euler.m_fPitch, euler.m_fYaw, euler.m_fRoll); }

	float m_fFOV;
	struct {
		float m_fPitch;
		float m_fYaw;
		float m_fRoll;
		vec4 m_quatOrient;
	} euler;
	vec3 m_vecUp;
	vec3 m_vecOrigin;
	vec3 m_vecDirection;
	vec3 m_vecMovement;
	int m_nScreenWidth;
	int m_nScreenHeight;

private:
	void UpdateEulerOrientation() {
		vec4 qYaw = quat_from_axis_angle(euler.m_fYaw, __YAW_AXIS);
		vec4 qPitch = quat_from_axis_angle(euler.m_fPitch, __PITCH_AXIS);
		vec4 qRoll = quat_from_axis_angle(euler.m_fRoll, __ROLL_AXIS);
		euler.m_quatOrient = Multiply(qYaw, Multiply(qPitch, qRoll));
	}
	inline vec3 GetForward() { return vec_rotate_with_quat(__FORWARD, euler.m_quatOrient); }
	inline vec3 GetUp() { return vec_rotate_with_quat(__FORWARD, euler.m_quatOrient); }
};

/////////////////////////// GAVNO SKOPIROVANO //////////////////////////////////////////
typedef struct {
	double x, y, z;
} VECTOR3D;

static const VECTOR3D YAW_AXIS = { 0.f, 1.f, 0.f };
static const VECTOR3D PITCH_AXIS = { 1.f, 0.f, 0.f };
static const VECTOR3D ROLL_AXIS = { 0.f, 0.f, 1.f };
static const VECTOR3D FORWARD = { 0.f, 0.f, -1.f };
static const VECTOR3D UP = { 0.f, 1.f, 0.f };

typedef struct {
	double s;
	VECTOR3D v;
} QUATERNION;

typedef struct {
	float yaw; // Rotation around the Y axis.
	float pitch; // Rotation around the X axis.
	float roll; // Rotation around the Z axis.
	QUATERNION orientation; // Cached quaternion equivalent of this euler object.
} EULER;

static VECTOR3D Add(VECTOR3D a, VECTOR3D b) //Ya implementado como ejemplo.
{
	VECTOR3D ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

static VECTOR3D Substract(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

static VECTOR3D Multiply(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;
}

static VECTOR3D MultiplyWithScalar(float scalar, VECTOR3D a) {
	VECTOR3D ret;
	ret.x = a.x * scalar;
	ret.y = a.y * scalar;
	ret.z = a.z * scalar;
	return ret;
}

static double Magnitude(VECTOR3D a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

static VECTOR3D Normalize(VECTOR3D a) {
	if (a.x == 0 && a.y == 0 && a.z == 0) return { 0, 0, 0 };
	return MultiplyWithScalar(1 / Magnitude(a), a);
}

static VECTOR3D CrossProduct(VECTOR3D a, VECTOR3D b) {
	return { a.y*b.z - a.z*b.y, // X
			 a.z*b.x - a.x*b.z, // Y
			 a.x*b.y - a.y*b.x };// Z
}

static double DotProduct(VECTOR3D a, VECTOR3D b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static QUATERNION QuaternionFromAngleAxis(float angle, VECTOR3D axis) {
	double theta = angle * DTOR;

	return {
		cos(theta / 2),
		MultiplyWithScalar(-sin(theta / 2), Normalize(axis))
	};
}

static QUATERNION QuaternionFromToVectors(VECTOR3D from, VECTOR3D to) {
	VECTOR3D uFrom = Normalize(from);
	VECTOR3D uTo = Normalize(to);

	return QuaternionFromAngleAxis(
		acos(DotProduct(uFrom, uTo)), // Angle form dot product.
		CrossProduct(uFrom, uTo));    // Axis to rotate around.
}

static QUATERNION Multiply(QUATERNION a, QUATERNION b) {
	return {
		a.s*b.s - a.v.x*b.v.x - a.v.y*b.v.y - a.v.z*b.v.z,
		{
		a.s*b.v.x + a.v.x*b.s + a.v.y*b.v.z - a.v.z*b.v.y,
		a.s*b.v.y - a.v.x*b.v.z + a.v.y*b.s + a.v.z*b.v.x,
		a.s*b.v.z + a.v.x*b.v.y - a.v.y*b.v.x + a.v.z*b.s   }
	};
}

static QUATERNION Conjugate(QUATERNION a) {
	return {
		a.s,
		{ -a.v.x, -a.v.y, -a.v.z }
	};
}

static VECTOR3D RotateWithQuaternion(VECTOR3D a, QUATERNION q) {
	return Multiply(q, Multiply({ 0, a }, Conjugate(q))).v;
}

static void updateEulerOrientation(EULER &euler) {
	QUATERNION qYaw = QuaternionFromAngleAxis(euler.yaw, YAW_AXIS);
	QUATERNION qPitch = QuaternionFromAngleAxis(euler.pitch,PITCH_AXIS);
	QUATERNION qRoll = QuaternionFromAngleAxis(euler.roll, ROLL_AXIS);

	euler.orientation = Multiply(qYaw, Multiply(qPitch, qRoll));
}

static VECTOR3D getForward(EULER euler) {
	return RotateWithQuaternion(FORWARD, euler.orientation);
}

static VECTOR3D getUp(EULER euler) {
	return RotateWithQuaternion(UP, euler.orientation);
}

class CCamera2
{
public:
	CCamera2() {}

	CCamera2(VECTOR3D pos, VECTOR3D rotation) {
		m_vecOrigin = pos;
		euler.orientation = { 0.f, 0.f, 0.f, 0.f };
		euler.pitch = rotation.x;
		euler.yaw = rotation.y;
		euler.roll = rotation.z;
		m_fFOV = 45.0f;
		UpdateEulerOrientation();
	}

	CCamera2(VECTOR3D pos, VECTOR3D rotation, float fov) {
		m_vecOrigin = pos;
		euler.orientation = { 0.f, 0.f, 0.f, 0.f };
		euler.pitch = rotation.x;
		euler.yaw = rotation.y;
		euler.roll = rotation.z;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}

	CCamera2(float posx, float posy, float posz, float pitch, float yaw, float roll, float fov) {
		m_vecOrigin = { posx, posy, posz };
		euler.orientation = { 0.f, 0.f, 0.f, 0.f };
		euler.pitch = pitch;
		euler.yaw = yaw;
		euler.roll = roll;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}

	CCamera2(float posx, float posy, float posz, float pitch, float yaw, float roll, float fov, QUATERNION quatdir) {
		m_vecOrigin = { posx, posy, posz };
		euler.orientation = quatdir;
		euler.pitch = pitch;
		euler.yaw = yaw;
		euler.roll = roll;
		m_fFOV = fov;
		UpdateEulerOrientation();
	}
	~CCamera2() {}

	void Init() {
		m_vecOrigin.x = 5;
		m_vecOrigin.y = 10;
		m_vecOrigin.z = 20;

		euler = { -25, 25, 0, {0, {0, 0, 0}} };
		updateEulerOrientation(euler);
		m_vecDirection = getForward(euler);
		m_vecUp = getUp(euler);
	}

	void UpdateViewport(int width, int height) {
		m_nScreenWidth = width;
		m_nScreenHeight = height;
	}

	void Update() {
		UpdateEulerOrientation();
		m_vecDirection = GetForward();
		m_vecUp = GetUp();
		m_vecMovement = Normalize(m_vecMovement);
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.z, m_vecDirection)); // updateForwardMovement
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.y, m_vecUp)); // updateUpMovement
		VECTOR3D sideVector = Normalize(CrossProduct(m_vecUp, m_vecDirection)); // updateSideMovement
		m_vecOrigin = Add(m_vecOrigin, MultiplyWithScalar(m_vecMovement.x, sideVector));
		m_vecMovement = { 0, 0, 0 };

		printf("(%f %f %f) (%f %f %f)\n", m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z, m_vecDirection.x, m_vecDirection.y, m_vecDirection.z);

		if (GetKeyState('W') & 0x80)
			m_vecMovement.z = 1.f;
		if (GetKeyState('S') & 0x80)
			m_vecMovement.z = -1.f;
		if (GetKeyState('A') & 0x80)
			m_vecMovement.x = 1.f;
		if (GetKeyState('D') & 0x80)
			m_vecMovement.x = -1.f;

		POINT pp;
		int centerX = m_nScreenWidth >> 1;
		int centerY = m_nScreenHeight >> 1;
		GetCursorPos(&pp);

		//If the mouse does not move, there is no point in performing calculations. Û :) 
		if (pp.x == centerX && pp.y == centerY)
			return;

		euler.yaw += (centerX - pp.x) * -0.1f;
		euler.pitch += (centerY - pp.y) * -0.1f;
		SetCursorPos(centerX, centerY);
	}

	//TODO: replace this to Matrix4x4
	void Look() {
		VECTOR3D target = Add(m_vecOrigin, m_vecDirection);
		gluLookAt(m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z, target.x, target.y, target.z, m_vecUp.x, m_vecUp.y, m_vecUp.z);
	}

	inline float &GetFOV() { return m_fFOV; } //field of view
	inline void SetFOV(float fov) { m_fFOV = fov; }
	inline vec3 &GetUpVector() { return *(vec3*)&m_vecUp; }
	inline void SetUpVector(VECTOR3D upVec) { m_vecUp = upVec; }
	inline vec3 &GetOrigin() { return *(vec3*)&m_vecOrigin; }
	inline void SetOrigin(VECTOR3D originVec) { m_vecOrigin = originVec; }
	inline vec3 &GetDirection() { return *(vec3*)&m_vecDirection; }
	inline void SetDirection(VECTOR3D dirVec) { m_vecDirection = dirVec; }
	inline vec3 &GetMovement() { return *(vec3*)&m_vecMovement; }
	inline void SetMovement(VECTOR3D movementVec) { m_vecMovement = movementVec; }
	//W = 1, S = -1
	inline void MoveForward(float val) { m_vecMovement.z = val; }

	//A = 1, D = -1
	inline void MoveStrafe(float val) { m_vecMovement.x = val; }

	//rotation
	inline float GetPitch() { return euler.pitch; }
	inline float GetYaw() { return euler.yaw; }
	inline float GetRoll() { return euler.roll; }
	inline vec3 GetRotation() { return vec3(euler.pitch, euler.yaw, euler.roll); }

	float m_fFOV;
	EULER euler;
	VECTOR3D m_vecUp;
	VECTOR3D m_vecOrigin;
	VECTOR3D m_vecDirection;
	VECTOR3D m_vecMovement;
	int m_nScreenWidth;
	int m_nScreenHeight;

private:
	void UpdateEulerOrientation() {
		QUATERNION qYaw = QuaternionFromAngleAxis(euler.yaw, YAW_AXIS);
		QUATERNION qPitch = QuaternionFromAngleAxis(euler.pitch, PITCH_AXIS);
		QUATERNION qRoll = QuaternionFromAngleAxis(euler.roll, ROLL_AXIS);
		euler.orientation = Multiply(qYaw, Multiply(qPitch, qRoll));
	}
	inline VECTOR3D GetForward() { return RotateWithQuaternion(FORWARD, euler.orientation); }
	inline VECTOR3D GetUp() { return RotateWithQuaternion(FORWARD, euler.orientation); }
};