#pragma once
#include <malloc.h>
#include "../../utilites/glmath.h"

typedef char chunk_cell_t;

struct vec3_compressed {
	char x, y, z;
};

struct voxel {
	char flags;
	//vec3_compressed pos;
	//vec3_compressed norm;
};

struct chunk {
	vec3 pos;
	chunk_cell_t chunk_width;
	chunk_cell_t chunk_height;
	voxel *voxels;
};

enum chunk_errors_e {
	CHUNK_OK,
	CHUNK_ALLOC_FAILURE,
	CHUNK_ERROR_BAD_PARAMETER
};

/**
* Chunks functions
*/
int chunk_alloc_voxels(chunk *pchunk, int width, int height);
voxel *chunk_find_voxel(chunk *pchunk, float x, float y, float z);

#define CHUNK_SIZE(width, height) ((width * height * width) * sizeof(voxel))

//TODO: хуй знает когда допилим
#define COORD2OFFSET(pchunk, x, y, z) ((x * (pchunk)->chunk_width + z) * (pchunk)->chunk_height + y) //(x * width + y) * width + z

#define OFFSET2COORD(x, y, z, pchunk, offset) \
    x = offset / ((pchunk)->chunk_width * (pchunk)->chunk_width)/* % ARRAY_SIZE*/;\
    y = (offset / (pchunk)->chunk_width) % (pchunk)->chunk_width;\
    z = offset % (pchunk)->chunk_width;

#define CHUNK_VOXEL_LAYER(pvox, pchunk) chunk_voxel_layer(pvox, pchunk)
#define CHUNK_LAYER_AREA(pchunk) ((pchunk)->chunk_width * (pchunk)->chunk_width)
#define CHUNK_PREV_VOXEL(pvox) ((voxel *)pvox - 1)
#define CHUNK_NEXT_VOXEL(pvox) ((voxel *)pvox + 1)
#define CHUNK_LEFT_VOXEL(pvox, pchunk) ((voxel *)pvox - (pchunk)->chunk_width)
#define CHUNK_RIGHT_VOXEL(pvox, pchunk) ((voxel *)pvox + (pchunk)->chunk_width)
#define CHUNK_BOTTOM_VOXEL(pvox, layerwidth) (pvox - layerwidth)
#define CHUNK_TOP_VOXEL(pvox, layerwidth) (pvox + layerwidth)


int chunk_voxel_layer(voxel *pcurrentvox, chunk *pchunk);

/**
* $brief	Returns the back voxel
* $param	Chunk structure address
* $param	Current voxel address
* $return	If current voxel is end voxel or voxel in edge, return NULL, otherwise back voxel address
*/
voxel *chunk_back_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief	Returns the front voxel
* $param	Chunk structure address
* $param	Current voxel address
* $return	If current voxel is end voxel or voxel in edge, return NULL, otherwise back voxel address
*/
voxel *chunk_front_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief	Returns the left voxel
* $param	Chunk structure address
* $param	Current voxel address
* $return	If current voxel is end voxel or voxel in edge, return NULL, otherwise back voxel address
*/
voxel *chunk_left_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief
* $param
* $param
* $return
*/
voxel *chunk_right_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief
* $param
* $param
* $return
*/
voxel *chunk_bottom_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief
* $param
* $param
* $return
*/
voxel *chunk_top_voxel(chunk *pchunk, voxel *pvox);

/**
* $brief
* $param
* $param
* $return
*/
bool chunk_voxel_on_corner(chunk *pchunk, voxel *pvox);

/**
* $brief
* $param
* $param
* $return
*/
bool chunk_voxel_on_edge(chunk *pchunk, voxel *pvox);

/**
* Voxels functions
*/
// Voxel types flags
#define VOXEL_ERROR	0
#define VOXEL_FLAG_AIR		(1 << 0)
#define VOXEL_FLAG_SOLID	(1 << 1)
#define VOXEL_FLAG_LIQUID	(1 << 2)

#define voxel_flags(pvox) ((pvox)->flags)
#define voxel_is_air(pvox) ((pvox)->flags & VOXEL_FLAG_AIR)
#define voxel_is_solid(pvox) ((pvox)->flags & VOXEL_FLAG_SOLID)
#define voxel_is_liquid(pvox) ((pvox)->flags & VOXEL_FLAG_LIQUID)
#define voxel_in_corner(x, z, pchunk) (!(x % (pchunk)->chunk_width) || x % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1) && !(z % (pchunk)->chunk_width) || z % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1))
#define voxel_in_edge(x, z, pchunk) (!(x % (pchunk)->chunk_width) || !(z % (pchunk)->chunk_width) || x % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1) || z % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1)) 
bool voxel_in_air(voxel *pvox, chunk *pchunk);