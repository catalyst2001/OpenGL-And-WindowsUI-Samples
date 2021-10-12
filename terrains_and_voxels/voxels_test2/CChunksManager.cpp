#include "CChunksManager.h"

CChunksManager::CChunksManager()
{
}

CChunksManager::~CChunksManager()
{
}

DWORD WINAPI thread(LPVOID param)
{
	((CChunk *)param)->RebuildMesh();
	return 0;
}

int CChunksManager::Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height)
{
	m_nChunkWidth = chunk_width;
	m_nChunkHeight = chunk_height;
	m_nChunksPerWidth = chunks_per_width;
	m_nChunksPerHeight = chunks_per_height;
	m_nNumberOfChunks = chunks_per_width * chunks_per_width * chunks_per_height;
	m_pChunks = new CChunk[m_nNumberOfChunks];
	//HANDLE *hThreads = (HANDLE *)malloc(m_nNumOfChunks * sizeof(HANDLE));
	CChunk *p_current_chunk = NULL;
	for (int y = 0; y < chunks_per_height; y++) {
		for (int x = 0; x < chunks_per_width; x++) {
			for (int z = 0; z < chunks_per_width; z++) {
				p_current_chunk = &m_pChunks[COORD2OFFSET2(chunks_per_height, chunks_per_width, x, y, z)];
				p_current_chunk->Init(vec3int(x * chunk_width, y * chunk_height, z * chunk_width),
					chunk_width,
					chunk_height,
					VOXEL_FLAG_SOLID);

				p_current_chunk->DebugDraw_ChunkBounds(true);
				p_current_chunk->DebugDraw_LastSelectTriangle(true);
				p_current_chunk->MarkIdle(false);
				p_current_chunk->RebuildMesh();
				//hThreads[i] = CreateThread(0, 0, thread, p_current_chunk, 0, 0);
			}
		}
	}
	//WaitForMultipleObjects(m_nNumOfChunks, hThreads, TRUE, INFINITE);
	//free(hThreads);
	return 0;
}

CVoxel *CChunksManager::GetRegionVoxel(int gx, int gy, int gz)
{
	CChunk *pchunk = GetRegionChunk(gx, gy, gz);
	if (!pchunk)
		return NULL;

	return pchunk->VoxelAt(gx % pchunk->GetChunkWidth(),
		gy % pchunk->GetChunkHeight(),
		gz % pchunk->GetChunkWidth());
}

CChunk *CChunksManager::GetRegionChunk(int gx, int gy, int gz)
{
	int xd = GLOBALCOORD2LOCAL(gx, m_nChunkWidth);
	int yd = GLOBALCOORD2LOCAL(gy, m_nChunkHeight);
	int zd = GLOBALCOORD2LOCAL(gz, m_nChunkWidth);

	//check bounds
	if (xd < 0 || yd < 0 || zd < 0)
		return NULL;

	if (xd >= m_nChunksPerWidth || yd >= m_nChunksPerHeight || zd >= m_nChunksPerWidth)
		return NULL;

	return &m_pChunks[COORD2OFFSET2(m_nChunksPerWidth, m_nChunksPerHeight, xd, yd, zd)];
}

bool CChunksManager::GetVoxelByRay(CChunk **ppchunk, CVoxel **ppvox, CRay &ray, vec3 *pos, float distance, int checkflag)
{
	float dist = 0.f;
	bool x = 0, y = 0, z = 0;
	while (dist < distance) {
		*(pos) = ray.m_origin + ray.m_direction * dist;
		round_vector(*(pos), *(pos), 1.0f);
		*(ppchunk) = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y, (int)(*(pos)).z);
		if (!*(ppchunk))
			goto __end;

		*(ppvox) = (*ppchunk)->VoxelAt(int((*(pos)).x) % m_nChunkWidth, int((*(pos)).y) % m_nChunkHeight, int((*(pos)).z) % m_nChunkWidth);
		if (!*(ppvox))
			goto __end;

		if ((*ppvox)->GetFlags() & checkflag) {
			return true;
		}
	__end:
		dist++;
	}
	return false;
}

bool CChunksManager::RemoveSolidVoxel(CRay &ray, float distance, int newflag)
{
	vec3 *pos;
	pos = (vec3*)&vec3(-1, -1, -1);
	CChunk *pchunk;
	CVoxel *pvoxel;
	if (GetVoxelByRay(&pchunk, &pvoxel, ray, pos, distance, VOXEL_FLAG_SOLID)) {
		if (!pchunk || !pvoxel)
			return false;
		int sx = int((*(pos)).x) % m_nChunkWidth, sy = int((*(pos)).y) % m_nChunkHeight,sz = int((*(pos)).z) % m_nChunkWidth;


		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		if (!sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z);
			if (!(pchunk))
				goto __sx_end;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz); 
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sx_end:
		if (!sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
			if (!(pchunk))
				goto __sy_end;
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sy_end:
		if (!sz) {
			pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y, (int)(*(pos)).z- m_nChunkWidth);
			if (!(pchunk))
				goto __sz_end;
			pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sz_end:
		if (!sx && !sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkWidth, (int)(*(pos)).z);
			if (!(pchunk))
				goto __sx_sy_end;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sx_sy_end:
		if (!sz && !sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkWidth, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto __sz_sy_end;
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sz_sy_end:
		if (!sx && !sz) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto __sx_sz_end;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sx_sz_end:
		if (!sz && !sy && !sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkWidth, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto __sx_sy_sz_end;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	__sx_sy_sz_end:
		return true;
	}
	return false;
}

bool CChunksManager::PlaceSolidVoxel(CRay &ray, float distance, int newflag)
{
	return false;
}
