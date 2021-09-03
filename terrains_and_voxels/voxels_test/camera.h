#pragma once
#ifndef __CAMERA
#define __CAMERA
/*
	Ёто глобально:
	camera cam(45.0, { 0.0f,0.0f,0.0f });
	Ёто в функцию отрисовки:
	cam.UpdateCameraState();
	gluLookAt(cam.m_vecOrigin.x, cam.m_vecOrigin.y, cam.m_vecOrigin.z, m_vecDirection.x, m_vecDirection.y, m_vecDirection.z, cam.m_vecUp.x, cam.m_vecUp.y, cam.m_vecUp.z);
	Ёто при создание окна, и при смене розрешени€:
	cam.m_iScreenWidth = width;
	cam.m_iScreenHeight = height;

	ћќ∆Ќќ —“ј¬»“№ ѕјћя“Ќ»  Ќј’”…
	UPD: ƒопилено 04.09.2021 в 0:59 ƒер€биным  ириллом с трех компилов,
	который между прочем даже сегодн€ не очень сильно выебал себе мозг, пока расчищал это говно! я сам до сих пор охуеваю!!!
	Ќе пизди тапочком. Ѕывает камеру нельз€ повернуть под 360 вокруг X, но посмотрим уже дальше из за чего это
*/
#include <Windows.h> // GetCursorPos/SetCursorPos
#include "../../utilites/glmath.h"
#include <math.h>

// Camera
// Update 04.09.2021
//
static vec3 YAW_AXIS = { 0, 1, 0 };
static vec3 PITCH_AXIS = { 1, 0, 0 };
static vec3 ROLL_AXIS = { 0, 0, 1 };
	   
static vec3 FORWARD = { 0, 0, -1 };
static vec3 UP = { 0, 1, 0 };

class CCamera {
public:
	CCamera() {};
	CCamera(float fov, vec3 vec) {
		m_fSensitivity = 0.1f;
		m_FOV = fov;
		m_vecOrigin.x = vec.x;
		m_vecOrigin.y = vec.y;
		m_vecOrigin.z = vec.z;
		m_Yaw = -25;
		m_Pitch = 25;
		//m_Roll = 0;
		m_Quat = quat(0.f, 0.f, 0.f, 0.f);
		updateEulerOrientation();
		m_vecDirection = getForward();
		m_vecUp = getUp();
	};
	~CCamera() {};


	void UpdateMouseInput() {
		POINT pp;
		GetCursorPos(&pp);
		int centerX = m_iScreenWidth >> 1;
		int centerY = m_iScreenHeight >> 1;
		if (pp.x == centerX && pp.y == centerY)
			return;

		m_Yaw -= (centerX - pp.x) * m_fSensitivity;
		m_Pitch -= (centerY - pp.y) * m_fSensitivity;
		SetCursorPos(centerX, centerY);
	}

	void UpdateCameraState() {
		UpdateMouseInput();
		updateEulerOrientation();
		m_vecDirection = getForward();
		m_vecUp = getUp();
		m_vecMovement = normalize(m_vecMovement);
		m_vecOrigin = add(m_vecOrigin, m_vecMovement.z * m_vecDirection); // updateForwardMovement
		m_vecOrigin = add(m_vecOrigin, m_vecMovement.y * m_vecUp); // updateUpMovement
		vec3 sideVector = normalize(cross(m_vecUp, m_vecDirection)); // updateSideMovement
		m_vecOrigin = add(m_vecOrigin, m_vecMovement.x * sideVector);
	}

	void updateEulerOrientation() {
		quat qYaw = quat_from_angle_axis(m_Yaw, YAW_AXIS);
		quat qPitch = quat_from_angle_axis(m_Pitch, PITCH_AXIS);
		quat qRoll = quat_from_angle_axis(m_Roll, ROLL_AXIS);
		m_Quat = mul(qYaw, mul(qPitch, qRoll));
	}

	inline vec3 getForward() { return rotate_with_quat(FORWARD, m_Quat); }
	inline vec3 getUp() { return rotate_with_quat(UP, m_Quat); }

	/**
	* Viewport updating
	*/
	inline void UpdateViewport(int screenWidth, int screenHeight) {
		m_iScreenWidth = screenWidth;
		m_iScreenHeight = screenHeight;
	}

	/**
	* Move
	*/
	inline void SetMovement(vec3 vecMove) { m_vecMovement = vecMove; }
	inline vec3 GetMovement() { return m_vecMovement; }
	inline void MoveForward() { m_vecMovement.z = 1.f; }
	inline void MoveBack() { m_vecMovement.z = -1.f; }
	inline void MoveLeft() { m_vecMovement.x = 1.f; }
	inline void MoveRight() { m_vecMovement.x = -1.f; }

	/**
	* Rotation
	*/
	inline void SetPitch(float pitch) { m_Pitch = pitch; }
	inline float GetPitch() { return m_Pitch; }
	inline void SetYaw(float yaw) { m_Yaw = yaw; }
	inline float GetYaw() { return m_Yaw; }
	inline void SetRoll(float roll) { m_Roll = roll; }
	inline float GetRoll() { return m_Roll; }
	inline void SetRotation(vec3 rot) {
		m_Pitch = rot.x;
		m_Yaw = rot.y;
		m_Roll = rot.z;
	}
	inline vec3 GetRotation() { return vec3(m_Pitch, m_Yaw, m_Roll); }
	
	/**
	* Activate/Disactivate camera
	*/
	inline void SetActive(bool active) { m_bActive = active; }
	inline bool IsActive() { return m_bActive; }

	vec3  m_vecOrigin;
	vec3  m_vecDirection;
	vec3  m_vecUp;
	vec3  m_vecMovement;
	float m_fSensitivity;
	float m_FOV;

	float m_Pitch;
	float m_Yaw;
	float m_Roll;
	quat  m_Quat;

	int  m_iScreenWidth;
	int  m_iScreenHeight;
	bool m_bActive;
};

#endif