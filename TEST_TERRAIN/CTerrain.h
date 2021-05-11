#pragma once
#include "CVector3.h"
#include <Windows.h>
#include <stdlib.h>
#include <gl/GL.h>
#include <gl/GLU.h>

typedef float terrain_vertex_t[3];

class CTerrain
{
public:
	CTerrain(short size);
	~CTerrain();

	void SetRenderDistance(short distance);
	void RenderTerrain(CVector3 camera_position);
	
	//debug
	void RenderTerrainBounds();

private:
	terrain_vertex_t* vterrain;
	short m_terrain_size;
	short m_render_distance;
};

