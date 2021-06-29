#pragma once

#include "../../common.h"

/*
Author: Perchik71 29/04/2021
Adapted for Fallout 4 and Fallout 4 CK

The original
URL: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/MemoryManager.h
*/

/*
==================
class MemoryManager

Main
==================
*/
class MemoryManager
{
private:
	// We do not describe constructors and destructors
	// The class is just a shell

	MemoryManager(VOID) = default;
	~MemoryManager(VOID) = default;
public:
	static LPVOID Allocate(MemoryManager *Manager, UINT64 Size, UINT32 Alignment, BOOL Aligned);
	static VOID   Deallocate(MemoryManager *Manager, LPVOID Memory, BOOL Aligned);
	static UINT64 Size(MemoryManager *Manager, LPVOID Memory);
};

/*
==================
class ScrapHeap

For small things (maximum 32, 64, 128 Mbytes size)
==================
*/
class ScrapHeap
{
private:
	// We do not describe constructors and destructors
	// The class is just a shell

	ScrapHeap(VOID) = default;
	~ScrapHeap(VOID) = default;
public:
	LPVOID Allocate(UINT64 Size, UINT32 Alignment);
	VOID   Deallocate(LPVOID Memory);
};