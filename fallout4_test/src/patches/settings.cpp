/*
Author: Perchik71 29/04/2021
This file is part of Fallout 4 Fixes source code.

Adapted for Fallout 4 and Fallout 4 CK
The original
URL: https://github.com/Perchik71/SkyrimSETest/blob/master/skyrim64_test/src/patches/settings.cpp
*/

#include "../common.h"

#include <string>

// clang-format off
struct
{
	LPCSTR Section;
	LPCSTR Key;
	std::string Value;
} Settings[] =
{
	// '!' means the variable is never used in the engine. It's included here anyway.
	// '?' means used by engine. Description not written or use is unknown.

	// Fallout4_Default.ini
	{ "General",		"fDefaultFov",							"90.000"		},		//  0  //  Field of view
	{ "General",		"fDefaultWorldFOV",						"90.000"		},		//  1  //  ^
	{ "General",		"fDefault1stPersonFOV",					"90.000"		},		//  2  //  ^
	{ "General",		"sIntroSequence",						""				},		//  3  //  Disable Bethesda intro video
	{ "General",		"bModManagerMenuEnabled",				"0"				},		//  4  //! Don't show the mod manager on the main menu
	{ "General",		"uGridsToLoad",							"5"				},		//  5  //  Maximum active cells at once (higher => unstable)
	{ "General",		"uInterior Cell Buffer",				"16"			},		//  6  //  uGrid interior cell buffer
	{ "General",		"uExterior Cell Buffer",				"128"			},		//  7  //  uGrid world cell buffer
	{ "General",		"bPreloadIntroSequence",				"0"				},		//  8  //  We don't care about the intro video any more (sIntroSequence)
	{ "General",		"bPreloadLinkedInteriors",				"0"				},		//  9  //?
	{ "General",		"bBackgroundLoadVMData",				"0"				},		//  10 //?
	{ "General",		"bEnableFileCaching",					"0"				},		//  11 //  Cache files in memory. Causes crash.
	{ "General",		"iNumHWThreads",						"12"			},		//  12 //! Has one reference, never used otherwise
	{ "General",		"bBorderRegionsEnabled",				"0"				},		//  13 //  Unlock map limits / disable "You cannot go this way"
	{ "General",		"bAnimateDoorPhysics",					"1"				},		//  14 //  ???
	{ "General",		"bFlyingMountFastTravelCruiseEnabled",	"1"				},		//  15 //  Fly on a dragon to fast travel destination
	{ "General",		"bAlwaysActive",						"1"				},		//  16 //  Don't pause the game, even if minimized/in the background
	{ "General",		"iFPSClamp",							"0"				},		//  17 //  Unlock internal FPS limiter #1
	{ "General",		"bPreemptivelyUnloadCells",				"1"				},		//  18 //?

	{ "Display",		"fSunShadowUpdateTime",					"1"				},		//  19 //  Smooth sun shadow transitions
	{ "Display",		"fSunUpdateThreshold",					"0"				},		//  20 //  http://forum.step-project.com/topic/9209-sun-shadow-transition-ini-settings/
	{ "Display",		"iVSyncPresentInterval",				"1"				},		//  21 //  Enable D3D11::Present() VSync
	{ "Display",		"bLockFramerate",						"1"				},		//  22 //  Lock internal FPS limiter #2
	{ "Display",		"bCompensateUnstableFrameTime",			"0"				},		//  23 //! Disable game tweaks from shader compile hitching
	{ "Display",		"iUnstableFrameTimeHistorySize",		"1"				},		//  24 //! ^
	{ "Display",		"bDeactivateAOOnSnow",					"0"				},		//  25 //?
	{ "Display",		"bEnableParallaxOcclusion",				"1"				},		//  26 //?
	{ "Display",		"bEnableSnowRimLighting",				"0"				},		//  27 //  Prevent weird glowing on snowy object edges at night
	{ "Display",		"bEnableDownsampleComputeShader",		"1"				},		//  28 //  ???
	{ "Display",		"bEnableFrontToBackPrepass",			"0"				},		//  29 //  Don't enable. Used if your GPU is significantly weaker than your CPU.
	{ "Display",		"bActorSelfShadowing",					"1"				},		//  30 //?
	{ "Display",		"bDrawLandShadows",						"1"				},		//  31 //?
	{ "Display",		"uLargeRefLODGridSize",					"21"			},		//  32 //  Maximum high LOD cells to keep active
	{ "Display",		"fShadowLODMaxStartFade",				"20000.000"		},		//  33 //  https://www.nexusmods.com/fallout4/mods/81/
	{ "Display",		"fSpecularLODMaxStartFade",				"20000.000"		},		//  34 //  ^ 
	{ "Display",		"iShadowMapResolutionPrimary",			"8192"			},		//  35 //  ^ 
	{ "Display",		"bSAOEnable",							"1"				},		//  36 //  Screen Space Ambient Occlusion. 
	{ "Display",		"fSAORadius",							"150"			},		//  37 //  ^ 
	{ "Display",		"fSAOBias",								"0"				},		//  38 //  ^ 
	{ "Display",		"fSAOIntensity",						"7"				},		//  39 //  ^ 
	{ "Display",		"fDecalLifetime",						"180"			},		//  40 //  Decals last for 3 minutes instead of 30 seconds

	// https://www.reddit.com/r/FalloutMods/comments/4wcj4j/fo4_cant_find_a_mod_to_center_3rd_person_camera/
	{ "Camera",			"f1rdPersonAimFOV",						"90.000"		},		//  Field of view 1rd
	{ "Camera",			"f3rdPersonAimFOV",						"70.000"		},		//  Field of view 3rd
	{ "Camera",			"f3rdPersonAimDist",					"50.000"		},		//? 
	{ "Camera",			"f3rdPersonAimDollySpeed",				"100.000"		},		//? 
	{ "Camera",			"f1st3rdSwitchDelay",					"0.050"			},		//? 
	{ "Camera",			"fVanityModeMinDist",					"150.000"		},		//?  
	{ "Camera",			"fVanityModeMaxDist",					"500.000"		},		//?  
	{ "Camera",			"fPitchZoomOutMaxDist",					"50.000"		},		//?  
	{ "Camera",			"fOverShoulderMeleeCombatAddY",			"0.500"			},		//  combat mode: Melee
	{ "Camera",			"fOverShoulderMeleeCombatPosZ",			"15.000"		},		//  ^ 
	{ "Camera",			"fOverShoulderMeleeCombatPosX",			"15.000"		},		//  ^
	{ "Camera",			"fOverShoulderCombatAddY",				"0.5000"		},		//  combat mode: Default  
	{ "Camera",			"fOverShoulderCombatPosZ",				"25.000"		},		//  ^
	{ "Camera",			"fOverShoulderCombatPosX",				"20.000"		},		//  ^
	{ "Camera",			"fOverShoulderPosZ",					"15.00"			},		//  idle
	{ "Camera",			"fOverShoulderPosX",					"0.000"			},		//  ^ 
	{ "Camera",			"bApplyCameraNodeAnimations",			"0"				},		//?  
	
	{ "Archive",		"bLoadEsmInMemory",						"1"				},		//  Cache ESMs in memory
	{ "Archive",		"bLoadArchiveInMemory",					"1"				},		//  Cache 'sArchiveToLoadInMemoryList' archives in memory
	{ "Archive",		"bForceAsync",							"0"				},		//? Causes crash
	{ "Archive",		"bKeepDLStringBlocksLoaded",			"1"				},		//?
	{ "Archive",		"bKeepILStringBlocksLoaded",			"1"				},		//?

	{ "Papyrus",		"iMaxAllocatedMemoryBytes",				"8388608"		},		//  Increase max script memory (8MB)
	{ "Papyrus",		"iMaxMemoryPageSize",					"1024"			},		//  Max single allocation size
	{ "Papyrus",		"iMinMemoryPageSize",					"256"			},		//  Min single allocation size
	{ "Papyrus",		"fArchiveInitBufferMB",					"8"				},		//! Has one reference, nullsub

	{ "Audio",			"uiInitialCacheSize",					"2097152"		},		//  Was 1MB, now 2MB
	{ "Audio",			"uiMaxAudioCacheSize",					"4194304"		},		//  Was 2MB, now 4MB
	{ "Audio",			"uMaxSizeForCachedSound",				"524288"		},		//  Was 256KB, now 512KB
	{ "Audio",			"uStreamingThreshold",					"5242880"		},		//  Was 4MB, now 5MB

	{ "Havok",			"fMaxTime",								"0.0111"		},		//  Physics frame time (90 FPS = 1 / 0.0111)
	{ "Havok",			"fMaxTimeComplex",						"0.0111"		},		//  Complex physics frame time
	{ "Havok",			"iNumThreads",							"2"				},		//! No references
	{ "Havok",			"fInAirFallingCharGravityMult",			"4"				},		//  More realistic player gravity

	{ "BackgroundLoad",	"bBackgroundCellLoads",					"0"				},		//?
	{ "BackgroundLoad",	"bLoadHelmetsInBackground",				"0"				},		//?
	{ "BackgroundLoad",	"bUseMultiThreadedFaceGen",				"0"				},		//?

	{ "Pathfinding",	"bBackgroundPathing",					"1"				},		//?
	{ "Pathfinding",	"bBackgroundNavmeshUpdate",				"1"				},		//?

	{ "Water",			"bReflectLODObjects",					"1"				},		//  1 activates the reflections.
	{ "Water",			"bReflectLODLand",						"1"				},		//  ^ 
	{ "Water",			"bReflectSky",							"1"				},		//  ^ 
	{ "Water",			"bReflectLODTrees",						"1"				},		//  ^

	{ "TerrainManager",	"fBlockLevel0Distance",					"75000.000"		},
	{ "TerrainManager",	"fBlockLevel1Distance",					"140000.000"	},
	{ "TerrainManager",	"fBlockMaximumDistance",				"500000.000"	},
	{ "TerrainManager",	"fSplitDistanceMult",					"1.5000"		},

	{ "Animation",		"bInitiallyLoadAllClips",				"0"				},		//  BREAKS ALL ANIMS - don't delay-load animations as needed. Load all up front. (IAnimationClipLoaderSingleton)

	{ "GamePlay",		"bAllowDragonFlightLocationDiscovery",	"1"				},		//  Discover new locations while on said dragon
	{ "GamePlay",		"bEssentialTakeNoDamage",				"1"				},		//  ???

	{ "Interface",		"bUseAllNonDefaultLoadScreensFirst",	"1"				},		//  "Makes the loading screens slightly less boring. Shows less common loading screen items more often."

	{ "Landscape",		"fLandFriction",						"30"			},		//  More realistic friction (terrain only, excludes roads)

	{ "Combat",			"f1PArrowTiltUpAngle",					"0.8"			},		//  Corrects the arrow tilt which makes arrows and bolts fire higher than they should,
	{ "Combat",			"f1PBoltTiltUpAngle",					"0.4"			},		//  ...as compared to the relative placement of the aiming reticule.
	{ "Combat",			"f3PArrowTiltUpAngle",					"2.5"			},		//  Vanilla value - effect unknown.

	{ "LOD",			"fLODFadeOutMultObjects",				"30.000"		},		//  Probably Lod ranges for those to disappear.
	{ "LOD",			"fLODFadeOutMultItems",					"10.000"		},		//  ^ 
	{ "LOD",			"fLODFadeOutMultActors",				"15.000"		},		//  ^ 

	{ "Grass",			"iMinGrassSize ",						"15"			},		//  Setting this lower should give you denser grass.

	{ "Imagespace",		"bDoDepthOfField",						"0"				},
	
	{ "Particles",		"iMaxDesired",							"3000"			},		// Bumped up from the 750 default
};
// clang-format on


