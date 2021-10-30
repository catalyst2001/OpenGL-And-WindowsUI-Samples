#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

/**
* Потокобезопасная очередь
*/
template <typename _type>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() : m_pQueueData(NULL), m_nCurrentTask(0) {}
	ThreadSafeQueue(int queuesize) : m_pQueueData(NULL), m_nCurrentTask(0) {
		m_pQueueData = (_type *)malloc(sizeof(_type) * queuesize > 0 ? queuesize : 4);
		assert(m_pQueueData);
	}
	~ThreadSafeQueue() {
		if (m_pQueueData)
			free(m_pQueueData);
	}

	bool Push(_type data) {
		assert(m_pQueueData);
		m_pQueueData[m_nCurrentTask] = data;
		InterlockedIncrement(&m_nCurrentTask);
	}

	_type Front() {
		_type data = m_pQueueData[m_nCurrentTask];
		InterlockedDecrement(&m_nCurrentTask);
		return data;
	}

	bool IsEmpty() { return !m_nCurrentTask; }

private:
	_type *m_pQueueData;
	uint32_t m_nCurrentTask;
};

template <typename _type>
class CCollector
{
public:
	CCollector() : m_nOffset(0), m_nSize(0), m_pData(NULL) {}
	~CCollector() {}

	bool Allocate(int size) {
		m_nSize = size;
		m_pData = (_type *)malloc(sizeof(_type) * m_nSize);
		assert(m_pData);
		return !!m_pData;
	}

	bool Reallocate(int newsize)
	{
		m_nSize = newsize;
		m_pData = (_type *)realloc(m_pData, sizeof(_type) * m_nSize);
		if (m_nOffset >= m_nSize)
			m_nOffset = m_nSize - 1;

		assert(m_pData);
		return !!m_pData;
	}

	void PushData(_type data) {
		m_pData[m_nOffset] = data;
		m_nOffset++;
	}

	_type PopData() {
		_type temp = m_pData[m_nOffset];
		m_nOffset--;
		return temp;
	}

	uint32_t Size() { return m_nSize; }
	uint32_t Offset() { return m_nOffset; }
	bool IsAllocated() { return !!m_pData; };
	void Free() { free(m_pData); }

	_type operator [](int ind) { return m_pData[ind]; }

private:
	uint32_t m_nOffset;
	uint32_t m_nSize;
	_type *m_pData;
};