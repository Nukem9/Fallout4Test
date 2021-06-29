#pragma once

#include "..\..\common.h"
#include "TESForm_CK.h"

/*
==================
TESCellViewSceneRenderInfo_CK

The object contains the current render state, as long as the sky is interested
==================
*/
class TESCellViewSceneRenderInfo_CK
{
public:
	BOOL IsSky(VOID) const;
};

/*
==================
TESCellViewScene_CK

As I understood the structure, but it can also be a class (without a description?). 
The object itself is permanently located at RAV+6D54CF8.
I was able to identify two structures in it, one containing information about the Cell, 
the other containing information about fog, light, whether the sky is on, etc.
==================
*/
class TESCellViewScene_CK
{
private:
	CHAR _pad1[0x58];
	TESForm_CK* cell_int_info;
	CHAR _pad2[0x38];
	TESCellViewSceneRenderInfo_CK* render_info;
public:
	/*
	==================
	TESCellViewScene_CK::IsEmpty

	When initialized 0x48 0x4C 0x50 0x54 are equal to 7FFFFFFF
	==================
	*/
	BOOL IsEmpty(VOID) const;
	/*
	==================
	TESCellViewScene_CK::IsLocationCell

	If this is a location in some place, then the pointer to the form is NULL
	How to find form the location that is displayed in the list, I haven't found it yet
	==================
	*/
	inline BOOL IsLocationCell(VOID) const { return cell_int_info == NULL; }
	/*
	==================
	TESCellViewScene_CK::IsInteriorsCell

	If the cell exists, the interior is loaded
	==================
	*/
	inline BOOL IsInteriorsCell(VOID) const { return cell_int_info != NULL; }
	/*
	==================
	TESCellViewScene_CK::GetCellInteriosInfo

	Returns the form of the interior
	==================
	*/
	inline TESForm_CK* GetCellInteriosInfo(VOID) const { return cell_int_info; }
	/*
	==================
	TESCellViewScene_CK::GetRenderInfo

	Returns the render information 
	==================
	*/
	inline TESCellViewSceneRenderInfo_CK* GetRenderInfo(VOID) const { return render_info; }
public:
	/*
	==================
	TESCellViewScene_CK::GetCellViewScene

	Returns the global view scene (rav+0x6D54CF8) 
	==================
	*/
	static TESCellViewScene_CK* GetCellViewScene(VOID);
public:
	~TESCellViewScene_CK(VOID);
public:
	READ_PROPERTY(GetCellInteriosInfo) TESForm_CK* CellInteriosInfo;
	READ_PROPERTY(GetRenderInfo) TESCellViewSceneRenderInfo_CK* RenderInfo;
}; 