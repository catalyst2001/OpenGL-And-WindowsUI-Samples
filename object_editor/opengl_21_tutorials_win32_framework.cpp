#include "opengl_21_tutorials_win32_framework.h"

// ----------------------------------------------------------------------------------------------------------------------------

CBuffer::CBuffer()
{
	SetDefaults();
}

CBuffer::~CBuffer()
{
	Empty();
}

void CBuffer::AddData(void *Data, int DataSize)
{
	int Remaining = BufferSize - Position;

	if(DataSize > Remaining)
	{
		BYTE *OldBuffer = Buffer;
		int OldBufferSize = BufferSize;

		int Needed = DataSize - Remaining;

		BufferSize += Needed > BUFFER_SIZE_INCREMENT ? Needed : BUFFER_SIZE_INCREMENT;

		Buffer = new BYTE[BufferSize];

		memcpy(Buffer, OldBuffer, OldBufferSize);

		delete [] OldBuffer;
	}

	memcpy(Buffer + Position, Data, DataSize);

	Position += DataSize;
}

void CBuffer::Empty()
{
	delete [] Buffer;

	SetDefaults();
}

void *CBuffer::GetData()
{
	return Buffer;
}

int CBuffer::GetDataSize()
{
	return Position;
}

void CBuffer::SetDefaults()
{
	Buffer = NULL;

	BufferSize = 0;
	Position = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------

int gl_max_texture_size = 0, gl_max_texture_max_anisotropy_ext = 0;

// ----------------------------------------------------------------------------------------------------------------------------

CTexture::CTexture()
{
	Texture = 0;
}

CTexture::~CTexture()
{
}

CTexture::operator GLuint ()
{
	return Texture;
}

bool CTexture::LoadTexture2D(char *FileName)
{
	CString DirectoryFileName = ModuleDirectory + "Textures\\" + FileName;

	int Width, Height, BPP;

	FIBITMAP *dib = GetBitmap(DirectoryFileName, Width, Height, BPP);

	if(dib == NULL)
	{
		ErrorLog.Append("Error loading texture " + DirectoryFileName + "!\r\n");
		return false;
	}

	GLenum Format = 0;

	if(BPP == 32) Format = GL_BGRA;
	if(BPP == 24) Format = GL_BGR;

	if(Format == 0)
	{
		ErrorLog.Append("Unsupported texture format (%s)!\r\n", FileName);
		FreeImage_Unload(dib);
		return false;
	}

	Destroy();

	glGenTextures(1, &Texture);

	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, Format, GL_UNSIGNED_BYTE, FreeImage_GetBits(dib));

	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(dib);

	return true;
}

bool CTexture::LoadTextureCubeMap(char **FileNames)
{
	int Width, Height, BPP;

	FIBITMAP *dib[6];

	bool Error = false;
	
	for(int i = 0; i < 6; i++)
	{
		CString DirectoryFileName = ModuleDirectory + "Textures\\" + FileNames[i];

		dib[i] = GetBitmap(DirectoryFileName, Width, Height, BPP);

		if(dib[i] == NULL)
		{
			ErrorLog.Append("Error loading texture " + DirectoryFileName + "!\r\n");
			Error = true;
		}
	}

	if(Error)
	{
		for(int i = 0; i < 6; i++)
		{
			FreeImage_Unload(dib[i]);
		}

		return false;
	}

	GLenum Format = 0;
	
	if(BPP == 32) Format = GL_BGRA;
	if(BPP == 24) Format = GL_BGR;

	if(Format == 0)
	{
		ErrorLog.Append("Unsupported texture format (%s)!\r\n", FileNames[5]);

		for(int i = 0; i < 6; i++)
		{
			FreeImage_Unload(dib[i]);
		}

		return false;
	}

	Destroy();

	glGenTextures(1, &Texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_max_texture_max_anisotropy_ext);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	for(int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, Width, Height, 0, Format, GL_UNSIGNED_BYTE, FreeImage_GetBits(dib[i]));
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	for(int i = 0; i < 6; i++)
	{
		FreeImage_Unload(dib[i]);
	}

	return true;
}

void CTexture::Destroy()
{
	glDeleteTextures(1, &Texture);
	Texture = 0;
}

FIBITMAP *CTexture::GetBitmap(char *FileName, int &Width, int &Height, int &BPP)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(FileName);

	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(FileName);
	}

	if(fif == FIF_UNKNOWN)
	{
		return NULL;
	}

	FIBITMAP *dib = NULL;

	if(FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, FileName);
	}

	if(dib != NULL)
	{
		int OriginalWidth = FreeImage_GetWidth(dib);
		int OriginalHeight = FreeImage_GetHeight(dib);

		Width = OriginalWidth;
		Height = OriginalHeight;

		if(Width == 0 || Height == 0)
		{
			FreeImage_Unload(dib);
			return NULL;
		}

		BPP = FreeImage_GetBPP(dib);

		if(Width > gl_max_texture_size) Width = gl_max_texture_size;
		if(Height > gl_max_texture_size) Height = gl_max_texture_size;

		if(!GLEW_ARB_texture_non_power_of_two)
		{
			Width = 1 << (int)floor((log((float)Width) / log(2.0f)) + 0.5f); 
			Height = 1 << (int)floor((log((float)Height) / log(2.0f)) + 0.5f);
		}

		if(Width != OriginalWidth || Height != OriginalHeight)
		{
			FIBITMAP *rdib = FreeImage_Rescale(dib, Width, Height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
	}

	return dib;
}

// ----------------------------------------------------------------------------------------------------------------------------

CShaderProgram::CShaderProgram()
{
	SetDefaults();
}

CShaderProgram::~CShaderProgram()
{
}

CShaderProgram::operator GLuint ()
{
	return Program;
}

bool CShaderProgram::Load(char *VertexShaderFileName, char *FragmentShaderFileName)
{
	bool Error = false;

	Destroy();

	Error |= ((VertexShader = LoadShader(VertexShaderFileName, GL_VERTEX_SHADER)) == 0);
	Error |= ((FragmentShader = LoadShader(FragmentShaderFileName, GL_FRAGMENT_SHADER)) == 0);

	if(Error)
	{
		Destroy();
		return false;
	}

	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);

	int LinkStatus;
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkStatus);

	if(LinkStatus == GL_FALSE)
	{
		ErrorLog.Append("Error linking program (%s, %s)!\r\n", VertexShaderFileName, FragmentShaderFileName);

		int InfoLogLength = 0;
		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
		if(InfoLogLength > 0)
		{
			char *InfoLog = new char[InfoLogLength];
			int CharsWritten  = 0;
			glGetProgramInfoLog(Program, InfoLogLength, &CharsWritten, InfoLog);
			ErrorLog.Append(InfoLog);
			delete [] InfoLog;
		}

		Destroy();

		return false;
	}

	return true;
}

void CShaderProgram::Destroy()
{
	glDetachShader(Program, VertexShader);
	glDetachShader(Program, FragmentShader);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	glDeleteProgram(Program);

	delete [] UniformLocations;
	delete [] AttribLocations;

	SetDefaults();
}

GLuint CShaderProgram::LoadShader(char *FileName, GLenum Type)
{
	CString DirectoryFileName = ModuleDirectory + "Shaders\\" + FileName;

	FILE *File;

	if(fopen_s(&File, DirectoryFileName, "rb") != 0)
	{
		ErrorLog.Append("Error loading file " + DirectoryFileName + "!\r\n");
		return 0;
	}

	fseek(File, 0, SEEK_END);
	long Size = ftell(File);
	fseek(File, 0, SEEK_SET);
	char *Source = new char[Size + 1];
	fread(Source, 1, Size, File);
	fclose(File);
	Source[Size] = 0;

	GLuint Shader = glCreateShader(Type);

	glShaderSource(Shader, 1, (const char**)&Source, NULL);
	delete [] Source;
	glCompileShader(Shader);

	int CompileStatus;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);

	if(CompileStatus == GL_FALSE)
	{
		ErrorLog.Append("Error compiling shader %s!\r\n", FileName);

		int InfoLogLength = 0;
		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
		if(InfoLogLength > 0)
		{
			char *InfoLog = new char[InfoLogLength];
			int CharsWritten  = 0;
			glGetShaderInfoLog(Shader, InfoLogLength, &CharsWritten, InfoLog);
			ErrorLog.Append(InfoLog);
			delete [] InfoLog;
		}

		glDeleteShader(Shader);

		return 0;
	}

	return Shader;
}

void CShaderProgram::SetDefaults()
{
	VertexShader = 0;
	FragmentShader = 0;

	Program = 0;

	UniformLocations = NULL;
	AttribLocations = NULL;
}

// ----------------------------------------------------------------------------------------------------------------------------

CCamera::CCamera()
{
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	
	CalculateViewMatrix();
}

CCamera::~CCamera()
{
}

void CCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position - Z * 0.05f;
	}

	CalculateViewMatrix();
}

void CCamera::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

