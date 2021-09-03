#pragma once
#include <malloc.h>

struct vec3_compressed {
	char x, y, z;
};

struct voxel {
	char flags;
	vec3_compressed pos;
	vec3_compressed norm;
};

struct chunk {
	struct {
		float x, y, z;
	} world_pos;
	voxel ***voxels;
};

enum chunk_errors_e {
	CHUNK_OK,
	CHUNK_ALLOC_FAILURE,
	CHUNK_ERROR_BAD_PARAMETER
};

/**
* Voxels functions
*/
inline int voxel_compute_memory(int width, int height) { return (width * height * width) * sizeof(voxel); }

/**
* Chunks functions
*/
int chunk_alloc_voxels(chunk *pchunk, int width, int height);