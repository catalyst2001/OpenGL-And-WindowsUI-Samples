#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <malloc.h>
#include "../../utilites/glmath.h"
#include "../../utilites/utmath.h" //draw bbox

#include <vector> //for renderer vertex buffer, indices buffer

typedef char chunk_cell_t;
typedef short voxel_cell_t;

struct vec3_compressed {
	char x, y, z;
};

struct triangle_t {
	vec3 p[3];
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

//TODO: check this formula!
#define COORD2OFFSET(pchunk, x, y, z) ((x * (pchunk)->chunk_width + z) * (pchunk)->chunk_height + y) //(x * width + y) * width + z

#define OFFSET2COORD(x, y, z, pchunk, offset) \
    x = offset / ((pchunk)->chunk_width * (pchunk)->chunk_width)/* % ARRAY_SIZE*/;\
    y = (offset / (pchunk)->chunk_width) % (pchunk)->chunk_width;\
    z = offset % (pchunk)->chunk_width;

#define CHUNK_LAYER_AREA(pchunk) ((pchunk)->chunk_width * (pchunk)->chunk_width)
#define CHUNK_PREV_VOXEL(pvox) ((voxel *)pvox - 1)
#define CHUNK_NEXT_VOXEL(pvox) ((voxel *)pvox + 1)
#define CHUNK_LEFT_VOXEL(pvox, pchunk) ((voxel *)pvox - (pchunk)->chunk_width)
#define CHUNK_RIGHT_VOXEL(pvox, pchunk) ((voxel *)pvox + (pchunk)->chunk_width)
#define CHUNK_BOTTOM_VOXEL(pvox, layerwidth) (pvox - layerwidth)
#define CHUNK_TOP_VOXEL(pvox, layerwidth) (pvox + layerwidth)

/**
* Voxel flags
*/
#define VOXEL_ERROR	0
#define VOXEL_FLAG_AIR		(1 << 0)
#define VOXEL_FLAG_SOLID	(1 << 1)
#define VOXEL_FLAG_LIQUID	(1 << 2)

/**
* Legacy macroses
*/
#define voxel_flags(pvox) ((pvox)->flags)
#define voxel_is_air(pvox) ((pvox)->flags & VOXEL_FLAG_AIR)
#define voxel_is_solid(pvox) ((pvox)->flags & VOXEL_FLAG_SOLID)
#define voxel_is_liquid(pvox) ((pvox)->flags & VOXEL_FLAG_LIQUID)
#define voxel_in_corner(x, z, pchunk) (!(x % (pchunk)->chunk_width) || x % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1) && !(z % (pchunk)->chunk_width) || z % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1))
#define voxel_in_edge(x, z, pchunk) (!(x % (pchunk)->chunk_width) || !(z % (pchunk)->chunk_width) || x % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1) || z % (pchunk)->chunk_width == ((pchunk)->chunk_width - 1)) 
bool voxel_in_air(voxel *pvox, chunk *pchunk);


/** -------------------------------------------------------------------------------------------------------------------------------------
* New code
* Updated: 14.09.2021
*/
#define COORD2OFFSET2(w, h, x, y, z) ((x * w + z) * h + y) //(x * width + y) * width + z
#define OFFSET2COORD2(x, y, z, w, offset) \
    x = offset / (w * w)/* % ARRAY_SIZE*/;\
    y = (offset / w) % w;\
    z = offset % w;

class CChunk;

class CVoxel
{
public:
	CVoxel() {}
	~CVoxel() {}

	inline voxel_cell_t GetFlags();
	void SetFlag(voxel_cell_t flag);
	inline bool IsEmpty();
	inline bool IsSolid();
	inline bool IsLiquid();
	inline bool InChunkCorner(CChunk *pchunk, int x, int z);
	inline bool InChunkEdge(CChunk *pchunk, int x, int z);
private:
	voxel_cell_t m_Flags;
};

#define COMPUTE_CHUNK_SIZE(w, h) (w * h * w * sizeof(CVoxel))

class CChunk
{
public:
	CChunk() : m_pVoxels(NULL), m_nWidth(16), m_nHeight(64), m_ChunkPos(0, 0, 0), m_vecMax(16, 16, 16), m_bIdle(true) {}
	CChunk(vec3int pos, int width, int height);
	~CChunk() {}

	int Init(vec3int pos, int width, int height, int flags = VOXEL_FLAG_AIR);
	int AllocVoxels(int width, int height, int flags = VOXEL_FLAG_AIR);
	int FreeVoxels();

	void SetChunkPosition(vec3int pos);
	void SetChunkPosition(int x, int y, int z);
	void SetChunkSize(int width, int height);
	inline int GetLayerArea();
	int GetChunkWidth();
	int GetChunkHeight();
	int GetNumberVoxels();
	CVoxel *GetVoxels();
	CVoxel *VoxelAt(int x, int y, int z);

	/**
	* Building/destroying voxels
	*/
	bool DestroyVoxelByRay(CRay &ray, int voxflags = VOXEL_FLAG_AIR);

	/**
	* For world manager
	*/
	bool HasIdle();
	void MarkIdle(bool idle);

	/**
	* Unsafe functions!
	*/
	//inline CVoxel *PrevVoxel(CVoxel *pvox)		{ return ((CVoxel *)pvox + 1); }
	//inline CVoxel *NextVoxel(CVoxel *pvox)		{ return ((CVoxel *)pvox - 1); }
	//inline CVoxel *LeftVoxel(CVoxel *pvox)		{ return ((CVoxel *)pvox - m_nWidth); }
	//inline CVoxel *RightVoxel(CVoxel *pvox)		{ return ((CVoxel *)pvox + m_nWidth); }
	//inline CVoxel *BottomVoxel(CVoxel *pvox)	{ return ((CVoxel *)pvox - GetLayerArea()); }
	//inline CVoxel *TopVoxel(CVoxel *pvox)		{ return ((CVoxel *)pvox + GetLayerArea()); }
	//int GetVoxelTerrace(CVoxel *pvox); //TODO: Implement this later

	void MarchCube(vec3 min_corner_pos);
	void DrawMesh(); //draw
	void ClearMesh();
	void BuildMesh();
	void RebuildMesh(); //rebuild mesh from voxels

	vec3 *GetVertices();
	int *GetIndices();
	int GetNumOfVertices();
	int GetNumOfIndices();

#ifdef DEBUG_DRAW
	void DebugDraw_ChunkBounds(bool b);
	void DebugDraw_ChunkVoxels(bool b);
	void DebugDraw_ChunkCubes(bool b);
	void DebugDraw_LastSelectTriangle(bool b);
	void DebugDraw_SetLastSelectTriangle(triangle_t &tri);

	bool m_nDDBounds;
	bool m_nDDVoxels;
	bool m_nDDCubes;
	bool m_bDDLastSelectTri;
	triangle_t m_LastSelectTriangle;
#endif

	vec3int m_ChunkPos;
	vec3int m_vecMax;
private:
	bool m_bIdle;
	int m_nWidth, m_nHeight;
	CVoxel *m_pVoxels;
	std::vector<vec3> m_vertices;
	std::vector<int> m_indices;
};