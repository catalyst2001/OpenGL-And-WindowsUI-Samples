#pragma once
#include "glmath.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>

/**
* Quaternion Camera Class
* Edited by: Kirill Deryabin, Roman Tynin
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
		m_vecMovement = normalize(m_vecMovement);
		m_vecOrigin = m_vecOrigin + m_vecDirection * m_vecMovement.z; // updateForwardMovement
		m_vecOrigin = m_vecOrigin + m_vecMovement.y * m_vecUp; // updateUpMovement
		vec3 sideVector = normalize(cross(m_vecUp, m_vecDirection)); // updateSideMovement
		m_vecOrigin = m_vecOrigin + m_vecMovement.x * sideVector;
		m_vecMovement = { 0, 0, 0 };

		POINT pp;
		int centerX = m_nScreenWidth >> 1;
		int centerY = m_nScreenHeight >> 1;
		GetCursorPos(&pp);

		//If the mouse does not move, there is no point in performing calculations. Û :) 
		if (pp.x == centerX && pp.y == centerY)
			return;

		euler.m_fYaw -= (centerX - pp.x) * 0.1f;
		euler.m_fPitch -= (centerY - pp.y) * 0.1f;
		SetCursorPos(centerX, centerY);
	}

	//TODO: replace this to Matrix4x4
	void Look() {
		vec3 target = m_vecOrigin + m_vecDirection;
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
		euler.m_quatOrient = qYaw * qPitch * qRoll;
	}
	inline vec3 GetForward() { return vec_rotate_with_quat(__FORWARD, euler.m_quatOrient); }
	inline vec3 GetUp() { return vec_rotate_with_quat(__FORWARD, euler.m_quatOrient); }
};