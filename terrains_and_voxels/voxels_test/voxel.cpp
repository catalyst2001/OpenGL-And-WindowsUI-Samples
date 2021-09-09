#include "voxel.h"

int chunk_alloc_voxels(chunk *pchunk, int width, int height)
{
	//limit coordinates in chunk to 250, since the maximum number can be 255
	//compression is used and float is limited to 1 byte
	//if (width < 1 || width > 250 || height < 1 || height > 250)
	//	return CHUNK_ERROR_BAD_PARAMETER;

	//compute of the required memory for one chunk using the formula: (chunk_width * chunk_height * chunk_width) * voxel_struct_size
	pchunk->voxels = (voxel *)malloc(CHUNK_SIZE(width, height));
	if (!pchunk->voxels)
		return CHUNK_ALLOC_FAILURE;

	pchunk->chunk_width = width;
	pchunk->chunk_height = height;
	return CHUNK_OK;
}

voxel *chunk_find_voxel(chunk *pchunk, float x, float y, float z)
{
	//check chunk coordinates bounds
	if (x < pchunk->pos.x || y < pchunk->pos.y || z < pchunk->pos.z)
		return NULL;

	if(x > pchunk->pos.x + (float)pchunk->chunk_width || y > pchunk->pos.y + (float)pchunk->chunk_height || z > pchunk->pos.z + (float)pchunk->chunk_width)
		return NULL;

	int ix = lround(x);
	int iy = lround(y);
	int iz = lround(z);
	return &pchunk->voxels[COORD2OFFSET(pchunk, ix, iy, iz)];
}

int chunk_voxel_layer(voxel *pcurrentvox, chunk *pchunk)
{
	
	return 0;
}

bool voxel_in_air(voxel *pvox, chunk *pchunk)
{
	//main voxel is air
	if (voxel_is_air(pvox))
		return false;

	voxel *p_computed_voxel = NULL;
	if ((p_computed_voxel = CHUNK_PREV_VOXEL(pvox)) < pchunk->voxels) {
		//is first voxel
	}

	//return voxel_is_air() && voxel_is_air(CHUNK_NEXT_VOXEL(pvox)) && voxel_is_air(CHUNK_LEFT_VOXEL(pvox, pchunk)) && voxel_is_air(CHUNK_RIGHT_VOXEL(pvox, pchunk)) && voxel_is_air(CHUNK_BOTTOM_VOXEL(pvox, pchunk)) && voxel_is_air(CHUNK_TOP_VOXEL(pvox, pchunk));
	return true;
}

