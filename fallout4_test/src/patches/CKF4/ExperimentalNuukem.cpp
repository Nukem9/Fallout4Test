#include "..\..\Common.h"
#include "ExperimentalNuukem.h"
#include "LogWindow.h"

#include <zydis\Zydis.h>

#include <chrono>
#include <intrin.h>
#include <tbb/concurrent_vector.h>

namespace Experimental
{
	struct __addr_t
	{
		uintptr_t Based;
		uintptr_t End;
	};

	struct range_t
	{
		__addr_t addr;
		DWORD protection;
	};

	namespace Nuukem
	{
		struct NullsubPatch
		{
			std::initializer_list<uint8_t> Signature;
			uint8_t JumpPatch[5];
			uint8_t CallPatch[5];
		};

		const NullsubPatch Patches[] =
		{
			// Nullsub || retn; int3; int3; int3; int3; || nop;
			{ { 0xC2, 0x00, 0x00 },{ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC },{ 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0xC3 },{ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC },{ 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x4C, 0x24, 0x08, 0xC3 },{ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC },{ 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x54, 0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08, 0xC3 },{ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC },{ 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x83, 0xEC, 0x28, 0x48, 0x8B, 0x4C, 0x24, 0x30, 0x0F, 0x1F, 0x44, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x28, 0xC3 },{ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC },{ 0x0F, 0x1F, 0x44, 0x00, 0x00 } },

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0xC3 },								// return this;
				{ 0x48, 0x89, 0xC8, 0xC3, 0xCC },																	// mov rax, rcx; retn; int3;
				{ 0x48, 0x89, 0xC8, 0x66, 0x90 }																	// mov rax, rcx; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x00, 0xC3 },				// return *(__int64 *)this;
				{ 0x48, 0x8B, 0x01, 0xC3, 0xCC },																	// mov rax, [rcx]; retn; int3;
				{ 0x48, 0x8B, 0x01, 0x66, 0x90 }																	// mov rax, [rcx]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x40, 0x08, 0xC3 },		// return *(__int64 *)(this + 0x8);
				{ 0x48, 0x8B, 0x41, 0x08, 0xC3 },																	// mov rax, [rcx + 0x8]; retn;
				{ 0x48, 0x8B, 0x41, 0x08, 0x90 }																	// mov rax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x40, 0x50, 0xC3 },		// return *(__int64 *)(this + 0x50);
				{ 0x48, 0x8B, 0x41, 0x50, 0xC3 },																	// mov rax, [rcx + 0x50]; retn;
				{ 0x48, 0x8B, 0x41, 0x50, 0x90 }																	// mov rax, [rcx + 0x50]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x00, 0xC3 },					// return *(__int32 *)this;
				{ 0x8B, 0x01, 0xC3, 0xCC, 0xCC },																	// mov eax, [rcx]; retn; int3; int3;
				{ 0x8B, 0x01, 0x0F, 0x1F, 0x00 }																	// mov eax, [rcx]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x40, 0x08, 0xC3 },				// return *(__int32 *)(this + 0x8);
				{ 0x8B, 0x41, 0x08, 0xC3, 0xCC },																	// mov eax, [rcx + 0x8]; retn; int3;
				{ 0x8B, 0x41, 0x08, 0x66, 0x90 }																	// mov eax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x40, 0x14, 0xC3 },				// return *(__int32 *)(this + 0x14);
				{ 0x8B, 0x41, 0x14, 0xC3, 0xCC },																	// mov eax, [rcx + 0x14]; retn; int3;
				{ 0x8B, 0x41, 0x14, 0x66, 0x90 }																	// mov eax, [rcx + 0x14]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x0F, 0xB6, 0x40, 0x08, 0xC3 },		// return ZERO_EXTEND(*(__int8 *)(this + 0x8));
				{ 0x0F, 0xB6, 0x41, 0x08, 0xC3 },																	// movzx eax, [rcx + 0x8]; retn;
				{ 0x0F, 0xB6, 0x41, 0x08, 0x90 }																	// movzx eax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x0F, 0xB6, 0x40, 0x26, 0xC3 },		// return ZERO_EXTEND(*(__int8 *)(this + 0x26));
				{ 0x0F, 0xB6, 0x41, 0x26, 0xC3 },																	// movzx eax, [rcx + 0x26]; retn;
				{ 0x0F, 0xB6, 0x41, 0x26, 0x90 }																	// movzx eax, [rcx + 0x26]; nop;
			},

