#include "CShader.h"

CShader::CShader(int shader_type, const char *name)
{

}

bool CShader::CreateFromString(int shader_type, const char *memory)
{
	uint32_t error;
	m_ShaderHandle = glCreateShader(shader_type);
	if (!m_ShaderHandle) {
		SetLastError("Error occurred in glCreateShader");
		return false;
	}
	glShaderSource(m_ShaderHandle, 1, &memory, NULL);
	glCompileShader(m_ShaderHandle);
	if(!GetShaderCompileStatus(m_ShaderHandle, m_pLastError, m_nErrorBufferSize))
		return false;

	m_Program = glCreateProgram();
	if (!m_Program) {
		SetLastError("Error occurred in glCreateProgram");
		return false;
	}
	glAttachShader(m_Program, m_ShaderHandle);
	glLinkProgram(m_Program);
	if (!GetProgramLinkStatus(m_Program, m_pLastError, m_nErrorBufferSize))
		return false;

	return true;
}

bool CShader::CreateFromFile(int shader_type, const char *filename)
{
	FILE *fp = NULL;
	if (fopen_s(&fp, filename, "rb") == EINVAL || !fp) { //TODO: fopen_s!!
		SetLastError("failed to open shader file");
		return false;
	}

	int32_t length;
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (length <= 0) {
		SetLastError("opened empty file");
		fclose(fp);
		return false;
	}

	char *p_shader_source = (char *)calloc(length + 1, sizeof(char));
	if (!p_shader_source) {
		SetLastError("failed allocate memory for shader source");
		return false;
	}
	
	if (fread(p_shader_source, 1, length, fp) != length) {
		SetLastError("failed read data from file");
		free(p_shader_source);
		fclose(fp);
		return false;
	}
	fclose(fp);

	bool status = CreateFromString(shader_type, p_shader_source);
	free(p_shader_source);
	return status;
}

bool CShader::GetShaderCompileStatus(uint32_t shader, char *buffer, int maxlen)
{
	int32_t status;
	glGetShaderiv(m_ShaderHandle, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(m_ShaderHandle, maxlen, NULL, buffer);
		return false;
	}
	return true;
}

bool CShader::GetProgramLinkStatus(uint32_t program, char *buffer, int maxlen)
{
	int32_t status;
	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(m_Program, maxlen, NULL, buffer);
		return false;
	}
	return true;
}

void CShader::SetErrorMessageBuffer(char *buffer, int maxlen)
{
	m_pLastError = buffer;
	m_nErrorBufferSize = maxlen;
}

void CShader::SetLastError(const char *string)
{
	strcpy_s(m_pLastError, m_nErrorBufferSize, string);
}
