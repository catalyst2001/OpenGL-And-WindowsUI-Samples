#pragma once
#include <stdint.h>
#include <stdio.h>
#include "voxel.h"
#include "CChunksManager.h"

#pragma pack(1)
struct region_header {
	uint32_t hashsum;
	uint32_t num_of_chunks_per_width;
	uint32_t num_of_chunks_per_height;
	uint32_t chunk_width;
	uint32_t chunk_height;
};
#pragma pack()

int region_save(CChunksManager *preg, const char *filename);
int region_load(CChunksManager *preg, const char *filename);