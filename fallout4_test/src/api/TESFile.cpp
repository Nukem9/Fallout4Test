//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
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
#include "../patches/CKF4/UIBaseWindow.h"
#include "../patches/CKF4/LogWindow.h"
#include "TESFile.h"

using namespace api;

INT32 TESFile::hk_LoadTESInfo(VOID)
{
	int error = LoadTESInfo(this);

	if (error != 0)
		return error;

	// If it's an ESM being loaded as the active file, force it to act like a normal ESP
	if (AllowSaveESM)
	{
		if (IsMaster() && IsActive())
		{
			_MESSAGE_FMT("Loading master file '%s' as a plugin\n", m_FileName);

			// Strip ESM flag, clear loaded ONAM data
			m_Flags &= ~FILE_RECORD_ESM;
			((void(__fastcall*)(TESFile*))OFFSET(0x805EF0, 0))(this);
		}
	}

	// If loading ESP files as masters, flag everything except for the currently active plugin
	if (AllowMasterESP)
	{
		if (!IsMaster() && !IsActive() && IsSelected())
		{
			_MESSAGE_FMT("Loading plugin file '%s' as a master\n", m_FileName);
			m_Flags |= FILE_RECORD_ESM;
		}
	}

	return 0;
}

INT64 TESFile::hk_WriteTESInfo(VOID)
{
	bool resetEsmFlag = false;

	if (AllowSaveESM)
	{
		if (IsActive())
		{
			LPCSTR extension = strrchr(m_FileName, '.');

			if (extension && !_stricmp(extension, ".esm"))
			{
				_MESSAGE_FMT("Regenerating ONAM data for master file '%s'...\n", m_FileName);

				((void(__fastcall*)(TESFile*))OFFSET(0x805F90, 0))(this);
				resetEsmFlag = true;
			}
		}
	}

	auto form = WriteTESInfo(this);

	if (resetEsmFlag)
		m_Flags &= ~FILE_RECORD_ESM;
	
	return form;
}

BOOL TESFile::IsMasterFileToBlacklist(VOID) {
	if (IsMaster()) {
		auto str = FileName;

		if (!str.Compare("fallout4.esm") ||
			!str.Compare("dlcrobot.esm") ||
			!str.Compare("dlcnukaworld.esm") ||
			!str.Compare("dlccoast.esm") ||
			!str.Compare("dlcworkshop01.esm") ||
			!str.Compare("dlcworkshop02.esm") ||
			!str.Compare("dlcworkshop03.esm")) {
			Core::Classes::UI::CUIMainWindow::MessageWarningDlg("Base game master files cannot be set as the active file.");
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

BSString TESFile::GetAuthorName(VOID) const { 
	if (IsMaster())
		return (LPCSTR)OFFSET(0x3853908, 0);

	return *m_authorName ? m_authorName : ""; 
}

VOID TESFile::Dump(VOID) {
	_MESSAGE_FMT("Address: %p", this);
	_MESSAGE_FMT("FileName: %s", *FileName);
	_MESSAGE_FMT("FilePath: %s", *FilePath);
	_MESSAGE_FMT("AuthorName: %s", *AuthorName);
	_MESSAGE_FMT("Description: %s", *Description);
	_MESSAGE_FMT("FileSize: %u", FileSize);
	
	_MESSAGE_FMT(" ");
	_MESSAGE_FMT("DependCount: %u", DependCount);
	auto count = DependCount;
	for (auto i = 0; i < count; i++) {
		auto file = DependArray[i];

		_MESSAGE_FMT("Address: %p", file);
		_MESSAGE_FMT("FileName: %s", *(file->FileName));
		_MESSAGE_FMT("FilePath: %s", *(file->FilePath));
		_MESSAGE_FMT("AuthorName: %s", *(file->AuthorName));
		_MESSAGE_FMT("Description: %s", *(file->Description));
		_MESSAGE_FMT("FileSize: %u", file->FileSize);
	}
}