#pragma once
#include "../../utilites/glmath.h"
#include "voxel.h"

enum worldgen_e {
	WG_OK = 0,						 //ошибок не произошло
	WG_FAILED_ALLOCATE_MEMORY_IN_CHUNK,  //не удалось выделить память для вокселей одного из чанков
	WG_FAILED_ALLOCATE_MEMORY_FOR_CHUNKS //не удалось выделить память для всех чанков (критическое состояние, генератор мира не может работать)
};

class CWorldGenerator
{
public:
	CWorldGenerator() {}
	~CWorldGenerator() {}

	int Init(int chunk_width, int chunk_height, int num_of_chunks);


};

