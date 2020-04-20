#pragma once

#include "config.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <d3d11.h>
#include <d3d11_2.h>

// Intel VTune
#if SKYRIM64_USE_VTUNE
#include <ittnotify.h>

extern __itt_heap_function ITT_AllocateCallback;
extern __itt_heap_function ITT_ReallocateCallback;
extern __itt_heap_function ITT_FreeCallback;
#endif

// TBBMalloc
#include <tbb/scalable_allocator.h>

// Detours
#include <detours/Detours.h>

// INIReader
#define INI_ALLOW_MULTILINE 0
#define INI_USE_STACK 0
#define INI_MAX_LINE 4096
#include "INIReader.h"

#include "xutil.h"
#include "dump.h"
#include "profiler.h"
#include "patches/offsets.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

extern INIReader g_INI;

extern uintptr_t g_ModuleBase;
extern uintptr_t g_ModuleSize;

extern uintptr_t g_CodeBase;	// .text or .textbss
extern uintptr_t g_CodeEnd;
extern uintptr_t g_RdataBase;	// .rdata
extern uintptr_t g_RdataEnd;
extern uintptr_t g_DataBase;	// .data
extern uintptr_t g_DataEnd;

enum class GAME_EXECUTABLE_TYPE
{
	UNKNOWN,

	LAUNCHER_FALLOUT4,
	GAME_FALLOUT4,
	CREATIONKIT_FALLOUT4,
};

extern GAME_EXECUTABLE_TYPE g_LoadType;
extern char g_GitVersion[64];

#define PatchIAT(detour, module, procname) Detours::IATHook(g_ModuleBase, (module), (procname), (uintptr_t)(detour));