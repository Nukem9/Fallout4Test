#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>

struct membuf : public std::streambuf
{
	membuf(DWORD len)
		: std::streambuf()
		, len(len)
		, src((len > 0) ? new CHAR[len] : NULL)
	{
		setg(src.get(), src.get(), src.get() + len);
	}

	// direct buffer access for file load.
	LPSTR get(VOID) { return src.get(); };
	DWORD size(VOID) const { return len; };

private:
	std::unique_ptr<CHAR> src;
	DWORD len;
};

std::map<std::wstring, membuf*> dllENBs =
{ 
	{ L"d3d11.dll", NULL },
	{ L"d3d10.dll", NULL },
	{ L"d3d9.dll",  NULL },
	{ L"d3dcompiler_46e.dll",  NULL },
	{ L"dxgi.dll",  NULL },
	{ L"enbimgui.dll",  NULL },
};

std::wstring app_path;

#ifdef HIDEWINDOW

VOID HideConsole(VOID)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

#endif

BOOL FileExists(const std::wstring fname)
{
	return std::experimental::filesystem::exists(app_path + fname);
}

VOID ReadFileToMemory(VOID)
{
	for (auto dll = dllENBs.begin(); dllENBs.end() != dll; dll++)
	{
		if (FileExists((*dll).first))
		{
#ifndef HIDEWINDOW
			std::wcout << L"File """ << (*dll).first.c_str() << L""" found and will be remove.\n";
#endif
			HANDLE fileDll = CreateFileW((app_path + (*dll).first).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (!fileDll)
			{
#ifndef HIDEWINDOW
				std::wcout << L"File """ << (*dll).first.c_str() << L""" failed open.\n";
#else
				std::wstring msg = std::wstring(L"File """) + (*dll).first.c_str() + L""" failed open.";
				MessageBoxW(0, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
#endif
				break;
			}

			DWORD len = GetFileSize(fileDll, NULL);

			(*dll).second = new membuf(len);

			DWORD m;
			if (!ReadFile(fileDll, (*dll).second->get(), len, &m, NULL))
			{
#ifndef HIDEWINDOW
				std::wcout << L"File """ << (*dll).first.c_str() << L""" failed read file.\n";
#endif
			}
			CloseHandle(fileDll);
#ifndef HIDEWINDOW
			std::wcout << L"File """ << (*dll).first.c_str() << L""" read " << m << L".\n";
#endif
			DeleteFileW((app_path + (*dll).first).c_str());
		}
	}
}

VOID WriteFileToFolder(VOID)
{
	for (auto dll = dllENBs.begin(); dllENBs.end() != dll; dll++)
	{
		if ((*dll).second)
		{
#ifndef HIDEWINDOW
			std::wcout << L"File """ << (*dll).first.c_str() << L""" was removed and will be restored.\n";
#endif
			HANDLE fileDll = CreateFileW((app_path + (*dll).first).c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			if (!fileDll)
			{
#ifndef HIDEWINDOW
				std::wcout << L"File """ << (*dll).first.c_str() << L""" failed create.\n";
#else
				std::wstring msg = std::wstring(L"File """) + (*dll).first.c_str() + L""" failed create.";
				MessageBoxW(0, msg.c_str(), L"Error", MB_OK | MB_ICONERROR); 
#endif
				break;
			}

			DWORD m;
			if (!WriteFile(fileDll, (*dll).second->get(), (*dll).second->size(), &m, NULL))
			{
#ifndef HIDEWINDOW
				std::wcout << L"File """ << (*dll).first.c_str() << L""" failed write file.\n";
#endif
			}
			CloseHandle(fileDll);
#ifndef HIDEWINDOW
			std::wcout << L"File """ << (*dll).first.c_str() << L""" write " << m << L".\n";
#endif
			delete (*dll).second;
			(*dll).second = NULL;
		}
	}
}

INT32 WINAPI main(INT32 argc, LPWSTR argv[])
{
#ifdef HIDEWINDOW
	HideConsole();
#endif
	WCHAR basePath[2048] = L"";
	GetFullPathNameW(argv[0], 2048, basePath, NULL);
	std::experimental::filesystem::path p = basePath;

	app_path = p.parent_path();
	app_path += L"\\";
#ifndef HIDEWINDOW
	std::wcout << L"Root dir: " << app_path << L"\n";
	std::wcout << L"Delete files.\n";
#endif
	ReadFileToMemory();
#ifndef HIDEWINDOW
	std::wcout << L"Open Creation Kit.\n";
#endif
	ShellExecuteW(NULL, L"open", L"CreationKit.exe", NULL, app_path.c_str(), SW_SHOW);
#ifndef HIDEWINDOW
	std::wcout << L"Wait 15 sec.\n";
#endif
	Sleep(15000);
#ifndef HIDEWINDOW
	std::wcout << L"Return files.\n";
#endif
	WriteFileToFolder();
}