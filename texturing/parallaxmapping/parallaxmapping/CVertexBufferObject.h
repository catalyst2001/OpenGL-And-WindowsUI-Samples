#pragma once
#include "glextsloader.h"

/**
* VBO
* GL_ELEMENT_ARRAY_BUFFER
* GL_ARRAY_BUFFER
*/
template <const int bufferobjecttype>
class CVBO
{
public:
	CVBO() : m_BufferId(0) {}
	~CVBO() {}

	inline void Create() { glGenBuffers(1, &m_BufferId); }
	inline void Bind() { glBindBuffer(bufferobjecttype, m_BufferId); }
	inline void Unbind() { glBindBuffer(bufferobjecttype, 0); }
	inline void BufferData(int elemsize, int nelems, void *dataptr, int usage = GL_STATIC_DRAW) {
		glBufferData(bufferobjecttype, elemsize * nelems, dataptr, usage);
	}
	inline void BufferData(int size, void *dataptr, int usage = GL_STATIC_DRAW) {
		glBufferData(bufferobjecttype, size, dataptr, usage);
	}

	/**
	* offset - in bytes
	*/
	inline void BufferModify(int offset, int elemsize, int nelems, void *newdata) {
		glBufferSubData(bufferobjecttype, offset, elemsize * nelems, newdata);
	}

	inline void Delete() { glDeleteBuffers(1, &m_BufferId);}
	operator int() { return m_BufferId; }
	operator unsigned int() { return m_BufferId; }
private:
	unsigned int m_BufferId;
};

class CVAO
{
public:
	CVAO() : m_VertexBufferObject(0) {}
	~CVAO() {}

	inline void Create() { glGenVertexArrays(1, &m_VertexBufferObject); }
	inline void Delete() { glDeleteVertexArrays(1, &m_VertexBufferObject); }
	inline void Bind() { glBindVertexArray(m_VertexBufferObject); }
	inline void Unbind() { glBindVertexArray(0); }
private:
	unsigned int m_VertexBufferObject;
};