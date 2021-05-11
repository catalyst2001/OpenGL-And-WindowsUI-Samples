#include "CTerrain.h"

CTerrain::CTerrain(short size) : m_terrain_size(size), m_render_distance(50)
{
	//  	 | Y 
	//  	 |
	//  	 |
	//  	 |
	//  	 |
	//  	 |
	//       |___________________ X
	//      /
	//     /
	//    /
	//   /
	//  / Z
	vterrain = (terrain_vertex_t*)calloc(m_terrain_size, sizeof(terrain_vertex_t));
	int new_axisX = 0,
		new_axisZ = 0;

	//vterrain[0][1] = vterrain[0][0] = 0.f;
	//for (int i = 0; i < m_terrain_size; i++) {
	//	vterrain[i + 1][0] = i + 1;
	//}
}

CTerrain::~CTerrain()
{
	if (vterrain)
		free(vterrain);
}

void CTerrain::SetRenderDistance(short distance)
{
	m_render_distance = distance;
}

void CTerrain::RenderTerrain(CVector3 camera_position)
{
	glColor3ub(255, 255, 255);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vterrain);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_terrain_size);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CTerrain::RenderTerrainBounds()
{
	glColor3ub(0, 255, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(m_terrain_size, 1.f, 0.f);
	glVertex3f(m_terrain_size, 1.f, m_terrain_size);
	glVertex3f(0.f, 1.f, m_terrain_size);
	glEnd();
}
