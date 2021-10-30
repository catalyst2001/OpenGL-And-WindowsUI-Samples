#include "CChunksManager.h"

CChunksManager::CChunksManager()
{
}

CChunksManager::~CChunksManager()
{
}

int CChunksManager::Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height)
{
	m_nChunkWidth = chunk_width;
	m_nChunkHeight = chunk_height;
	m_nChunksPerWidth = chunks_per_width;
	m_nChunksPerHeight = chunks_per_height;
	m_nNumberOfChunks = chunks_per_width * chunks_per_width * chunks_per_height;
	m_pChunks = new CChunk[m_nNumberOfChunks];
	CChunk *p_current_chunk = NULL;
	for (int y = 0; y < chunks_per_height; y++) {
		for (int x = 0; x < chunks_per_width; x++) {
			for (int z = 0; z < chunks_per_width; z++) {
				p_current_chunk = &m_pChunks[COORD2OFFSET2(chunks_per_height, chunks_per_width, x, y, z)];
				p_current_chunk->Init(vec3int(x * chunk_width, y * chunk_height, z * chunk_width),
					chunk_width,
					chunk_height,
					VOXEL_FLAG_AIR);

				p_current_chunk->DebugDraw_ChunkBounds(false);
				p_current_chunk->MarkIdle(false);
				p_current_chunk->RebuildMesh();
			}
		}
	}
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
	vec3int ipos = vec3int((int)pos->x, (int)pos->y, (int)pos->z);
	int sx = ipos.x % m_nChunkWidth;
	int sy = ipos.y % m_nChunkHeight;
	int sz = ipos.z % m_nChunkWidth;
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight) && ipos.x == (m_nChunkWidth * m_nChunksPerWidth)) {
		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}

	//
	//	y z
	//
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
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
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sz) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
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
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth) && ipos.z == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
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
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz && !sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}

	//
	//	y
	//
	if (ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz) {
			pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz && !sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}

	//
	//	z
	//
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sy && !sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y, ipos.z - m_nChunkWidth);
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
	int sx = int(pos->x) % m_nChunkWidth;
	int sy = int(pos->y) % m_nChunkHeight;
	int sz = int(pos->z) % m_nChunkWidth;
	if (!sx) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sy) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y - m_nChunkHeight, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sx && !sy) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y - m_nChunkHeight, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz && !sy) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y - m_nChunkHeight, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sx && !sz) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz && !sy && !sx) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y - m_nChunkHeight, (int)pos->z - m_nChunkWidth);
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
		*(ppchunk) = GetRegionChunk((int)pos->x, (int)pos->y, (int)pos->z);
		if (!*(ppchunk))
			goto __end;

		*krai = (int(pos->x) == m_nChunkWidth * m_nChunksPerWidth || int(pos->y) == m_nChunkHeight * m_nChunksPerHeight || int(pos->z) == m_nChunkWidth * m_nChunksPerWidth);

		*(ppvox) = (*ppchunk)->VoxelAt(
			int(pos->x) == m_nChunkWidth * m_nChunksPerWidth ? m_nChunkWidth : (int(pos->x) % m_nChunkWidth),
			int(pos->y) == m_nChunkHeight * m_nChunksPerHeight ? m_nChunkHeight : (int(pos->y) % m_nChunkHeight),
			int(pos->z) == m_nChunkWidth * m_nChunksPerWidth ?  m_nChunkWidth : (int(pos->z) % m_nChunkWidth)
		);
		
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

