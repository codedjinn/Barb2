#ifndef HIGHPERFORMANCETIMER_H_INCLUDED
#define HIGHPERFORMANCETIMER_H_INCLUDED

#include <stdint.h>

namespace Core
{

/// <summary>
/// Provides high resolution timing functionality. 
/// </summary>
class HighPerformanceTimer
{
	public:
	
		HighPerformanceTimer();
		~HighPerformanceTimer();

		/// <summary>
		/// Starts the timer.
		/// </summary>
		void Start();

		/// <summary>
		/// Stop the timer.
		/// </summary>
		void Stop();

		/// <summary>
		/// Returns the elapsed time interval in microseconds.
		/// </summary>
		long double GetTimeMicroseconds();

		/// <summary>
		/// Returns the elapsed time interval in milliseconds.
		/// </summary>
		long double GetTimeMilliseconds();

	protected:

		int64_t m_Frequency;
		int64_t m_StartCount;
		int64_t m_StopCount;

		/// <summary>
		/// The interval of time that has passed, stored as a fraction of a second.
		/// </summary>
		long double m_Interval;
};

}

#endif // HIGHPERFORMANCETIMER_H_INCLUDED