/*
==================
Sys_LoadSettingFromIni
==================
*/
VOID FIXAPI Sys_LoadSettingFromIni(VOID)
{
	Settings[0].Value = g_INI.Get("Tweaks", "General_fDefaultFov", "90.000");
	Settings[1].Value = g_INI.Get("Tweaks", "General_fDefaultWorldFOV", "90.000");
	Settings[2].Value = g_INI.Get("Tweaks", "General_fDefault1stPersonFOV", "90.000");
	Settings[3].Value = g_INI.Get("Tweaks", "General_sIntroSequence", "");
	Settings[4].Value = g_INI.Get("Tweaks", "General_bModManagerMenuEnabled", "0");
	Settings[5].Value = g_INI.Get("Tweaks", "General_uGridsToLoad", "5");
	Settings[6].Value = g_INI.Get("Tweaks", "General_uInterior_Cell_Buffer", "16");
	Settings[7].Value = g_INI.Get("Tweaks", "General_uExterior_Cell_Buffer", "128");
	Settings[8].Value = g_INI.Get("Tweaks", "General_bPreloadIntroSequence", "0");
	Settings[9].Value = g_INI.Get("Tweaks", "General_bPreloadLinkedInteriors", "0");
	Settings[10].Value = g_INI.Get("Tweaks", "General_bBackgroundLoadVMData", "0");
	Settings[11].Value = g_INI.Get("Tweaks", "General_bEnableFileCaching", "0");
	Settings[12].Value = g_INI.Get("Tweaks", "General_iNumHWThreads", "12");
	Settings[13].Value = g_INI.Get("Tweaks", "General_bBorderRegionsEnabled", "0");
	Settings[14].Value = g_INI.Get("Tweaks", "General_bAnimateDoorPhysics", "1");
	Settings[15].Value = g_INI.Get("Tweaks", "General_bFlyingMountFastTravelCruiseEnabled", "1");
	Settings[16].Value = g_INI.Get("Tweaks", "General_bAlwaysActive", "1");
	Settings[17].Value = g_INI.Get("Tweaks", "General_iFPSClamp", "0");
	Settings[18].Value = g_INI.Get("Tweaks", "General_bPreemptivelyUnloadCells", "1");

	Settings[19].Value = g_INI.Get("Tweaks", "Display_fSunShadowUpdateTime", "1");
	Settings[20].Value = g_INI.Get("Tweaks", "Display_fSunUpdateThreshold", "0");
	Settings[21].Value = g_INI.Get("Tweaks", "Display_iVSyncPresentInterval", "1");
	Settings[22].Value = g_INI.Get("Tweaks", "Display_bLockFramerate", "1");
	Settings[23].Value = g_INI.Get("Tweaks", "Display_bCompensateUnstableFrameTime", "0");
	Settings[24].Value = g_INI.Get("Tweaks", "Display_iUnstableFrameTimeHistorySize", "1");
	Settings[25].Value = g_INI.Get("Tweaks", "Display_bDeactivateAOOnSnow", "0");
	Settings[26].Value = g_INI.Get("Tweaks", "Display_bEnableParallaxOcclusion", "1");
	Settings[27].Value = g_INI.Get("Tweaks", "Display_bEnableSnowRimLighting", "0");
	Settings[28].Value = g_INI.Get("Tweaks", "Display_bEnableDownsampleComputeShader", "1");
	Settings[29].Value = g_INI.Get("Tweaks", "Display_bEnableFrontToBackPrepass", "0");
	Settings[30].Value = g_INI.Get("Tweaks", "Display_bActorSelfShadowing", "1");
	Settings[31].Value = g_INI.Get("Tweaks", "Display_bDrawLandShadows", "1");
	Settings[32].Value = g_INI.Get("Tweaks", "Display_uLargeRefLODGridSize", "21");
	Settings[33].Value = g_INI.Get("Tweaks", "Display_fShadowLODMaxStartFade", "20000.000");
	Settings[34].Value = g_INI.Get("Tweaks", "Display_fSpecularLODMaxStartFade", "20000.000");
	Settings[35].Value = g_INI.Get("Tweaks", "Display_iShadowMapResolutionPrimary", "8192");
	Settings[36].Value = g_INI.Get("Tweaks", "Display_bSAOEnable", "1");
	Settings[37].Value = g_INI.Get("Tweaks", "Display_fSAORadius", "150");
	Settings[38].Value = g_INI.Get("Tweaks", "Display_fSAOBias", "0");
	Settings[39].Value = g_INI.Get("Tweaks", "Display_fSAOIntensity", "7");
	Settings[40].Value = g_INI.Get("Tweaks", "Display_fDecalLifetime", "180");

	Settings[41].Value = g_INI.Get("Tweaks", "Camera_f1rdPersonAimFOV", "90.000");
	Settings[42].Value = g_INI.Get("Tweaks", "Camera_f3rdPersonAimFOV", "70.000");
	Settings[43].Value = g_INI.Get("Tweaks", "Camera_f3rdPersonAimDist", "50.000");
	Settings[44].Value = g_INI.Get("Tweaks", "Camera_f3rdPersonAimDollySpeed", "100.000");
	Settings[45].Value = g_INI.Get("Tweaks", "Camera_f1st3rdSwitchDelay", "0.050");
	Settings[46].Value = g_INI.Get("Tweaks", "Camera_fVanityModeMinDist", "100.000");
	Settings[47].Value = g_INI.Get("Tweaks", "Camera_fVanityModeMaxDist", "400.000");
	Settings[48].Value = g_INI.Get("Tweaks", "Camera_fPitchZoomOutMaxDist", "50.000");
	Settings[49].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderMeleeCombatAddY", "0.500");
	Settings[50].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderMeleeCombatPosZ", "15.000");
	Settings[51].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderMeleeCombatPosX", "15.000");
	Settings[52].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderCombatAddY", "0.5000");
	Settings[53].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderCombatPosZ", "25.000");
	Settings[54].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderCombatPosX", "20.000");
	Settings[55].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderPosZ", "15.000");
	Settings[56].Value = g_INI.Get("Tweaks", "Camera_fOverShoulderPosX", "0");
	Settings[57].Value = g_INI.Get("Tweaks", "Camera_bApplyCameraNodeAnimations", "0");
	
	Settings[58].Value = g_INI.Get("Tweaks", "Archive_bLoadEsmInMemory", "1");
	Settings[59].Value = g_INI.Get("Tweaks", "Archive_bLoadArchiveInMemory", "1");
	Settings[60].Value = g_INI.Get("Tweaks", "Archive_bForceAsync", "0");
	Settings[61].Value = g_INI.Get("Tweaks", "Archive_bKeepDLStringBlocksLoaded", "1");
	Settings[62].Value = g_INI.Get("Tweaks", "Archive_bKeepILStringBlocksLoaded", "1");
	
	Settings[63].Value = g_INI.Get("Tweaks", "Papyrus_iMaxAllocatedMemoryBytes", "8388608");
	Settings[64].Value = g_INI.Get("Tweaks", "Papyrus_iMaxMemoryPageSize", "1024");
	Settings[65].Value = g_INI.Get("Tweaks", "Papyrus_iMinMemoryPageSize", "256");
	Settings[66].Value = g_INI.Get("Tweaks", "Papyrus_fArchiveInitBufferMB", "8");

	Settings[67].Value = g_INI.Get("Tweaks", "Audio_uiInitialCacheSize", "2097152");
	Settings[68].Value = g_INI.Get("Tweaks", "Audio_uiMaxAudioCacheSize", "4194304");
	Settings[69].Value = g_INI.Get("Tweaks", "Audio_uMaxSizeForCachedSound", "524288");
	Settings[70].Value = g_INI.Get("Tweaks", "Audio_uStreamingThreshold", "5242880");

	Settings[71].Value = g_INI.Get("Tweaks", "Havok_fMaxTime", "0.0111");
	Settings[72].Value = g_INI.Get("Tweaks", "Havok_fMaxTimeComplex", "0.0111");
	Settings[73].Value = g_INI.Get("Tweaks", "Havok_iNumThreads", "2");
	Settings[74].Value = g_INI.Get("Tweaks", "Havok_fInAirFallingCharGravityMult", "4");

	Settings[75].Value = g_INI.Get("Tweaks", "BackgroundLoad_bBackgroundCellLoads", "0");
	Settings[76].Value = g_INI.Get("Tweaks", "BackgroundLoad_bLoadHelmetsInBackground", "0");
	Settings[77].Value = g_INI.Get("Tweaks", "BackgroundLoad_bUseMultiThreadedFaceGen", "0");

	Settings[78].Value = g_INI.Get("Tweaks", "Pathfinding_bBackgroundPathing", "1");
	Settings[79].Value = g_INI.Get("Tweaks", "Pathfinding_bBackgroundNavmeshUpdate", "1");

	Settings[80].Value = g_INI.Get("Tweaks", "Water_bReflectLODObjects", "1");
	Settings[81].Value = g_INI.Get("Tweaks", "Water_bReflectLODLand", "1");
	Settings[82].Value = g_INI.Get("Tweaks", "Water_bReflectSky", "1");
	Settings[83].Value = g_INI.Get("Tweaks", "Water_bReflectLODTrees", "1");

	Settings[84].Value = g_INI.Get("Tweaks", "TerrainManager_fBlockLevel0Distance", "75000.000");
	Settings[85].Value = g_INI.Get("Tweaks", "TerrainManager_fBlockLevel1Distance", "140000.000");
	Settings[86].Value = g_INI.Get("Tweaks", "TerrainManager_fBlockMaximumDistance", "500000.000");
	Settings[87].Value = g_INI.Get("Tweaks", "TerrainManager_fSplitDistanceMult", "1.500");

	Settings[88].Value = g_INI.Get("Tweaks", "Animation_bInitiallyLoadAllClips", "0");

	Settings[89].Value = g_INI.Get("Tweaks", "GamePlay_bAllowDragonFlightLocationDiscovery", "1");
	Settings[90].Value = g_INI.Get("Tweaks", "GamePlay_bEssentialTakeNoDamage", "1");

	Settings[91].Value = g_INI.Get("Tweaks", "Interface_bUseAllNonDefaultLoadScreensFirst", "1");

	Settings[92].Value = g_INI.Get("Tweaks", "Landscape_fLandFriction", "30");

	Settings[93].Value = g_INI.Get("Tweaks", "Combat_f1PArrowTiltUpAngle", "0.8");
	Settings[94].Value = g_INI.Get("Tweaks", "Combat_f1PBoltTiltUpAngle", "0.4");
	Settings[95].Value = g_INI.Get("Tweaks", "Combat_f3PArrowTiltUpAngle", "2.5");

	Settings[96].Value = g_INI.Get("Tweaks", "LOD_fLODFadeOutMultObjects", "30.000");
	Settings[97].Value = g_INI.Get("Tweaks", "LOD_fLODFadeOutMultItems", "10.000");
	Settings[98].Value = g_INI.Get("Tweaks", "LOD_fLODFadeOutMultActors", "15.000");

	Settings[99].Value = g_INI.Get("Tweaks", "Grass_iMinGrassSize", "15");

	Settings[100].Value = g_INI.Get("Tweaks", "Imagespace_bDoDepthOfField", "0");

	Settings[101].Value = g_INI.Get("Tweaks", "Particles_iMaxDesired", "3000");
}

