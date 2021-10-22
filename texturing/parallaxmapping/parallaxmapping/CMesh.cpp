#include "CMesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
}

void CMesh::CreateMesh(vec3 position, vec3 rotation, Vertex *p_vertices, uint32_t num_of_verts, uint32_t *p_indices, uint32_t num_of_indices)
{
	assert(p_vertices);
	assert(p_indices);
	m_nVertices = num_of_verts;
	m_nIndices = num_of_indices;
	m_matRotation = Mat3x3_identity;

	//vertex arrays object
	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);

	//vertex buffers
	glGenBuffers(2, m_nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO[vbo_vertices]);
	glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(Vertex), p_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nVBO[vbo_indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(uint32_t), p_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));
	glEnableVertexAttribArray(VERTEX_POSITION);

	glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, normal));
	glEnableVertexAttribArray(VERTEX_NORMAL);

	glVertexAttribPointer(VERTEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, uv));
	glEnableVertexAttribArray(VERTEX_TEXCOORD);
}

void CMesh::DeleteMesh()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, m_nVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_nVAO);
}
