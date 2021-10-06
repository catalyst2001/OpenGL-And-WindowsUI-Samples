#include "CWorldGenerator.h"

DWORD WINAPI thresad(LPVOID param)
{
	((CChunk *)param)->RebuildMesh();
	return 0;
}

int CWorldGenerator::Init(int chunk_width, int chunk_height, int num_of_chunks)
{
	//int error = WG_OK;
	//int minChunkY = /*-(chunk_height / 2)*/0.f;
	//m_nNumOfChunks = num_of_chunks * num_of_chunks;
	//m_pChunks = new CChunk[m_nNumOfChunks];

	//HANDLE *hThreads = (HANDLE *)malloc(m_nNumOfChunks * sizeof(HANDLE));

	//int i = 0;
	//CChunk *p_current_chunk = NULL;
	//for (int x = 0; x < num_of_chunks; x++) {
	//	for (int z = 0; z < num_of_chunks; z++) {
	//		p_current_chunk = &m_pChunks[i];
	//		p_current_chunk->Init(vec3int(x * chunk_width, minChunkY, z * chunk_width), chunk_width, chunk_height, VOXEL_FLAG_SOLID);
	//		p_current_chunk->DebugDraw_ChunkBounds(true);
	//		p_current_chunk->DebugDraw_LastSelectTriangle(true);
	//		p_current_chunk->MarkIdle(false);
	//		//p_current_chunk->RebuildMesh();
	//		hThreads[i] = CreateThread(0, 0, thread, p_current_chunk, 0, 0);
	//		i++;
	//	}
	//}
	//WaitForMultipleObjects(m_nNumOfChunks, hThreads, TRUE, INFINITE);
	//free(hThreads);
	return 0;
}
