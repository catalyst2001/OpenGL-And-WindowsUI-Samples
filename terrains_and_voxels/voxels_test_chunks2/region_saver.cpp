#include "region_saver.h"

int region_save(CChunksManager *preg, const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		printf("Failed create file '%s'!\n", filename);
		return 0;
	}

	region_header reghdr;
	reghdr.hashsum = 0;
	reghdr.chunk_width = preg->m_nChunkWidth;
	reghdr.chunk_height = preg->m_nChunkHeight;
	reghdr.num_of_chunks_per_width = preg->m_nChunksPerWidth;
	reghdr.num_of_chunks_per_height = preg->m_nChunksPerHeight;
	if (fwrite(&reghdr, sizeof(reghdr), 1, fp) != sizeof(region_header)) {
		printf("Failed write header!\n");
		return 0;
	}

	int num_of_chunks_in_region = reghdr.num_of_chunks_per_width * reghdr.num_of_chunks_per_height * reghdr.num_of_chunks_per_width;
	for (int i = 0; i < num_of_chunks_in_region; i++) {
		CChunk *pchunk = &preg->m_pChunks[i];
		fwrite(pchunk->m_pVoxels, sizeof(CVoxel), pchunk->m_nWidth * pchunk->m_nWidth * pchunk->m_nHeight, fp);
	}
	fclose(fp);
	return 1;
}

int region_load(CChunksManager *preg, const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		printf("Failed create file '%s'!\n", filename);
		return 0;
	}

	region_header reghdr;
	if (fread(&reghdr, sizeof(reghdr), 1, fp) != sizeof(reghdr)) {
		printf("Error reading header from file!\n");
		return 0;
	}
	preg->m_nChunkWidth = reghdr.chunk_width;
	preg->m_nChunkHeight = reghdr.chunk_height;
	preg->m_nNumberOfChunks = reghdr.num_of_chunks_per_width * reghdr.num_of_chunks_per_height * reghdr.num_of_chunks_per_width;
	preg->m_nChunksPerWidth = reghdr.num_of_chunks_per_width;
	preg->m_nChunksPerHeight = reghdr.num_of_chunks_per_height;
	//preg->m_pChunks = new CChunk[preg->m_nNumberOfChunks];

	CChunk *pchunk = NULL;
	for (int i = 0; i < preg->m_nNumberOfChunks; i++) {
		pchunk = &preg->m_pChunks[i];
		//pchunk->AllocVoxels(preg->m_nChunkWidth, preg->m_nChunkHeight);
		fread(pchunk->m_pVoxels, sizeof(CVoxel), preg->m_nChunkWidth * preg->m_nChunkWidth * preg->m_nChunkHeight, fp);
	}
	fclose(fp);
}
