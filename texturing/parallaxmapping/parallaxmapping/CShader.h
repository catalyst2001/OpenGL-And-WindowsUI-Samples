#pragma once
#include <stdio.h>
#include <stdint.h>
#include "glextsloader.h"

class CShader
{
public:
	CShader() : m_ShaderHandle(NULL), m_Program(NULL), m_pLastError(NULL) {}
	~CShader() {}
	CShader(int shader_type, const char *name);
	bool CreateFromString(int shader_type, const char *line);
	bool CreateFromFile(int shader_type, const char *filename);

	bool GetShaderCompileStatus(uint32_t shader, char *buffer, int maxlen);
	bool GetProgramLinkStatus(uint32_t program, char *buffer, int maxlen);

	void SetErrorMessageBuffer(char *buffer, int maxlen);
	inline void Use() { glUseProgram(m_Program); }
	inline void Unuse() { glUseProgram(NULL); }
private:
	void SetLastError(const char *string);

	int32_t m_nErrorBufferSize;
	char *m_pLastError;
	uint32_t m_ShaderHandle;
	uint32_t m_Program;
};