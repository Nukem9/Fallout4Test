#pragma once

#include "../../common.h"

class bhkThreadMemorySource
{
public:
	CHAR _pad0[0x8];
	CRITICAL_SECTION m_CritSec;

public:
	DECLARE_CONSTRUCTOR_HOOK(bhkThreadMemorySource);

	bhkThreadMemorySource(VOID);
	virtual ~bhkThreadMemorySource(VOID);
	virtual LPVOID blockAlloc(UINT64 numBytes);
	virtual VOID   blockFree(LPVOID p, UINT64 numBytes);
	virtual LPVOID bufAlloc(UINT64& reqNumBytesInOut);
	virtual VOID   bufFree(LPVOID p, UINT64 numBytes);
	virtual LPVOID bufRealloc(LPVOID pold, UINT64 oldNumBytes, UINT64& reqNumBytesInOut);
	virtual VOID   blockAllocBatch(LPVOID *ptrsOut, UINT64 numPtrs, UINT64 blockSize);
	virtual VOID   blockFreeBatch(LPVOID *ptrsIn, UINT64 numPtrs, UINT64 blockSize);
	virtual VOID   getMemoryStatistics(class MemoryStatistics& u);
	virtual UINT64 getAllocatedSize(const LPVOID obj, UINT64 nbytes);
	virtual VOID   resetPeakMemoryStatistics(VOID);
#if FALLOUT4
	virtual LPVOID getExtendedInterface(VOID);
#endif
};
static_assert_offset(bhkThreadMemorySource, m_CritSec, 0x10);