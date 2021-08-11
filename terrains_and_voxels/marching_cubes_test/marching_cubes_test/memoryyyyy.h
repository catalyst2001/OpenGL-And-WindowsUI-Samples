#pragma once
#include <stdlib.h>
#include "vector3d.h"

template <typename _T>
class DynamicHeap
{
public:
	DynamicHeap() : m_pVerts(nullptr), m_Size(0), m_Index(0), m_Reserve(1) {}
	DynamicHeap(int startsize, int reserve) {
		m_pVerts = (_T *)malloc(startsize * sizeof(_T));
		m_Size = startsize;
		m_Reserve = reserve;
		m_Index = 0;
	}

	void Add(_T vert) {
		if (!m_pVerts) {
			m_Size = 1;
			m_Index = 0;
			goto forcealloc;
		}

		if ((m_Index + 1) == m_Size) {
		forcealloc:
			m_Size += m_Reserve;
			m_pVerts = (_T *)realloc(m_pVerts, sizeof(_T) * m_Size);
		}
		m_pVerts[m_Index] = vert;
		m_Index++;
	}

	inline _T *GetData() { return m_pVerts; }
	inline unsigned int GetCount() { return m_Index; }
	inline void Clear() { m_Index = 0; }
	inline void Free() {
		if (m_pVerts) {
			free(m_pVerts);
			m_pVerts = nullptr;
		}
	}

	~DynamicHeap() {
		if (m_pVerts)
			free(m_pVerts);
	}

private: //TODO: chnage this
	int m_Size;
	int m_Index;
	int m_Reserve;
	_T *m_pVerts;
};