/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

///------------------------------------------------------------------------------------------------
/// Profiler.h
///
/// Defines a system performance profiling counter class.
/// This system takes the easy way and uses #ifdefs to define OS specific code.
///
///------------------------------------------------------------------------------------------------

#ifndef PROFILER_H_INCLUDED
#define PROFILER_H_INCLUDED

#include <iostream>
#include <inttypes.h>

#ifdef _WIN32				/// Are we compiling on a Win32 system?
#include <windows.h>
#endif

/**
* Profiler:
* Data structure representing a high precision profiling utility.
*
* Maintenance:
* On deallocation, all objects of this type will automatically release any allocated memory,
* so no manual cleanup is required.
*/
class Profiler
{
	private:

#ifdef _WIN32
		static int64_t performanceCounterFrequency;
		/// The number of performance counter ticks per second.

		int64_t startTime;
		/// The starting time for the profiled segment.

		int64_t stopTime;
		/// The ending time for the profiled segment.
#endif

	public:

		Profiler()
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
			QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
#endif
		}

		void start()
		/// Sets the start time for the performance counter.
		/// @note You must call the stop() method before any valid results can be queried.
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
#endif
		}

		void stop()
		/// Sets the stop time for the performance counter.
		/// @note After this method is called, you can safely call the getTime* methods.
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
#endif
		}

		float getTimeSeconds() const
		/// @return The time interval between the start and stop times represented in seconds.
		{
#ifdef _WIN32
			long double time	= ((long double)stopTime - (long double)startTime) /
								  (long double)performanceCounterFrequency;

			return ((float)time);
#endif
		}

		float getTimeMilliseconds() const
		/// @return The time interval between the start and stop times represented in milliseconds.
		{
#ifdef _WIN32
			long double time	= (long double)(stopTime - startTime) /
								  (long double)(performanceCounterFrequency);

			return ((float)time * 1000.0f);
#endif
		}

		static void initializePerformanceCounters()
		/// Performs any initialization the profiler system needs. Depending on the implementation, this may do a lot, or nothing at all.
		{
#ifdef _WIN32
			BOOL result	= QueryPerformanceFrequency((LARGE_INTEGER*)&performanceCounterFrequency);
			if (!result)
			{
				std::cerr << "TProfiler::High performance counter not supported on this machine" << std::endl;
				std::cin.get();
				exit(1);
			}
#endif
		}
};

#endif // PROFILER_H_INCLUDED
