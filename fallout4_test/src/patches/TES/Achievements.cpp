#include "..\..\common.h"

/*

Author: Perchik71 29/04/2021
This file is part of Fallout 4 Fixes source code.

*/

// clang-format off
struct
{
	const char* BytesToFind;
	const char* BytePatch;
	int PatchSize;
	int AddressModifier;
} const Patches[] =
{
	// https://github.com/Sumwunn/AchievementsModsEnabler
	{
		"C3 40 32 FF 48 89 5C 24 40 48 89 6C 24 48",
		"\xB0\x00\xC3",
		3,
		-0x29
	},

	// https://github.com/Sumwunn/AchievementsModsEnabler
	{ 
		"C3 C6 44 24 38 00 48 8D 44 24 38 48 89 5C 24 20",
		"\xB0\x00\xC3",
		3,
		-0x28
	},
};
// clang-format on


/*
==================
Fix_PatchAchievements

Gives you the opportunity to play with mods and get achievements
==================
*/
BOOL FIXAPI Fix_PatchAchievements(VOID)
{
	// Loop through each fix and exit on the first found
	for (auto& patch : Patches)
	{
		uintptr_t addr = XUtil::FindPattern(g_CodeBase, g_CodeEnd - g_CodeBase, patch.BytesToFind);

		if (!addr)
			continue;

		XUtil::PatchMemory((addr + patch.AddressModifier), (PBYTE)patch.BytePatch, patch.PatchSize);
		return TRUE;
	}

	return FALSE;
}
