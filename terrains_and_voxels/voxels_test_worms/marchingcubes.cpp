#include "marchingcubes.h"

int get_cube_config(cube_t cube, float surface)
{
	int confidx = 0;
	for (int i = 0; i < 8; i++)
		if (cube[i] > surface)
			confidx |= (1 << i);

	return confidx;
}

void ChunkMeshRenderer::march_cube(float x, float y, float z, float surface, cube_t cube)
{
	int confindex = get_cube_config(cube, surface);
	if (!confindex || confindex == 255)
		return;

	int index;
	int edgeidx = 0;
	for (int i = 0; i < 5; i++) {
		for (int p = 0; p < 3; p++) {
			index = tritable[confindex][edgeidx];
			if (index == -1)
				return;

			vec3 pos(x, y, z);
			vec3 vert1 = pos + edgestable[index][0];
			vec3 vert2 = pos + edgestable[index][1];
			vec3 vertPosition = (vert1 + vert2) / 2.f;
			m_Verts.push_back(vertPosition);

			int cc = m_Verts.size() - 1;
			m_Indices.push_back(cc);
			edgeidx++;
		}
	}
}
