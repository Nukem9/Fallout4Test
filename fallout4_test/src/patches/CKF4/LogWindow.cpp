#include "../../common.h"
#include "../../erase_if.h"
#include <tbb/concurrent_vector.h>
#include <Richedit.h>
#include <unordered_set>
#include "EditorUI.h"
#include "UIThemeMode.h"
#include "LogWindow.h"
#include "MainWindow.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace LogWindow
{
	HWND LogWindowHandle;
	HANDLE ExternalPipeReaderHandle;
	HANDLE ExternalPipeWriterHandle;
	FILE *OutputFileHandle;

	uint64_t HashLast = 0xFFFFFFFF;

	tbb::concurrent_vector<std::string> PendingMessages;
	std::unordered_set<uint64_t> MessageBlacklist;

	HWND FIXAPI GetWindow(VOID)
	{
		return LogWindowHandle;
	}

	HANDLE FIXAPI GetStdoutListenerPipe(VOID)
	{
		return ExternalPipeWriterHandle;
	}

	BOOL FIXAPI Initialize(VOID)
	{
		if (!LoadLibraryA("MSFTEDIT.dll"))
			return FALSE;

		// Build warning blacklist stored in the ini file
		LoadWarningBlacklist();

		// File output
		const std::string logPath = g_INI.Get("CreationKit_Log", "OutputFile", "none");

		if (strcmp(logPath.c_str(), "none") != 0)
		{
			if (fopen_s(&OutputFileHandle, logPath.c_str(), "w") != 0)
				OutputFileHandle = NULL;

			AssertMsgVa(OutputFileHandle, "Unable to open the log file '%s' for writing. To disable, set the 'OutputFile' INI option to 'none'.", logPath.c_str());

			atexit([]()
			{
				if (OutputFileHandle)
					fclose(OutputFileHandle);
			});
		}

		std::thread asyncLogThread([]()
		{
			UITheme::InitializeThread();

			// Output window
			auto instance = (HINSTANCE)GetModuleHandleA(NULL);

			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = instance;
			wc.hIcon = LoadIconA(instance, MAKEINTRESOURCE(0x13E));
			wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			wc.lpszClassName = TEXT("RTEDITLOG");
			wc.hIconSm = wc.hIcon;

			if (!RegisterClassExA(&wc))
				return false;

			LogWindowHandle = CreateWindowExA(0, TEXT("RTEDITLOG"), TEXT("Log"), WS_OVERLAPPEDWINDOW, 64, 64, 1024, 480, NULL, NULL, instance, NULL);

			if (!LogWindowHandle)
				return false;

			LogWindow::Log("MemoryManager::ScrapHeap set %u Mb", DWORD(g_ScrapSize / 1048576));

			if (g_bhkMemSize >= 0x40000000)
				LogWindow::Log("MemoryManager::bhkThreadMemorySource set %u Gb", DWORD(g_bhkMemSize / 1073741824));
			else
				LogWindow::Log("MemoryManager::bhkThreadMemorySource set %u Mb", DWORD(g_bhkMemSize / 1048576));

			// Poll every 100ms for new lines
			SetTimer(LogWindowHandle, UI_LOG_CMD_ADDTEXT, 100, NULL);

			MSG msg;
			while (GetMessageA(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			return true;
		});

		asyncLogThread.detach();
		return TRUE;
	}

	BOOL FIXAPI CreateStdoutListener(VOID)
	{
		SECURITY_ATTRIBUTES saAttr = { 0 };
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.lpSecurityDescriptor = NULL;
		saAttr.bInheritHandle = TRUE;

		if (!CreatePipe(&ExternalPipeReaderHandle, &ExternalPipeWriterHandle, &saAttr, 0))
			return FALSE;

		// Ensure the read handle to the pipe for STDOUT is not inherited
		if (!SetHandleInformation(ExternalPipeReaderHandle, HANDLE_FLAG_INHERIT, 0))
			return FALSE;

		std::thread pipeReader([]()
		{
			CHAR logBuffer[16384] = {};

			while (TRUE)
			{
				CHAR buffer[4096] = {};
				DWORD bytesRead;

				BOOL succeeded = ReadFile(ExternalPipeReaderHandle, buffer, ARRAYSIZE(buffer) - 1, &bytesRead, NULL) != 0;

				// Bail if there's nothing left or the process exited
				if (!succeeded || bytesRead <= 0)
					break;

				strcat_s(logBuffer, buffer);

				// Flush on every newline and skip empty/whitespace strings
				while (LPSTR end = strchr(logBuffer, '\n'))
				{
					*end = '\0';
					size_t len = (size_t)(end - logBuffer);

					while (strchr(logBuffer, '\r'))
						*strchr(logBuffer, '\r') = ' ';

					if (len > 0 && (len > 1 || logBuffer[0] != ' '))
						LogWarning(6, "%s", logBuffer);

					strcpy_s(logBuffer, end + 1);
				}
			}

			CloseHandle(ExternalPipeReaderHandle);
			CloseHandle(ExternalPipeWriterHandle);
		});

		pipeReader.detach();
		return TRUE;
	}

	VOID FIXAPI LoadWarningBlacklist(VOID)
	{
		if (!g_INI.GetBoolean("CreationKit", "WarningBlacklist", FALSE))
			return;

		// Keep reading entries until an empty one is hit

		auto fname = "CreationKitWarnings.txt";

		std::string message;
		fs::path filename = fs::absolute(fs::current_path() / fname);

		if (!fs::exists(filename))
			Log("File '%s' not found. Recommend creating one to skip the specified messages.", fname);
		else
		{
			std::ifstream in(filename);

			// Keep reading entries until an end
			while (!in.eof())
			{
				std::getline(in, message);

				
				std::erase_if(message, [](auto const& x) { return x == '\n' || x == '\r'; });
				XUtil::Str::trim(message);

				if (message.empty())
					continue;

				MessageBlacklist.emplace(XUtil::MurmurHash64A(message.c_str(), message.length()));
			}

			in.close();
		}

		// localized errors
		filename = fs::absolute(fs::current_path() / "Localize/CreationKitWarnings.txt");

		if (fs::exists(filename))
		{
			std::ifstream in(filename);

			// Keep reading entries until an end
			while (!in.eof())
			{
				std::getline(in, message);
				std::erase_if(message, [](auto const& x) { return x == '\n' || x == '\r'; });
				XUtil::Str::trim(message);

				if (message.empty())
					continue;

				MessageBlacklist.emplace(XUtil::MurmurHash64A(message.c_str(), message.length()));
			}

			in.close();
		}
	}

	LRESULT CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		static HWND richEditHwnd;
		static BOOL autoScroll;

		switch (Message)
		{
		case WM_CREATE:
		{
			auto info = (const CREATESTRUCT *)lParam;

			// Create the rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
			uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_READONLY;

			richEditHwnd = CreateWindowExW(0, MSFTEDIT_CLASS, L"", style, 0, 0, info->cx, info->cy, Hwnd, NULL, info->hInstance, NULL);
			autoScroll = TRUE;

			if (!richEditHwnd)
				return -1;

			// Set a better font & convert Twips to points (1 point = 20 Twips)
			CHARFORMAT2A format = {};
			format.cbSize = sizeof(format);
			format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;
			format.yHeight = g_INI.GetInteger("CreationKit_Log", "FontSize", 10) * 20;
			format.wWeight = (WORD)g_INI.GetInteger("CreationKit_Log", "FontWeight", FW_NORMAL);
			strcpy_s(format.szFaceName, g_INI.Get("CreationKit_Log", "Font", "Consolas").c_str());

			SendMessageA(richEditHwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)& format);

			// Subscribe to EN_MSGFILTER and EN_SELCHANGE
			SendMessageA(richEditHwnd, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS | ENM_SELCHANGE);

			// Set default position
			INT32 winX = g_INI.GetInteger("CreationKit_Log", "X", info->x);
			INT32 winY = g_INI.GetInteger("CreationKit_Log", "Y", info->y);
			INT32 winW = g_INI.GetInteger("CreationKit_Log", "Width", info->cx);
			INT32 winH = g_INI.GetInteger("CreationKit_Log", "Height", info->cy);

			MoveWindow(Hwnd, winX, winY, winW, winH, FALSE);

			if (((winW != 0) && (winH != 0)) && (!g_INI.GetBoolean("CreationKit_Log", "TurnOffLogWindow", FALSE)))
			{
				ShowWindow(Hwnd, SW_SHOW);
				UpdateWindow(Hwnd);
			}
		}
		return S_OK;

		case WM_DESTROY:
			DestroyWindow(richEditHwnd);
			return S_OK;

		case WM_SIZE:
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			MoveWindow(richEditHwnd, 0, 0, w, h, TRUE);
		}
		break;

		case WM_ACTIVATE:
		{
			if (wParam != WA_INACTIVE)
				SetFocus(richEditHwnd);
		}
		return S_OK;

		case WM_CLOSE:
			ShowWindow(Hwnd, SW_HIDE);
			MainWindow::GetMainMenuObj().GetItem(UI_EXTMENU_SHOWLOG).Checked = FALSE;
			return S_OK;

		case WM_NOTIFY:
		{
			static uint64_t lastClickTime;
			auto notification = (const LPNMHDR)lParam;

			if (notification->code == EN_MSGFILTER)
			{
				auto msgFilter = (const MSGFILTER *)notification;

				if (msgFilter->msg == WM_LBUTTONDBLCLK)
					lastClickTime = GetTickCount64();
			}
			else if (notification->code == EN_SELCHANGE)
			{
				auto selChange = (const SELCHANGE *)notification;

				// Mouse double click with a valid selection -> try to parse form id
				if ((GetTickCount64() - lastClickTime > 1000) || selChange->seltyp == SEL_EMPTY)
					break;

				CHAR lineData[4096];
				*(uint16_t *)&lineData[0] = ARRAYSIZE(lineData);

				// Get the line number & text from the selected range
				LRESULT lineIndex = SendMessageA(richEditHwnd, EM_LINEFROMCHAR, selChange->chrg.cpMin, 0);
				LRESULT charCount = SendMessageA(richEditHwnd, EM_GETLINE, lineIndex, (LPARAM)&lineData);

				if (charCount > 0)
				{
					lineData[charCount - 1] = '\0';

					// Capture each hexadecimal form id in the format of "(XXXXXXXX)"
					for (LPSTR p = lineData; p[0] != '\0'; p++)
					{
						if (p[0] == '(' && strlen(p) >= 10 && p[9] == ')')
						{
							uint32_t id = strtoul(&p[1], NULL, 16);
							PostMessageA(MainWindow::GetWindow(), WM_COMMAND, UI_EDITOR_OPENFORMBYID, id);
						}
					}
				}

				lastClickTime = GetTickCount64() + 1000;
			}
		}
		break;

		case WM_TIMER:
		{
			if (wParam != UI_LOG_CMD_ADDTEXT)
				break;

			if (PendingMessages.size() <= 0)
				break;

			return WndProc(Hwnd, UI_LOG_CMD_ADDTEXT, 0, 0);
		}
		return S_OK;

		case UI_LOG_CMD_ADDTEXT:
		{
			SendMessageA(richEditHwnd, WM_SETREDRAW, FALSE, 0);

			// Save old position if not scrolling
			POINT scrollRange;

			if (!autoScroll)
				SendMessageA(richEditHwnd, EM_GETSCROLLPOS, 0, (WPARAM)&scrollRange);

			// Get a copy of all elements and clear the global list
			auto messages(std::move(PendingMessages));

			for (std::string msg : messages)
			{
				// Move caret to the end, then write
				CHARRANGE range;
				range.cpMax = LONG_MAX;
				range.cpMin = LONG_MAX;

				SendMessageA(richEditHwnd, EM_EXSETSEL, 0, (LPARAM)&range);
				SendMessageA(richEditHwnd, EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
			}

			if (!autoScroll)
				SendMessageA(richEditHwnd, EM_SETSCROLLPOS, 0, (WPARAM)&scrollRange);

			SendMessageA(richEditHwnd, WM_SETREDRAW, TRUE, 0);
			RedrawWindow(richEditHwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
		}
		return S_OK;

		case UI_LOG_CMD_CLEARTEXT:
		{
			char emptyString[1];
			emptyString[0] = '\0';
			HashLast = 0xFFFFFFFF;

			SendMessageA(richEditHwnd, WM_SETTEXT, 0, (LPARAM)&emptyString);
		}
		return S_OK;

		case UI_LOG_CMD_AUTOSCROLL:
			autoScroll = (BOOL)wParam;
			return S_OK;
		}

		return DefWindowProc(Hwnd, Message, wParam, lParam);
	}

	VOID FIXAPI LogVa(LPCSTR Format, va_list Va)
	{
		std::string message;
		message.resize(2048);
		message.resize(_vsnprintf(&message[0], _TRUNCATE, Format, Va));

		// Un-escape newline and carriage return characters
		std::erase_if(message, [](auto const& x) { return x == '\n' || x == '\r'; });
		XUtil::Str::trim(message);

		if (!message.length())
			return;

		uint64_t Hash = XUtil::MurmurHash64A(message.c_str(), message.length());	
		if (Hash == HashLast || MessageBlacklist.count(Hash))
			return;
	
		HashLast = Hash;
		message += "\n";

		if (OutputFileHandle)
		{
			fputs(message.c_str(), OutputFileHandle);
			fflush(OutputFileHandle);
		}

		if (PendingMessages.size() < 50000)
			PendingMessages.emplace_back(message.c_str());
	}

	VOID FIXAPI LogWcVa(LPCWSTR Format, va_list Va)
	{
		std::wstring message;
		message.resize(2048);
		message.resize(_vsnwprintf(&message[0], _TRUNCATE, Format, Va));

		std::string conv_message = XUtil::Conversion::WideToAnsi(message);

		Log("%s", conv_message.c_str());
	}

	VOID FIXAPI Log(LPCSTR Format, ...)
	{
		va_list va;

		va_start(va, Format);
		LogVa(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWc(LPCWSTR Format, ...)
	{
		va_list va;

		va_start(va, Format);
		LogWcVa(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWarningVa(INT32 Type, LPCSTR Format, va_list& Va)
	{
		static LPCSTR typeList[34] =
		{
			"DEFAULT", 
			"SYSTEM",
			"COMBAT",
			"ANIMATION",
			"AI",
			"SCRIPTS",
			"SAVELOAD",
			"DIALOGUE",
			"QUESTS",
			"PACKAGES",
			"EDITOR",
			"MODELS",
			"TEXTURES",
			"PLUGINS",
			"MASTERFILE",
			"FORMS",
			"MAGIC",
			"SHADERS",
			"RENDERING",
			"PATHFINDING",
			"MENUS",
			"AUDIO",
			"CELLS",
			"HAVOK",
			"FACEGEN",
			"WATER",
			"INGAME",
			"MEMORY",
			"PERFORMANCE",
			"LOOTJOY",
			"VATS",
			"DISMEMBER",
			"COMPANION",
			"WORKSHOP",
		};

		CHAR buffer[2048];
		_vsnprintf_s(buffer, _TRUNCATE, Format, Va);

		Log("%s: %s", typeList[Type], buffer);
	}

	VOID FIXAPI LogWarning(INT32 Type, LPCSTR Format, ...)
	{
		va_list va;

		va_start(va, Format);
		LogWarningVa(Type, Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWarningUnknown1(LPCSTR Format, ...)
	{
		va_list va;

		va_start(va, Format);
		LogVa(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWarningUnknown2(INT64 Unused, LPCSTR Format, ...)
	{
		va_list va;

		va_start(va, Format);
		LogVa(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogAssert(LPCSTR File, INT32 Line, LPCSTR Message, ...)
	{
		if (!Message)
			Message = "<No message>";

		CHAR buffer[2048];
		va_list va;

		va_start(va, Message);
		_vsnprintf_s(buffer, _TRUNCATE, Message, va);
		va_end(va);

		Log("ASSERTION: %s (%s line %d)", buffer, File, Line);
	}
}