vec3 CCamera::OnKeys(BYTE Keys, float FrameTime)
{
	float Speed = 5.0f;

	if(Keys & 0x40) Speed *= 2.0f;
	if(Keys & 0x80) Speed *= 0.5f;

	float Distance = Speed * FrameTime;

	vec3 Up(0.0f, 1.0f, 0.0f);
	vec3 Right = X;
	vec3 Forward = cross(Up, Right);

	Up *= Distance;
	Right *= Distance;
	Forward *= Distance;

	vec3 Movement;

	if(Keys & 0x01) Movement += Forward;
	if(Keys & 0x02) Movement -= Forward;
	if(Keys & 0x04) Movement -= Right;
	if(Keys & 0x08) Movement += Right;
	if(Keys & 0x10) Movement += Up;
	if(Keys & 0x20) Movement -= Up;

	return Movement;
}

void CCamera::OnMouseMove(int dx, int dy)
{
	float Sensitivity = 0.25f;

	Position -= Reference;

	if(dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if(dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if(Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	Position = Reference + Z * length(Position);

	CalculateViewMatrix();
}

void CCamera::OnMouseWheel(float zDelta)
{
	Position -= Reference;

	if(zDelta < 0 && length(Position) < 500.0f)
	{
		Position += Position * 0.1f;
	}

	if(zDelta > 0 && length(Position) > 0.05f)
	{
		Position -= Position * 0.1f;
	}

	Position += Reference;

	CalculateViewMatrix();
}

void CCamera::SetPerspective(float fovy, float aspect, float n, float f)
{
	ProjectionMatrix = perspective(fovy, aspect, n, f);
	ProjectionMatrixInverse = inverse(ProjectionMatrix);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	ViewProjectionMatrixInverse = ViewMatrixInverse * ProjectionMatrixInverse;
}

void CCamera::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	ViewProjectionMatrixInverse = ViewMatrixInverse * ProjectionMatrixInverse;
}

// ----------------------------------------------------------------------------------------------------------------------------

CIndexArray::CIndexArray()
{
	SetDefaults();
}

CIndexArray::~CIndexArray()
{
	Empty();
}

void CIndexArray::AddIndex(int Index)
{
	int *NewIndices = new int[IndicesCount + 1];

	for(int i = 0; i < IndicesCount; i++)
	{
		NewIndices[i] = Indices[i];
	}

	delete [] Indices;

	Indices = NewIndices;

	Indices[IndicesCount++] = Index;
}

void CIndexArray::Empty()
{
	delete [] Indices;

	SetDefaults();
}

int CIndexArray::GetIndexAt(int Index)
{
	return Indices[Index];
}

int CIndexArray::GetIndicesCount()
{
	return IndicesCount;
}

bool CIndexArray::IndexIn(int Index)
{
	for(int i = 0; i < IndicesCount; i++)
	{
		if(Indices[i] == Index)
		{
			return true;
		}
	}

	return false;
}

void CIndexArray::SetDefaults()
{
	Indices = NULL;
	IndicesCount = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------

CObject::CObject()
{
	SetDefaults();
}

CObject::~CObject()
{
}

bool CObject::Load(const char *FileName)
{
	CString DirectoryFileName = ModuleDirectory + "Objects\\" + FileName;

	FILE *File;

	if(fopen_s(&File, DirectoryFileName, "rb") != 0)
	{
		ErrorLog.Append("Error loading file " + DirectoryFileName + "!\r\n");
		return false;
	}

	Destroy();

	char *Source;
	long SourceLength;

	fseek(File, 0, SEEK_END);
	SourceLength = ftell(File);
	fseek(File, 0, SEEK_SET);
	Source = new char[SourceLength + 1];
	fread(Source, 1, SourceLength, File);
	Source[SourceLength] = 0;

	fclose(File);

	for(long i = 0; i < SourceLength; i++)
	{
		if(Source[i] == '\r' || Source[i] == '\n') Source[i] = 0;
	}

	float si, ss, x, y, z, r, g, b, s, t;
	int v1, v2, v3, c1, c2, c3, tc1, tc2, tc3, n1, n2, n3;

	char *Line = Source;
	char *End = Source + SourceLength;

	while(Line < End)
	{
		while(Line < End && (*Line == ' ' || *Line == '\t')) Line++;

		if(Line[0] == '#')
		{
			// skip comment line
		}
		else if(sscanf_s(Line, "v %f %f %f", &x, &y, &z) == 3)
		{
			VerticesCount++;
		}
		else if(sscanf_s(Line, "c %f %f %f", &r, &g, &b) == 3)
		{
			ColorsCount++;
		}
		else if(sscanf_s(Line, "tc %f %f", &s, &t) == 2)
		{
			TexCoordsCount++;
		}
		else if(sscanf_s(Line, "n %f %f %f", &x, &y, &z) == 3)
		{
			NormalsCount++;
		}
		else if(sscanf_s(Line, "t %d/%d/%d/%d %d/%d/%d/%d %d/%d/%d/%d", &v1, &c1, &tc1, &n1, &v2, &c2, &tc2, &n2, &v3, &c3, &tc3, &n3) == 12)
		{
			if(v1 < 1 || v1 > VerticesCount || v2 < 1 || v2 > VerticesCount || v3 < 1 || v3 > VerticesCount) { ErrorLog.Append("Invalid vertex index!"); Destroy(); return false; }
			if(c1 < 0 || c1 > ColorsCount || c2 < 0 || c2 > ColorsCount || c3 < 0 || c3 > ColorsCount) { ErrorLog.Append("Invalid color index!"); Destroy(); return false; }
			if(tc1 < 0 || tc1 > TexCoordsCount || tc2 < 0 || tc2 > TexCoordsCount || tc3 < 0 || tc3 > TexCoordsCount) { ErrorLog.Append("Invalid texture coordinate index!"); Destroy(); return false; }
			if(n1 < 0 || n1 > NormalsCount || n2 < 0 || n2 > NormalsCount || n3 < 0 || n3 > NormalsCount) { ErrorLog.Append("Invalid normal index!"); Destroy(); return false; }

			TrianglesCount++;
		}

		while(Line < End && *Line != 0) Line++;
		while(Line < End && *Line == 0) Line++;
	}

	if(VerticesCount > 0) Vertices = new vec3[VerticesCount];
	if(ColorsCount > 0) Colors = new vec3[ColorsCount];
	if(TexCoordsCount > 0) TexCoords = new vec2[TexCoordsCount];
	if(NormalsCount > 0) Normals = new vec3[NormalsCount];

	if(TrianglesCount > 0) IndexArray = new CIndexArrayElement[TrianglesCount * 3];

	VerticesCount = ColorsCount = TexCoordsCount = NormalsCount = TrianglesCount = 0;

	Line = Source;

	while(Line < End)
	{
		while(Line < End && (*Line == ' ' || *Line == '\t')) Line++;

		if(Line[0] == '#')
		{
			// skip comment line
		}
		else if(Line[0] == 'd' && Line[1] == 'm' && (Line[2] == ' ' || Line[2] == '\t'))
        {
			char *DiffuseMapFileName = Line + 2;

			while(DiffuseMapFileName < End && (*DiffuseMapFileName == ' ' || *DiffuseMapFileName == '\t')) DiffuseMapFileName++;

			char *Character = DiffuseMapFileName;

			while(Character < End && *Character != 0 && *Character != ' ' && *Character != '\t') Character++;
			while(Character < End && *Character != 0) { *Character = 0; Character++; }

			if(!DiffuseMap.LoadTexture2D(DiffuseMapFileName))
			{
				return false;
			}
		}
		else if(Line[0] == 'n' && Line[1] == 'm' && (Line[2] == ' ' || Line[2] == '\t'))
        {
			char *NormalMapFileName = Line + 2;

			while(NormalMapFileName < End && (*NormalMapFileName == ' ' || *NormalMapFileName == '\t')) NormalMapFileName++;

			char *Character = NormalMapFileName;

			while(Character < End && *Character != 0 && *Character != ' ' && *Character != '\t') Character++;
			while(Character < End && *Character != 0) { *Character = 0; Character++; }

			if(!NormalMap.LoadTexture2D(NormalMapFileName))
			{
				return false;
			}
		}
		else if(sscanf_s(Line, "si %f", &si) == 1)
		{
			SpecularIntensity = si;
		}
		else if(sscanf_s(Line, "ss %f", &ss) == 1)
		{
			SpecularShininess = ss;
		}
		else if(sscanf_s(Line, "v %f %f %f", &x, &y, &z) == 3)
		{
			Vertices[VerticesCount++] = vec3(x, y, z);
		}
		else if(sscanf_s(Line, "c %f %f %f", &r, &g, &b) == 3)
		{
			Colors[ColorsCount++] = vec3(r, g, b);
		}
		else if(sscanf_s(Line, "tc %f %f", &s, &t) == 2)
		{
			TexCoords[TexCoordsCount++] = vec2(s, t);
		}
		else if(sscanf_s(Line, "n %f %f %f", &x, &y, &z) == 3)
		{
			Normals[NormalsCount++] = normalize(vec3(x, y, z));
		}
		else if(sscanf_s(Line, "t %d/%d/%d/%d %d/%d/%d/%d %d/%d/%d/%d", &v1, &c1, &tc1, &n1, &v2, &c2, &tc2, &n2, &v3, &c3, &tc3, &n3) == 12)
		{
			int ia = TrianglesCount * 3, ib = ia + 1, ic = ib + 1;

			IndexArray[ia].v = v1 - 1; IndexArray[ib].v = v2 - 1; IndexArray[ic].v = v3 - 1;
			IndexArray[ia].c = c1 - 1; IndexArray[ib].c = c2 - 1; IndexArray[ic].c = c3 - 1;
			IndexArray[ia].tc = tc1 - 1; IndexArray[ib].tc = tc2 - 1; IndexArray[ic].tc = tc3 - 1;
			IndexArray[ia].n = n1 - 1; IndexArray[ib].n = n2 - 1; IndexArray[ic].n = n3 - 1;

			TrianglesCount++;
		}

		while(Line < End && *Line != 0) Line++;
		while(Line < End && *Line == 0) Line++;
	}

	delete [] Source;

	/*GenerateVertexArrayAndVertexBufferObject();*/

	return true;
}

void CObject::Optimize()
{
	for(int i = VerticesCount - 1; i >= 0; i--)
	{
		DeleteVertex(i);
	}

	for(int i = ColorsCount - 1; i >= 0; i--)
	{
		DeleteColor(i);
	}

	for(int i = TexCoordsCount - 1; i >= 0; i--)
	{
		DeleteTexCoord(i);
	}

	for(int i = NormalsCount - 1; i >= 0; i--)
	{
		DeleteNormal(i);
	}
}

bool CObject::Save(const char *FileName)
{
	CString DirectoryFileName = ModuleDirectory + "Objects\\" + FileName;

	FILE *File;

	if(fopen_s(&File, DirectoryFileName, "wb+") != 0)
	{
		ErrorLog.Append("Error creating file " + DirectoryFileName + "!\r\n");
		return false;
	}

	for(int i = 0; i < VerticesCount; i++)
	{
		fprintf(File, "v %f %f %f\r\n", Vertices[i].x, Vertices[i].y, Vertices[i].z);
	}

	if(VerticesCount > 0) fprintf(File, "\r\n");

	for(int i = 0; i < ColorsCount; i++)
	{
		fprintf(File, "c %f %f %f\r\n", Colors[i].r, Colors[i].g, Colors[i].b);
	}

	if(ColorsCount > 0) fprintf(File, "\r\n");

	for(int i = 0; i < TexCoordsCount; i++)
	{
		fprintf(File, "tc %f %f\r\n", TexCoords[i].s, TexCoords[i].t);
	}

	if(TexCoordsCount > 0) fprintf(File, "\r\n");

	for(int i = 0; i < NormalsCount; i++)
	{
		fprintf(File, "n %f %f %f\r\n", Normals[i].x, Normals[i].y, Normals[i].z);
	}

	if(NormalsCount > 0) fprintf(File, "\r\n");

	for(int t = 0; t < TrianglesCount; t++)
	{
		int ia = t * 3, ib = ia + 1, ic = ib + 1;

		fprintf(File, "t %d/%d/%d/%d %d/%d/%d/%d %d/%d/%d/%d\r\n", IndexArray[ia].v + 1, IndexArray[ia].c + 1, IndexArray[ia].tc + 1, IndexArray[ia].n + 1, IndexArray[ib].v + 1, IndexArray[ib].c + 1, IndexArray[ib].tc + 1, IndexArray[ib].n + 1, IndexArray[ic].v + 1, IndexArray[ic].c + 1, IndexArray[ic].tc + 1, IndexArray[ic].n + 1);
	}

	if(TrianglesCount > 0) fprintf(File, "\r\n");

	fclose(File);

	return true;
}

void CObject::RenderIndexArray()
{
	glBegin(GL_TRIANGLES);

	for(int i = 0; i < TrianglesCount; i++)
	{
		int ia = i * 3, ib = ia + 1, ic = ib + 1;

		if(IndexArray[ia].c >= 0) glColor3fv(&Colors[IndexArray[ia].c]); else glColor3f(1.0f, 1.0f, 1.0f);
		if(IndexArray[ia].tc >= 0) glTexCoord2fv(&TexCoords[IndexArray[ia].tc]); else glTexCoord2f(0.0f, 0.0f);
		if(IndexArray[ia].n >= 0) glNormal3fv(&Normals[IndexArray[ia].n]); else glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&Vertices[IndexArray[ia].v]);

		if(IndexArray[ib].c >= 0) glColor3fv(&Colors[IndexArray[ib].c]); else glColor3f(1.0f, 1.0f, 1.0f);
		if(IndexArray[ib].tc >= 0) glTexCoord2fv(&TexCoords[IndexArray[ib].tc]); else glTexCoord2f(0.0f, 0.0f);
		if(IndexArray[ib].n >= 0) glNormal3fv(&Normals[IndexArray[ib].n]); else glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&Vertices[IndexArray[ib].v]);

		if(IndexArray[ic].c >= 0) glColor3fv(&Colors[IndexArray[ic].c]); else glColor3f(1.0f, 1.0f, 1.0f);
		if(IndexArray[ic].tc >= 0) glTexCoord2fv(&TexCoords[IndexArray[ic].tc]); else glTexCoord2f(0.0f, 0.0f);
		if(IndexArray[ic].n >= 0) glNormal3fv(&Normals[IndexArray[ic].n]); else glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&Vertices[IndexArray[ic].v]);
	}

	glEnd();
}

