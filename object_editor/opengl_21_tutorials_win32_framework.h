// ----------------------------------------------------------------------------------------------------------------------------
//
// Version 2.02
//
// ----------------------------------------------------------------------------------------------------------------------------

#include <windows.h>

#include "glmath.h"
#include "string.h"

#include <gl/glew.h> // http://glew.sourceforge.net/
#include <gl/wglew.h>

#include <FreeImage.h> // http://freeimage.sourceforge.net/

// ----------------------------------------------------------------------------------------------------------------------------

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

// ----------------------------------------------------------------------------------------------------------------------------

extern CString ModuleDirectory, ErrorLog;

// ----------------------------------------------------------------------------------------------------------------------------

#define BUFFER_SIZE_INCREMENT 1048576

// ----------------------------------------------------------------------------------------------------------------------------

class CBuffer
{
private:
	BYTE *Buffer;
	int BufferSize, Position;

public:
	CBuffer();
	~CBuffer();

	void AddData(void *Data, int DataSize);
	void Empty();
	void *GetData();
	int GetDataSize();

private:
	void SetDefaults();
};

// ----------------------------------------------------------------------------------------------------------------------------

extern int gl_max_texture_size, gl_max_texture_max_anisotropy_ext;

// ----------------------------------------------------------------------------------------------------------------------------

class CTexture
{
protected:
	GLuint Texture;

public:
	CTexture();
	~CTexture();

	operator GLuint ();

	bool LoadTexture2D(char *FileName);
	bool LoadTextureCubeMap(char **FileNames);
	void Destroy();

protected:
	FIBITMAP *CTexture::GetBitmap(char *FileName, int &Width, int &Height, int &BPP);
};

// ----------------------------------------------------------------------------------------------------------------------------

class CShaderProgram
{
protected:
	GLuint VertexShader, FragmentShader, Program;

public:
	GLuint *UniformLocations, *AttribLocations;

public:
	CShaderProgram();
	~CShaderProgram();

	operator GLuint ();

	bool Load(char *VertexShaderFileName, char *FragmentShaderFileName);
	void Destroy();

protected:
	GLuint LoadShader(char *FileName, GLenum Type);
	void SetDefaults();
};

// ----------------------------------------------------------------------------------------------------------------------------

class CCamera
{
public:
	vec3 X, Y, Z, Position, Reference;

public:
	mat4x4 ViewMatrix, ViewMatrixInverse, ProjectionMatrix, ProjectionMatrixInverse, ViewProjectionMatrix, ViewProjectionMatrixInverse;

public:
	CCamera();
	~CCamera();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void Move(const vec3 &Movement);
	vec3 OnKeys(BYTE Keys, float FrameTime);
	void OnMouseMove(int dx, int dy);
	void OnMouseWheel(float zDelta);
	void SetPerspective(float fovy, float aspect, float n, float f);

private:
	void CalculateViewMatrix();
};

// ----------------------------------------------------------------------------------------------------------------------------

class CIndexArray
{
protected:
	int *Indices;
	int IndicesCount;

public:
	CIndexArray();
	~CIndexArray();

	void AddIndex(int Index);
	void Empty();
	int GetIndexAt(int Index);
	int GetIndicesCount();
	bool IndexIn(int Index);

protected:
	void SetDefaults();
};

// ----------------------------------------------------------------------------------------------------------------------------

class CIndexArrayElement
{
public:
	int v, c, tc, n;
};

// ----------------------------------------------------------------------------------------------------------------------------

class CVertexArrayElement
{
public:
	vec3 Vertex;
	vec3 Color;
	vec2 TexCoord;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
};

// ----------------------------------------------------------------------------------------------------------------------------

class CObject
{
protected:
	mat4x4 ModelMatrix;
	mat3x3 TangentMatrix, NormalMatrix;

protected:
	int VerticesCount, ColorsCount, TexCoordsCount, NormalsCount;

protected:
	int TrianglesCount;

protected:
	CTexture DiffuseMap, NormalMap;

protected:
	float SpecularIntensity, SpecularShininess;

protected:
	vec3 *Vertices;
	vec3 *Colors;
	vec2 *TexCoords;
	vec3 *Normals;

protected:
	CIndexArrayElement *IndexArray;

protected:
	CVertexArrayElement *VertexArray;
	GLuint VertexBufferObject;

public:
	CObject();
	~CObject();

	bool Load(const char *FileName);
	void Optimize();
	bool Save(const char *FileName);
	void RenderIndexArray();
	/*void RenderVertexArray(int TangentAttribLocation = -1, int BitangentAttribLocation = -1);*/
	/*void RenderVertexBufferObject(int TangentAttribLocation = -1, int BitangentAttribLocation = -1);*/
	void RenderVertices();
	void RenderEdges(int TriangleIndex = -1);
	void RenderNormals();
	void Destroy();