/*
==================
hk_GetPrivateProfileStringA

Replacement WINAPI GetPrivateProfileStringA
==================
*/
DWORD WINAPI hk_GetPrivateProfileStringA(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName)
{
	// Check for overrides first
	for (auto& setting : Settings)
	{
		if (!_stricmp(lpAppName, setting.Section) && !_stricmp(lpKeyName, setting.Key))
		{
			strcpy_s(lpReturnedString, nSize, setting.Value.c_str());
			return (DWORD)setting.Value.length();
		}
	}

	return GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
}

/*
==================
hk_GetPrivateProfileIntA

Replacement WINAPI GetPrivateProfileIntA
==================
*/
UINT WINAPI hk_GetPrivateProfileIntA(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName)
{
	// Check for overrides first
	for (auto& setting : Settings)
	{
		if (!_stricmp(lpAppName, setting.Section) && !_stricmp(lpKeyName, setting.Key))
			return atoi(setting.Value.c_str());
	}

	return GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);
}

/*
==================
Fix_PatchSettings

Reconfigures the game
==================
*/
VOID FIXAPI Fix_PatchSettings(VOID)
{
	Sys_LoadSettingFromIni();

	PatchIAT(hk_GetPrivateProfileStringA, "kernel32.dll", "GetPrivateProfileStringA");
	PatchIAT(hk_GetPrivateProfileIntA, "kernel32.dll", "GetPrivateProfileIntA");
}