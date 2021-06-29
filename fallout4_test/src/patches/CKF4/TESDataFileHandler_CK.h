#pragma once

#include "TESFile_CK.h"
#include "../TES/BSTList.h"

#include <vector>

/// <summary>
/// Class that works with archived game files
/// </summary>
class TESDataFileHandler_CK
{
public:
	using TESFileList_CK = BSSimpleList<TESFile_CK*>;
	using TESFileListPtr_CK = TESFileList_CK*;
	using TESFileArray_CK = std::vector<TESFile_CK*>;
public:
	/// <summary>
	/// Used in a hook, do not use!
	/// </summary>
	/// <param name="Unknown">Unknown value that is equal to zero</param>
	/// <returns>Return True if loading and defining dependencies is successful</returns>
	bool Load(int Unknown);							
	/// <summary>
	/// Used in a hook, do not use!
	/// </summary>
	/// <param name="">No parms</param>
	/// <returns>Return True if some initialization is successful</returns>
	bool InitUnknownDataSetTextStatusBar(void);	
	/// <summary>
	/// Used in a hook, do not use!
	/// </summary>
	/// <param name="File">Pointer to the archive file</param>
	static void DetectSelectFile(TESFile_CK* File);
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="">No parms</param>
	/// <returns>Return an list of all archives</returns>
	static TESFileListPtr_CK GetArchiveFiles(void);
	/// <summary>
	/// Self-written array with selected archives.
	/// </summary>
	/// <param name="">No parms</param>
	/// <returns>Return an array of selected archives</returns>
	static TESFileArray_CK* GetSelectedFiles(void);
	/// <summary>
	/// Can be NULL. If only Master Files are loaded.
	/// </summary>
	/// <param name="">No parms</param>
	/// <returns>Returns the active file that has target save</returns>
	TESFile_CK* GetActiveFile(void) const;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="">No parms</param>
	/// <returns>Return True if active file is exists</returns>
	bool IsActiveFile(void) const;
public:
	/// <summary>
	/// do not use!
	/// </summary>
	/// <param name="">No parms</param>
	static void Initialize(void);
public:
	/// <summary>
	/// Return an list of all archives.
	/// </summary>
	__declspec(property(get = GetArchiveFiles)) TESFileListPtr_CK ArchiveFiles;
	/// <summary>
	/// Return an array of selected archives.
	/// </summary>
	__declspec(property(get = GetSelectedFiles)) TESFileArray_CK* SelectedFiles;
	/// <summary>
	/// Returns the active file that has target save. Can be NULL. If only Master Files are loaded.
	/// </summary>
	__declspec(property(get = GetActiveFile)) TESFile_CK* ActiveFile;
};

extern TESDataFileHandler_CK* FileHandler;