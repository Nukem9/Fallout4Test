//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
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

#pragma once
#pragma pack(push, 1)

#include "TESActorValue.h"
#include "TESObjectSTAT.h"
#include "TESObjectMSTT.h"
#include "TESObjectSCOL.h"
#include "TESObjectCELL.h"
#include "TESObjectREFR.h"
#include "BGSLayer.h"

class TESDataHandler {
public:
	struct ModList {
		BSSimpleList<TESFile> modInfoList;		// 00
		BSTArray<TESFile*> loadedMods;			// 10
	};

	using UnkArray = BSTArray<LPVOID>;
	using TESFileList = BSSimpleList<TESFile>;
	using TESFileArray = BSTArray<TESFile*>;
private:
	INT64 unk00;							// 00
	UnkArray unk08;							// 08
	UnkArray unk20;							// 20
	UnkArray unk38;							// 38
	UnkArray unk50;							// 50
public:
	TESFormArray arrNONE;					// 68 Form Type 0
	TESFormArray arrTES4;					// 80 Form Type 1
	TESFormArray arrGRUP;					// 98 Form Type 2
	TESFormArray arrGMST;					// B0 Form Type 3
	TESFormArray arrKYWD;					// C8 Form Type 4
	TESFormArray arrLCRT;					// E0 Form Type 5
	TESFormArray arrAACT;					// F8 Form Type 6
	TESFormArray arrTRNS;					// 110 Form Type 7
	TESFormArray arrCMPO;					// 128 Form Type 8
	TESFormArray arrTXST;					// 140 Form Type 9
	TESFormArray arrMICN;					// 158 Form Type 10
	TESFormArray arrGLOB;					// 170 Form Type 11
	TESFormArray arrDMGT;					// 188 Form Type 12
	TESFormArray arrCLAS;					// 1A0 Form Type 13
	TESFormArray arrFACT;					// 1B8 Form Type 14
	TESFormArray arrHDPT;					// 1D0 Form Type 15
	TESFormArray arrEYES;					// 1E8 Form Type 16
	TESFormArray arrRACE;					// 200 Form Type 17
	TESFormArray arrSOUN;					// 218 Form Type 18
	TESFormArray arrASPC;					// 230 Form Type 19
	TESFormArray arrSKIL;					// 248 Form Type 20
	TESFormArray arrMGEF;					// 260 Form Type 21
	TESFormArray arrSCPT;					// 278 Form Type 22
	TESFormArray arrLTEX;					// 290 Form Type 23
	TESFormArray arrENCH;					// 2A8 Form Type 24
	TESFormArray arrSPEL;					// 2C0 Form Type 25
	TESFormArray arrSCRL;					// 2D8 Form Type 26
	TESFormArray arrACTI;					// 2F0 Form Type 27
	TESFormArray arrTACT;					// 308 Form Type 28
	TESFormArray arrARMO;					// 320 Form Type 29
	TESFormArray arrBOOK;					// 338 Form Type 30
	TESFormArray arrCONT;					// 350 Form Type 31
	TESFormArray arrDOOR;					// 368 Form Type 32
	TESFormArray arrINGR;					// 380 Form Type 33
	TESFormArray arrLIGH;					// 398 Form Type 34
	TESFormArray arrMISC;					// 3B0 Form Type 35
	BSTArray<TESObjectSTAT*> arrSTAT;		// 3C8 Form Type 36  
	BSTArray<TESObjectSCOL*> arrSCOL;		// 3E0 Form Type 37 
	BSTArray<TESObjectMSTT*> arrMSTT;		// 3F8 Form Type 38 
	TESFormArray arrGRAS;					// 410 Form Type 39
	TESFormArray arrTREE;					// 428 Form Type 40
	TESFormArray arrFLOR;					// 440 Form Type 41
	TESFormArray arrFURN;					// 458 Form Type 42
	TESFormArray arrWEAP;					// 470 Form Type 43
	TESFormArray arrAMMO;					// 488 Form Type 44
	TESFormArray arrNPC_;					// 4A0 Form Type 45
	TESFormArray arrLVLN;					// 4B8 Form Type 46
	TESFormArray arrKEYM;					// 4D0 Form Type 47
	TESFormArray arrALCH;					// 4E8 Form Type 48
	TESFormArray arrIDLM;					// 500 Form Type 49
	TESFormArray arrNOTE;					// 518 Form Type 50
	TESFormArray arrPROJ;					// 530 Form Type 51
	TESFormArray arrHAZD;					// 548 Form Type 52
	TESFormArray arrBNDS;					// 560 Form Type 53
	TESFormArray arrSLGM;					// 578 Form Type 54
	TESFormArray arrTERM;					// 590 Form Type 55
	TESFormArray arrLVLI;					// 5A8 Form Type 56
	TESFormArray arrWTHR;					// 5C0 Form Type 57
	TESFormArray arrCLMT;					// 5D8 Form Type 58
	TESFormArray arrSPGD;					// 5F0 Form Type 59
	TESFormArray arrRFCT;					// 608 Form Type 60
	TESFormArray arrREGN;					// 620 Form Type 61
	TESFormArray arrNAVI;					// 638 Form Type 62
	BSTArray<TESObjectCELL*> arrCELL;		// 650 Form Type 63
	BSTArray<TESObjectREFR*> arrREFR;		// 668 Form Type 64
	TESFormArray arrACHR;					// 680 Form Type 65
	TESFormArray arrPMIS;					// 698 Form Type 66
	TESFormArray arrPARW;					// 6B0 Form Type 67
	TESFormArray arrPGRE;					// 6C8 Form Type 68
	TESFormArray arrPBEA;					// 6E0 Form Type 69
	TESFormArray arrPFLA;					// 6F8 Form Type 70
	TESFormArray arrPCOM;					// 710 Form Type 71
	TESFormArray arrPBAR;					// 728 Form Type 72
	TESFormArray arrPHZD;					// 740 Form Type 73
	TESFormArray arrWRLD;					// 758 Form Type 74
	TESFormArray arrLAND;					// 770 Form Type 75
	TESFormArray arrNAVM;					// 788 Form Type 76
	TESFormArray arrTLOD;					// 7A0 Form Type 77
	TESFormArray arrDIAL;					// 7B8 Form Type 78
	TESFormArray arrINFO;					// 7D0 Form Type 79
	TESFormArray arrQUST;					// 7E8 Form Type 80
	TESFormArray arrIDLE;					// 800 Form Type 81
	TESFormArray arrPACK;					// 818 Form Type 82
	TESFormArray arrCSTY;					// 830 Form Type 83
	TESFormArray arrLSCR;					// 848 Form Type 84
	TESFormArray arrLVSP;					// 860 Form Type 85
	TESFormArray arrANIO;					// 878 Form Type 86
	TESFormArray arrWATR;					// 890 Form Type 87
	TESFormArray arrEFSH;					// 8A8 Form Type 88
	TESFormArray arrTOFT;					// 8C0 Form Type 89
	TESFormArray arrEXPL;					// 8D8 Form Type 90
	TESFormArray arrDEBR;					// 8F0 Form Type 91
	TESFormArray arrIMGS;					// 908 Form Type 92
	TESFormArray arrIMAD;					// 920 Form Type 93
	TESFormArray arrFLST;					// 938 Form Type 94
	TESFormArray arrPERK;					// 950 Form Type 95
	TESFormArray arrBPTD;					// 968 Form Type 96
	TESFormArray arrADDN;					// 980 Form Type 97
	BSTArray<TESActorValue*> arrAVIF;		// 998 Form Type 98 
	TESFormArray arrCAMS;					// 9B0 Form Type 99
	TESFormArray arrCPTH;					// 9C8 Form Type 100
	TESFormArray arrVTYP;					// 9E0 Form Type 101
	TESFormArray arrMATT;					// 9F8 Form Type 102
	TESFormArray arrIPCT;					// A10 Form Type 103
	TESFormArray arrIPDS;					// A28 Form Type 104
	TESFormArray arrARMA;					// A40 Form Type 105
	TESFormArray arrECZN;					// A58 Form Type 106
	TESFormArray arrLCTN;					// A70 Form Type 107
	TESFormArray arrMESG;					// A88 Form Type 108
	TESFormArray arrRGDL;					// AA0 Form Type 109
	TESFormArray arrDOBJ;					// AB8 Form Type 110
	TESFormArray arrDFOB;					// AD0 Form Type 111
	TESFormArray arrLGTM;					// AE8 Form Type 112
	TESFormArray arrMUSC;					// B00 Form Type 113
	TESFormArray arrFSTP;					// B18 Form Type 114
	TESFormArray arrFSTS;					// B30 Form Type 115
	TESFormArray arrSMBN;					// B48 Form Type 116
	TESFormArray arrSMQN;					// B60 Form Type 117
	TESFormArray arrSMEN;					// B78 Form Type 118
	TESFormArray arrDLBR;					// B90 Form Type 119
	TESFormArray arrMUST;					// BA8 Form Type 120
	TESFormArray arrDLVW;					// BC0 Form Type 121
	TESFormArray arrWOOP;					// BD8 Form Type 122
	TESFormArray arrSHOU;					// BF0 Form Type 123
	TESFormArray arrEQUP;					// C08 Form Type 124
	TESFormArray arrRELA;					// C20 Form Type 125
	TESFormArray arrSCEN;					// C38 Form Type 126
	TESFormArray arrASTP;					// C50 Form Type 127
	TESFormArray arrOTFT;					// C68 Form Type 128
	TESFormArray arrARTO;					// C80 Form Type 129
	TESFormArray arrMATO;					// C98 Form Type 130
	TESFormArray arrMOVT;					// CB0 Form Type 131
	TESFormArray arrSNDR;					// CC8 Form Type 132
	TESFormArray arrDUAL;					// CE0 Form Type 133
	TESFormArray arrSNCT;					// CF8 Form Type 134
	TESFormArray arrSOPM;					// D10 Form Type 135
	TESFormArray arrCOLL;					// D28 Form Type 136
	TESFormArray arrCLFM;					// D40 Form Type 137
	TESFormArray arrREVB;					// D58 Form Type 138
	TESFormArray arrPKIN;					// D70 Form Type 139
	TESFormArray arrRFGP;					// D88 Form Type 140
	TESFormArray arrAMDL;					// DA0 Form Type 141
	BSTArray<BGSLayer*> arrLAYR;			// DB8 Form Type 142
	TESFormArray arrCOBJ;					// DD0 Form Type 143
	TESFormArray arrOMOD;					// DE8 Form Type 144
	TESFormArray arrMSWP;					// E00 Form Type 145
	TESFormArray arrZOOM;					// E18 Form Type 146
	TESFormArray arrINNR;					// E30 Form Type 147
	TESFormArray arrKSSM;					// E48 Form Type 148
	TESFormArray arrAECH;					// E60 Form Type 149
	TESFormArray arrSCCO;					// E78 Form Type 150
	TESFormArray arrAORU;					// E90 Form Type 151
	TESFormArray arrSCSN;					// EA8 Form Type 152
	TESFormArray arrSTAG;					// EC0 Form Type 153
	TESFormArray arrNOCM;					// ED8 Form Type 154
	TESFormArray arrLENS;					// EF0 Form Type 155
	TESFormArray arrLSPR;					// F08 Form Type 156
	TESFormArray arrGDRY;					// F20 Form Type 157
	TESFormArray arrOVIS;					// F38 Form Type 158
private:
	LPVOID UnkPtrF50;						// F50
public:
	NiTArray<TESObjectCELL*> cellList;		// F58
	NiTArray<TESForm*> addonNodes;			// F70			
private:
	DWORD64 unkF88;							// F88
	DWORD64 unkF90;							// F90
	DWORD64 unkF98;							// F98
	DWORD unkFA0;							// FA0 - FormID?
	DWORD unkFA4;							// FA4
	TESFile* activePlugin;					// FA8
public:
	ModList modList;						// FB0
public:
	bool Load(int Unknown);
	bool InitUnknownDataSetTextStatusBar(void);
public:
	INLINE const TESFileList* GetMods(VOID) const { return &modList.modInfoList; }
	INLINE const TESFileArray* GetLoadedMods(VOID) const { return &modList.loadedMods; }
	INLINE TESFile* GetActiveMod(VOID) const { return activePlugin; }
	INLINE BOOL IsActiveMod(VOID) const { return activePlugin != NULL; }
	BOOL IsLoaded(VOID) const;
public:
	inline static BOOL(*SaveTESFile)(TESDataHandler*, LPCSTR);
public:
	static VOID Initialize(VOID);
public:
	READ_PROPERTY(GetMods) TESFileList* Mods;
	READ_PROPERTY(GetLoadedMods) const TESFileArray* LoadedMods;
	READ_PROPERTY(GetActiveMod) TESFile* ActiveMod;
};

