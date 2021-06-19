#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/gl.h>

// C++ 14
template <typename TYPE>
class DynamicHeap
{
public:
	DynamicHeap() : m_pData(nullptr), m_Reserve(1), m_Current(0), m_Size(1) {}
	DynamicHeap(int startsize, int reserve) {
		m_pData = nullptr;
		m_Reserve = reserve > 0 ? reserve : 1; //check reserve bytes correct
		m_Current = 0; //set position in buffer to zero
		//if set start size
		if (startsize > 0) {
			m_Size = startsize;
			m_pData = (TYPE *)malloc(sizeof(TYPE) * startsize);
		}
	}

	//~DynamicHeap() { if (m_pData) free(m_pData); }

	void Push(TYPE type) {
		if (!m_pData)
			m_pData = (TYPE *)malloc(sizeof(TYPE) * 1);

		assert(m_pData);
		if ((m_Current + 1) == m_Size) {
			m_Size += m_Reserve;
			m_pData = (TYPE *)realloc(m_pData, sizeof(TYPE) * m_Size);
		}
		m_pData[m_Current] = type;
		m_Current++;
	}

	TYPE Pop() {
		assert(m_pData);
		TYPE *ptr = &m_pData[m_Current];
		if (m_Current > 0)
			m_Current--;
		return *ptr;
	}

	TYPE operator[] (int idx) {
		assert(idx >= 0);
		return m_pData[idx];
	}

	int Size() { return m_Current; }
	TYPE *Data() { return m_pData; }
	void Free() { if (m_pData) { free(m_pData); m_pData = nullptr; } }

private:
	TYPE *m_pData;
	int m_Current;
	int m_Size;
	int m_Reserve;
};

struct Rect { int x1, y1, x2, y2, x3, y3, x4, y4; };
struct TexCoords { float x1, y1, x2, y2, x3, y3, x4, y4; };
struct Color3 { unsigned char r, g, b; };
struct Color4 { unsigned char r, g, b, a; };

class IBaseGUIElement {
public:
	int m_id;
	char m_type;
	long m_flags;
	int m_textureid;
	Rect m_Rect;
	TexCoords m_TexCoords;
	Color4 m_ColorBackground;
	Color4 m_ColorEdge;
	DynamicHeap<IBaseGUIElement*> m_Childs;

	virtual void Create(IBaseGUIElement *parent, int x, int y, int width, int height) = 0;
	virtual void Destroy() = 0;
	virtual void Render() = 0;
};

class CCanvas : IBaseGUIElement
{
public:
	void Create(IBaseGUIElement *parent, int x, int y, int width, int height) {
		//	(4)-----------------(3)
		//	|				     |
		//	|				     |
		//	|				     |
		//	(1)-----------------(2)
		m_Rect.x1 = x;
		m_Rect.y1 = y;
		m_Rect.x2 = x + width;
		m_Rect.y2 = y;
		m_Rect.x3 = m_Rect.x2;
		m_Rect.y3 = y + height;
		m_Rect.x4 = x;
		m_Rect.y4 = m_Rect.y3;

		m_TexCoords.x1 = 0.f; m_TexCoords.y1 = 0.f;
		m_TexCoords.x2 = 1.f; m_TexCoords.y2 = 0.f;
		m_TexCoords.x3 = 1.f; m_TexCoords.y3 = 1.f;
		m_TexCoords.x4 = 0.f; m_TexCoords.y4 = 1.f;
	}

	void Destroy() {

	}

	void Render() {
		glVertexPointer((sizeof(Rect) / sizeof(int) / 2), GL_INT, 0, &m_Rect);
		glDrawArrays(GL_QUADS, 0, 4);
	}
};

class CGui : IBaseGUIElement
{
public:
	CGui() {}
	~CGui() {}

	void Create(IBaseGUIElement *parent, int x, int y, int width, int height) {}
	void Destroy() {}

	void Render() {
		for (int i = 0; i < m_Childs.Size(); i++) {
			IBaseGUIElement *elem = m_Childs[i];
			elem->Render();
		}
	}

	IBaseGUIElement *AddCanvas(CCanvas *cv, IBaseGUIElement *pparent, int id, int x, int y, int width, int height, Color4 background, Color4 edge) {
		cv->Create(pparent, x, y, width, height);
		m_Childs.Push((IBaseGUIElement *)cv);
		return (IBaseGUIElement *)cv;
	}
};

