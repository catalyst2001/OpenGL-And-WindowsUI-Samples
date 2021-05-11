#pragma once

typedef float vec3_t[3];

void vector_dot_product() {}


struct ray_t {
	vec3_t origin;
	vec3_t direction;
};

void ray_start(ray_t *pray, vec3_t origin, vec3_t direction)
{
	pray->origin[0] = origin[0];
	pray->origin[1] = origin[1];
	pray->origin[2] = origin[2];
	pray->direction[0] = direction[0];
	pray->direction[1] = direction[1];
	pray->direction[2] = direction[2];
}

void ray_plane_intersection(ray_t *pray)
{

}