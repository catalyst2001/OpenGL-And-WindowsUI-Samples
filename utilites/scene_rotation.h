#pragma once
#include <gl/GL.h>
#include "glmath.h"

class CSceneRotation
{
public:
	CSceneRotation() {
		m_position(0.f, 0.f, 0.f);
		m_rotation(0.f, 0.f, 0.f);
		m_fRotationSpeed = 1.f;
		m_fScaleSpeed = 1.f;
	}

	CSceneRotation(float x, float y, float z, float pitch = 0.f, float yaw = 0.f, float roll = 0.f, float rotspeed = 1.f, float sclspeed = 1.f) {
		m_position(x, y, z);
		m_rotation(pitch, yaw, roll);
		m_fRotationSpeed = rotspeed;
		m_fScaleSpeed = sclspeed;
	}
	~CSceneRotation() {}

	void Look() {
		glTranslatef(m_position.x, m_position.y, m_position.z);
		glRotatef(m_rotation.x, 1.f, 0.f, 0.f);
		glRotatef(m_rotation.y, 0.f, 1.f, 0.f);
		glRotatef(m_rotation.z, 0.f, 0.f, 1.f);
	}

	inline void Scaling(bool b) { m_bScaling = b; }
	inline void Rotating(bool b) { m_bRotation = b; }
	inline void SetRotationSpeed(float speed) { m_fRotationSpeed = speed; }
	inline void SetScaleSpeed(float speed) { m_fScaleSpeed = speed; }

	void MouseMove(int x, int y) {
		newX = x;
		newY = y;
		float deltaX = (newX - prevX) * m_fRotationSpeed;
		float deltaY = (newY - prevY) * m_fRotationSpeed;
		if (m_bRotation) {
			m_rotation.x += deltaY;
			m_rotation.y += deltaX;
		}

		if (m_bScaling)
			m_position.z += (newY - prevY) * m_fScaleSpeed;

		prevX = newX;
		prevY = newY;
	}

	vec3 m_position;
	vec3 m_rotation;
private:
	float m_fRotationSpeed;
	float m_fScaleSpeed;
	bool m_bRotation;
	bool m_bScaling;
	int newX, newY;
	int prevX, prevY;
};