/*void CObject::RenderVertexArray(int TangentAttribLocation, int BitangentAttribLocation)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 68, (BYTE*)VertexArray + 0);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 68, (BYTE*)VertexArray + 12);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 68, (BYTE*)VertexArray + 24);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 68, (BYTE*)VertexArray + 32);

	if(TangentAttribLocation >= 0 && BitangentAttribLocation >= 0)
	{
		glEnableVertexAttribArray(TangentAttribLocation);
		glVertexAttribPointer(TangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 68, (BYTE*)VertexArray + 44);

		glEnableVertexAttribArray(BitangentAttribLocation);
		glVertexAttribPointer(BitangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 68, (BYTE*)VertexArray + 56);
	}

	glDrawArrays(GL_TRIANGLES, 0, TrianglesCount * 3);

	if(TangentAttribLocation >= 0 && BitangentAttribLocation >= 0)
	{
		glDisableVertexAttribArray(BitangentAttribLocation);
		glDisableVertexAttribArray(TangentAttribLocation);
	}

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}*/

/*void CObject::RenderVertexBufferObject(int TangentAttribLocation, int BitangentAttribLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 68, (void*)0);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 68, (void*)12);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 68, (void*)24);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 68, (void*)32);

	if(TangentAttribLocation >= 0 && BitangentAttribLocation >= 0)
	{
		glEnableVertexAttribArray(TangentAttribLocation);
		glVertexAttribPointer(TangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 68, (void*)44);

		glEnableVertexAttribArray(BitangentAttribLocation);
		glVertexAttribPointer(BitangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 68, (void*)56);
	}

	glDrawArrays(GL_TRIANGLES, 0, TrianglesCount * 3);

	if(TangentAttribLocation >= 0 && BitangentAttribLocation >= 0)
	{
		glDisableVertexAttribArray(BitangentAttribLocation);
		glDisableVertexAttribArray(TangentAttribLocation);
	}

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}*/

void CObject::RenderVertices()
{
	glBegin(GL_POINTS);

	for(int i = 0; i < VerticesCount; i++)
	{
		glVertex3fv(&Vertices[i]);
	}

	glEnd();
}

void CObject::RenderEdges(int TriangleIndex)
{
	glBegin(GL_LINES);

	if(TriangleIndex == -1)
	{
		for(int i = 0; i < TrianglesCount; i++)
		{
			int ia = i * 3, ib = ia + 1, ic = ib + 1;

			glVertex3fv(&Vertices[IndexArray[ia].v]); glVertex3fv(&Vertices[IndexArray[ib].v]);
			glVertex3fv(&Vertices[IndexArray[ib].v]); glVertex3fv(&Vertices[IndexArray[ic].v]);
			glVertex3fv(&Vertices[IndexArray[ic].v]); glVertex3fv(&Vertices[IndexArray[ia].v]);
		}	
	}
	else
	{
		int ia = TriangleIndex * 3, ib = ia + 1, ic = ib + 1;

		glVertex3fv(&Vertices[IndexArray[ia].v]); glVertex3fv(&Vertices[IndexArray[ib].v]);
		glVertex3fv(&Vertices[IndexArray[ib].v]); glVertex3fv(&Vertices[IndexArray[ic].v]);
		glVertex3fv(&Vertices[IndexArray[ic].v]); glVertex3fv(&Vertices[IndexArray[ia].v]);
	}

	glEnd();
}

