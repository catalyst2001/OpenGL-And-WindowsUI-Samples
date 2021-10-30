#pragma once
#include <Windows.h> //threads api
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../../utilites/glmath.h"
#include "voxel.h"
#include "CChunksManager.h"

enum worldgen_type_e {
	WG_BIOMES = 0,
	WG_TERRAIN,	//местность (овраги, горы и прочие возвышенности)
	WG_CAVES,		//пещеры
	WG_RIVERS,		//реки
	WG_CITIES,		//города с помощью расстановки префабов
	WG_MINES		//городские шахты
};

/**
* ДОЛГО ДУМАЛ КАК СДЕЛАТЬ И ПОКА ЧТО ОСТАВЛЮ ТАК
* Так как генератор в мире у нас не один, 
* 
* 
* 
*/
class IWorldGenerator
{
public:
	virtual void RegionVoxel(int x, int y, int z) = 0;
};

/**
* Генерация местности
*/
class CTerrainGenerator : public IWorldGenerator
{
public:
	CTerrainGenerator();
	~CTerrainGenerator();

	void RegionVoxel(int x, int y, int z);
};

class CWorldGenerator
{
public:
	CWorldGenerator();
	CWorldGenerator(CChunksManager *pchnkmgr);
	~CWorldGenerator();

	inline void SetManager(CChunksManager *pchnkmgr) { m_pChunksManager = pchnkmgr; }

private:
	HANDLE m_hMutex;
	void QueryProcessorInfo();
	bool QueryProcessorSpeed();

	double m_fGenerationElapsedTime;
	uint32_t m_nLogicalProcessors;
	uint32_t m_nProcessorSpeedMHz;
	uint32_t m_nGenerationType;
	CChunksManager *m_pChunksManager;
};