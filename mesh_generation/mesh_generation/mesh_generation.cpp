#include "../../utilites/glwnd.h"
#include <map>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
using namespace std;

struct pair_hash {
	template<typename T1, typename T2>
	size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

using IndexPair = std::pair<unsigned, unsigned>;
using IndexLookup = std::unordered_map<IndexPair, unsigned, pair_hash>;

#define ICO_X .525731112119133606f
#define ICO_Z .850650808352039932f

static const unsigned IcoVertCount = 12;
static const unsigned IcoIndexCount = 60;

#include <math.h>

class vector3
{
public:
	vector3() {}
	vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	vector3 operator*(vector3 v) {
		vector3 out;
		out.x = x * v.x;
		out.y = y * v.y;
		out.z = z * v.z;
		return out;
	}

	vector3 operator*(float c) {
		x *= c;
		y *= c;
		z *= c;
		return *this;
	}

	vector3 operator/(vector3 v) {
		vector3 out;
		out.x = x / v.x;
		out.y = y / v.y;
		out.z = z / v.z;
		return out;
	}

	vector3 operator/(float v) {
		vector3 out;
		out.x = x / v;
		out.y = y / v;
		out.z = z / v;
		return out;
	}

	vector3 operator+(vector3 v) {
		vector3 out;
		out.x = x + v.x;
		out.y = y + v.y;
		out.z = z + v.z;
		return out;
	}

	vector3 operator+=(vector3 v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	float length() { return (float)sqrt(x * x + y * y + z * z); }

	vector3 normalize() {
		vector3 out;
		float k;
		float len = this->length();
		k = 1.0f / len;
		out.x = x * k;
		out.y = y * k;
		out.z = z * k;
		return out;
	}

	float x, y, z;
};

class vector2
{
public:
	vector2() {}
	vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	vector2 operator*(vector2 v) {
		vector2 out;
		out.x = x * v.x;
		out.y = y * v.y;
		return out;
	}

	vector2 operator*(float c) {
		x *= c;
		y *= c;
		return *this;
	}

	vector2 operator/(vector2 v) {
		vector2 out;
		out.x = x / v.x;
		out.y = y / v.y;
		return out;
	}

	vector2 operator/(float v) {
		vector2 out;
		out.x = x / v;
		out.y = y / v;
		return out;
	}

	vector2 operator+(vector2 v) {
		vector2 out;
		out.x = x + v.x;
		out.y = y + v.y;
		return out;
	}

