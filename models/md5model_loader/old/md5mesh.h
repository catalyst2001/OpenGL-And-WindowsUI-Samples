#pragma once
#include "main.h"
#include "vec.h"
#include "quat.h"

/* Joint */
struct md5_joint_t
{
	char name[64];
	int parent;

	vec3_t pos;
	quat4_t orient;
};

/* Vertex */
struct md5_vertex_t
{
	vec2_t st;

	int start; /* start weight */
	int count; /* weight count */
};

/* Triangle */
struct md5_triangle_t
{
	int index[3];
};

/* Weight */
struct md5_weight_t
{
	int joint;
	float bias;

	vec3_t pos;
};

/* MD5 mesh */
struct md5_mesh_t
{
	struct md5_vertex_t *vertices;
	struct md5_triangle_t *triangles;
	struct md5_weight_t *weights;

	int num_verts;
	int num_tris;
	int num_weights;

	char shader[256];
};

/* MD5 model structure */
struct md5_model_t
{
	struct md5_joint_t *baseSkel;
	struct md5_mesh_t *meshes;

	int num_joints;
	int num_meshes;
};

/**
 * Load an MD5 model from file.
 */
int ReadMD5Model(const char *filename, struct md5_model_t *mdl);

/**
 * Free resources allocated for the model.
 */
void FreeModel(struct md5_model_t *mdl);

/**
 * Prepare a mesh for drawing.  Compute mesh's final vertex positions
 * given a skeleton.  Put the vertices in vertex arrays.
 */
void PrepareMesh(const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton);

void AllocVertexArrays();
void FreeVertexArrays();

//DBG
vec3_t* GetVertsArray();
GLuint* GetVertsIndices();
int GetMaxVerts();
int GetMaxTris();