#pragma once
#ifdef _MSC_VER //windows
 #include <Windows.h>
 #include <intrin.h>
 #include <malloc.h>
#else //linux
 #include <stdlib.h>
#endif
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifdef _DEBUG
 #include <stdarg.h>
 int __msg(const char *text);
 int __fastassertion(const char *format, ...);

 #define PP(x) #x
 #define STR(x) PP(x)

 #ifdef _MSC_VER
  #define asrt_continue IDRETRY
  #define asrt_cancel IDCANCEL
 #else
  #define asrt_continue 0
  #define asrt_cancel 0
 #endif
 #define eassert(exp) if(!(exp)) if(__fastassertion("Expression: %s\nFile: %s\nLine: %d\nFunction: %s\n\nPress RETRY for continue code execution\nPress CANCEL for terminate application", STR(exp), __FILE__, __LINE__, __FUNCTION__) == asrt_cancel) __debugbreak();
 #define eassertex(exp, note) if(!(exp)) if(__fastassertion("%s\n\nExpression: %s\nFile: %s\nLine: %d\nFunction: %s\n\nPress RETRY for continue code execution\nPress CANCEL for terminate application", note, STR(exp), __FILE__, __LINE__, __FUNCTION__) == asrt_cancel) __debugbreak();
#else
 #define eassert(exp)
#endif

void atomic_add(int *pp, int val);
void atomic_increment(int *pp);
void atomic_exchange16(int *pa, int *pb);
void atomic_exchange_pointers(int *pa, int *pb);

#define PREVENT_ERRORS

//---------------------------------------------------------------------------
// �DynamicHeap
// 
// Thread unsafe memory class
//---------------------------------------------------------------------------
template <typename _Type>
class �DynamicHeap
{
public:
	�DynamicHeap() : m_nReserved(1), m_nCurrent(0), m_nSize(NULL), m_pData(NULL) {}
	�DynamicHeap(int startsize, int reserve) {
		m_nReserved = SizeCorrect(reserve);
		m_nSize = SizeCorrect(startsize);
		m_nCurrent = 0;
		m_pData = (_Type *)malloc(sizeof(_Type) * m_nSize); //������ ��������� ������
		eassertex(m_pData, "Couldn't allocate memory!") //�� ����������? ������ assertion failed
	}

	~�DynamicHeap() {
		if (m_pData) { //���� ������ ���� �������� �� ����������� �
			free(m_pData);
			m_pData = NULL;
		}
	}

	// �������� ������
	void Resize(int newsize) {
		//���������, �� ����� �� ��������� NULL.
		//���� �����, ������ ���� ������ ���������� ���� � ��� �� ��������
		if (!m_pData) {
			m_nSize = SizeCorrect(newsize);
			m_pData = (_Type *)malloc(sizeof(_Type) * m_nSize); //�������� ������ ������ ��� �� ����� ��� �� ���� ��������
			eassertex(m_pData, "Couldn't allocate memory!") //���� ������ �� �������� ���������� assertion failed
			return;
		}
		m_pData = (_Type *)realloc(m_pData, m_nSize); //����������������� ����� ���������� ������
		eassertex(m_pData, "Couldn't allocate memory!")
	}

	//���������� ������� � �����
	void Push(_Type elem) {
		bool result = AutomaticResize(); //��������� �� �������� �� �� �����. ���� �������� �� AutomaticResize ����� ���������������� ������
		eassertex(result, "Couldn't reallocate memory");
#ifdef PREVENT_ERRORS
		if (!result)
			return;
#endif
		m_pData[m_nCurrent] = elem;
		m_nCurrent++;
	}
	
	//���������� ��������� ��������� ���� �� ������ � �����
	void PushMulti(_Type *elems, int countelems) {
		eassertex(elems, "variable elems is NULL")
		for (int i = 0; i < countelems; i++)
			Push(elems[i]);
	}

	//��������� ��������� �������
	_Type Pop() {
		eassertex(m_nCurrent, "CDynamicHeap::Pop() Underflow");
		eassertex(m_pData, "Memory is not allocated");
		_Type elem;
#ifdef PREVENT_ERRORS
		if (!m_pData)
			return elem;
#endif
		elem = m_pData[m_nCurrent];
#ifdef PREVENT_ERRORS
		if(m_nCurrent > 0)
#endif	
			m_nCurrent--;
		return elem;
	}

	//������� (������ ������ � ������ � ��� ����������� ������� ������ ������ ����� ����������������)
	void Clear() { m_nCurrent = 0; }

protected:
	//������ size ������ > 0
	inline int SizeCorrect(int size) { return (size > 0) ? size : 1; } 

	//�������������� ����������������� � ������ ���� ����� ������������ � ����� ����
	//���������� 1 ���� ������ ���� ���������������� �������, ����� 0
	bool AutomaticResize() {
		if (!m_pData)
			goto _allocate;

		if ((m_nCurrent + 1) => m_nSize) {
			m_nSize += m_nReserved;
		_allocate:
			m_pData = (_Type *)malloc(sizeof(_Type) * m_nSize);
			if (!m_pData)
				return false;
		}
		return true;
	}
private:
	uint32_t m_nReserved;
	uint32_t m_nCurrent;
	uint32_t m_nSize;
	_Type *m_pData;
};