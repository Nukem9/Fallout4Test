#pragma once

#include "../../common.h"
#include "../TES/BSTArray.h"

class TESForm_CK
{
private:
	CHAR _pad[0x8];
	UINT32 FormFlags;
	UINT32 FormID;
public:
	inline BOOL GetActive(VOID) const { return (FormFlags & 2) != 0; }
	inline uint32_t GetFormID(VOID) const { return FormID; }
	std::string GetEditID(VOID) const;
public:
	READ_PROPERTY(GetActive) BOOL Active;
	READ_PROPERTY(GetFormID) UINT32 FormID;
	READ_PROPERTY(GetEditID) std::string EditID;
public:
	using Array = BSTArray<TESForm_CK*>;

	virtual ~TESForm_CK();
	static TESForm_CK* GetFormByNumericID(UINT32 SearchID);
};