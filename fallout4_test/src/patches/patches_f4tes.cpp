#include "../common.h"
#include "deflate.h"
#include "boost_search_array.h"
#include "TES/MemoryManager.h"
#include "TES/bhkThreadMemorySource.h"
#include "TES/BSTArray.h"

#include <string>	
#include <intrin.h>

/*

This file is part of Fallout 4 Fixes source code.

*/

VOID FIXAPI Fix_PatchSettings(VOID);
VOID FIXAPI Fix_PatchMemory(VOID);
VOID FIXAPI Fix_PatchThreading(VOID);
VOID FIXAPI Fix_PatchWindow(VOID);
BOOL FIXAPI Fix_PatchAchievements(VOID);


/*
==================
Sys_GetGameVersion
==================
*/
std::string FIXAPI Sys_GetGameVersion(VOID)
{
	CHAR szExeName[1024];
	GetModuleFileNameA(GetModuleHandleA(NULL), szExeName, ARRAYSIZE(szExeName));

	DWORD dwBuffSize = GetFileVersionInfoSizeA(szExeName, 0);
	LPBYTE pBuff = new BYTE[dwBuffSize];
	std::string str = "";

	if (GetFileVersionInfoA(szExeName, 0, dwBuffSize, pBuff))
	{
		VS_FIXEDFILEINFO* fi = NULL;
		UINT uLen = 0;
		VerQueryValueA(pBuff, "\\", (LPVOID*)&fi, &uLen);

		WORD dwVerMajor = (WORD)(fi->dwProductVersionMS >> 16);
		WORD dwVerMinor = (WORD)(fi->dwProductVersionMS & 0xFF);
		WORD dwPatchLevel = (WORD)(fi->dwProductVersionLS >> 16);
		WORD dwVerBuild = (WORD)(fi->dwProductVersionLS & 0xFF);

		CHAR buff[1024];
		snprintf(buff, ARRAYSIZE(buff), "%u.%u.%u.%u", dwVerMajor, dwVerMinor, dwPatchLevel, dwVerBuild);
		str = buff;
	}

	return str;
}

/*
==================
MainFix_PatchFallout4Game

Implements the code in the process game
==================
*/
VOID FIXAPI MainFix_PatchFallout4Game(VOID)
{
	if (Sys_GetGameVersion() == "1.10.163.0")
	{
		// Left for the future...
	}
	else
	{
		CHAR modulePath[MAX_PATH];
		GetModuleFileNameA(GetModuleHandleA(NULL), modulePath, ARRAYSIZE(modulePath));

		CHAR message[1024];
		sprintf_s(message,
			"Unknown Fallout4 version detected. Patches are disabled.\n\n"
			"Required versions:\n"
			"Fallout4.exe 1.10.163.0\n"
			"\nExecutable path: %s", modulePath);

		MessageBoxA(NULL, message, "Version Check", MB_ICONERROR);
		return;
	}

	//
	// Settings
	//
	if (g_INI.GetBoolean("Fallout4", "SettingsPatch", FALSE))
	{
		Fix_PatchSettings();
	}

	if (g_INI.GetBoolean("Fallout4", "AchievementsPatch", FALSE))
	{
		// opens access to steam achievements with mods
		if (!Fix_PatchAchievements())
		{
			MessageBoxA(NULL, "Failed to find at least one achievement patch address", "???", 0);
		}
	}

	//
	// MemoryManager
	//
	if (g_INI.GetBoolean("Fallout4", "MemoryPatch", FALSE))
	{
		Fix_PatchMemory();

		XUtil::PatchMemory(OFFSET(0xD0C160, 0), { 0xC3 });							// [XGB  ] MemoryManager - Default/Static/File heaps
		XUtil::PatchMemory(OFFSET(0x1B0EDB0, 0), { 0xC3 });							// [1GB  ] BSSmallBlockAllocator
		XUtil::DetourJump(OFFSET(0x1E21B10, 0), &bhkThreadMemorySource::__ctor__);	// [512MB][1GB  ][2GB  ] bhkThreadMemorySource
		XUtil::PatchMemory(OFFSET(0x1B13DF0, 0), { 0xC3 });							// [32MB ][64MB ][128MB] ScrapHeap init
		XUtil::PatchMemory(OFFSET(0x1B14740, 0), { 0xC3 });							// [32MB ][64MB ][128MB] ScrapHeap deinit
																					// [128MB] BSScaleformSysMemMapper is untouched due to complexity

		XUtil::PatchMemory(OFFSET(0x1DD67D4, 0), { (BYTE)(((UINT32)g_bhkMemSize & 0xFF000000) >> 24) });

		XUtil::DetourJump(OFFSET(0x1B0EFD0, 0), &MemoryManager::Allocate);
		XUtil::DetourJump(OFFSET(0x1B0F2E0, 0), &MemoryManager::Deallocate);
		XUtil::DetourJump(OFFSET(0x1B13F70, 0), &ScrapHeap::Allocate);
		XUtil::DetourJump(OFFSET(0x1B14580, 0), &ScrapHeap::Deallocate);
	}

	//
	// Threads
	//
	//if (g_INI.GetBoolean("Fallout4", "ThreadingPatch", FALSE))
//	{
	Fix_PatchThreading();
//	}

	//
	// Window
	//
	Fix_PatchWindow();

	//
	// Loading optimizations
	//

	INT32 cpuinfo[4];
	__cpuid(cpuinfo, 1);

	// Utilize SSE4.1 instructions if available
	if ((cpuinfo[2] & (1 << 19)) != 0)
	{
		std::vector<uintptr_t> matches = XUtil::FindPatterns(g_CodeBase, g_CodeEnd - g_CodeBase,
			"83 FB FF 75 1F 48 8B 0A 49 8B 03 49 39 04 09 41 0F 44 D8 41 FF C0 49 83 C1 08 45 3B C2 72 E1");
		std::vector<uintptr_t>::iterator match;

		XUtil::Parallel::for_each(match = matches.begin(), matches.end(), [](auto it) { 
			XUtil::PatchMemoryNop(it, 0x1F);
			XUtil::PatchMemory(it - 6, { 0x48, 0x89, 0xD1, 0x4C, 0x89, 0xDA, 0x48, 0x83, 0xEC, 0x30 });
			XUtil::DetourCall(it + 4, &Fix_BoostArraySearchItem);
			XUtil::PatchMemory(it + 9, { 0x48, 0x83, 0xC4, 0x30, 0x89, 0xC3, 0x49, 0x89, 0xD3, 0x48, 0x89, 0xCA });
			});
	}

	XUtil::DetourCall(OFFSET(0x13267D, 0), &hk_inflateInit);
	XUtil::DetourCall(OFFSET(0x1326AF, 0), &hk_inflate);
}