void CObject::RenderNormals()
{
	glBegin(GL_LINES);

	for(int i = 0; i < TrianglesCount; i++)
	{
		int ia = i * 3, ib = ia + 1, ic = ib + 1;

		bool HasPerVertexNormals = false;

		if(Normals[IndexArray[ia].n].x != Normals[IndexArray[ib].n].x || Normals[IndexArray[ib].n].x != Normals[IndexArray[ic].n].x)
		{
			HasPerVertexNormals = true;
		}
		else if(Normals[IndexArray[ia].n].y != Normals[IndexArray[ib].n].y || Normals[IndexArray[ib].n].y != Normals[IndexArray[ic].n].y)
		{
			HasPerVertexNormals = true;
		}
		else if(Normals[IndexArray[ia].n].z != Normals[IndexArray[ib].n].z || Normals[IndexArray[ib].n].z != Normals[IndexArray[ic].n].z)
		{
			HasPerVertexNormals = true;
		}

		vec3 VertexA = *(vec3*)&(ModelMatrix * vec4(Vertices[IndexArray[ia].v], 1.0f));
		vec3 VertexB = *(vec3*)&(ModelMatrix * vec4(Vertices[IndexArray[ib].v], 1.0f));
		vec3 VertexC = *(vec3*)&(ModelMatrix * vec4(Vertices[IndexArray[ic].v], 1.0f));

		if(HasPerVertexNormals)
		{
			glVertex3fv(&VertexA); glVertex3fv(&(VertexA + normalize(NormalMatrix * Normals[IndexArray[ia].n]) * 0.125f));
			glVertex3fv(&VertexB); glVertex3fv(&(VertexB + normalize(NormalMatrix * Normals[IndexArray[ib].n]) * 0.125f));
			glVertex3fv(&VertexC); glVertex3fv(&(VertexC + normalize(NormalMatrix * Normals[IndexArray[ic].n]) * 0.125f));
		}
		else
		{
			vec3 Middle = (VertexA + VertexB + VertexC) / 3.0f;

			glVertex3fv(&Middle); glVertex3fv(&(Middle + normalize(NormalMatrix * Normals[IndexArray[ia].n]) * 0.125f));
		}
	}

	glEnd();
}

void CObject::Destroy()
{
	DiffuseMap.Destroy();
	NormalMap.Destroy();

	delete [] Vertices;
	delete [] Colors;
	delete [] TexCoords;
	delete [] Normals;

	delete [] IndexArray;
	delete [] VertexArray;

	glDeleteBuffers(1, &VertexBufferObject);

	SetDefaults();
}

void CObject::SetModelMatrix(const mat4x4 &ModelMatrix)
{
	this->ModelMatrix = ModelMatrix;
	TangentMatrix = mat3x3(ModelMatrix);
	NormalMatrix = transpose(inverse(TangentMatrix));
}

mat4x4& CObject::GetModelMatrix()
{
	return ModelMatrix;
}

mat3x3& CObject::GetTangentMatrix()
{
	return TangentMatrix;
}

mat3x3& CObject::GetNormalMatrix()
{
	return NormalMatrix;
}

GLuint CObject::GetDiffuseMap()
{
	return DiffuseMap;
}

GLuint CObject::GetNormalMap()
{
	return NormalMap;
}

float CObject::GetSpecularIntensity()
{
	return SpecularIntensity;
}

float CObject::GetSpecularShininess()
{
	return SpecularShininess;
}

vec3 CObject::GetMiddle()
{
	vec3 Middle;

	if(VerticesCount > 0)
	{
		vec3 Min, Max;

		Min = Max = Vertices[0];

		for(int i = 1; i < VerticesCount; i++)
		{
			if(Vertices[i].x > Min.x) Min.x = Vertices[i].x;
			if(Vertices[i].y > Min.y) Min.y = Vertices[i].y;
			if(Vertices[i].z > Min.z) Min.z = Vertices[i].z;

			if(Vertices[i].x < Max.x) Max.x = Vertices[i].x;
			if(Vertices[i].y < Max.y) Max.y = Vertices[i].y;
			if(Vertices[i].z < Max.z) Max.z = Vertices[i].z;
		}

		Middle = (Min + Max) / 2.0f;
	}

	return Middle;
}

int CObject::GetVerticesCount()
{
	return VerticesCount;
}

vec3 CObject::GetVertex(int VertexIndex)
{
	return Vertices[VertexIndex];
}

int CObject::GetVertexIndex(vec3 Vertex)
{
	for(int i = 0; i < VerticesCount; i++)
	{
		if(Vertices[i].x == Vertex.x && Vertices[i].y == Vertex.y && Vertices[i].z == Vertex.z)
		{
			return i;
		}
	}

	return -1;
}

int CObject::AddVertex(vec3 Vertex)
{
	int Index = GetVertexIndex(Vertex);

	if(Index == -1)
	{
		vec3 *NewVertices = new vec3[VerticesCount + 1];

		for(int i = 0; i < VerticesCount; i++)
		{
			NewVertices[i] = Vertices[i];
		}

		delete [] Vertices;

		Vertices = NewVertices;

		Index = VerticesCount;

		Vertices[Index] = Vertex;

		VerticesCount++;
	}

	return Index;
}

bool CObject::IsVertexReferenced(int VertexIndex)
{
	for(int i = 0; i < TrianglesCount * 3; i++)
	{
		if(IndexArray[i].v == VertexIndex)
		{
			return true;
		}
	}

	return false;
}

void CObject::DeleteVertex(int VertexIndex)
{
	if(VertexIndex >= 0 && VertexIndex < VerticesCount && !IsVertexReferenced(VertexIndex))
	{
		for(int i = VertexIndex; i < VerticesCount - 1; i++)
		{
			Vertices[i] = Vertices[i + 1];
		}

		VerticesCount--;

		if(VerticesCount > 0)
		{
			vec3 *NewVertices = new vec3[VerticesCount];

			for(int i = 0; i < VerticesCount; i++)
			{
				NewVertices[i] = Vertices[i];
			}

			delete [] Vertices;

			Vertices = NewVertices;
		}
		else
		{
			delete [] Vertices;
			Vertices = NULL;
		}

		for(int i = 0; i < TrianglesCount * 3; i++)
		{
			if(IndexArray[i].v > VertexIndex)
			{
				IndexArray[i].v--;
			}
		}
	}
}

void CObject::DeleteVertex(vec3 Vertex)
{
	DeleteVertex(GetVertexIndex(Vertex));
}

int CObject::GetColorsCount()
{
	return ColorsCount;
}

vec3 CObject::GetColor(int ColorIndex)
{
	return Colors[ColorIndex];
}

int CObject::GetColorIndex(vec3 Color)
{
	for(int i = 0; i < ColorsCount; i++)
	{
		if(Colors[i].r == Color.r && Colors[i].g == Color.g && Colors[i].b == Color.b)
		{
			return i;
		}
	}

	return -1;
}

int CObject::AddColor(vec3 Color)
{
	int Index = GetColorIndex(Color);

	if(Index == -1)
	{
		vec3 *NewColors = new vec3[ColorsCount + 1];

		for(int i = 0; i < ColorsCount; i++)
		{
			NewColors[i] = Colors[i];
		}

		delete [] Colors;

		Colors = NewColors;

		Index = ColorsCount;

		Colors[Index] = Color;

		ColorsCount++;
	}

	return Index;
}

bool CObject::IsColorReferenced(int ColorIndex)
{
	for(int i = 0; i < TrianglesCount * 3; i++)
	{
		if(IndexArray[i].c == ColorIndex)
		{
			return true;
		}
	}

	return false;
}

void CObject::DeleteColor(int ColorIndex)
{
	if(ColorIndex >= 0 && ColorIndex < ColorsCount && !IsColorReferenced(ColorIndex))
	{
		for(int i = ColorIndex; i < ColorsCount - 1; i++)
		{
			Colors[i] = Colors[i + 1];
		}

		ColorsCount--;

		if(ColorsCount > 0)
		{
			vec3 *NewColors = new vec3[ColorsCount];

			for(int i = 0; i < ColorsCount; i++)
			{
				NewColors[i] = Colors[i];
			}

			delete [] Colors;

			Colors = NewColors;
		}
		else
		{
			delete [] Colors;
			Colors = NULL;
		}

		for(int i = 0; i < TrianglesCount * 3; i++)
		{
			if(IndexArray[i].c > ColorIndex)
			{
				IndexArray[i].c--;
			}
		}
	}
}

void CObject::DeleteColor(vec3 Color)
{
	DeleteColor(GetColorIndex(Color));
}

int CObject::GetTexCoordsCount()
{
	return TexCoordsCount;
}

vec2 CObject::GetTexCoord(int TexCoordIndex)
{
	return TexCoords[TexCoordIndex];
}

int CObject::GetTexCoordIndex(vec2 TexCoord)
{
	for(int i = 0; i < TexCoordsCount; i++)
	{
		if(TexCoords[i].s == TexCoord.s && TexCoords[i].t == TexCoord.t)
		{
			return i;
		}
	}

	return -1;
}

int CObject::AddTexCoord(vec2 TexCoord)
{
	int Index = GetTexCoordIndex(TexCoord);

	if(Index == -1)
	{
		vec2 *NewTexCoords = new vec2[TexCoordsCount + 1];

		for(int i = 0; i < TexCoordsCount; i++)
		{
			NewTexCoords[i] = TexCoords[i];
		}

		delete [] TexCoords;

		TexCoords = NewTexCoords;

		Index = TexCoordsCount;

		TexCoords[Index] = TexCoord;

		TexCoordsCount++;
	}

	return Index;
}

bool CObject::IsTexCoordReferenced(int TexCoordIndex)
{
	for(int i = 0; i < TrianglesCount * 3; i++)
	{
		if(IndexArray[i].tc == TexCoordIndex)
		{
			return true;
		}
	}

	return false;
}

