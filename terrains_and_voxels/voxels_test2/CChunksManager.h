#pragma once
#include "voxel.h"

//OK: реализовать расчет смещения в трехмерном массиве чанков по формуле (РЕАЛИЗОВАНО)
//xpos = min.x / chunk_width
//ypos = min.y / chunk_height
//zpos = min.z / chunk_width

#define GLOBALCOORD2LOCAL(x, w) (x / w)

//Region Controller
class CChunksManager
{
public:
	CChunksManager();
	~CChunksManager();

	int Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height);

	CChunk *GetRegionChunk(int gx, int gy, int gz);
	CVoxel *GetRegionVoxel(int gx, int gy, int gz);
	bool GetVoxelByRay(CChunk **ppchunk, CVoxel **ppvox, CRay &ray, vec3 *pos, float distance, int checkflag);
	bool RemoveSolidVoxel(CRay &ray, float distance, int newflag = VOXEL_FLAG_AIR);
	bool PlaceSolidVoxel(CRay &ray, float distance, int newflag = VOXEL_FLAG_SOLID);

	int m_nChunksPerWidth;
	int m_nChunksPerHeight;
	CChunk *m_pChunks;
	int m_nChunkWidth;
	int m_nChunkHeight;
	int m_nNumberOfChunks;
private:
};