	vector2 operator+=(vector2 v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	float x, y;
};

static const vector3 IcoVerts[] = {
	vector3(0, -ICO_X, -ICO_Z),
	vector3(-ICO_X, -ICO_Z,      0),
	vector3(ICO_Z,      0, -ICO_X),
	vector3(ICO_X, -ICO_Z,      0),
	vector3(ICO_X,  ICO_Z,      0),
	vector3(-ICO_X, -ICO_Z,      0),
	vector3(ICO_Z,      0,  ICO_X),
	vector3(0, -ICO_X,  ICO_Z),
	vector3(ICO_X,  ICO_Z,      0),
	vector3(-ICO_X, -ICO_Z,      0),
	vector3(0,  ICO_X,  ICO_Z),
	vector3(-ICO_Z,      0,  ICO_X),
	vector3(ICO_X,  ICO_Z,      0),
	vector3(-ICO_X, -ICO_Z,      0),
	vector3(-ICO_X,  ICO_Z,      0),
	vector3(-ICO_Z,      0, -ICO_X),
	vector3(ICO_X,  ICO_Z,      0),
	vector3(-ICO_X, -ICO_Z,      0),
	vector3(0,   ICO_X, -ICO_Z),
	vector3(0,  -ICO_X, -ICO_Z),
	vector3(ICO_X,  ICO_Z,      0),
	vector3(ICO_Z,      0, -ICO_X)
};

static const vector2 IcoUvs[] = {
	vector2(0.0,      0.157461),
	vector2(0.090909, 0.0),
	vector2(0.090909, 0.314921),  //
	vector2(0.181818, 0.157461),  // Verts & UVs are ordered by U then Y coords,
	vector2(0.181818, 0.472382),  //
	vector2(0.272727, 0.0),       //      4   8   C   G   K
	vector2(0.272727, 0.314921),  //     / \ / \ / \ / \ / \ 
	vector2(0.363636, 0.157461),  //    2---6---A---E---I---L
	vector2(0.363636, 0.472382),  //   / \ / \ / \ / \ / \ /
	vector2(0.454545, 0.0),       //  0---3---7---B---F---J
	vector2(0.454545, 0.314921),  //   \ / \ / \ / \ / \ /
	vector2(0.545454, 0.157461),  //    1   5   9   D	  H
	vector2(0.545454, 0.472382),  //
	vector2(0.636363, 0.0),       // [4, 8, C, G, K] have the same position vert
	vector2(0.636363, 0.314921),  // [1, 5, 9, D, H] have the same position vert
	vector2(0.727272, 0.157461),  // [0, J]          have the same position vert
	vector2(0.727272, 0.472382),  // [2, L]          have the same position vert
	vector2(0.818181, 0.0),       // 
	vector2(0.818181, 0.314921),
	vector2(0.90909,  0.157461),
	vector2(0.90909,  0.472382),
	vector2(1.0,      0.314921)
};

static const unsigned IcoIndex[] = {
	 0,  1,  3, // Bottom
	 3,  5,  7,
	 7,  9, 11,
	11, 13, 15,
	15, 17, 19,
	 0,  3,  2, // Middle
	 2,  3,  6,
	 3,  7,  6,
	 6,  7, 10,
	 7, 11, 10,
	10, 11, 14,
	11, 15, 14,
	14, 15, 18,
	15, 19, 18,
	18, 19, 21,
	 2,  6,  4, // Top
	 6, 10,  8,
	10, 14, 12,
	14, 18, 16,
	18, 21, 20
};

unsigned CreateVertexForEdge(IndexLookup& lookup, vector3* verts, vector2* uvs, unsigned first, unsigned second, unsigned& currentVertCount)
{
	// Order pair by smallest index first because first, second is equivalient
	// to second, first but the hash is not
	IndexPair edge = first < second ? IndexPair(first, second) : IndexPair(second, first);

	// Check the edge for a vert
	auto inserted = lookup.insert({ edge, currentVertCount });

	// If there wasn't one, create a new vert in the middle of the edge
	if (inserted.second) {
		if (verts) verts[currentVertCount] = (verts[first] + verts[second]) / 2;
		if (uvs)  uvs[currentVertCount++] = (uvs[first] + uvs[second]) / 2;
	}

	// Return the index of the vert
	return inserted.first->second;
}

void SubDevideVerts(
	vector3* verts,
	vector2* uvs,
	unsigned* index,
	unsigned& currentIndexCount,
	unsigned& currentVertCount)
{
	IndexLookup lookup;

	// Next index is going to be the current index * the 16 new indices seen below

	unsigned* next = new unsigned[16 * currentIndexCount];
	printf("currentIndexCount == %d\n", currentIndexCount);
	unsigned nextCount = 0;

	//       i + 2                    i + 2
	//        / \                      / \
	//       /   \                    /   \
	//      /     \                  /     \
	//     /       \    ------->   mid2---mid1
	//    /         \              / \     / \
	//   /           \            /   \   /   \
	//  /             \          /     \ /     \
	// i-------------i + 1      i-----mid0----i + 1

	for (unsigned i = 0; i < currentIndexCount; i += 3) {
		unsigned mid0 = CreateVertexForEdge(lookup, verts, uvs, index[i], index[i + ((i + 1) % 3)], currentVertCount);
		unsigned mid1 = CreateVertexForEdge(lookup, verts, uvs, index[i + 1], index[i + ((i + 2) % 3)], currentVertCount);
		unsigned mid2 = CreateVertexForEdge(lookup, verts, uvs, index[i + 2], index[i + ((i + 3) % 3)], currentVertCount);
		next[nextCount++] = index[i];     next[nextCount++] = mid0; next[nextCount++] = mid2;
		next[nextCount++] = index[i + 1]; next[nextCount++] = mid1; next[nextCount++] = mid0;
		next[nextCount++] = index[i + 2]; next[nextCount++] = mid2; next[nextCount++] = mid1;
		next[nextCount++] = mid0;         next[nextCount++] = mid1; next[nextCount++] = mid2;
	}

	// Update index with new index
	memcpy(index, next, nextCount * sizeof(unsigned));
	currentIndexCount = nextCount;

	delete[] next;
}

std::tuple<unsigned*, vector3*, vector2*> MakeIcosphere(unsigned resolution)
{
	// For every resolution, each face of the initial mesh gets split into 4 triangles.
	// Most of the vertices end up begin shared between many different triangles
	// so the number of vertices works out to a geometric sequence summing 4^R
	//
	//     •-------•               •---•---•
	//    / \     /               / \ / \ /
	//   /   \   /	  ------->   •---•---•
	//  /     \ /               / \ / \ /
	// •-------•               •---•---•

	unsigned res = (unsigned)pow(4, resolution);

	unsigned indexCount = 60 * res;
	unsigned vertCount = 22 + 60 * (1 - res) / -3;

	printf("indexCount: %d\n", indexCount);
	printf("vertCount: %d\n", vertCount);
	printf("vertCount: %d\n", vertCount);
	unsigned* indices = new unsigned[indexCount];
	vector3* verts = new vector3[vertCount];
	vector2* uvs = new vector2[vertCount];

	memcpy(indices, IcoIndex, IcoIndexCount * sizeof(unsigned));
	memcpy(verts, IcoVerts, IcoVertCount * sizeof(vector3));
	memcpy(uvs, IcoUvs, IcoVertCount * sizeof(vector2));

	// Verts & Index

	unsigned currentIndexCount = IcoIndexCount;
	unsigned currentVertCount = IcoVertCount;
	for (unsigned i = 0; i < resolution; i++) {
		SubDevideVerts(verts, uvs, indices, currentIndexCount, currentVertCount);
	}

	// At this point we have a tessellated icosahedron, but most of the points don't
	// have length = 1. Normalizing them it will push all but the initial points out
	// creating a sphere 

	for (unsigned i = 0; i < vertCount; i++) {
		verts[i].normalize();
	}

	return { indices, verts, uvs };
}

INT Width, Height;

std::tuple<unsigned*, vector3*, vector2*> icosphere;

unsigned int *indexes = nullptr;
vector3 *verts = nullptr;
vector2 *uvs = nullptr;

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	gluPerspective(45.0, Width / (double)Height, 1.0, 100.0);
	glTranslatef(0.f, 0.f, 10.f);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, indexes);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void fn_window_resize(HWND hWnd, int width, int height)
{
	if (!height)
		height = 1;

	Width = width;
	Height = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void fn_mousemove(HWND hWnd, int x, int y)
{
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
}

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		if (key == 27) //esc code
			exit(0); //close program
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	std::tie(indexes, verts, uvs) = MakeIcosphere(100);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Sample OpenGL Application", __FILE__ __TIME__,
		24,					  //Colors bits
		32,					  //Depth bits
		fn_draw,			  //Draw function
		fn_window_resize,	  //Window resize function
		fn_mousemove,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		fn_charinput,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0,					  //Window position X
		0,					  //Window position Y
		800,				  //Window width
		600);				  //Window height
	return 0;
}
