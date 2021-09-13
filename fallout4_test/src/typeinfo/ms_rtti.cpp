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

#include "../common.h"
#include "ms_rtti.h"

extern "C"
{
	typedef void* (*malloc_func_t)(size_t);
	typedef void(*free_func_t)(void*);
	char *__unDNameEx(char *outputString, const char *name, int maxStringLength, malloc_func_t pAlloc, free_func_t pFree, char *(__fastcall *pGetParameter)(int), unsigned int disableFlags);
}

namespace MSRTTI
{
	using namespace detail;

	std::vector<Info> Tables;

	void Initialize()
	{
		for (uintptr_t i = g_RdataBase; i < (g_RdataEnd - sizeof(uintptr_t) - sizeof(uintptr_t)); i++)
		{
			// Skip all non-2-aligned addresses. Not sure if this is OK or it skips tables.
			if (i % 2 != 0)
				continue;

			//
			// This might be a valid RTTI entry, so check if:
			// - The COL points to somewhere in .rdata
			// - The COL has a valid signature
			// - The first virtual function points to .text
			//
			uintptr_t addr = *(uintptr_t *)i;
			uintptr_t vfuncAddr = *(uintptr_t *)(i + sizeof(uintptr_t));

			if (!IsWithinRDATA(addr) || !IsWithinCODE(vfuncAddr))
				continue;

			auto locator = reinterpret_cast<CompleteObjectLocator *>(addr);

			if (!IsValidCOL(locator))
				continue;

			Info info;
			info.VTableAddress = i + sizeof(uintptr_t);
			info.VTableOffset = locator->Offset;
			info.VFunctionCount = 0;
			info.RawName = locator->TypeDescriptor.Get()->name;
			info.Locator = locator;

			// Demangle
			info.Name = __unDNameEx(nullptr, info.RawName + 1, 0, malloc, free, nullptr, 0x2800);

			// Determine number of virtual functions
			for (uintptr_t j = info.VTableAddress; j < (g_RdataEnd - sizeof(uintptr_t)); j += sizeof(uintptr_t))
			{
				if (!IsWithinCODE(*(uintptr_t *)j))
					break;

				info.VFunctionCount++;
			}

			// Done
			Tables.push_back(info);
		}
	}

	void Dump(FILE *File)
	{
		for (const Info& info : Tables)
			fprintf(File, "`%s`: VTable [0x%p, 0x%p offset, %lld functions] `%s`\n", info.Name, info.VTableAddress - g_ModuleBase, info.VTableOffset, info.VFunctionCount, info.RawName);
	}

	const Info *Find(const char *Name, bool Exact)
	{
		auto results = FindAll(Name, Exact);
		AssertMsgDebug(results.size() == 1, "Had no results or had more than 1 result");

		return results.at(0);
	}

	std::vector<const Info *> FindAll(const char *Name, bool Exact)
	{
		// Multiple classes can have identical names but different vtable displacements,
		// so return all that match
		std::vector<const Info *> results;

		for (const Info& info : Tables)
		{
			if (Exact)
			{
				if (!strcmp(info.Name, Name))
					results.push_back(&info);
			}
			else
			{
				if (strcasestr(info.Name, Name))
					results.push_back(&info);
			}
		}

		return results;
	}

	namespace detail
	{
		bool IsWithinRDATA(uintptr_t Address)
		{
			return (Address >= g_RdataBase && Address <= g_RdataEnd) ||
				(Address >= g_DataBase && Address <= g_DataEnd);
		}

		bool IsWithinCODE(uintptr_t Address)
		{
			return Address >= g_CodeBase && Address <= g_CodeEnd;
		}

		bool IsValidCOL(CompleteObjectLocator *Locator)
		{
			return Locator->Signature == CompleteObjectLocator::COL_Signature64 && IsWithinRDATA(Locator->TypeDescriptor.Address());
		}

		const char *strcasestr(const char *String, const char *Substring)
		{
			const char *a, *b;

			for (; *String; *String++)
			{
				a = String;
				b = Substring;

				while (toupper(*a++) == toupper(*b++))
					if (!*b)
						return String;
			}

			return nullptr;
		}
	}
}