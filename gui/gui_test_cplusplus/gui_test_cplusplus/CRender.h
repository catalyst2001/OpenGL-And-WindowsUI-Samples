#pragma once
#include <Windows.h>
#include <gl/GL.h>

class CGLArrays
{
public:
	CGLArrays() : m_bVertexArrayState(false), m_bTextureCoordsArrayState(false), m_bNormalsArrayState(false) {}
	CGLArrays(bool bVertex, bool bTextureCoords, bool bNormals) {
		m_bVertexArrayState = bVertex;
		m_bTextureCoordsArrayState = bTextureCoords;
		m_bNormalsArrayState = bNormals;
	}
	~CGLArrays() {}

	///////// VERTEX ARRAY
	void EnableVertexArray() {
		glEnableClientState(GL_VERTEX_ARRAY);
		m_bVertexArrayState = true;
	}

	void DisableVertexArrays() {
		glDisableClientState(GL_VERTEX_ARRAY);
		m_bVertexArrayState = false;
	}

	inline bool IsEnabledVertexArray() { return m_bVertexArrayState; }

	///////// TEX COORDS ARRAY
	void EnableTextureCoordsArray() {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		m_bTextureCoordsArrayState = true;
	}

	void DisableTextureCoordsArray() {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		m_bTextureCoordsArrayState = false;
	}

	inline bool IsEnabledTextureCoordsArray() { return m_bTextureCoordsArrayState; }

	///////// NORMALS ARRAY
	void EnableNormalsArray() {
		glEnableClientState(GL_NORMAL_ARRAY);
		m_bNormalsArrayState = true;
	}

	void DisableNormalsArray() {
		glDisableClientState(GL_NORMAL_ARRAY);
		m_bNormalsArrayState = false;
	}

	inline bool IsEnabledNormalsArray() { return m_bNormalsArrayState; }

private:
	bool m_bVertexArrayState;
	bool m_bTextureCoordsArrayState;
	bool m_bNormalsArrayState;
};