extern TESDataHandler* FileHandler;

/*
==================
TESUnknown_6D54CF8

As I understood the structure, but it can also be a class (without a description?).
The object itself is permanently located at RAV+6D54CF8.
I was able to identify two structures in it, one containing information about the Cell,
the other containing information about fog, light, whether the sky is on, etc.
==================
*/
class TESUnknown_6D54CF8 {
public:
	/*
	==================
	TESCellViewSceneRenderInfo

	The object contains the current render state, as long as the sky is interested.
	==================
	*/
	class TESRenderInfo {
	public:
		BOOL IsSky(VOID) const;
	};
private:
	CHAR _pad1[0x58];
	TESObjectCELL* cell_int_info;
	CHAR _pad2[0x38];
	TESRenderInfo* render_info;
public:
	BOOL IsEmpty(VOID) const;
	inline BOOL IsLocationCell(VOID) const { return cell_int_info == NULL; }
	inline BOOL IsInteriorsCell(VOID) const { return cell_int_info != NULL; }
	inline TESObjectCELL* GetInterios(VOID) const { return cell_int_info; }
	inline TESRenderInfo* GetRenderInfo(VOID) const { return render_info; }
public:
	static TESUnknown_6D54CF8* GetInstance(VOID);
public:
	~TESUnknown_6D54CF8(VOID);
public:
	READ_PROPERTY(GetInterios) TESObjectCELL* Interios;
	READ_PROPERTY(GetRenderInfo) TESRenderInfo* RenderInfo;
};

#pragma pack(pop)
