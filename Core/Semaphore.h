#pragma once

/// <summary>
/// Defines a Semaphore syncronization primitive. 
/// Must be redefined for every different operating system.
/// </summary>

#include <cassert>

#ifdef _WIN32   // Windows OS (x64 and x86)

#include <Windows.h>

namespace Core
{

class Semaphore
{
	public:

		Semaphore(int initialCount = 0)
		{
			assert(initialCount >= 0);
			
			m_Handle = CreateSemaphore(NULL,
									   initialCount,
									   LONG_MAX,
									   NULL);
		}

		~Semaphore()
		{
			CloseHandle(m_Handle);
		}

		void Wait()
		{
			WaitForSingleObject(m_Handle, INFINITE);
		}

		void Signal(int count = 1)
		{
			ReleaseSemaphore(m_Handle, count, NULL);
		}

	private:

		HANDLE m_Handle;

		Semaphore(const Semaphore& rvalue) = delete;
		Semaphore& operator= (const Semaphore& rvalue) = delete;
};

}

#else 
#error Core::Semaphore not defined for this OS!
#endif // _WIN32