void CObject::DeleteTexCoord(int TexCoordIndex)
{
	if(TexCoordIndex >= 0 && TexCoordIndex < TexCoordsCount && !IsTexCoordReferenced(TexCoordIndex))
	{
		for(int i = TexCoordIndex; i < TexCoordsCount - 1; i++)
		{
			TexCoords[i] = TexCoords[i + 1];
		}

		TexCoordsCount--;

		if(TexCoordsCount > 0)
		{
			vec2 *NewTexCoords = new vec2[TexCoordsCount];

			for(int i = 0; i < TexCoordsCount; i++)
			{
				NewTexCoords[i] = TexCoords[i];
			}

			delete [] TexCoords;

			TexCoords = NewTexCoords;
		}
		else
		{
			delete [] TexCoords;
			TexCoords = NULL;
		}

		for(int i = 0; i < TrianglesCount * 3; i++)
		{
			if(IndexArray[i].tc > TexCoordIndex)
			{
				IndexArray[i].tc--;
			}
		}
	}
}

void CObject::DeleteTexCoord(vec2 TexCoord)
{
	DeleteTexCoord(GetTexCoordIndex(TexCoord));
}

int CObject::GetNormalsCount()
{
	return NormalsCount;
}

vec3 CObject::GetNormal(int NormalIndex)
{
	return Normals[NormalIndex];
}

int CObject::GetNormalIndex(vec3 Normal)
{
	for(int i = 0; i < NormalsCount; i++)
	{
		if(Normals[i].x == Normal.x && Normals[i].y == Normal.y && Normals[i].z == Normal.z)
		{
			return i;
		}
	}

	return -1;
}

int CObject::AddNormal(vec3 Normal)
{
	int Index = GetNormalIndex(Normal);

	if(Index == -1)
	{
		vec3 *NewNormals = new vec3[NormalsCount + 1];

		for(int i = 0; i < NormalsCount; i++)
		{
			NewNormals[i] = Normals[i];
		}

		delete [] Normals;

		Normals = NewNormals;

		Index = NormalsCount;

		Normals[Index] = Normal;

		NormalsCount++;
	}

	return Index;
}

bool CObject::IsNormalReferenced(int NormalIndex)
{
	for(int i = 0; i < TrianglesCount * 3; i++)
	{
		if(IndexArray[i].n == NormalIndex)
		{
			return true;
		}
	}

	return false;
}

void CObject::DeleteNormal(int NormalIndex)
{
	if(NormalIndex >= 0 && NormalIndex < NormalsCount && !IsNormalReferenced(NormalIndex))
	{
		for(int i = NormalIndex; i < NormalsCount - 1; i++)
		{
			Normals[i] = Normals[i + 1];
		}

		NormalsCount--;

		if(NormalsCount > 0)
		{
			vec3 *NewNormals = new vec3[NormalsCount];

			for(int i = 0; i < NormalsCount; i++)
			{
				NewNormals[i] = Normals[i];
			}

			delete [] Normals;

			Normals = NewNormals;
		}
		else
		{
			delete [] Normals;
			Normals = NULL;
		}

		for(int i = 0; i < TrianglesCount * 3; i++)
		{
			if(IndexArray[i].n > NormalIndex)
			{
				IndexArray[i].n--;
			}
		}
	}
}

void CObject::DeleteNormal(vec3 Normal)
{
	DeleteNormal(GetNormalIndex(Normal));
}

int CObject::AddTriangle(int VertexIndexA, int VertexIndexB, int VertexIndexC, const vec3 &ViewSpaceBasisVectorZ)
{
	int Index = -1;

	for(int i = 0; i < TrianglesCount; i++)
	{
		int ia = i * 3, ib = ia + 1, ic = ib + 1;

		bool t1 = IndexArray[ia].v == VertexIndexA || IndexArray[ia].v == VertexIndexB || IndexArray[ia].v == VertexIndexC;
		bool t2 = IndexArray[ib].v == VertexIndexA || IndexArray[ib].v == VertexIndexB || IndexArray[ib].v == VertexIndexC;
		bool t3 = IndexArray[ic].v == VertexIndexA || IndexArray[ic].v == VertexIndexB || IndexArray[ic].v == VertexIndexC;

		if(t1 && t2 && t3)
		{
			Index = i;
			break;
		}
	}

	if(Index == -1)
	{
		vec3 AB = normalize(Vertices[VertexIndexB] - Vertices[VertexIndexA]);
		vec3 AC = normalize(Vertices[VertexIndexC] - Vertices[VertexIndexA]);

		vec3 Normal = normalize(cross(AB, AC));

		float ABdotAC = dot(AB, AC), NdotVSBVZ = dot(Normal, ViewSpaceBasisVectorZ);

		if(abs(ABdotAC) < 1.0f && NdotVSBVZ > 0.0f)
		{
			CIndexArrayElement *NewIndexArray = new CIndexArrayElement[(TrianglesCount + 1) * 3];

			for(int i = 0; i < TrianglesCount * 3; i++)
			{
				NewIndexArray[i] = IndexArray[i];
			}

			delete [] IndexArray;

			IndexArray = NewIndexArray;

			int ia = TrianglesCount * 3, ib = ia + 1, ic = ib + 1;

			vec3 Normal = normalize(cross(Vertices[VertexIndexB] - Vertices[VertexIndexA], Vertices[VertexIndexC] - Vertices[VertexIndexA]));

			int NormalIndex = AddNormal(Normal);

			IndexArray[ia].v = VertexIndexA;
			IndexArray[ia].c = -1;
			IndexArray[ia].tc = -1;
			IndexArray[ia].n = NormalIndex;

			IndexArray[ib].v = VertexIndexB;
			IndexArray[ib].c = -1;
			IndexArray[ib].tc = -1;
			IndexArray[ib].n = NormalIndex;

			IndexArray[ic].v = VertexIndexC;
			IndexArray[ic].c = -1;
			IndexArray[ic].tc = -1;
			IndexArray[ic].n = NormalIndex;

			Index = TrianglesCount;

			TrianglesCount++;
		}
	}

	return Index;
}

void CObject::DeleteTriangle(int TriangleIndex)
{
	if(TriangleIndex >= 0 && TriangleIndex < TrianglesCount)
	{
		for(int i = TriangleIndex * 3; i < (TrianglesCount - 1) * 3; i++)
		{
			IndexArray[i] = IndexArray[i + 3];
		}

		TrianglesCount--;

		if(TrianglesCount > 0)
		{
			CIndexArrayElement *NewIndexArray = new CIndexArrayElement[TrianglesCount * 3];

			for(int i = 0; i < TrianglesCount * 3; i++)
			{
				NewIndexArray[i] = IndexArray[i];
			}

			delete [] IndexArray;

			IndexArray = NewIndexArray;
		}
		else
		{
			delete [] IndexArray;
			IndexArray = NULL;
		}
	}
}

bool CObject::RayVerticesIntersectionTest(const vec3 &Origin, const vec3 &Ray, float MaxDistance, const vec3 &Normal, int &VertexIndex)
{
	bool VertexFound = false;

	float NdotR = -dot(Normal, Ray);

	float MinDistance = MaxDistance;

	for(int i = 0; i < VerticesCount; i++)
	{
		float D = -dot(Normal, Vertices[i]);

		float Distance = (dot(Normal, Origin) + D) / NdotR;

		if(Distance > 0.0f)
		{
			vec3 Point = Origin + Ray * Distance;

			Distance = length(Point - Vertices[i]);

			if(Distance < 0.03125f && Distance < MinDistance)
			{
				VertexFound = true;

				VertexIndex = i;

				MinDistance = Distance;
			}
		}
	}

	return VertexFound;
}

bool CObject::RayTrianglesIntersectionTest(const vec3 &Origin, const vec3 &Ray, float MaxDistance, int &TriangleIndex)
{
	bool TriangleFound = false;

	float MinDistance = MaxDistance;

	for(int i = 0; i < TrianglesCount; i++)
	{
		int ia = i * 3, ib = ia + 1, ic = ib + 1;

		vec3 Normal = normalize(cross(Vertices[IndexArray[ib].v] - Vertices[IndexArray[ia].v], Vertices[IndexArray[ic].v] - Vertices[IndexArray[ia].v]));
		float D = -dot(Normal, Vertices[IndexArray[ia].v]);

		float NdotR = -dot(Normal, Ray);

		if(NdotR > 0.0f)
		{
			float Distance = (dot(Normal, Origin) + D) / NdotR;

			if(Distance > 0.0f && Distance < MinDistance)
			{
				vec3 Point = Origin + Ray * Distance;

				vec3 N1 = normalize(cross(Normal, Vertices[IndexArray[ib].v] - Vertices[IndexArray[ia].v]));
				float D1 = -dot(N1, Vertices[IndexArray[ia].v]);

				if(dot(N1, Point) + D1 >= 0.0f)
				{
					vec3 N2 = normalize(cross(Normal, Vertices[IndexArray[ic].v] - Vertices[IndexArray[ib].v]));
					float D2 = -dot(N2, Vertices[IndexArray[ib].v]);

					if(dot(N2, Point) + D2 >= 0.0f)
					{
						vec3 N3 = normalize(cross(Normal, Vertices[IndexArray[ia].v] - Vertices[IndexArray[ic].v]));
						float D3 = -dot(N3, Vertices[IndexArray[ic].v]);

						if(dot(N3, Point) + D3 >= 0.0f)
						{
							TriangleFound = true;

							TriangleIndex = i;

							MinDistance = Distance;
						}
					}
				}
			}
		}
	}

	return TriangleFound;
}

