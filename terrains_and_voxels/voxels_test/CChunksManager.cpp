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
	int minChunkY = /*-(chunk_height / 2)*/0.f;
	m_nNumOfChunks = chunks_per_width * chunks_per_width * chunks_per_height;
	m_pChunks = new CChunk[m_nNumOfChunks];

	//HANDLE *hThreads = (HANDLE *)malloc(m_nNumOfChunks * sizeof(HANDLE));

	CChunk *p_current_chunk = NULL;
	for (int y = 0; y < chunks_per_height; y++) {
		for (int x = 0; x < chunks_per_width; x++) {
			for (int z = 0; z < chunks_per_width; z++) {
				p_current_chunk = &m_pChunks[COORD2OFFSET2(chunks_per_height, chunks_per_width, x, y, z)];
				p_current_chunk->Init(vec3int(x * chunk_width, y * chunk_height, z * chunk_width), chunk_width, chunk_height, VOXEL_FLAG_SOLID);
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

CChunk *CChunksManager::ChunkAt(int gx, int gy, int gz)
{



	return NULL;
}
