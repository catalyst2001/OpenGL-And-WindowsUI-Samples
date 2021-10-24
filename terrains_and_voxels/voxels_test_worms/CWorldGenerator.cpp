#include "CWorldGenerator.h"

CWorldGenerator::CWorldGenerator()
{
}

CWorldGenerator::CWorldGenerator(CChunksManager *pchnkmgr)
{
	m_pChunksManager = pchnkmgr;
}

CWorldGenerator::~CWorldGenerator()
{
}

void CWorldGenerator::QueryProcessorInfo()
{
	SYSTEM_INFO sysinf;
	GetSystemInfo(&sysinf);
	m_nLogicalProcessors = sysinf.dwNumberOfProcessors;
}

bool CWorldGenerator::QueryProcessorSpeed()
{
	__try {
		//insert code
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
	return false;
}

CTerrainGenerator::CTerrainGenerator()
{
}

CTerrainGenerator::~CTerrainGenerator()
{
}

void CTerrainGenerator::RegionVoxel(int x, int y, int z)
{
}
