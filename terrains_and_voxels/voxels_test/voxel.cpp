#include "voxel.h"


// offsets from the minimal corner to other corners
static vec3 cornerOffsets[8] =
{
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f }
};

// offsets from the minimal corner to 2 ends of the edges
static vec3 edgeVertexOffsets[12][2] =
{
	{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
	{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
	{ { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } },
	{ { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
	{ { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
	{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } },
	{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f } },
	{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
	{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f } }
};

// list of triangles/vertices for every possible case
// up to 15 vertices per case and -1 indicates end of sequence
static int triangleTable[][16] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

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

bool GetChunkMinByRay(vec3 &chunkmin, CRay &ray, float distance)
{
	float t = 0.f;
	while (t < distance) {
		chunkmin = ray.m_origin + normalize(ray.m_direction) * t;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------------------
int CChunk::GetChunkWidth()
{
	return m_nWidth;
}

int CChunk::GetChunkHeight()
{
	return m_nHeight;
}

CVoxel *CChunk::GetVoxels()
{
	return m_pVoxels;
}

int CChunk::GetNumberVoxels()
{
	return m_nWidth * m_nHeight * m_nWidth;
}

CVoxel *CChunk::VoxelAt(int x, int y, int z)
{
	//?????????, ?? ??????? ?? ?? ?? ??????? ????? ?? ???????????,
	if (x < 0 || y < 0 || z < 0)
		return NULL;

	//????? ?? ?????? ?? ??????? ?????? ? ?????????
	//???????????? ?????????? ????? ???? ?? 0 ?? m_nWidth-1. ?????????? ? ? ???????
	if (x > m_nWidth || y > m_nHeight || z > m_nWidth)
		return NULL;

	return &m_pVoxels[COORD2OFFSET2(m_nWidth, m_nHeight, x, y, z)];
}

vec3 TriangleNearPoint2(vec3 &p0, vec3 &p1, vec3 &p2, vec3 &point)
{
	vec3 verts[] = { p0 , p1 , p2 };
	float dist[3];
	dist[0] = distance(p0, point);
	dist[1] = distance(p1, point);
	dist[2] = distance(p2, point);
	if (dist[0] < dist[1]) {
		if (dist[0] < dist[2]) {
			return verts[0];
		}
		else {
			return verts[2];
		}
	}
	else if (dist[1] < dist[2]) {
		return verts[1];
	}
	else {
		return verts[2];
	}
}

bool CChunk::DestroyVoxelByRay_Legacy(CRay &ray, int voxflags)
{
	int num_of_verts = m_vertices.size();
	if (!num_of_verts)
		return false;

	vec3 intersection;
	CVoxel *p_vox = NULL;
	triangle_t *p_triangle = (triangle_t *)m_vertices.data();
	int num_of_triangles = num_of_verts / 3;
	for (int i = 0; i < num_of_triangles; i++) {
		if (ray.TriangleIntersection(p_triangle[i].p[0], p_triangle[i].p[1], p_triangle[i].p[2], intersection)) {
			intersection = TriangleNearPoint2(p_triangle[i].p[0], p_triangle[i].p[1], p_triangle[i].p[2], intersection);
			round_vector(intersection, intersection, 1.f);
			if ((p_vox = VoxelAt((int)intersection.x, (int)intersection.y, (int)intersection.z))) {
				p_vox->SetFlag(voxflags);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool CChunk::DestroyVoxel(CRay &ray, float distance, int voxflags)
{
	float t = 0.f;
	while (t < distance) {
		vec3 curr = ray.m_origin + normalize(ray.m_direction) * t;
		round_vector(curr, curr, 1.0);

		CVoxel *pvox = VoxelAt(curr.x, curr.y, curr.z);
		if (!pvox) {
			t++;
			continue;
		}

		if (pvox->IsSolid()) {
			pvox->SetFlag(voxflags);
			return true;
		}
		t++;
	}
	return false;
}

bool CChunk::FindVoxelByRay(CVoxel **ppvoxel, vec3 *ppos, CRay &ray, int checkflag, float distance, float stepoccuracy)
{
	float t = 0.f;
	while (t < distance) {
		vec3 pos = ray.m_origin + normalize(ray.m_direction) * t;
		round_vector(pos, pos, 1.0);
		*ppvoxel = VoxelAt(pos.x, pos.y, pos.z);
		if (!(*ppvoxel)) {
			t++;
			continue;
		}

		if ((*ppvoxel)->GetFlags() & checkflag) {
			if (ppos) *ppos = pos;
			return true;
		}
		
		t += stepoccuracy;
	}
	return false;
}

bool CChunk::ChangeVoxelFlagsByRay(CRay &ray, float distance, int checkflag, int newflag)
{
	CVoxel *pvox = NULL;
	if (!FindVoxelByRay(&pvox, NULL, ray, checkflag, distance, 1.0f))
		return false;

	pvox->SetFlag(newflag);
	return true;
}

bool CChunk::PlaceVoxel(CRay &ray, float distance, int voxflags)
{
	printf("~~~ CChunk::PlaceVoxel called!\n");
	vec3 voxpos;
	CVoxel *pvox = NULL;
	if (!FindVoxelByRay(&pvox, &voxpos, ray, VOXEL_FLAG_AIR, distance, 1.0f)) {
		printf("~~~ Voxel not found!\n");
		return false;
	}

	CVoxel *pbotvox = VoxelAt(voxpos.x, voxpos.y - 1, voxpos.z);
	if (!pbotvox) {
		printf("~~~ Voxel not found!\n");
		return false;
	}

	if (pbotvox->GetFlags() & VOXEL_FLAG_SOLID) {
		if (VoxelInAir(voxpos.x, voxpos.y, voxpos.z)) {
			printf("~~~ Voxel in air\n");
			return false;
		}
		pvox->SetFlag(voxflags);
		printf("~~~ OK! VOX IS SET\n");
		return true;
	}
	return false;
}

bool CChunk::VoxelInAir(int locx, int locy, int locz)
{
	CVoxel *pvox = NULL;

	//????????? ??? ??????? ??????? ????? ?????????????? ?? ?????????? ? ???????
	//???? ??? ??????? ??????, ?? ??? ?????? ? ???????? ??????? ?????? ??????? :)
	if (!(pvox = VoxelAt(locx, locy, locz)) || (pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ?????, ???? ?? ?? ?????? ? ???????? ?? ?? ????????
	if (!(pvox = VoxelAt(locx - 1, locy, locz)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ??????
	if (!(pvox = VoxelAt(locx + 1, locy, locz)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ???????
	if (!(pvox = VoxelAt(locx, locy, locz + 1)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ?????
	if (!(pvox = VoxelAt(locx, locy, locz - 1)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ??????
	if (!(pvox = VoxelAt(locx, locy + 1, locz)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	//????????? ??????? ?????
	if (!(pvox = VoxelAt(locx, locy - 1, locz)) || !(pvox->GetFlags() & VOXEL_FLAG_AIR))
		return false;

	return false;
}

bool CChunk::PointInChunk(int x, int y, int z)
{
	return m_ChunkPos.x < x && x < m_vecMax.x && m_ChunkPos.y < y && y < m_vecMax.y && m_ChunkPos.z < z && z < m_vecMax.x;
}

bool CChunk::HasIdle()
{
	return m_bIdle;
}

void CChunk::MarkIdle(bool idle)
{
	m_bIdle = idle;
}

void CChunk::ClearMesh()
{
	m_vertices.clear();
	m_uvs.clear();
	m_indices.clear();
}

void CChunk::MarchCube(vec3 min_corner_pos)
{
	// construct case index from 8 corner samples
	vec2 uvtypecord[2][3] = { 
		{ vec2(0.f, 1.f) , vec2(1.f, 1.f) , vec2(0.f, 0.f) },
		{ vec2(0.f, 0.f) , vec2(1.f, 1.f) , vec2(1.f, 0.f) }
	};

	int caseIndex = 0;
	for (int i = 0; i < 8; i++) {
		
		//?????????????? 04.10.2021
		//vec3 res = min_corner_pos + cornerOffsets[i];

		//????????? 04.10.2021
		//????????? ????????? ?????????? ?????? ????? ?? ?????????? ??????????
		//Lp - ????????? ?????
		//Gp - ?????????? ?????
		//Min - ?????? ????????
		//???????: Lp = Gp - Min
		vec3 localpoint = min_corner_pos - vec3(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z);
		vec3 res = localpoint + cornerOffsets[i];

		//? ?????????? ???? ????, ???? ???????? ??????? ? ?????????, ???? ?? ?? ? ???????? ?? ?? ???????
		//???? ??????? ???? ? ?? ???????, ???? ???? ????? ????????? ? ???????????? ????
		//????? ?????????, ?? ????? ?? ?? ?? ?????? ????? ?? ??????
		//??????? ?? x ? z ?? ?????????, ?????? ??? ??? ?????? ???????? ????? ?? ?????? ?????
		CVoxel *pvox = VoxelAt(res.x, res.y, res.z);
		if (pvox && pvox->IsSolid())
			caseIndex |= 1 << i;
	}

	// early out if entirely inside or outside the volume
	if (caseIndex == 0 || caseIndex == 255)
		return;

	int caseVert = 0;
	int uv_hueta = 0;
	bool uv_type = 1;
	for (int i = 0; i < 5; i++) {
		for (int tri = 0; tri < 3; tri++) {
			// get edge index
			int edgeCase = triangleTable[caseIndex][caseVert];
			if (edgeCase == -1)
				return;

			// Commented 04.10.2021 --->
			//vec3 chunkPosition = vec3(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z);
			//vec3 vert1 = /*chunkPosition + */min_corner_pos + edgeVertexOffsets[edgeCase][0]; // beginning of the edge
			//vec3 vert2 = /*chunkPosition + */min_corner_pos + edgeVertexOffsets[edgeCase][1]; // end of the edge		
			// <---

			//????????? 04.10.2021
			//????? ??? ???? ?????????? ? ??????? ???????? ???? ?????? ???????? ?????
			vec3 chunkPosition = vec3(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z);
			vec3 vert1 = min_corner_pos + edgeVertexOffsets[edgeCase][0]; // beginning of the edge
			vec3 vert2 = min_corner_pos + edgeVertexOffsets[edgeCase][1]; // end of the edge

#ifndef USE_INTERP
			vec3 vertPos = (vert1 + vert2) / 2.0f; // non interpolated version - in the middle of the edge
			m_vertices.push_back(vertPos);
			if (!(uv_hueta % 3)) {
				uv_type = !uv_type;
			}
			
			m_uvs.push_back(uvtypecord[uv_type][uv_hueta%3]);
			uv_hueta++;
#else
				// interpolate along the edge
			float s1 = /*SampleValue(*/length(min_corner_pos + edgeVertexOffsets[edgeCase][0]);
			float s2 = /*SampleValue(*/length(min_corner_pos + edgeVertexOffsets[edgeCase][1]);
			float dif = s1 - s2;
			if (dif == 0.0f)
				dif = 0.5f;
			else
				dif = s1 / dif;

			// Lerp
			vec3 vertPosInterpolated = vert1 + ((vert2 - vert1) * dif);
			m_verts.push_back(vertPosInterpolated);
#endif
			m_indices.push_back(m_vertices.size() - 1);
			caseVert++;
		}
	}
}

void CChunk::GNormal()
{
	vec3 normal;
	triangle_t *p_triangle = (triangle_t *)m_vertices.data();
	for (int i = 0; i < m_vertices.size() / 3; i++) {
		ComputeNormalWithTriangle(normal, p_triangle[i].p[0], p_triangle[i].p[1], p_triangle[i].p[2]);
		m_normals.push_back(normal);
	}
}

void CChunk::DrawMesh()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1].texID);
	glTexCoordPointer(2, GL_FLOAT, 0, m_uvs.data());
	glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
#ifdef DEBUG_DRAW
	glPushAttrib(GL_CURRENT_BIT);
	glColor3ub(0, 100, 0);
	//draw debug chunk bounds
	if (m_nDDBounds) {
		DrawBBox(m_ChunkPos, m_vecMax);
	}

	//draw debug voxels
	//if (m_nDDVoxels) {
	//	glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());
	//	glDrawArrays(GL_POINTS, 0, (m_nWidth * m_nHeight * m_nWidth));
	//}

	//draw debug voxels cubes
	if (m_nDDCubes) {
		//TODO: m_nDDCubes not implemented
	}
	glPopAttrib();
#endif
}

void CChunk::BuildMesh()
{
	//old (changed 04.10.2021)
	/*
	for (int y = 0; y < m_nHeight; y++)
		for (int z = 0; z < m_nWidth; z++)
			for (int x = 0; x < m_nWidth; x++)
				MarchCube(vec3(x, y, z));
	*/
	for (int y = m_ChunkPos.y; y < m_vecMax.y; y++) {
		for (int z = m_ChunkPos.z; z < m_vecMax.z; z++) {
			for (int x = m_ChunkPos.x; x < m_vecMax.x; x++) {
				MarchCube(vec3(x, y, z));
			}
		}
	}
}

void CChunk::RebuildMesh()
{
	GNormal();
	ClearMesh();
	BuildMesh();
}

vec3 *CChunk::GetVertices()
{
	return m_vertices.data();
}

int *CChunk::GetIndices()
{
	return m_indices.data();
}

int CChunk::GetNumOfVertices()
{
	return m_vertices.size();
}

int CChunk::GetNumOfIndices()
{
	return m_indices.size();
}

#ifdef DEBUG_DRAW
void CChunk::DebugDraw_ChunkBounds(bool b)
{
	m_nDDBounds = b;
}

void CChunk::DebugDraw_ChunkVoxels(bool b)
{
	m_nDDVoxels = b;
}

void CChunk::DebugDraw_ChunkCubes(bool b)
{
	m_nDDCubes = b;
}

void CChunk::DebugDraw_LastSelectTriangle(bool b)
{
	m_bDDLastSelectTri = b;
}

void CChunk::DebugDraw_SetLastSelectTriangle(triangle_t &tri)
{
	m_LastSelectTriangle = tri;
}

#endif

CChunk::CChunk(vec3int pos, int width, int height)
{
	Init(pos, width, height);
}

int CChunk::Init(vec3int pos, int width, int height, int flags)
{
	m_nWidth = width;
	m_nHeight = height;
	m_ChunkPos = pos;
	m_vecMax.x = pos.x + m_nWidth;
	m_vecMax.y = pos.y + m_nHeight;
	m_vecMax.z = pos.z + m_nWidth;

	//????????? ? ?????? ? ?????? ?? 1, ????? ????????? ????????? ??? ????? ?????????
	return AllocVoxels(m_nWidth + 1, m_nHeight + 1, flags); //TODO: ||
}

int CChunk::AllocVoxels(int width, int height, int flags)
{
	int size_in_bytes = COMPUTE_CHUNK_SIZE(width, height);
	m_pVoxels = (CVoxel *)malloc(size_in_bytes);
	if (!m_pVoxels)
		return 0;
	
	//if (flags)
	memset(m_pVoxels, flags, size_in_bytes);
	return 1;
}

int CChunk::FreeVoxels()
{
	if (m_pVoxels) {
		free(m_pVoxels);
		return 1;
	}
	return 0;
}

void CChunk::SetChunkPosition(vec3int pos)
{
	m_ChunkPos = pos;
}

void CChunk::SetChunkPosition(int x, int y, int z)
{
	m_ChunkPos.x = x;
	m_ChunkPos.y = y;
	m_ChunkPos.z = z;
}

void CChunk::SetChunkSize(int width, int height)
{
	m_nWidth = width;
	m_nHeight = height;
}

inline int CChunk::GetLayerArea()
{
	return m_nWidth * m_nHeight;
}

//-------------------------------------------------------------------------------------------------------------------------------
inline voxel_cell_t CVoxel::GetFlags()
{
	return m_Flags;
}

void CVoxel::SetFlag(voxel_cell_t flag)
{
	m_Flags = flag;
}

inline bool CVoxel::IsEmpty()
{
	return (m_Flags & VOXEL_FLAG_AIR);
}

inline bool CVoxel::IsSolid()
{
	return (m_Flags & VOXEL_FLAG_SOLID);
}

inline bool CVoxel::IsLiquid()
{
	return (m_Flags & VOXEL_FLAG_LIQUID);
}

inline bool CVoxel::InChunkCorner(CChunk *pchunk, int x, int z)
{
	return (!(x % pchunk->GetChunkWidth()) || x % pchunk->GetChunkWidth() == (pchunk->GetChunkWidth() - 1) && !(z % pchunk->GetChunkWidth()) || z % pchunk->GetChunkWidth() == (pchunk->GetChunkWidth() - 1));
}

inline bool CVoxel::InChunkEdge(CChunk * pchunk, int x, int z)
{
	return (!(x % pchunk->GetChunkWidth() || !(z % pchunk->GetChunkWidth() || x % pchunk->GetChunkWidth() == (pchunk->GetChunkWidth() - 1) || z % pchunk->GetChunkWidth() == (pchunk->GetChunkWidth() - 1))));
}
