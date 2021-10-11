#pragma once
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

template <typename _T>
class DynamicHeap
{
public:
	DynamicHeap() : m_pData(NULL), m_nSize(0), m_nCurrentOffset(0) {}
	~DynamicHeap() {}

	bool Alloc(int _startsize = 0, int _reserve = 1) {
		m_nCurrentOffset = 0;
		m_nSize = _startsize;
		m_nReserve = _reserve;

		int ret = InternalAutorealloc();
		assert(ret); //automatic reallocate memory
		return ret;
	}

	int Push(_T elem) {
		assert(InternalAutorealloc());
		m_pData[m_nCurrentOffset] = elem;
		return m_nCurrentOffset++;
	}

	_T Pop() {
		assert(m_pData);
		return m_pData[m_nCurrentOffset--];
	}

	_T operator[](int index) {
		assert(index > 0 && index < m_nSize && m_pData);
		return m_pData[index];
	}

	_T *operator&() { m_pData; }

	inline void		SetSize(int newsize)		{ m_nSize = newsize; }
	inline int		Size()						{ return m_nSize; }
	inline int		Offset()					{ return m_nCurrentOffset; }
	inline void		SetReserve(int newreserve)	{ m_nReserve = newreserve; }
	inline int		Reserve()					{ return m_nReserve; }
	inline _T		*GetData()					{ return m_pData; }

	inline void		Clear() { m_nCurrentOffset = 0; }

	void Free() {
		if (m_pData) {
			free(m_pData);
			m_pData = NULL;
			m_nCurrentOffset = 0;
			m_nSize = 0;
		}
	}

	inline bool IsFree() { return !!m_pData; }
	
private:
	bool InternalAutorealloc() {
		if ((m_nCurrentOffset + 1) >= m_nSize) {
			m_nSize += m_nReserve;
			m_pData = (_T *)realloc(m_pData, sizeof(_T));
			if (!m_pData)
				return false;
		}
		return true;
	}

	int m_nCurrentOffset;
	int m_nSize;
	int m_nReserve;
	_T *m_pData;
};