#include "../../common.h"
#include "TESDataFileHandler_CK.h"
#include "UIProgressDialog.h"
#include "LogWindow.h"

TESDataFileHandler_CK* FileHandler;
TESDataFileHandler_CK::TESFileArray_CK g_SelectedFilesArray;

void TESDataFileHandler_CK::Initialize(void)
{
	FileHandler = (TESDataFileHandler_CK*)OFFSET(0x6D67960, 0);

	// Recognition of loaded files
	XUtil::DetourClassCall(OFFSET(0x801AA7, 0), &TESDataFileHandler_CK::DetectSelectFile);
}

bool TESDataFileHandler_CK::Load(int Unknown)
{
	g_SelectedFilesArray.clear();

	// loads, checks.
	return ((bool(__fastcall*)(TESDataFileHandler_CK*, int))OFFSET(0x7D9D80, 0))(this, Unknown);
}

bool TESDataFileHandler_CK::InitUnknownDataSetTextStatusBar(void)
{
	// Replacing Tips with a progress Bar
	if (g_INI.GetBoolean("CreationKit", "UI", false) && g_INI.GetBoolean("CreationKit", "ReplacingTipsWithProgressBar", false))
	{
		// set to progressbar
		Core::Classes::UI::ProgressDialog->MessageText = "Loading Files... Initializing...";
		Core::Classes::UI::ProgressDialog->Marquee = TRUE;
	}

	// Unknown. Initializes something.
	return ((bool(__fastcall*)(TESDataFileHandler_CK*))OFFSET(0x7D66A0, 0))(this);
}

void TESDataFileHandler_CK::DetectSelectFile(TESFile_CK* File)
{
	// Sometimes duplicated
	if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), File) == g_SelectedFilesArray.end())
	{
		if (File->IsActive()) 
			LogWindow::Log("Load active file %s...", File->FileName.c_str());
		else if (File->IsMaster() || File->IsSmallMaster())
			LogWindow::Log("Load master file %s...", File->FileName.c_str());
		else
			LogWindow::Log("Load file %s...", File->FileName.c_str());

		g_SelectedFilesArray.push_back(File);
	}

	((void(__fastcall*)(TESFile_CK*))OFFSET(0x7FFF10, 0))(File);
}

TESDataFileHandler_CK::TESFileListPtr_CK TESDataFileHandler_CK::GetArchiveFiles(void)
{
	return (TESDataFileHandler_CK::TESFileListPtr_CK)OFFSET(0x6D68910, 0);
}

TESDataFileHandler_CK::TESFileArray_CK* TESDataFileHandler_CK::GetSelectedFiles(void)
{
	return &g_SelectedFilesArray;
}

TESFile_CK* TESDataFileHandler_CK::GetActiveFile(void) const
{
	return (TESFile_CK*)(((char*)this) + 0xFA8);
}

bool TESDataFileHandler_CK::IsActiveFile(void) const
{
	return GetActiveFile() != nullptr;
}