void CChunksManager::SetVoxel(vec3int position, int flags)
{
	//check is not valid coords
	if(position.x < 0 || position.y < 0 || position.z < 0)
		return;

	vec3 voxpos(position.x, position.y, position.z);
	if (position.x == GetRegionWidth() || position.y == GetRegionHeight() || position.z == GetRegionWidth()) {
		iskl_krai(&voxpos, flags);
	}
	else
	{
		CVoxel *pvox = GetRegionVoxel(position.x, position.y, position.z);
		if (!pvox)
			return;

		pvox->SetFlag(flags);
		iskl(&voxpos, flags);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChunksGeneratorTest::CChunksGeneratorTest()
{
}

CChunksGeneratorTest::~CChunksGeneratorTest()
{
}

void CChunksGeneratorTest::InitProperties(int distance, int chunk_width, int chunk_height)
{
	m_nChunkWidth = chunk_width;
	m_nChunkHeight = chunk_height;
	m_nDistanceInChunks = distance;

	int layer_area = (m_nDistanceInChunks * 2) + 1;
	m_nNumberOfChunks = layer_area * layer_area;
}

int CChunksGeneratorTest::StartGeneration2(vec3int &playerpos, int distance, int chunk_width, int chunk_height)
{
	InitProperties(distance, chunk_width, chunk_height);
	m_Chunks.reserve(m_nNumberOfChunks);

	//пока что из центра мира а не с места спавна игрока
	for (int x = -m_nDistanceInChunks; x <= m_nDistanceInChunks; x++) {
		for (int z = -m_nDistanceInChunks; z <= m_nDistanceInChunks; z++) {

			//добавляем новые чанки вокруг главного
			CChunk chunk;
			chunk.InitNoAlloc(vec3int(x * m_nChunkWidth, 0, z * m_nChunkWidth), m_nChunkWidth, m_nChunkHeight);
			chunk.DebugDraw_ChunkBounds(true);
			chunk.DebugDraw_ChunkVoxels(false);
			m_Chunks.push_back(chunk);
			
			//ищем центральный чанк
			CChunk *p_curr_chunk = &m_Chunks[m_Chunks.size() - 1];
			if (!p_curr_chunk->m_ChunkPos.x && !p_curr_chunk->m_ChunkPos.y && !p_curr_chunk->m_ChunkPos.z) //так же проверка по центру мира а не по месту спавна игрока
				m_pMainChunk = p_curr_chunk;
		}
	}
	return 1;
}

void CChunksGeneratorTest::Update(vec3int pos)
{
	if (pos.x < m_pMainChunk->m_ChunkPos.x || pos.z < m_pMainChunk->m_ChunkPos.z || pos.x >= m_pMainChunk->m_vecMax.x || pos.z >= m_pMainChunk->m_vecMax.z) {
		//printf("%d MainChunkPos( %d %d %d )  player( %d %d %d ) vmax( %d %d %d )\n",
		//	m_pMainChunk->m_ChunkPos.x, m_pMainChunk->m_ChunkPos.y, m_pMainChunk->m_ChunkPos.z,
		//	pos.x, pos.y, pos.z,
		//	m_pMainChunk->m_vecMax.x, m_pMainChunk->m_vecMax.y, m_pMainChunk->m_vecMax.z
		//	);

		vec3int new_chunk_dir;
		for (int i = 0; i < m_Chunks.size(); i++) {
			if (pos.x >= m_Chunks[i].m_ChunkPos.x && pos.z >= m_Chunks[i].m_ChunkPos.z && pos.x < m_Chunks[i].m_vecMax.x && pos.z < m_Chunks[i].m_vecMax.z) {
				new_chunk_dir = vec3int((m_Chunks[i].m_ChunkPos.x - m_pMainChunk->m_ChunkPos.x) / m_nChunkWidth, 0, (m_Chunks[i].m_ChunkPos.z - m_pMainChunk->m_ChunkPos.z) / m_nChunkWidth); //TODO:  / m_nChunkWidth
				m_pMainChunk = &m_Chunks[i];
				printf("new_chunk_dir( %d %d %d )\n", new_chunk_dir.x, new_chunk_dir.y, new_chunk_dir.z);
				break;
			}
		}

		for (int i = m_Chunks.size() - 1; i >= 0; i--) {
			int ax = abs(m_Chunks[i].m_ChunkPos.x - m_pMainChunk->m_ChunkPos.x) / m_nChunkWidth;
			int az = abs(m_Chunks[i].m_ChunkPos.z - m_pMainChunk->m_ChunkPos.z) / m_nChunkWidth;
			printf("abss ( %d %d )\n", ax, az);
			if (ax > m_nDistanceInChunks || az > m_nDistanceInChunks) {
				//m_Chunks.erase(m_Chunks.begin() + i); // pointer stuff
				m_Chunks[i].DebugDraw_ChunkBounds(false);
				//break;
			}
		}

		//ищем по краям чанки , генерируем их и переставляем местами с удалёнными
#define GenerateChunk(x, y, z)
		if (new_chunk_dir.x != 0) {
			for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
				GenerateChunk(m_pMainChunk->m_ChunkPos.x + newChunkDirection.x * m_nDistanceInChunks, m_pMainChunk->m_ChunkPos.z + i);
			}
		}

		if (new_chunk_dir.z != 0) {
			for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
				GenerateChunk(m_pMainChunk->m_ChunkPos.x + i, m_pMainChunk->m_ChunkPos.z + newChunkDirection.z * m_nDistanceInChunks);
			}
		}
	}
}

