#pragma once
#include "voxel.h"

//OK: реализовать расчет смещения в трехмерном массиве чанков по формуле (РЕАЛИЗОВАНО)
//xpos = min.x / chunk_width
//ypos = min.y / chunk_height
//zpos = min.z / chunk_width

class CChunksManager
{
public:
	CChunksManager();
	~CChunksManager();

	int Init(int chunk_width, int chunk_height, int num_of_chunks);


	int m_nNumOfChunks;
	CChunk *m_pChunks;

	int m_nVisibleChunks;
	float m_nChunkLoadDistance;
};

