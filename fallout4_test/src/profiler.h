//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once

#if !FALLOUT4_USE_PROFILER
#define ProfileCounterInc(Name)			((void)0)
#define ProfileCounterAdd(Name, Add)	((void)0)
#define ProfileTimer(Name)				((void)0)

#define ProfileGetValue(Name)			(0)
#define ProfileGetDeltaValue(Name)		(0)
#define ProfileGetTime(Name)			(0.0)
#define ProfileGetDeltaTime(Name)		(0.0)
#else
#include <intrin.h>
#include <array>
#include <unordered_map>

#define EXPAND_MACRO(x) x
#define LINEID EXPAND_MACRO(__z)__COUNTER__

#define ProfileCounterInc(Name)			Profiler::ScopedCounter<COMPILE_TIME_CRC32_INDEX(Name)>(__FILE__, __FUNCTION__, Name)
#define ProfileCounterAdd(Name, Add)	Profiler::ScopedCounter<COMPILE_TIME_CRC32_INDEX(Name)>(__FILE__, __FUNCTION__, Name, Add)
#define ProfileTimer(Name)				Profiler::ScopedTimer<COMPILE_TIME_CRC32_INDEX(Name)> LINEID(__FILE__, __FUNCTION__, Name)

#define ProfileGetValue(Name)			Profiler::GetValue<COMPILE_TIME_CRC32_STR(Name)>()
#define ProfileGetDeltaValue(Name)		Profiler::GetDeltaValue<COMPILE_TIME_CRC32_STR(Name)>()
#define ProfileGetTime(Name)			Profiler::GetTime<COMPILE_TIME_CRC32_STR(Name)>()
#define ProfileGetDeltaTime(Name)		Profiler::GetDeltaTime<COMPILE_TIME_CRC32_STR(Name)>()

namespace Profiler
{
	namespace Internal
	{
#include "profiler_internal.h"
	}

	template<uint32_t UniqueIndex>
	class ScopedCounter
	{
	private:
		static_assert(UniqueIndex < Internal::MaxEntries, "Increase max array size");

		ScopedCounter() = delete;
		ScopedCounter(ScopedCounter&) = delete;

	public:
		inline ScopedCounter(const char *File, const char *Function, const char *Name)
		{
			if (!m_Entry.Init)
				m_Entry = { 0, 0, File, Function, Name, true };

			InterlockedIncrement64(&m_Entry.Value);
		}

		inline ScopedCounter(const char *File, const char *Function, const char *Name, int64_t Add)
		{
			if (!m_Entry.Init)
				m_Entry = { 0, 0, File, Function, Name, true };

			InterlockedAdd64(&m_Entry.Value, Add);
		}

	private:
		Internal::Entry& m_Entry = Internal::GlobalCounters[UniqueIndex];
	};

	template<uint32_t UniqueIndex>
	class ScopedTimer
	{
	private:
		static_assert(UniqueIndex < Internal::MaxEntries, "Increase max array size");

		ScopedTimer() = delete;
		ScopedTimer(ScopedTimer&) = delete;

		__forceinline void GetTime(LARGE_INTEGER *Counter)
		{
#if 1
			uint32_t temp;
			Counter->QuadPart = __rdtscp(&temp);
#else
			QueryPerformanceCounter(Counter);
#endif
		}

	public:
		__forceinline ScopedTimer(const char *File, const char *Function, const char *Name)
		{
			if (!m_Entry.Init)
				m_Entry = { 0, 0, File, Function, Name, true };

			GetTime(&m_Start);
		}

		__forceinline ~ScopedTimer()
		{
			LARGE_INTEGER endTime;
			GetTime(&endTime);

			InterlockedAdd64(&m_Entry.Value, endTime.QuadPart - m_Start.QuadPart);
		}

	private:
		Internal::Entry& m_Entry = Internal::GlobalCounters[UniqueIndex];
		LARGE_INTEGER m_Start;
	};

	int64_t GetValue(uint32_t CRC);
	int64_t GetDeltaValue(uint32_t CRC);
	double GetTime(uint32_t CRC);
	double GetDeltaTime(uint32_t CRC);

	template<uint32_t CRC>
	int64_t GetValue()
	{
		return GetValue(CRC);
	}

	template<uint32_t CRC>
	int64_t GetDeltaValue()
	{
		return GetDeltaValue(CRC);
	}

	template<uint32_t CRC>
	double GetTime()
	{
		return GetTime(CRC);
	}

	template<uint32_t CRC>
	double GetDeltaTime()
	{
		return GetDeltaTime(CRC);
	}

	float GetProcessorUsagePercent();
	float GetThreadUsagePercent();
	float GetGpuUsagePercent(int GpuIndex = 0);
}
#endif // FALLOUT4_USE_PROFILER