	void SetModelMatrix(const mat4x4 &ModelMatrix);
	mat4x4& GetModelMatrix();
	mat3x3& GetTangentMatrix();
	mat3x3& GetNormalMatrix();
	GLuint GetDiffuseMap();
	GLuint GetNormalMap();
	float GetSpecularIntensity();
	float GetSpecularShininess();

	vec3 GetMiddle();

	int GetVerticesCount();
	vec3 GetVertex(int VertexIndex);
	int GetVertexIndex(vec3 Vertex);
	int AddVertex(vec3 Vertex);
	bool IsVertexReferenced(int VertexIndex);
	void DeleteVertex(int VertexIndex);
	void DeleteVertex(vec3 Vertex);

	int GetColorsCount();
	vec3 GetColor(int ColorIndex);
	int GetColorIndex(vec3 Color);
	int AddColor(vec3 Color);
	bool IsColorReferenced(int ColorIndex);
	void DeleteColor(int ColorIndex);
	void DeleteColor(vec3 Color);

	int GetTexCoordsCount();
	vec2 GetTexCoord(int TexCoordIndex);
	int GetTexCoordIndex(vec2 TexCoord);
	int AddTexCoord(vec2 TexCoord);
	bool IsTexCoordReferenced(int TexCoordIndex);
	void DeleteTexCoord(int TexCoordIndex);
	void DeleteTexCoord(vec2 TexCoord);

	int GetNormalsCount();
	vec3 GetNormal(int NormalIndex);
	int GetNormalIndex(vec3 Normal);
	int AddNormal(vec3 Normal);
	bool IsNormalReferenced(int NormalIndex);
	void DeleteNormal(int NormalIndex);
	void DeleteNormal(vec3 Normal);

	int AddTriangle(int VertexIndexA, int VertexIndexB, int VertexIndexC, const vec3 &ViewSpaceBasisVectorZ);
	void DeleteTriangle(int TriangleIndex);

	bool RayVerticesIntersectionTest(const vec3 &Origin, const vec3 &Ray, float MaxDistance, const vec3 &Normal, int &VertexIndex);
	bool RayTrianglesIntersectionTest(const vec3 &Origin, const vec3 &Ray, float MaxDistance, int &TriangleIndex);

protected:
	/*void GenerateVertexArrayAndVertexBufferObject();*/
	void SetDefaults();
};

// ----------------------------------------------------------------------------------------------------------------------------

class COpenGLRenderer
{
protected:
	int LastX, LastY, LastClickedX, LastClickedY;

protected:
	int Width, Height;

protected:
	CCamera Camera;

protected:
	CShaderProgram Lighting;

protected:
	int ObjectId;
	CString ObjectFileName;
	CObject Object;

protected:
	int Index;
	CIndexArray IndexArray;

protected:
	int EditMode;

protected:
	vec3 IntersectionPlaneNormal, IntersectionPoint;
	float IntersectionPlaneD, IntersectionPointRoundFactor;
	bool IntersectionPointFound, VertexFound, TriangleFound;

protected:
	bool RenderVertices, RenderEdges, RenderNormals, ShowAxisGrid;

public:
	CString Text;

public:
	COpenGLRenderer();
	~COpenGLRenderer();

	bool Init();
	void Render(float FrameTime);
	void Resize(int Width, int Height);
	void Destroy();

	void CheckCameraKeys(float FrameTime);

	void OnKeyDown(UINT Key);
	void OnLButtonDown(int X, int Y);
	void OnLButtonUp(int X, int Y);
	void OnMouseMove(int X, int Y);
	void OnMouseWheel(short zDelta);
	void OnRButtonDown(int X, int Y);
	void OnRButtonUp(int X, int Y);

protected:
	void RenderSelectionCross(vec3 Vertex);
	void RenderSelectionBox(vec3 Vertex);
	void FindPrimitive();
};

// ----------------------------------------------------------------------------------------------------------------------------

class COpenGLView
{
protected:
	char *Title;
	int Width, Height, Samples;
	HWND hWnd;
	HGLRC hGLRC;

protected:
	COpenGLRenderer OpenGLRenderer;

public:
	COpenGLView();
	~COpenGLView();

	bool Init(HINSTANCE hInstance, char *Title, int Width, int Height, int Samples);
	void Show(bool Maximized = false);
	void MessageLoop();
	void Destroy();

	void OnKeyDown(UINT Key);
	void OnLButtonDown(int X, int Y);
	void OnLButtonUp(int X, int Y);
	void OnMouseMove(int X, int Y);
	void OnMouseWheel(short zDelta);
	void OnPaint();
	void OnRButtonDown(int X, int Y);
	void OnRButtonUp(int X, int Y);
	void OnSize(int Width, int Height);
};

// ----------------------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

// ----------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow);
