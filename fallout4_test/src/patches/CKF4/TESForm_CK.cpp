#include "TESForm_CK.h"

TESForm_CK* TESForm_CK::GetFormByNumericID(UINT32 SearchID)
{
	return ((TESForm_CK*(__fastcall*)(UINT32))OFFSET(0x853220, 0))(SearchID);
}

std::string TESForm_CK::GetEditID(VOID) const
{
	return ((LPCSTR(__fastcall*)(const TESForm_CK*))OFFSET(0x1E63F0, 0))(this);
}