void CChunksGeneratorTest::OnChunksLoad(vec3int &playerpos)
{
}

//////////////////////// THREADS AND GENERATION ///////////////////////////////
CRITICAL_SECTION cs;
CCollector<CVoxel *> g_chunks_voxels;
uint32_t ready_threads;

DWORD WINAPI ThreadProcGeneration(LPVOID parameter)
{
	DWORD thread_id = GetCurrentThreadId();
	chunk_information_s *p_chunk_info = (chunk_information_s *)parameter;
	printf("Thread %d is started. Chunk width: %d  height: %d  startX: %d  startZ: %d\n",
		thread_id, p_chunk_info->chunk_width, p_chunk_info->chunk_height, p_chunk_info->xpos, p_chunk_info->zpos);

	/* allocate memory for voxels */
	CVoxel *p_voxels = new CVoxel[p_chunk_info->chunk_width * p_chunk_info->chunk_width * p_chunk_info->chunk_height];

	/* failed allocate memory for voxels in chunk */
	if (!p_voxels)
		return -1;

	/* walk all over the space to generate */
	SimplexNoise perlin;
	for (int y = 0; y < p_chunk_info->chunk_height; y++) {
		for (int z = 0; z < p_chunk_info->chunk_width; z++) {
			for (int x = 0; x < p_chunk_info->chunk_width; x++) {
				if (perlin.noise((float)x, (float)z) > 0.01f) {
					p_voxels[COORD2OFFSET2(p_chunk_info->chunk_width, p_chunk_info->chunk_height, x, y, z)].SetFlag(VOXEL_FLAG_SOLID);
				}
			}
		}
	}

	/* set voxels heap address to chunk */
	EnterCriticalSection(&cs);
	p_chunk_info->p_chunk->SetVoxels(p_voxels);
	p_chunk_info->p_chunk->RebuildMesh();
	LeaveCriticalSection(&cs);

	InterlockedIncrement(&ready_threads);
	printf("Thread %d is dead\n", thread_id);
	return 1;
}

#define COORD2D2OFFS(width, x, y) ((x * width) + y)

int CChunksGeneratorTest::StartGeneration(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height)
{


	return 1; //TODO: ПОКА ЧТО ТУТ СТОПИМ



	/* Get number of logical processors */
	SYSTEM_INFO sysinf;
	int32_t number_of_processors;
	GetSystemInfo(&sysinf);
	number_of_processors = sysinf.dwNumberOfProcessors;
	ready_threads = number_of_processors;

	/* allocate memory for thread handles for each core */
	HANDLE *p_core_threads = (HANDLE *)malloc(sizeof(HANDLE) * number_of_processors);
	if (!p_core_threads)
		return 1;

	/* init critical section for synchronized access to queue */
	InitializeCriticalSection(&cs);

	int i = 0;
	int x = 0, z = 0;
	CChunk *p_chunk = NULL;
	for (int i = 0; i < m_nNumberOfChunks; i++, z++) {
		if (!(i % m_nChunksPerWidth)) {
			x++;
			z = 0;
		}
		//p_chunk = &m_pChunks[COORD2D2OFFS(m_nChunksPerWidth, x, z)];
		p_chunk->InitNoAlloc(vec3int(x * chunk_width, 0, z * chunk_width), chunk_width, chunk_height);

		while (ready_threads < number_of_processors)
			Sleep(10);

		/* Fill chunk information for generation in thread */
		chunk_information_s chunk_info;
		chunk_info.chunk_width = m_nChunkWidth;
		chunk_info.chunk_height = m_nChunkHeight;
		chunk_info.xpos = x * m_nChunkWidth;
		chunk_info.zpos = z * m_nChunkWidth;
		chunk_info.p_chunk = p_chunk; //set current chunk address
		p_core_threads[i] = CreateThread(NULL, NULL, ThreadProcGeneration, &chunk_info, NULL, NULL);
	}

	//p_current_chunk->DebugDraw_ChunkBounds(false);
	//p_current_chunk->MarkIdle(false);
	//p_current_chunk->RebuildMesh();
	return 0;
}