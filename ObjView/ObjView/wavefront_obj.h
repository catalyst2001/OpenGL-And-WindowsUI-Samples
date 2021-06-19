#pragma once
#include "main.h"

struct Vertex3 { float x, y, z; };
struct TextureCoord { float u, v; };

struct Group {
	int num_of_vertices;
	Vertex3 vertices;
	
};

struct Model {
	int num_of_groups;
	int num_of_objects;
	Group *groups;
};

enum {
	COUNT_VERTICES = 0,
	COUNT_TEXCOORDS,
	COUNT_NORMALS,
	COUNT_PLANES,
	COUNT_VERTEX_PARAMETERS,
	COUNT_GROUPS,
	COUNT_OBJECTS
};

int get_count(FILE *fp, int type)
{
	int count = 0;
	long offset = ftell(fp);
	char buffer[256];
	while (!feof(fp) && !ferror(fp)) {
		fgets(buffer, sizeof(buffer), fp);
		if (buffer[0] == '#')
			continue;

		switch (type) {
		case COUNT_VERTICES:
			if (buffer[0] == 'v')
				count++;
			break;

		case COUNT_TEXCOORDS:
			if (buffer[0] == 'v' && buffer[1] == 't')
				count++;
			break;

		case COUNT_NORMALS:
			if (buffer[0] == 'v' && buffer[1] == 'n')
				count++;
			break;

		case COUNT_PLANES:
			if (buffer[0] == 'f')
				count++;
			break;

		case COUNT_VERTEX_PARAMETERS:
			if (buffer[0] == 'v' && buffer[0] == 'p')
				count++;
			break;

		case COUNT_GROUPS:
			if (buffer[0] == 'g')
				count++;
			break;

		case COUNT_OBJECTS:
			if (buffer[0] == 'o')
				count++;
			break;
		}
	}

	if (offset != ftell(fp))
		fseek(fp, offset, SEEK_SET);

	return count;
}

void LoadWavefrontObj(Model *pmodel, const char *filename)
{
	FILE *fp = fopen(filename, "rt");


}