			{
				{ 0x89, 0x54, 0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08, 0x8B, 0x44, 0x24, 0x10, 0x48, 0x8B, 0x4C, 0x24, 0x08, 0x0F, 0xB7, 0x04, 0x41, 0xC3 },	// return ZERO_EXTEND(*(unsigned __int16 *)(a1 + 2i64 * a2));
				{ 0x0F, 0xB7, 0x04, 0x51, 0xC3 },																												// movzx eax, word ptr ds:[rcx+rdx*2]; retn;
				{ 0x0F, 0xB7, 0x04, 0x51, 0x90 }																												// movzx eax, word ptr ds:[rcx+rdx*2]; nop;
			},
		};

		const NullsubPatch *FindNullsubPatch(uintptr_t SourceAddress, uintptr_t TargetFunction)
		{
			for (auto& patch : Patches)
			{
				if (!memcmp((void *)TargetFunction, patch.Signature.begin(), patch.Signature.size()))
					return &patch;
			}

			return nullptr;
		}

		bool PatchNullsub(uintptr_t SourceAddress, uintptr_t TargetFunction, const NullsubPatch *Patch)
		{
			const bool isJump = *(uint8_t *)SourceAddress == 0xE9;

			// Check if the given function is "unoptimized" and remove the branch completely
			if (!Patch)
				Patch = FindNullsubPatch(SourceAddress, TargetFunction);

			if (Patch)
			{
				if (isJump)
					memcpy((void *)SourceAddress, Patch->JumpPatch, 5);
				else
					memcpy((void *)SourceAddress, Patch->CallPatch, 5);

				return true;
			}

			return false;
		}

		uint64_t patch_edit_and_continue(const range_t *ranges)
		{
			//
			// Remove any references to the giant trampoline table generated for edit & continue
			//
			// Before: [Function call] -> [E&C trampoline] -> [Function]
			// After:  [Function call] -> [Function]
			//
			tbb::concurrent_vector<std::pair<uintptr_t, const NullsubPatch *>> nullsubTargets;
			tbb::concurrent_vector<uintptr_t> branchTargets;

			// Enumerate all functions present in the x64 exception directory section
			auto ntHeaders = (PIMAGE_NT_HEADERS64)(g_ModuleBase + ((PIMAGE_DOS_HEADER)g_ModuleBase)->e_lfanew);
			const auto sectionRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress;
			const auto sectionSize = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;

			Assert(sectionRVA > 0 && sectionSize > 0);

			auto functionEntries = (PRUNTIME_FUNCTION)(g_ModuleBase + sectionRVA);
			auto functionEntryCount = sectionSize / sizeof(RUNTIME_FUNCTION);

			// Init threadsafe instruction decoder
			ZydisDecoder decoder;
			Assert(ZYDIS_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)));

			XUtil::Parallel::for_each(&functionEntries[0], &functionEntries[functionEntryCount],
				[&branchTargets, &nullsubTargets, &decoder](const RUNTIME_FUNCTION& Function)
			{
				const uintptr_t ecTableStart = OFFSET(0x1005, 0);
				const uintptr_t ecTableEnd = OFFSET(0x86994, 0);

				for (uint32_t offset = Function.BeginAddress; offset < Function.EndAddress;)
				{
					const uintptr_t ip = g_ModuleBase + offset;
					const uint8_t opcode = *(uint8_t *)ip;
					ZydisDecodedInstruction instruction;

					if (!ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (void *)ip, ZYDIS_MAX_INSTRUCTION_LENGTH, ip, &instruction)))
					{
						// Decode failed. Always increase byte offset by 1.
						offset += 1;
						continue;
					}

					offset += instruction.length;

					// Must be a call or a jump
					if (opcode != 0xE9 && opcode != 0xE8)
						continue;

					uintptr_t destination = ip + *(int32_t *)(ip + 1) + 5;

					// if (destination is within E&C table)
					if (destination >= ecTableStart && destination < ecTableEnd && *(uint8_t *)destination == 0xE9)
					{
						// Determine where the E&C trampoline jumps to, then remove it. Each function is processed separately so thread
						// safety is not an issue when patching. The 0xE9 opcode never changes.
						uintptr_t real = destination + *(int32_t *)(destination + 1) + 5;

						int32_t disp = (int32_t)(real - ip) - 5;
						memcpy((void *)(ip + 1), &disp, sizeof(disp));

						if (auto patch = FindNullsubPatch(ip, real))
							nullsubTargets.emplace_back(ip, patch);
						else
							branchTargets.emplace_back(ip);
					}
				}
			});

			uint64_t patchCount = nullsubTargets.size() + branchTargets.size();
	
			for (auto patch : nullsubTargets)
			{
				
				uintptr_t destination = patch.first + *(int32_t *)(patch.first + 1) + 5;

				if (PatchNullsub(patch.first, destination, patch.second))
					patchCount++;
			}

			// Secondary pass to remove nullsubs missed or created above
			for (uintptr_t ip : branchTargets)
			{
				uintptr_t destination = ip + *(int32_t *)(ip + 1) + 5;

				if (PatchNullsub(ip, destination, nullptr))
					patchCount++;
			}

			return patchCount;
		}

		uint64_t patch_mem_init(const range_t *ranges)
		{
			//
			// Remove the thousands of [code below] since they're useless checks:
			//
			// if ( dword_141ED6C88 != 2 ) // MemoryManager initialized flag
			//     sub_140C00D30((__int64)&unk_141ED6800, &dword_141ED6C88);
			//
			std::vector<uintptr_t> matches = XUtil::FindPatterns(ranges[0].addr.Based, ranges[0].addr.End - ranges[0].addr.Based,
				"83 3D ? ? ? ? 02 74 13 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8");

			auto func = [](auto it) { memcpy((void *)it, "\xEB\x1A", 2); };

			std::vector<uintptr_t>::iterator match;
			XUtil::Parallel::for_each(match = matches.begin(), matches.end(), func);

			return matches.size();
		}
	}

	void __stdcall RunOptimizations(void)
	{
		using namespace std::chrono;
		auto timerStart = high_resolution_clock::now();
		
		range_t ranges[] = {
			{ { g_CodeBase, g_CodeEnd }, 0 },			// .text and .bsstext
			{ { g_RdataBase, g_RdataEnd }, 0 },			// .rdata
			{ { g_DataBase, g_DataEnd }, 0 }			// .data
		};

		// Mark every page as writable
		for (auto& range : ranges)
			Assert(VirtualProtect((void *)range.addr.Based, range.addr.End - range.addr.Based, PAGE_READWRITE, &range.protection));

		std::array<uint64_t, 2> counts
		{
			Nuukem::patch_mem_init(ranges),
			Nuukem::patch_edit_and_continue(ranges),
		};

		// Then restore the old permissions
		for (auto& range : ranges)
		{
			Assert(VirtualProtect((void *)range.addr.Based, range.addr.End - range.addr.Based, range.protection, &range.protection));
			Assert(FlushInstructionCache(GetCurrentProcess(), (void *)range.addr.Based, range.addr.End - range.addr.Based));
		}

		auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();
		
		LogWindow::Log("%s: (%llu + %llu) = %llu patches applied in %llums.\n", __FUNCTION__,
			counts[0], counts[1], counts[0] + counts[1], duration);
	}
}