/*void CObject::GenerateVertexArrayAndVertexBufferObject()
{
	VertexArray = new CVertexArrayElement[TrianglesCount * 3];

	for(int i = 0; i < TrianglesCount; i++)
	{
		int ia = i * 3, ib = ia + 1, ic = ib + 1;

		VertexArray[ia].Vertex = Vertices[IndexArray[ia].v];
		if(IndexArray[ia].c >= 0) VertexArray[ia].Color = Colors[IndexArray[ia].c]; else VertexArray[ia].Color = vec3(1.0f, 1.0f, 1.0f);
		if(IndexArray[ia].tc >= 0) VertexArray[ia].TexCoord = TexCoords[IndexArray[ia].tc];
		if(IndexArray[ia].n >=0) VertexArray[ia].Normal = Normals[IndexArray[ia].n];

		VertexArray[ib].Vertex = Vertices[IndexArray[ib].v];
		if(IndexArray[ib].c >= 0) VertexArray[ib].Color = Colors[IndexArray[ib].c]; else VertexArray[ib].Color = vec3(1.0f, 1.0f, 1.0f);
		if(IndexArray[ib].tc >= 0) VertexArray[ib].TexCoord = TexCoords[IndexArray[ib].tc];
		if(IndexArray[ib].n >= 0) VertexArray[ib].Normal = Normals[IndexArray[ib].n];

		VertexArray[ic].Vertex = Vertices[IndexArray[ic].v];
		if(IndexArray[ic].c >= 0) VertexArray[ic].Color = Colors[IndexArray[ic].c]; else VertexArray[ic].Color = vec3(1.0f, 1.0f, 1.0f);
		if(IndexArray[ic].tc >= 0) VertexArray[ic].TexCoord = TexCoords[IndexArray[ic].tc];
		if(IndexArray[ic].n >= 0) VertexArray[ic].Normal = Normals[IndexArray[ic].n];

		vec3 vdab = VertexArray[ib].Vertex - VertexArray[ia].Vertex;
		vec3 vdac = VertexArray[ic].Vertex - VertexArray[ia].Vertex;

		vec2 tcdab = VertexArray[ib].TexCoord - VertexArray[ia].TexCoord;
		vec2 tcdac = VertexArray[ic].TexCoord - VertexArray[ia].TexCoord;

		float r = 1.0f / (tcdab.x * tcdac.y - tcdab.y * tcdac.x);

		vec3 Normal = normalize(cross(vdab, vdac));
		vec3 Tangent = normalize((vdab * tcdac.y  - vdac * tcdab.y) * r);
		vec3 Bitangent = normalize((vdac * tcdab.x  - vdab * tcdac.x) * r);

		if(VertexArray[ia].Normal.x == 0.0 && VertexArray[ia].Normal.y == 0.0 && VertexArray[ia].Normal.z == 0.0)
		{
			VertexArray[ia].Normal = Normal;
			VertexArray[ia].Tangent = Tangent;
			VertexArray[ia].Bitangent = Bitangent;
		}
		else
		{
			VertexArray[ia].Normal = normalize(VertexArray[ia].Normal);
			VertexArray[ia].Tangent = normalize(Tangent - VertexArray[ia].Normal * dot(VertexArray[ia].Normal, Tangent));
			VertexArray[ia].Bitangent = cross(VertexArray[ia].Normal, VertexArray[ia].Tangent);
	    }

		if(VertexArray[ib].Normal.x == 0.0 && VertexArray[ib].Normal.y == 0.0 && VertexArray[ib].Normal.z == 0.0)
		{
			VertexArray[ib].Normal = Normal;
			VertexArray[ib].Tangent = Tangent;
			VertexArray[ib].Bitangent = Bitangent;
		}
		else
		{
			VertexArray[ib].Normal = normalize(VertexArray[ib].Normal);
			VertexArray[ib].Tangent = normalize(Tangent - VertexArray[ib].Normal * dot(VertexArray[ib].Normal, Tangent));
			VertexArray[ib].Bitangent = cross(VertexArray[ib].Normal, VertexArray[ib].Tangent);
        }

		if(VertexArray[ic].Normal.x == 0.0 && VertexArray[ic].Normal.y == 0.0 && VertexArray[ic].Normal.z == 0.0)
		{
			VertexArray[ic].Normal = Normal;
			VertexArray[ic].Tangent = Tangent;
			VertexArray[ic].Bitangent = Bitangent;
		}
		else
		{
			VertexArray[ic].Normal = normalize(VertexArray[ic].Normal);
			VertexArray[ic].Tangent = normalize(Tangent - VertexArray[ic].Normal * dot(VertexArray[ic].Normal, Tangent));
			VertexArray[ic].Bitangent = cross(VertexArray[ic].Normal, VertexArray[ia].Tangent);
	    }
	}

	glGenBuffers(1, &VertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, TrianglesCount * 3 * 68, VertexArray, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}*/

void CObject::SetDefaults()
{
	ModelMatrix = mat4x4();
	TangentMatrix = mat3x3();
	NormalMatrix = mat3x3();

	VerticesCount = ColorsCount = TexCoordsCount = NormalsCount = TrianglesCount = 0;

	SpecularIntensity = SpecularShininess = 0.0f;

	Vertices = NULL;
	Colors = NULL;
	TexCoords = NULL;
	Normals = NULL;

	IndexArray = NULL;

	VertexArray = NULL;
	VertexBufferObject = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------

bool ObjectFileExists(int ObjectId)
{
	CString FileName;

	FileName.Set("object%d.objs", ObjectId);

	CString DirectoryFileName = ModuleDirectory + "Objects\\" + FileName;

	FILE *File;

	if(fopen_s(&File, DirectoryFileName, "rb") == 0)
	{
		fclose(File);

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------------------------------------------------------

COpenGLRenderer::COpenGLRenderer()
{
	EditMode = 1;

	IntersectionPlaneNormal = vec3(0.0f, 1.0f, 0.0f);
	IntersectionPlaneD = 0.0f;
	IntersectionPointRoundFactor = 0.125f;
	IntersectionPointFound = VertexFound = TriangleFound = false;

	RenderVertices = true;
	RenderEdges = true;
	RenderNormals = false;
	ShowAxisGrid = true;
}

COpenGLRenderer::~COpenGLRenderer()
{
}

bool COpenGLRenderer::Init()
{
	bool Error = false;

	Error |= !Lighting.Load("lighting.vs", "lighting.fs");

	Lighting.UniformLocations = new GLuint[1];
	Lighting.UniformLocations[0] = glGetUniformLocation(Lighting, "CameraPosition");

	if(Error)
	{
		return false;
	}

	ObjectId = 1;

	ObjectFileName.Set("object%d.objs", ObjectId);

	Object.Load(ObjectFileName);

	Text = ObjectFileName;

	Camera.Look(vec3(3.0f, 3.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), true);

	return true;
}

void COpenGLRenderer::Render(float FrameTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&Camera.ViewMatrix);

	float d = 5.0f;

	if(ShowAxisGrid)
	{
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f,1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f,1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f,0.1f,1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f,0.1f,1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f,-0.1f,1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);

		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_LINES);

		if(IntersectionPlaneNormal.x == 1.0f)
		{
			for(float i = -d; i <= d; i += 1.0f)
			{
				glVertex3f(-IntersectionPlaneD,  i, -d);
				glVertex3f(-IntersectionPlaneD,  i,  d);
				glVertex3f(-IntersectionPlaneD, -d,  i);
				glVertex3f(-IntersectionPlaneD,  d,  i);
			}
		}

		if(IntersectionPlaneNormal.y == 1.0f)
		{
			for(float i = -d; i <= d; i += 1.0f)
			{
				glVertex3f( i, -IntersectionPlaneD, -d);
				glVertex3f( i, -IntersectionPlaneD,  d);
				glVertex3f(-d, -IntersectionPlaneD,  i);
				glVertex3f( d, -IntersectionPlaneD,  i);
			}
		}

		if(IntersectionPlaneNormal.z == 1.0f)
		{
			for(float i = -d; i <= d; i += 1.0f)
			{
				glVertex3f( i, -d, -IntersectionPlaneD);
				glVertex3f( i,  d, -IntersectionPlaneD);
				glVertex3f(-d,  i, -IntersectionPlaneD);
				glVertex3f( d,  i, -IntersectionPlaneD);
			}
		}

		glEnd();
	}

	glUseProgram(Lighting);

	glUniform3fv(Lighting.UniformLocations[0], 1, &Camera.Position);

	Object.RenderIndexArray();

	glUseProgram(0);

	if(RenderVertices)
	{
		glPointSize(3.0f);
		glColor3f(0.0f, 0.5f, 1.0f);
		Object.RenderVertices();
		glPointSize(1.0f);
	}

	if(RenderEdges)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		Object.RenderEdges();
	}

	if(RenderNormals)
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		Object.RenderNormals();
	}

	glDisable(GL_CULL_FACE);

	if(EditMode == 1 && IntersectionPointFound)
	{
		RenderSelectionCross(IntersectionPoint);
	}

	if(EditMode == 2 && VertexFound)
	{
		RenderSelectionBox(Object.GetVertex(Index));
	}

	if(EditMode == 2 && IndexArray.GetIndicesCount() > 0)
	{
		for(int i = 0; i < IndexArray.GetIndicesCount(); i++)
		{
			RenderSelectionBox(Object.GetVertex(IndexArray.GetIndexAt(i)));
		}
	}

	if(EditMode == 3 && TriangleFound)
	{
		glLineWidth(3.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		Object.RenderEdges(Index);
		glLineWidth(1.0f);
	}

	if(ShowAxisGrid)
	{
		glColor4f(0.0, 1.0f, 0.0f, 0.25f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

		if(IntersectionPlaneNormal.x == 1.0f)
		{
			glVertex3f(-IntersectionPlaneD, -d,  d);
			glVertex3f(-IntersectionPlaneD, -d, -d);
			glVertex3f(-IntersectionPlaneD,  d, -d);
			glVertex3f(-IntersectionPlaneD,  d,  d);
		}

		if(IntersectionPlaneNormal.y == 1.0f)
		{
			glVertex3f(-d, -IntersectionPlaneD,  d);
			glVertex3f( d, -IntersectionPlaneD,  d);
			glVertex3f( d, -IntersectionPlaneD, -d);
			glVertex3f(-d, -IntersectionPlaneD, -d);
		}

		if(IntersectionPlaneNormal.z == 1.0f)
		{
			glVertex3f(-d, -d, -IntersectionPlaneD);
			glVertex3f( d, -d, -IntersectionPlaneD);
			glVertex3f( d,  d, -IntersectionPlaneD);
			glVertex3f(-d,  d, -IntersectionPlaneD);
		}

		glEnd();

		glDisable(GL_BLEND);
	}

	glDisable(GL_DEPTH_TEST);
}

void COpenGLRenderer::Resize(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;

	glViewport(0, 0, Width, Height);

	Camera.SetPerspective(45.0f, (float)Width / (float)Height, 0.125f, 512.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&Camera.ProjectionMatrix);
}

void COpenGLRenderer::Destroy()
{
	Lighting.Destroy();

	Object.Destroy();
}

void COpenGLRenderer::CheckCameraKeys(float FrameTime)
{
	BYTE Keys = 0x00;

	if(GetKeyState('W') & 0x80) Keys |= 0x01;
	if(GetKeyState('S') & 0x80) Keys |= 0x02;
	if(GetKeyState('A') & 0x80) Keys |= 0x04;
	if(GetKeyState('D') & 0x80) Keys |= 0x08;
	if(GetKeyState('R') & 0x80) Keys |= 0x10;
	if(GetKeyState('F') & 0x80) Keys |= 0x20;

	if(GetKeyState(VK_SHIFT) & 0x80) Keys |= 0x40;
	if(GetKeyState(VK_CONTROL) & 0x80) Keys |= 0x80;

	if(Keys & 0x3F)
	{
		Camera.Move(Camera.OnKeys(Keys, FrameTime));

		FindPrimitive();
	}
}

void COpenGLRenderer::RenderSelectionCross(vec3 Vertex)
{
	float d = IntersectionPointRoundFactor;

	glLineWidth(3.0f);

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(Vertex.x - d, Vertex.y, Vertex.z);
	glVertex3f(Vertex.x + d, Vertex.y, Vertex.z);

	glVertex3f(Vertex.x, Vertex.y - d, Vertex.z);
	glVertex3f(Vertex.x, Vertex.y + d, Vertex.z);

	glVertex3f(Vertex.x, Vertex.y, Vertex.z - d);
	glVertex3f(Vertex.x, Vertex.y, Vertex.z + d);

	glEnd();

	glLineWidth(1.0f);
}

void COpenGLRenderer::RenderSelectionBox(vec3 Vertex)
{
	float d = 0.03125f;

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z + d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z + d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z - d);

	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z + d);
	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z + d);

	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x - d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y - d, Vertex.z + d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z - d);
	glVertex3f(Vertex.x + d, Vertex.y + d, Vertex.z + d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z - d);
	glVertex3f(Vertex.x - d, Vertex.y + d, Vertex.z + d);

	glEnd();
}

