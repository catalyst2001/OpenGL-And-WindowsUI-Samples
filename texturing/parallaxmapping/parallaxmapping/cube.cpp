#include "cube.h"

vec2 uvs[] = {
	// Front Face
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),

	// Back Face
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),

	// Top Face
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),

	// Bottom Face
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),

	// Right face
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
	vec2(0.0f, 0.0f),

	// Left Face
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
};

vec3 normals[] = {
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f,-1.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f,-1.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(-1.0f, 0.0f, 0.0f)
};

vec3 vertices[] = {
	// Front Face
	vec3(-1.0f, -1.0f,  1.0f),
	vec3(1.0f, -1.0f,  1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(-1.0f,  1.0f,  1.0f),

	// Back Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(-1.0f,  1.0f, -1.0f),
	vec3(1.0f,  1.0f, -1.0f),
	vec3(1.0f, -1.0f, -1.0f),

	// Top Face
	vec3(-1.0f,  1.0f, -1.0f),
	vec3(-1.0f,  1.0f,  1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(1.0f,  1.0f, -1.0f),

	// Bottom Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(1.0f, -1.0f, -1.0f),
	vec3(1.0f, -1.0f,  1.0f),
	vec3(-1.0f, -1.0f,  1.0f),

	// Right face
	vec3(1.0f, -1.0f, -1.0f),
	vec3(1.0f,  1.0f, -1.0f),
	vec3(1.0f,  1.0f,  1.0f),
	vec3(1.0f, -1.0f,  1.0f),

	// Left Face
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(-1.0f, -1.0f,  1.0f),
	vec3(-1.0f,  1.0f,  1.0f),
	vec3(-1.0f,  1.0f, -1.0f)
};
