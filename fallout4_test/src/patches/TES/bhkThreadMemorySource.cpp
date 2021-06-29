#include "MemoryManager.h"
#include "bhkThreadMemorySource.h"

bhkThreadMemorySource::bhkThreadMemorySource(VOID)
{
	InitializeCriticalSection(&m_CritSec);
}

bhkThreadMemorySource::~bhkThreadMemorySource(VOID)
{
	DeleteCriticalSection(&m_CritSec);
}

LPVOID bhkThreadMemorySource::blockAlloc(UINT64 numBytes)
{
	return MemoryManager::Allocate(NULL, numBytes, 16, TRUE);
}

VOID bhkThreadMemorySource::blockFree(LPVOID p, UINT64 numBytes)
{
	MemoryManager::Deallocate(NULL, p, TRUE);
}

LPVOID bhkThreadMemorySource::bufAlloc(UINT64& reqNumBytesInOut)
{
	return blockAlloc(reqNumBytesInOut);
}

VOID bhkThreadMemorySource::bufFree(LPVOID p, UINT64 numBytes)
{
	return blockFree(p, numBytes);
}

LPVOID bhkThreadMemorySource::bufRealloc(LPVOID pold, UINT64 oldNumBytes, UINT64& reqNumBytesInOut)
{
	LPVOID p = blockAlloc(reqNumBytesInOut);
	memcpy(p, pold, oldNumBytes);
	blockFree(pold, oldNumBytes);

	return p;
}

VOID bhkThreadMemorySource::blockAllocBatch(LPVOID *ptrsOut, UINT64 numPtrs, UINT64 blockSize)
{
	for (INT32 i = 0; i < numPtrs; i++)
		ptrsOut[i] = blockAlloc(blockSize);
}

VOID bhkThreadMemorySource::blockFreeBatch(LPVOID *ptrsIn, UINT64 numPtrs, UINT64 blockSize)
{
	for (INT32 i = 0; i < numPtrs; i++)
		blockFree(ptrsIn[i], blockSize);
}

VOID bhkThreadMemorySource::getMemoryStatistics(class MemoryStatistics& u)
{
	// Nothing
}

UINT64 bhkThreadMemorySource::getAllocatedSize(const LPVOID obj, UINT64 nbytes)
{
	Assert(FALSE);
	return 0;
}

VOID bhkThreadMemorySource::resetPeakMemoryStatistics(VOID)
{
	// Nothing
}

#if FALLOUT4
LPVOID bhkThreadMemorySource::getExtendedInterface(VOID)
{
	return NULL;
}
#endif