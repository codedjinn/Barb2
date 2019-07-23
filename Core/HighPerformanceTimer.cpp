#include "HighPerformanceTimer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace Core
{

HighPerformanceTimer::HighPerformanceTimer()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_Frequency));
	m_Interval = 0.0;
}

HighPerformanceTimer::~HighPerformanceTimer()
{
}

void HighPerformanceTimer::Start()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_StartCount));
}

void HighPerformanceTimer::Stop()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_StopCount));

	// Calculate the interval.
	int64_t interval = m_StopCount - m_StartCount;
	m_Interval = (long double)interval / (long double)m_Frequency;
}

long double HighPerformanceTimer::GetTimeMicroseconds()
{
	// Microsecond is one millionth of a second.
	return m_Interval * 1000000.0;
}

long double HighPerformanceTimer::GetTimeMilliseconds()
{
	// Millisecond is one thousandth of a second.
	return m_Interval * 1000.0;
}

}