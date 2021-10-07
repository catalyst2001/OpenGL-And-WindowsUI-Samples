#pragma once
#include "voxel.h"

//OK: ����������� ������ �������� � ���������� ������� ������ �� ������� (�����������)
//xpos = min.x / chunk_width
//ypos = min.y / chunk_height
//zpos = min.z / chunk_width

class CChunksManager
{
public:
	CChunksManager();
	~CChunksManager();

	int Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height);

	CChunk *ChunkAt(int gx, int gy, int gz);

	int m_nNumOfChunks;
	CChunk *m_pChunks;

	int m_nVisibleChunks;
	float m_nChunkLoadDistance;
};

