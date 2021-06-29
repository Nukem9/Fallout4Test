#include "..\common.h"

/*
Author: Perchik71 29/04/2021
This file is part of Fallout 4 Fixes source code.

Adapted for Fallout 4 and Fallout 4 CK
The original
URL: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/threading.cpp
*/


/*
==================
hk_SetThreadPriority

Replacement WINAPI SetThreadPriority
==================
*/
BOOL WINAPI hk_SetThreadPriority(HANDLE hThread, int nPriority)
{
	// Don't allow a priority below normal - Skyrim doesn't have many "idle" threads
	return SetThreadPriority(hThread, std::max(THREAD_PRIORITY_NORMAL, nPriority));
}

/*
==================
hk_SetThreadAffinityMask

Replacement WINAPI SetThreadAffinityMask
==================
*/
DWORD_PTR WINAPI hk_SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
{
	// Don't change anything
	return 0xFFFFFFFF;
}

/*
==================
hk_Sleep

Replacement WINAPI Sleep
==================
*/
VOID WINAPI hk_Sleep(DWORD dwMilliseconds)
{
	// Bethesda's spinlock calls Sleep(0) every iteration until 10,000. Then it
	// uses Sleep(1). Even with 0ms waits, there's a tiny performance penalty.
	if (dwMilliseconds <= 1)
		Sleep(0);
	else
		Sleep(dwMilliseconds);
}

/*
==================
Fix_PatchThreading

Implements the code in the process
Changes the behavior of the main thread of the application and the OS
==================
*/
VOID FIXAPI Fix_PatchThreading(VOID)
{
	PatchIAT(hk_SetThreadPriority, "kernel32.dll", "SetThreadPriority");
	PatchIAT(hk_SetThreadAffinityMask, "kernel32.dll", "SetThreadAffinityMask");
	PatchIAT(hk_Sleep, "kernel32.dll", "Sleep");

	// no abort/retry/fail errors
	SetErrorMode(SEM_FAILCRITICALERRORS);

	// Process that has priority above NORMAL_PRIORITY_CLASS but below HIGH_PRIORITY_CLASS.
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
}