int round(float number)
{
	return number >= 0.0f ? (int)(number + 0.5f) : (int)(number - 0.5f);
}

void COpenGLRenderer::FindPrimitive()
{
	vec4 Point = vec4(((float)LastX / Width) * 2.0f - 1.0f, ((float)(Height - 1 - LastY) / Height) * 2.0f - 1.0f, 0.0f, 1.0f);

	Point = Camera.ViewProjectionMatrixInverse * Point;
	Point /= Point.w;

	vec3 Ray = normalize(*(vec3*)&Point - Camera.Position);

	if(EditMode == 1)
	{
		IntersectionPointFound = false;

		float NdotR = -dot(IntersectionPlaneNormal, Ray);

		if(NdotR != 0.0f)
		{
			float Distance = (dot(IntersectionPlaneNormal, Camera.Position) + IntersectionPlaneD) / NdotR;

			if(Distance > 0.125f)
			{
				IntersectionPoint = Camera.Position + Ray * Distance;

				IntersectionPoint.x = round(IntersectionPoint.x / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;
				IntersectionPoint.y = round(IntersectionPoint.y / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;
				IntersectionPoint.z = round(IntersectionPoint.z / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;

				IntersectionPointFound = true;
			}
		}
	}

	if(EditMode == 2)
	{
		VertexFound = Object.RayVerticesIntersectionTest(Camera.Position, Ray, 1048576.0f, Camera.Z, Index);
	}

	if(EditMode == 3)
	{
		TriangleFound = Object.RayTrianglesIntersectionTest(Camera.Position, Ray, 1048576.0f, Index);
	}
}

void COpenGLRenderer::OnKeyDown(UINT Key)
{
	switch(Key)
	{
		case VK_F1:
			IntersectionPlaneNormal = vec3(1.0f, 0.0f, 0.0f);
			IntersectionPlaneD = 0.0f;
			break;

		case VK_F2:
			IntersectionPlaneNormal = vec3(0.0f, 1.0f, 0.0f);
			IntersectionPlaneD = 0.0f;
			break;

		case VK_F3:
			IntersectionPlaneNormal = vec3(0.0f, 0.0f, 1.0f);
			IntersectionPlaneD = 0.0f;
			break;

		case VK_F4:
			ShowAxisGrid = !ShowAxisGrid;
			break;

		case VK_F5:
			EditMode = 1;
			break;

		case VK_F6:
			EditMode = 2;
			break;

		case VK_F7:
			EditMode = 3;
			break;

		case '1':
			IntersectionPointRoundFactor = 1.0f;
			break;

		case '2':
			IntersectionPointRoundFactor = 0.5f;
			break;

		case '3':
			IntersectionPointRoundFactor = 0.25f;
			break;

		case '4':
			IntersectionPointRoundFactor = 0.125f;
			break;

		case '5':
			RenderVertices = !RenderVertices;
			break;

		case '6':
			RenderEdges = !RenderEdges;
			break;

		case '7':
			RenderNormals = !RenderNormals;
			break;

		case 'T':
			IntersectionPlaneD -= IntersectionPointRoundFactor;
			break;

		case 'G':
			IntersectionPlaneD += IntersectionPointRoundFactor;
			break;

		case 'C':
			Camera.Move(Object.GetMiddle() - Camera.Reference);
			break;

		case 'N':
			Object.Destroy();
			ObjectId = 1;
			while(ObjectFileExists(ObjectId)) ObjectId++;
			ObjectFileName.Set("object%d.objs", ObjectId);
			Text = ObjectFileName;
			break;

		case 'L':
			Object.Load(ObjectFileName);
			break;

		case 'O':
			Object.Optimize();
			break;

		case 'V':
			Object.Save(ObjectFileName);
			break;

		case VK_PRIOR:
			if(ObjectFileExists(ObjectId - 1))
			{
				Object.Destroy();
				ObjectId--;
				ObjectFileName.Set("object%d.objs", ObjectId);
				Object.Load(ObjectFileName);
				Text = ObjectFileName;
			}
			break;

		case VK_NEXT:
			if(ObjectFileExists(ObjectId + 1))
			{
				Object.Destroy();
				ObjectId++;
				ObjectFileName.Set("object%d.objs", ObjectId);
				Object.Load(ObjectFileName);
				Text = ObjectFileName;
			}
			break;
	}

	FindPrimitive();
}

void COpenGLRenderer::OnLButtonDown(int X, int Y)
{
	LastClickedX = X;
	LastClickedY = Y;
}

void COpenGLRenderer::OnLButtonUp(int X, int Y)
{
	if(X == LastClickedX && Y == LastClickedY)
	{
		if(EditMode == 1 && IntersectionPointFound)
		{
			Object.AddVertex(IntersectionPoint);
		}

		if(EditMode == 2)
		{
			if(VertexFound)
			{
				if(!IndexArray.IndexIn(Index))
				{
					IndexArray.AddIndex(Index);
				}
	
				if(IndexArray.GetIndicesCount() == 3)
				{
					Object.AddTriangle(IndexArray.GetIndexAt(0), IndexArray.GetIndexAt(1), IndexArray.GetIndexAt(2), Camera.Z);

					IndexArray.Empty();
				}
			}
			else
			{
				IndexArray.Empty();
			}
		}
	}
}

void COpenGLRenderer::OnMouseMove(int X, int Y)
{
	if(GetKeyState(VK_RBUTTON) & 0x80)
	{
		Camera.OnMouseMove(LastX - X, LastY - Y);
	}

	LastX = X;
	LastY = Y;

	FindPrimitive();
}

void COpenGLRenderer::OnMouseWheel(short zDelta)
{
	Camera.OnMouseWheel(zDelta);

	FindPrimitive();
}

void COpenGLRenderer::OnRButtonDown(int X, int Y)
{
	LastClickedX = X;
	LastClickedY = Y;
}

void COpenGLRenderer::OnRButtonUp(int X, int Y)
{
	if(X == LastClickedX && Y == LastClickedY)
	{
		if(EditMode == 1 && IntersectionPointFound)
		{
			Object.DeleteVertex(IntersectionPoint);
		}

		if(EditMode == 2)
		{
			if(VertexFound)
			{
				Object.DeleteVertex(Index);

				FindPrimitive();
			}
			else
			{
				IndexArray.Empty();
			}
		}

		if(EditMode == 3 && TriangleFound)
		{
			Object.DeleteTriangle(Index);

			FindPrimitive();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------

COpenGLRenderer OpenGLRenderer;

// ----------------------------------------------------------------------------------------------------------------------------

CString ModuleDirectory, ErrorLog;

// ----------------------------------------------------------------------------------------------------------------------------

void GetModuleDirectory()
{
	char *moduledirectory = new char[256];
	GetModuleFileName(GetModuleHandle(NULL), moduledirectory, 256);
	*(strrchr(moduledirectory, '\\') + 1) = 0;
	ModuleDirectory = moduledirectory;
	delete [] moduledirectory;
}

// ----------------------------------------------------------------------------------------------------------------------------

COpenGLView::COpenGLView()
{
}

COpenGLView::~COpenGLView()
{
}

bool COpenGLView::Init(HINSTANCE hInstance, char *Title, int Width, int Height, int Samples)
{
	this->Title = Title;
	this->Width = Width;
	this->Height = Height;

	WNDCLASSEX WndClassEx;

	memset(&WndClassEx, 0, sizeof(WNDCLASSEX));

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = WndProc;
	WndClassEx.hInstance = hInstance;
	WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.lpszClassName = "Win32OpenGLWindowClass";

	if(RegisterClassEx(&WndClassEx) == 0)
	{
		ErrorLog.Set("RegisterClassEx failed!");
		return false;
	}

	DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, Title, Style, 0, 0, Width, Height, NULL, NULL, hInstance, NULL);

	if(hWnd == NULL)
	{
		ErrorLog.Set("CreateWindowEx failed!");
		return false;
	}

	HDC hDC = GetDC(hWnd);

	if(hDC == NULL)
	{
		ErrorLog.Set("GetDC failed!");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int PixelFormat = ChoosePixelFormat(hDC, &pfd);

	if(PixelFormat == 0)
	{
		ErrorLog.Set("ChoosePixelFormat failed!");
		return false;
	}

	static int MSAAPixelFormat = 0;

	if(SetPixelFormat(hDC, MSAAPixelFormat == 0 ? PixelFormat : MSAAPixelFormat, &pfd) == FALSE)
	{
		ErrorLog.Set("SetPixelFormat failed!");
		return false;
	}

	hGLRC = wglCreateContext(hDC);

	if(hGLRC == NULL)
	{
		ErrorLog.Set("wglCreateContext failed!");
		return false;
	}

	if(wglMakeCurrent(hDC, hGLRC) == FALSE)
	{
		ErrorLog.Set("wglMakeCurrent failed!");
		return false;
	}

	if(glewInit() != GLEW_OK)
	{
		ErrorLog.Set("glewInit failed!");
		return false;
	}

	if(!GLEW_VERSION_2_1)
	{
		ErrorLog.Set("OpenGL 2.1 not supported!");
		return false;
	}

	if(MSAAPixelFormat == 0 && Samples > 0)
	{
		if(GLEW_ARB_multisample && WGLEW_ARB_pixel_format)
		{
			while(Samples > 0)
			{
				UINT NumFormats = 0;

				int PFAttribs[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
					WGL_COLOR_BITS_ARB, 32,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB, Samples,
					0
				};

				if(wglChoosePixelFormatARB(hDC, PFAttribs, NULL, 1, &MSAAPixelFormat, &NumFormats) == TRUE && NumFormats > 0) break;

				Samples--;
			}

			wglDeleteContext(hGLRC);
			DestroyWindow(hWnd);
			UnregisterClass(WndClassEx.lpszClassName, hInstance);

			return Init(hInstance, Title, Width, Height, Samples);
		}
		else
		{
			Samples = 0;
		}
	}

	this->Samples = Samples;

	GetModuleDirectory();

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_max_texture_size);

	if(GLEW_EXT_texture_filter_anisotropic)
	{
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_max_texture_max_anisotropy_ext);
	}

	if(WGLEW_EXT_swap_control)
	{
		wglSwapIntervalEXT(0);
	}

	return OpenGLRenderer.Init();
}

void COpenGLView::Show(bool Maximized)
{
	RECT dRect, wRect, cRect;

	GetWindowRect(GetDesktopWindow(), &dRect);
	GetWindowRect(hWnd, &wRect);
	GetClientRect(hWnd, &cRect);

	wRect.right += Width - cRect.right;
	wRect.bottom += Height - cRect.bottom;
	wRect.right -= wRect.left;
	wRect.bottom -= wRect.top;
	wRect.left = dRect.right / 2 - wRect.right / 2;
	wRect.top = dRect.bottom / 2 - wRect.bottom / 2;

	MoveWindow(hWnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);

	ShowWindow(hWnd, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}

void COpenGLView::MessageLoop()
{
	MSG Msg;

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void COpenGLView::Destroy()
{
	if(GLEW_VERSION_2_1)
	{
		OpenGLRenderer.Destroy();
	}

	wglDeleteContext(hGLRC);
	DestroyWindow(hWnd);
}

void COpenGLView::OnKeyDown(UINT Key)
{
	OpenGLRenderer.OnKeyDown(Key);
}

void COpenGLView::OnLButtonDown(int X, int Y)
{
	OpenGLRenderer.OnLButtonDown(X, Y);
}

void COpenGLView::OnLButtonUp(int X, int Y)
{
	OpenGLRenderer.OnLButtonUp(X, Y);
}

void COpenGLView::OnMouseMove(int X, int Y)
{
	OpenGLRenderer.OnMouseMove(X, Y);
}

void COpenGLView::OnMouseWheel(short zDelta)
{
	OpenGLRenderer.OnMouseWheel(zDelta);
}

void COpenGLView::OnPaint()
{
	static DWORD LastFPSTime = GetTickCount(), LastFrameTime = LastFPSTime, FPS = 0;

	PAINTSTRUCT ps;

	HDC hDC = BeginPaint(hWnd, &ps);

	DWORD Time = GetTickCount();

	float FrameTime = (Time - LastFrameTime) * 0.001f;

	LastFrameTime = Time;

	if(Time - LastFPSTime > 1000)
	{
		CString Text = Title;

		if(OpenGLRenderer.Text[0] != 0)
		{
			Text.Append(" - " + OpenGLRenderer.Text);
		}

		Text.Append(" - %dx%d", Width, Height);
		Text.Append(", ATF %dx", gl_max_texture_max_anisotropy_ext);
		Text.Append(", MSAA %dx", Samples);
		Text.Append(", FPS: %d", FPS);
		Text.Append(" - %s", glGetString(GL_RENDERER));
		
		SetWindowText(hWnd, Text);

		LastFPSTime = Time;
		FPS = 0;
	}
	else
	{
		FPS++;
	}

	OpenGLRenderer.CheckCameraKeys(FrameTime);

	OpenGLRenderer.Render(FrameTime);

	SwapBuffers(hDC);

	EndPaint(hWnd, &ps);

	InvalidateRect(hWnd, NULL, FALSE);
}

void COpenGLView::OnRButtonDown(int X, int Y)
{
	OpenGLRenderer.OnRButtonDown(X, Y);
}

void COpenGLView::OnRButtonUp(int X, int Y)
{
	OpenGLRenderer.OnRButtonUp(X, Y);
}

void COpenGLView::OnSize(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;

	OpenGLRenderer.Resize(Width, Height);
}

// ----------------------------------------------------------------------------------------------------------------------------

COpenGLView OpenGLView;

// ----------------------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			OpenGLView.OnKeyDown((UINT)wParam);
			break;

		case WM_LBUTTONDOWN:
			OpenGLView.OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			OpenGLView.OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSEMOVE:
			OpenGLView.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			break;

		case 0x020A: // WM_MOUSWHEEL
			OpenGLView.OnMouseWheel(HIWORD(wParam));
			break;

		case WM_PAINT:
			OpenGLView.OnPaint();
			break;

		case WM_RBUTTONDOWN:
			OpenGLView.OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			OpenGLView.OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_SIZE:
			OpenGLView.OnSize(LOWORD(lParam), HIWORD(lParam));
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

// ----------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	char *AppName = "Object editor";

	if(OpenGLView.Init(hInstance, AppName, 800, 600, 4))
	{
		OpenGLView.Show();
		OpenGLView.MessageLoop();
	}
	else
	{
		MessageBox(NULL, ErrorLog, AppName, MB_OK | MB_ICONERROR);
	}

	OpenGLView.Destroy();

	return 0;
}
