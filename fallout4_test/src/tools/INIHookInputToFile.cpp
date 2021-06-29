#include "INIHookInputToFile.h"
#include "..\xutil.h"

namespace Tools
{
	FILE* fHandle = nullptr;
	const char szFileName[MAX_PATH] = ".\\ini_hook.log";

	UINT WINAPI hk_GetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
	{
		UINT uResult = GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);

		fopen_s(&fHandle, szFileName, "a");
		if (fHandle)
		{
			fprintf(fHandle, "SectionName: %s; ParamName: %s; DefaultValue: %d; Value: %d\n", lpAppName, lpKeyName, nDefault, uResult);
			fclose(fHandle);
		}

		return uResult;
	}

	DWORD WINAPI hk_GetPrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
	{
		DWORD dwResult = GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);

		fopen_s(&fHandle, szFileName, "a");
		if (fHandle)
		{
			fprintf(fHandle, "SectionName: %s; ParamName: %s; DefaultValue: %s; Value: %s\n", lpAppName, lpKeyName, lpDefault, lpReturnedString);
			fclose(fHandle);
		}

		return dwResult;
	}

	void __stdcall IniHookInputInit(void)
	{
		DeleteFileA(szFileName);

		// bool values
		XUtil::DetourCall(OFFSET(0x26051F9, 0), &hk_GetPrivateProfileIntA);
		// float values
		XUtil::DetourCall(OFFSET(0x260506A, 0), &hk_GetPrivateProfileStringA);
	}
}

