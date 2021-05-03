#include "md5mesh.h"
/* vertex array related stuff */
int max_verts = 0;
int max_tris = 0;

vec3_t *vertexArray = NULL;
GLuint *vertexIndices = NULL;

//////////////-> DBG

vec3_t* GetVertsArray() {
	return vertexArray;
}

GLuint* GetVertsIndices() {
	return vertexIndices;
}

int GetMaxVerts()
{
	return max_verts;
}

int GetMaxTris() {
	return max_tris;
}
/////////////<- DBG

/**
 * Load an MD5 model from file.
 */
int ReadMD5Model(const char *filename, struct md5_model_t *mdl)
{
	char buff[512];
	int version;
	int curr_mesh = 0;
	int i;

	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		fprintf(stderr, "Error: couldn't open \"%s\"!\n", filename);
		return 0;
	}

	while (!feof(fp))
	{
		/* Read whole line */
		fgets(buff, sizeof(buff), fp);
		if (sscanf(buff, " MD5Version %d", &version) == 1)
		{
			if (version != 10)
			{
				/* Bad version */
				fprintf(stderr, "Error: bad model version\n");
				fclose(fp);
				return 0;
			}
		}
		else if (sscanf(buff, " numJoints %d", &mdl->num_joints) == 1)
		{
			if (mdl->num_joints > 0)
			{
				/* Allocate memory for base skeleton joints */
				mdl->baseSkel = (struct md5_joint_t *)calloc(mdl->num_joints, sizeof(struct md5_joint_t));
			}
		}
		else if (sscanf(buff, " numMeshes %d", &mdl->num_meshes) == 1)
		{
			if (mdl->num_meshes > 0)
			{
				/* Allocate memory for meshes */
				mdl->meshes = (struct md5_mesh_t *)calloc(mdl->num_meshes, sizeof(struct md5_mesh_t));
			}
		}
		else if (strncmp(buff, "joints {", 8) == 0)
		{
			/* Read each joint */
			for (i = 0; i < mdl->num_joints; ++i)
			{
				struct md5_joint_t *joint = &mdl->baseSkel[i];

				/* Read whole line */
				fgets(buff, sizeof(buff), fp);

				if (sscanf(buff, "%s %d ( %f %f %f ) ( %f %f %f )",
					joint->name, &joint->parent,
					&joint->pos[0], &joint->pos[1], &joint->pos[2],
					&joint->orient[0], &joint->orient[1], &joint->orient[2]) == 8)
				{
					/* Compute the w component */
					Quat_computeW(joint->orient);
				}
			}
		}
		else if (strncmp(buff, "mesh {", 6) == 0)
		{
			struct md5_mesh_t *mesh = &mdl->meshes[curr_mesh];
			int vert_index = 0;
			int tri_index = 0;
			int weight_index = 0;
			float fdata[4];
			int idata[3];

			while ((buff[0] != '}') && !feof(fp))
			{
				/* Read whole line */
				fgets(buff, sizeof(buff), fp);

				if (strstr(buff, "shader "))
				{
					int quote = 0, j = 0;

					/* Copy the shader name whithout the quote marks */
					for (i = 0; i < sizeof(buff) && (quote < 2); ++i)
					{
						if (buff[i] == '\"')
							quote++;

						if ((quote == 1) && (buff[i] != '\"'))
						{
							mesh->shader[j] = buff[i];
							j++;
						}
					}
				}
				else if (sscanf(buff, " numverts %d", &mesh->num_verts) == 1)
				{
					if (mesh->num_verts > 0)
					{
						/* Allocate memory for vertices */
						mesh->vertices = (struct md5_vertex_t *)
							malloc(sizeof(struct md5_vertex_t) * mesh->num_verts);
					}

					if (mesh->num_verts > max_verts)
						max_verts = mesh->num_verts;
				}
				else if (sscanf(buff, " numtris %d", &mesh->num_tris) == 1)
				{
					if (mesh->num_tris > 0)
					{
						/* Allocate memory for triangles */
						mesh->triangles = (struct md5_triangle_t *)malloc(sizeof(struct md5_triangle_t) * mesh->num_tris);
					}

					if (mesh->num_tris > max_tris)
						max_tris = mesh->num_tris;
				}
				else if (sscanf(buff, " numweights %d", &mesh->num_weights) == 1)
				{
					if (mesh->num_weights > 0)
					{
						/* Allocate memory for vertex weights */
						mesh->weights = (struct md5_weight_t *)
							malloc(sizeof(struct md5_weight_t) * mesh->num_weights);
					}
				}
				else if (sscanf(buff, " vert %d ( %f %f ) %d %d", &vert_index,
					&fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
				{
					/* Copy vertex data */
					mesh->vertices[vert_index].st[0] = fdata[0];
					mesh->vertices[vert_index].st[1] = fdata[1];
					mesh->vertices[vert_index].start = idata[0];
					mesh->vertices[vert_index].count = idata[1];
				}
				else if (sscanf(buff, " tri %d %d %d %d", &tri_index,
					&idata[0], &idata[1], &idata[2]) == 4)
				{
					/* Copy triangle data */
					mesh->triangles[tri_index].index[0] = idata[0];
					mesh->triangles[tri_index].index[1] = idata[1];
					mesh->triangles[tri_index].index[2] = idata[2];
				}
				else if (sscanf(buff, " weight %d %d %f ( %f %f %f )", &weight_index, &idata[0], &fdata[3], &fdata[0], &fdata[1], &fdata[2]) == 6)
				{
					/* Copy vertex data */
					mesh->weights[weight_index].joint = idata[0];
					mesh->weights[weight_index].bias = fdata[3];
					mesh->weights[weight_index].pos[0] = fdata[0];
					mesh->weights[weight_index].pos[1] = fdata[1];
					mesh->weights[weight_index].pos[2] = fdata[2];
				}
			}
			curr_mesh++;
		}
	}
	fclose(fp);
	return 1;
}

/**
 * Free resources allocated for the model.
 */
void FreeModel(struct md5_model_t *mdl)
{
	int i;

	if (mdl->baseSkel)
	{
		free(mdl->baseSkel);
		mdl->baseSkel = NULL;
	}

	if (mdl->meshes)
	{
		/* Free mesh data */
		for (i = 0; i < mdl->num_meshes; ++i)
		{
			if (mdl->meshes[i].vertices)
			{
				free(mdl->meshes[i].vertices);
				mdl->meshes[i].vertices = NULL;
			}

			if (mdl->meshes[i].triangles)
			{
				free(mdl->meshes[i].triangles);
				mdl->meshes[i].triangles = NULL;
			}

			if (mdl->meshes[i].weights)
			{
				free(mdl->meshes[i].weights);
				mdl->meshes[i].weights = NULL;
			}
		}

		free(mdl->meshes);
		mdl->meshes = NULL;
	}
}

/**
 * Prepare a mesh for drawing.  Compute mesh's final vertex positions
 * given a skeleton.  Put the vertices in vertex arrays.
 */
void PrepareMesh(const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton)
{
	int i, j, k;

	/* Setup vertex indices */
	for (k = 0, i = 0; i < mesh->num_tris; ++i)
	{
		for (j = 0; j < 3; ++j, ++k)
			vertexIndices[k] = mesh->triangles[i].index[j];
	}

	/* Setup vertices */
	for (i = 0; i < mesh->num_verts; ++i)
	{
		vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };
		/* Calculate final vertex to draw with weights */
		for (j = 0; j < mesh->vertices[i].count; ++j)
		{
			const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
			const struct md5_joint_t *joint = &skeleton[weight->joint];

			/* Calculate transformed vertex for this weight */
			vec3_t wv;
			Quat_rotatePoint(joint->orient, weight->pos, wv);

			/* The sum of all weight->bias should be 1.0 */
			finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
			finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
			finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
		}
		vertexArray[i][0] = finalVertex[0];
		vertexArray[i][1] = finalVertex[1];
		vertexArray[i][2] = finalVertex[2];
	}
}

void AllocVertexArrays()
{
	vertexArray = (vec3_t *)malloc(sizeof(vec3_t) * max_verts);
	vertexIndices = (GLuint *)malloc(sizeof(GLuint) * max_tris * 3);
}

void FreeVertexArrays()
{
	if (vertexArray)
	{
		free(vertexArray);
		vertexArray = NULL;
	}
	if (vertexIndices)
	{
		free(vertexIndices);
		vertexIndices = NULL;
	}
}