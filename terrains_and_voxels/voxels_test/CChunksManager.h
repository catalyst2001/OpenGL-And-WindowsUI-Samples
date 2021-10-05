#pragma once
#include "voxel.h"

//TODO: реализовать расчет смещения в трехмерном массиве чанков по формуле:
//xpos = min.x / chunk_width
//ypos = min.y / chunk_height
//zpos = min.z / chunk_width

class CChunksManager
{
public:
	CChunksManager();
	~CChunksManager();
};

