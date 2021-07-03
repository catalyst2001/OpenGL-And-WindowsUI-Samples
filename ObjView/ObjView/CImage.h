#pragma once

class IImage {
public:
	virtual int		Load(const char *texturename) = 0;
	virtual void	Unload() = 0;
	virtual void	FlipY() = 0;
};

class CImage
{
public:
	CImage() {}
	CImage(const char *texturename) {

	}
	~CImage() {}

	unsigned int m_textureid;
private:

};

