#pragma once
//#if !defined(__gl_h_) || !defined(__GL_H__)
//#include <gl/GL.h>
//#endif

class CGLLoader
{
public:
	CGLLoader() {}
	~CGLLoader() {}

	void Init()
	{
	}

	int m_nExtensions;
	char **m_ppExtensionsNames;
};

