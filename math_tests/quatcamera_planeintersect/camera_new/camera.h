#pragma once
#ifndef __CAMERA
#define __CAMERA
#include "quat.h"
#include "../../utilites/glmath.h"
#include <Windows.h>

static vec3 YAW_AXIS = { 0, 1, 0 };
static vec3 PITCH_AXIS = { 1, 0, 0 };
static vec3 ROLL_AXIS = { 0, 0, 1 };
	   
static vec3 FORWARD = { 0, 0, -1 };
static vec3 UP = { 0, 1, 0 };
	   
static vec3 movement;

static void updateEulerOrientation(EULER &euler) {
	Quat qYaw = QuaternionFromAngleAxis(euler.yaw, YAW_AXIS);
	Quat qPitch = QuaternionFromAngleAxis(euler.pitch, PITCH_AXIS);
	Quat qRoll = QuaternionFromAngleAxis(euler.roll, ROLL_AXIS);


	euler.orientation = Multiply(qYaw, Multiply(qPitch, qRoll));
}

static vec3 getForward(EULER euler) {
	return RotateWithQuaternion(FORWARD, euler.orientation);
}

static vec3 getUp(EULER euler) {
	return RotateWithQuaternion(UP, euler.orientation);
}


static vec3 Add(vec3 a, vec3 b) //Ya implementado como ejemplo.
{
	vec3 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

class camera {
public:
	camera() {};
	camera(double fov, vec3 vec) {
		aperture = fov;

		position.x = vec.x;
		position.y = vec.y;
		position.z = vec.z;

		euler = { -25, 25, 0, {0, {0, 0, 0}} };
		updateEulerOrientation(euler);
		direction = getForward(euler);
		up = getUp(euler);
	};
	~camera() {};

	vec3 position;
	vec3 direction;
	vec3 up;

	double aperture;    // in degrees
	int screenwidth, screenheight;
	EULER euler;

	void MouseMove(int x, int y) {
		POINT pp;
		GetCursorPos(&pp);
		int deltaX = ((screenwidth >> 1) - pp.x);
		int deltaY = ((screenheight >> 1) - pp.y);
		euler.yaw += deltaX * -0.1f;
		euler.pitch += deltaY * -0.1f;
		SetCursorPos(screenwidth >> 1, screenheight >> 1);
	}

	void UpdateCameraState() {
		updateEulerOrientation(euler);
		direction = getForward(euler);
		up = getUp(euler);

		movement = normalize(movement);

		// updateForwardMovement
		position = Add(position, MultiplyWithScalar(movement.z, direction));
		// updateUpMovement
		position = Add(position, MultiplyWithScalar(movement.y, up));
		// updateSideMovement
		vec3 sideVector = normalize(cross(up, direction));
		position = Add(position, MultiplyWithScalar(movement.x, sideVector));

		movement = { 0, 0, 0 };
	}

	void updateEulerOrientation(EULER &euler) {
		Quat qYaw = QuaternionFromAngleAxis(euler.yaw, YAW_AXIS);
		Quat qPitch = QuaternionFromAngleAxis(euler.pitch, PITCH_AXIS);
		Quat qRoll = QuaternionFromAngleAxis(euler.roll, ROLL_AXIS);

		euler.orientation = Multiply(qYaw, Multiply(qPitch, qRoll));
	}

};

#endif