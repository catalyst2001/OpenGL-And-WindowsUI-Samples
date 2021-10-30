#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "CChunksManager.h"
#include "voxel.h"

#include "../../utilites/glmath.h"

class CGenerator
{
public:


	CGenerator();
	~CGenerator();

	void StartGeneration2(int chunkwidth, int chunkheight, int chunksx, int chunksz);

	int m_nChunkWidth;
	int m_nChunkHeight;
};


