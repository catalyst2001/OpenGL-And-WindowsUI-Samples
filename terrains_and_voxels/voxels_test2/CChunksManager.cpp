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

	if (gx == m_nChunkWidth * m_nChunksPerWidth || gy == m_nChunkHeight * m_nChunksPerHeight || gz == m_nChunkWidth * m_nChunksPerWidth) {
		if (gx == m_nChunkWidth * m_nChunksPerWidth) {
			xd--;
		}
		if (gy == m_nChunkHeight * m_nChunksPerHeight) {
			yd--;
		}
		if (gz == m_nChunkWidth * m_nChunksPerWidth) {
			zd--;
		}
	}

	//check bounds
	if (xd < 0 || yd < 0 || zd < 0)
		return NULL;

	if (xd >= m_nChunksPerWidth || yd >= m_nChunksPerHeight || zd >= m_nChunksPerWidth)
		return NULL;

	return &m_pChunks[COORD2OFFSET2(m_nChunksPerWidth, m_nChunksPerHeight, xd, yd, zd)];
}


void CChunksManager::iskl_krai(vec3 *pos, int newflag) {
	CChunk *pchunk;
	CVoxel *pvoxel;
	int sx = int((*(pos)).x) % m_nChunkWidth, sy = int((*(pos)).y) % m_nChunkHeight, sz = int((*(pos)).z) % m_nChunkWidth;
	if (int((*(pos)).z) == (m_nChunkWidth * m_nChunksPerWidth) && int((*(pos)).y) == (m_nChunkHeight * m_nChunksPerHeight) && int((*(pos)).x) == (m_nChunkWidth * m_nChunksPerWidth)) {
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			return;// goto __sx_sz_sy_end;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}
	//
	//	y z
	//
	if (int((*(pos)).z) == (m_nChunkWidth * m_nChunksPerWidth) && int((*(pos)).y) == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sx_y_z;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sx_y_z:
		pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}
	//
	//	y x
	//
	if (int((*(pos)).x) == (m_nChunkWidth * m_nChunksPerWidth) && int((*(pos)).y) == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sz) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sz_y_x;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sz_y_x:
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}
	//
	//	x z
	//
	if (int((*(pos)).x) == (m_nChunkWidth * m_nChunksPerWidth) && int((*(pos)).z) == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sy_x_z;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sy_x_z:
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}
	//
	//	x
	//
	if (int((*(pos)).x) == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
			if (!(pchunk))
				goto sy_x;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sy_x:
		if (!sz) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sz_x;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sz_x:
		if (!sz && !sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sz_sy_x;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sz_sy_x:
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
	//
	//	y
	//
	if (int((*(pos)).y) == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
			if (!(pchunk))
				goto sx_y;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sx_y:
		if (!sz) {
			pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sz_y;
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sz_y:
		if (!sz && !sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sz_sy_x;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sz_sz_y:
		pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
	//
	//	z
	//
	if (int((*(pos)).z) == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sy_z;
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sy_z:
		if (!sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sx_z;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sx_z:
		if (!sy && !sx) {
			pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
			if (!(pchunk))
				goto sx_sy_z;
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	sx_sy_z:
		pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
}

void CChunksManager::iskl(vec3 *pos, int newflag)
{
	CChunk *pchunk;
	CVoxel *pvoxel;
	int sx = int((*(pos)).x) % m_nChunkWidth, sy = int((*(pos)).y) % m_nChunkHeight, sz = int((*(pos)).z) % m_nChunkWidth;
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
		pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			goto __sz_end;
		pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
__sz_end:
	if (!sx && !sy) {
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z);
		if (!(pchunk))
			goto __sx_sy_end;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
__sx_sy_end:
	if (!sz && !sy) {
		pchunk = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
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
		pchunk = GetRegionChunk((int)(*(pos)).x - m_nChunkWidth, (int)(*(pos)).y - m_nChunkHeight, (int)(*(pos)).z - m_nChunkWidth);
		if (!(pchunk))
			return;
		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
}

bool CChunksManager::GetVoxelByRay(CChunk **ppchunk, CVoxel **ppvox, CRay &ray, vec3 *pos, bool *krai, float distance, int checkflag)
{
	float dist = 0.f;
	while (dist < distance) {
		*(pos) = ray.m_origin + ray.m_direction * dist;
		round_vector(*(pos), *(pos), 1.0f);
		*(ppchunk) = GetRegionChunk((int)(*(pos)).x, (int)(*(pos)).y, (int)(*(pos)).z);
		if (!*(ppchunk))
			goto __end;

		if (int((*(pos)).x) == m_nChunkWidth * m_nChunksPerWidth || int((*(pos)).y) == m_nChunkHeight * m_nChunksPerHeight || int((*(pos)).z) == m_nChunkWidth * m_nChunksPerWidth) {
			*krai = true;
		}
		else {
			*krai = false;
		}

		*(ppvox) = (*ppchunk)->VoxelAt(
		int((*(pos)).x) == m_nChunkWidth * m_nChunksPerWidth ? m_nChunkWidth : (int((*(pos)).x) % m_nChunkWidth), int((*(pos)).y) == m_nChunkHeight * m_nChunksPerHeight ? m_nChunkHeight : (int((*(pos)).y) % m_nChunkHeight), int((*(pos)).z) == m_nChunkWidth * m_nChunksPerWidth ?  m_nChunkWidth : (int((*(pos)).z) % m_nChunkWidth));
		
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
	bool krai;
	pos = (vec3*)&vec3(-1, -1, -1);
	CChunk *pchunk;
	CVoxel *pvoxel;
	if (GetVoxelByRay(&pchunk, &pvoxel, ray, pos, &krai, distance, VOXEL_FLAG_SOLID)) {
		if (!pchunk || !pvoxel)
			return false;

		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		
		if (krai) {
			iskl_krai(pos, newflag);
		}
		else if(!krai){
			iskl(pos, newflag);
		}

		return true;
	}
	return false;
}

bool CChunksManager::PlaceSolidVoxel(CRay &ray, float distance, int newflag)
{
	return false;
}
