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

#include "../[Common]/StdAfx.h"
#include "TESForm.h"

#include "..\[Patches]\boost_search_array.h"

VOID TESForm::LoadForm(TESFile* file) {
	thisVirtualCall<VOID>(0xD0, this, file);
}

VOID TESForm::SaveForm(TESFile* file) {
	thisVirtualCall<VOID>(0xD8, this, file);
}

TESForm::FormType TESForm::GetType(VOID) const {
	return thisVirtualCall<TESForm::FormType>(0x120, this);
}

VOID TESForm::DebugInfo(LPSTR buffer, DWORD dwSize) const {
	thisVirtualCall<VOID>(0x128, this, buffer, dwSize);
}

VOID TESForm::MarkAsDeleted(BOOL state) {
	thisVirtualCall<VOID>(0x198, this, state);
}

VOID TESForm::MarkAsChanged(BOOL state) {
	thisVirtualCall<VOID>(0x1A0, this, state);
}

DWORD TESForm::GetEditIDLength(VOID) const {
	return thisVirtualCall<DWORD>(0x230, this);
}

LPCSTR TESForm::GetEditID(VOID) const {
	return thisVirtualCall<LPCSTR>(0x238, this);
}

BSString TESForm::GetTypeShortStr(VOID) const {
	struct BGSTypeConvertShortItem {
		DWORD64 type_id;
		LPCSTR Name;
		CHAR ShortName[4];
		CHAR pad[4];
	};
	static_assert(sizeof(BGSTypeConvertShortItem) == 0x18);
	BGSTypeConvertShortItem* arr = (BGSTypeConvertShortItem*)(OFFSET(0x454DA60, 0));
	return arr[(DWORD)GetType()].Name;
}

BSString TESForm::GetID(VOID) const {
	if (!GetEditIDLength())
		return "";
	else
		return GetEditID();
}

TESForm* GetFormByNumericID(const DWORD SearchID) {
	return fastCall<TESForm*>(0x853220, SearchID);
}

BOOL TESForm::AlteredFormList_ElementExists(TESFormArray* Array, TESForm*& Entry) {
	if (!Array)
		return FALSE;

	if (Array->QSize() < 16)
	{
		auto count = Array->QSize();
		auto elemn = Array->QBuffer();

		while (count)
		{
			if (*elemn == Entry)
				return TRUE;

			elemn++;
			count--;
		}

		return FALSE;
	}
	else
		return Experimental::QSIMDFastSearchArrayItemQWORD(*(BSTArray<UINT64>*)Array, reinterpret_cast<UINT64&>(Entry)) != (DWORD)-1;
}