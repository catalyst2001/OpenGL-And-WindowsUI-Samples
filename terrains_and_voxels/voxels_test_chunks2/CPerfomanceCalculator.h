#pragma once
#include <Windows.h>
#include <stdio.h>

class CPerfomanceCalculator
{
public:
	inline CPerfomanceCalculator() : m_fTotalElapsedTime(0.0) {}
	inline ~CPerfomanceCalculator() {}

	inline void Start() { m_fStartTime = (double)GetTickCount(); }
	inline void End()
	{
		m_fElapsedTime = (double)GetTickCount() - m_fStartTime;
		m_fTotalElapsedTime += m_fElapsedTime;
	}

	inline void PrintElapsedTime()
	{
		float time_seconds = m_fElapsedTime / 1000;
		printf("Elapsed time: ( %f milliseconds  %f seconds  %f minutes)\n", (float)m_fElapsedTime, time_seconds, time_seconds / 60.f);
	}

	inline void PrintTotalElapsedTime()
	{
		float time_seconds = m_fTotalElapsedTime / 1000;
		printf("Total elapsed time: ( %f milliseconds  %f seconds  %f minutes)\n", (float)m_fTotalElapsedTime, time_seconds, time_seconds / 60.f);
	}
private:
	double m_fStartTime;
	double m_fElapsedTime;
	double m_fTotalElapsedTime;
};

