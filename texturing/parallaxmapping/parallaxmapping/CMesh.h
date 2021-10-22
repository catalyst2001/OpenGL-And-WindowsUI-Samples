#pragma once
#include <stdint.h>
#include <assert.h>
#include "glextsloader.h"
#include "../../../utilites/glmath.h"

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 uv;
};

#define VERTEX_POSITION 0
#define VERTEX_NORMAL 1
#define VERTEX_TEXCOORD 2

#define LEGACY_RENDER

//TODO: USE ONE VAO FOR ALL MESHES!
class CMesh
{
public:
	CMesh();
	~CMesh();

	void CreateMesh(vec3 position, vec3 rotation, Vertex *p_vertices, uint32_t num_of_verts, uint32_t *p_indices, uint32_t num_of_indices);
	void DeleteMesh();

	inline void DrawMesh() {
#ifdef LEGACY_RENDER
		glVertexPointer(2, GL_FLOAT, offsetof(Vertex, position), m_pVertices);
		glNormalPointer(GL_FLOAT, offsetof(Vertex, normal), m_pVertices);
		glTexCoordPointer(2, GL_FLOAT, offsetof(Vertex, uv), m_pVertices);
		glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, m_pIndices);
#else
		glBindVertexArray(m_nVAO);
		glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, NULL);
#endif
	}

	//math
	inline void RotateFromEuler(vec3 euler_deg) { EulerAnglesToMatrix3x3(m_matRotation, euler_deg); }

private:
	enum {
		vbo_vertices = 0,
		vbo_indices
	};
	const char *m_pTag;
	vec3 m_vecPosition;
	mat3x3 m_matRotation;
	uint32_t m_nVertices;
	uint32_t m_nIndices;
#ifdef LEGACY_RENDER
	Vertex *m_pVertices;
	uint32_t *m_pIndices;
#else
	uint32_t m_nVBO[2];
	uint32_t m_nVAO;
#endif
};