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

#include "..\..\[Common]\StdAfx.h"

#include "Editor.h"
#include "EditorUI.h"
#include "UIThemeMode.h"
#include "MainWindow.h"

namespace fs = std::filesystem;

namespace LogWindow {
	BOOL bInitializeChannel = FALSE;
	std::unordered_set<uint64_t> MessageBlacklist;

	VOID FIXAPI LoadWarningBlacklist(VOID);

	VOID IChannel::WriteVa(LPCSTR Format, va_list Va) {
		auto len = _vsnprintf(NULL, 0, Format, Va);

		std::string message;
		message.resize(len);
		_vsnprintf(&message[0], _TRUNCATE, Format, Va);

		if (_skipdel) {
			// Un-escape newline and carriage return characters
			std::erase_if(message, [](auto const& x) { return x == '\n' || x == '\r'; });
			XUtil::Str::trim(message);
		}

		if (!message.length() || !Check(&message[0]))
			return;

		message += "\n";

		Add(&message[0]);
	}

	VOID IChannel::Write(LPCSTR Format, ...) {
		va_list va;

		va_start(va, Format);
		WriteVa(Format, va);
		va_end(va);
	}

	IChannel::IChannel(VOID) : _skipdel(FALSE)
	{}

	VOID IPendingMessages::QueuePend(LPCSTR Text) {
		std::vector<std::string>* pend = (std::vector<std::string>*)(_pend);

		if (pend->size() < _limit)
			pend->emplace_back(Text);
	}

	DWORD IPendingMessages::CountPend(VOID) const {
		std::vector<std::string>* pend = (std::vector<std::string>*)(_pend);
		return pend->size();
	}

	IPendingMessages::IPendingMessages(DWORD limit) : _pend((LPVOID)(new std::vector<std::string>)), _limit(limit)
	{}

	IPendingMessages::~IPendingMessages(VOID) {
		delete ((std::vector<std::string>*)(_pend));
	}
	
	VOID ChannelBased::Add(LPCSTR Text) {
		WriteFile(Text);
		QueuePend(Text);
		_storage.append(Text);
	}

	BOOL ChannelBased::Check(LPCSTR Text) {
		uint64_t Hash = XUtil::MurmurHash64A(Text, strlen(Text));
		if (Hash == _hashlast || MessageBlacklist.count(Hash))
			return FALSE;

		_hashlast = Hash;

		return TRUE;
	}

	ChannelBased::ChannelBased(LPCSTR _filename, DWORD limit_pend) : 
		IChannel(), IPendingMessages(limit_pend), IOutputFileHandler(_filename), _hashlast(0xFFFFFFFF)
	{}

	VOID IOutputFileHandler::WriteFile(LPCSTR Text) {
		if (_handle && _writefile) {
			fputs(Text, _handle);
			fflush(_handle);
		}
	}

	IOutputFileHandler::IOutputFileHandler(LPCSTR _filename) {
		if (_filename)
			_handle = fopen(_filename, "w");
		else
			_handle = NULL;
	}

	IOutputFileHandler::~IOutputFileHandler(VOID) {
		if (_handle)
			fclose(_handle);
	}

	Channel::Channel(LPCSTR _namech, LPCSTR _filename) : ChannelBased(_filename) {
		if (_namech)
			strcpy_s(_name, _namech);
		else
			strcpy_s(_name, "DEFAULT");
	}

	Channel* DefaultChannel = NULL;
	Channel* ChangeLogChannel = NULL;
	Channel* CurrentChannel = NULL;

	HWND LogWindowHandle;
	HWND BorderEditHwnd;
	HWND LabelHwnd;
	HWND richEditHwnd;
	HWND BtnDefChnHwnd;
	HWND BtnLogChnHwnd;

	HWND FIXAPI GetWindow(VOID) {
		return LogWindowHandle;
	}

	VOID FIXAPI ChangeChannel(Channel* channel) {
		if (channel && (CurrentChannel != channel)) {
			CurrentChannel = channel;

			CHARRANGE range;
			range.cpMax = LONG_MAX;
			range.cpMin = 0;

			hk_SendMessageA(richEditHwnd, EM_EXSETSEL, 0, (LPARAM)&range);
			hk_SendMessageA(richEditHwnd, EM_REPLACESEL, FALSE, (LPARAM)CurrentChannel->GetStorage());

			Button_SetCheck(BtnDefChnHwnd, CurrentChannel == DefaultChannel ? BST_CHECKED : BST_UNCHECKED);
			Button_SetCheck(BtnLogChnHwnd, CurrentChannel == ChangeLogChannel ? BST_CHECKED : BST_UNCHECKED);
		}
	}

	//////////////////////////////////////////////////////////

	BOOL FIXAPI SaveRichTextToFile(LPCSTR _filename) {
		FILE* f;
		if (fopen_s(&f, _filename, "wt"))
			return FALSE;

		if (DefaultChannel) {
			fputs("Channel \"DEFAULT\":\n", f);
			fputs(DefaultChannel->GetStorage(), f);
		}

		if (ChangeLogChannel) {
			fputs("Channel \"Change Log\":\n", f);
			fputs(ChangeLogChannel->GetStorage(), f);
		}

		fclose(f);

		return TRUE;
	}

	//////////////////////////////////////////////////////////

	BOOL FIXAPI Initialize(VOID) {
		if (!LoadLibraryA("MSFTEDIT.dll") || !bInitializeChannel)
			return FALSE;
		
		/// Create thread and window

		std::thread asyncLogThread([]() {
			UITheme::InitializeThread();

			// Output window
			auto instance = (HINSTANCE)GetModuleHandleA(NULL);

			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(WNDCLASSEX);

			if (UITheme::IsEnabledMode())
				wc.hbrBackground = (HBRUSH)UITheme::Theme::Comctl32GetSysColorBrush(COLOR_BTNFACE);
			else
				wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);

			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = instance;
			wc.hIcon = LoadIconA(instance, MAKEINTRESOURCE(0x13E));
			wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
			wc.lpszClassName = TEXT("RTEDITLOG");
			wc.hIconSm = wc.hIcon;

			if (!RegisterClassExA(&wc))
				return FALSE;

			LogWindowHandle = CreateWindowExA(0, wc.lpszClassName, TEXT("Console Window"), WS_OVERLAPPEDWINDOW,
				64, 64, 1024, 480, NULL, NULL, instance, NULL);

			if (!LogWindowHandle)
				return FALSE;

			// Poll every 100ms for new lines
			SetTimer(LogWindowHandle, UI_LOG_CMD_ADDTEXT, 100, NULL);

			MSG msg;
			while (GetMessageA(&msg, NULL, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			return TRUE;
		});

		asyncLogThread.detach();
		return TRUE;
	}

	VOID FIXAPI InitializeChannel(VOID) {
		/// Initialize DEFAULT channel
		DefaultChannel = new Channel(NULL, NULL);
		Assert(DefaultChannel);

		CurrentChannel = DefaultChannel;

		/// Initialize "Change Log" channel

		// Build warning blacklist stored in the ini file
		LoadWarningBlacklist();

		// File output
		const std::string logPath = g_INI->Get("CreationKit_Log", "OutputFile", "none");

		if (strcmp(logPath.c_str(), "none") != 0) {
			ChangeLogChannel = new Channel("Change Log", logPath.c_str());
			Assert(ChangeLogChannel);

			AssertMsgVa(ChangeLogChannel->IsOpen(),
				"Unable to open the log file '%s' for writing. To disable, set the 'OutputFile' INI option to 'none'.", logPath.c_str());

			atexit([]() {
				if (ChangeLogChannel)
					delete ChangeLogChannel;
				});

			ChangeLogChannel->OutputFile = TRUE;
		}
		else {
			ChangeLogChannel = new Channel("Change Log", NULL);
			Assert(ChangeLogChannel);
		}

		ChangeLogChannel->SkipDelimeter = TRUE;
		bInitializeChannel = TRUE;
	}

	VOID FIXAPI LoadWarningBlacklist(VOID) {
		if (!g_INI->GetBoolean("CreationKit", "WarningBlacklist", FALSE))
			return;

		// Keep reading entries until an empty one is hit

		auto fname = "CreationKitWarnings.txt";

		std::string message;
		fs::path filename = fs::absolute(fs::current_path() / fname);

		if (!fs::exists(filename))
			Log("File '%s' not found. Recommend creating one to skip the specified messages.", fname);
		else {
			std::ifstream in(filename);

			// Keep reading entries until an end
			while (!in.eof()) {
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

		if (fs::exists(filename)) {
			std::ifstream in(filename);

			// Keep reading entries until an end
			while (!in.eof()) {
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

	LRESULT CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		static BOOL autoScroll;

		switch (Message) {
		case WM_CREATE: {
			auto info = (const CREATESTRUCT*)lParam;

			if (UITheme::IsEnabledMode()) {
				HDC dc = GetDC(Hwnd);
				SelectObject(dc, UITheme::Theme::ThemeFont->Handle);
				//ReleaseDC(Hwnd, dc);
			}

			BorderEditHwnd = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BLACKFRAME, 8, 40, info->cx - 16, info->cy - 48,
				Hwnd, NULL, info->hInstance, NULL);

			// Create the rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
			uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_READONLY;

			richEditHwnd = CreateWindowExW(0, MSFTEDIT_CLASS, L"", style, 1, 1, info->cx - 18, info->cy - 50, 
				BorderEditHwnd, NULL, info->hInstance, NULL);
			autoScroll = TRUE;

			if (!richEditHwnd)
				return -1;

			// Set a better font & convert Twips to points (1 point = 20 Twips)
			CHARFORMAT2A format = {};
			format.cbSize = sizeof(format);
			format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;
			format.yHeight = g_INI->GetInteger("CreationKit_Log", "FontSize", 10) * 20;
			format.wWeight = (WORD)g_INI->GetInteger("CreationKit_Log", "FontWeight", FW_NORMAL);
			strcpy_s(format.szFaceName, g_INI->Get("CreationKit_Log", "Font", "Consolas").c_str());

			SendMessageA(richEditHwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);

			// Subscribe to EN_MSGFILTER and EN_SELCHANGE
			SendMessageA(richEditHwnd, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS | ENM_SELCHANGE);

			// Create Btn

			style = WS_VISIBLE | WS_CHILD | BS_CENTER | BS_PUSHLIKE | BS_CHECKBOX;

			BtnDefChnHwnd = CreateWindowExA(0, "BUTTON", DefaultChannel->Name, style, 90, 8, 100, 24,
				Hwnd, (HMENU)UI_LOG_CMD_DEFAULTCHANNEL, info->hInstance, NULL);

			BtnLogChnHwnd = CreateWindowExA(0, "BUTTON", ChangeLogChannel->Name, style, 198, 8, 100, 24,
				Hwnd, (HMENU)UI_LOG_CMD_CHANGELOGCHANNEL, info->hInstance, NULL);

			LabelHwnd = CreateWindowExW(0, L"STATIC", L"Channels:", WS_CHILD | WS_VISIBLE, 8, 12, 74, 16,
				Hwnd, NULL, info->hInstance, NULL);

			Button_SetCheck(BtnDefChnHwnd, BST_CHECKED);

			// Set default position
			INT32 winX = g_INI->GetInteger("CreationKit_Log", "X", info->x);
			INT32 winY = g_INI->GetInteger("CreationKit_Log", "Y", info->y);
			INT32 winW = g_INI->GetInteger("CreationKit_Log", "Width", info->cx);
			INT32 winH = g_INI->GetInteger("CreationKit_Log", "Height", info->cy);

			MoveWindow(Hwnd, winX, winY, winW, winH, FALSE);

			if (((winW != 0) && (winH != 0)) && (!g_INI->GetBoolean("CreationKit_Log", "TurnOffLogWindow", FALSE))) {
				ShowWindow(Hwnd, SW_SHOW);
				UpdateWindow(Hwnd);
			}
		}
		return S_OK;

		case WM_DESTROY:
			DestroyWindow(LabelHwnd);
			DestroyWindow(BtnDefChnHwnd);
			DestroyWindow(BtnLogChnHwnd);
			DestroyWindow(richEditHwnd);
			DestroyWindow(BorderEditHwnd);
			return S_OK;

		// Don't let us reduce the window too much
		case WM_GETMINMAXINFO: {
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 300;
			lpMMI->ptMinTrackSize.y = 200;

			return S_OK;
		}

		case WM_SIZE: {
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			
			MoveWindow(BorderEditHwnd, 8, 40, w - 16, h - 48, TRUE);

			RECT rc;
			GetClientRect(BorderEditHwnd, &rc);
			MoveWindow(richEditHwnd, 1, 1, rc.right - 2, rc.bottom - 2, TRUE);
		}
		break;

		case WM_ACTIVATE: {
			if (wParam != WA_INACTIVE)
				SetFocus(richEditHwnd);
		}
		return S_OK;

		case WM_CLOSE:
			ShowWindow(Hwnd, SW_HIDE);
			MainWindow::GetMainMenuObj().GetItem(UI_EXTMENU_SHOWLOG).Checked = FALSE;
			return S_OK;

		case WM_NOTIFY: {
			static uint64_t lastClickTime;
			auto notification = (const LPNMHDR)lParam;

			if (notification->code == EN_MSGFILTER) {
				auto msgFilter = (const MSGFILTER *)notification;

				if (msgFilter->msg == WM_LBUTTONDBLCLK)
					lastClickTime = GetTickCount64();
			}
			else if (notification->code == EN_SELCHANGE) {
				if (CurrentChannel != ChangeLogChannel)
					break;
				
				auto selChange = (const SELCHANGE*)notification;

				// Mouse double click with a valid selection -> try to parse form id
				if ((GetTickCount64() - lastClickTime > 1000) || selChange->seltyp == SEL_EMPTY)
					break;

				CHAR lineData[4096];
				*(uint16_t*)&lineData[0] = ARRAYSIZE(lineData);

				// Get the line number & text from the selected range
				LRESULT lineIndex = hk_SendMessageA(richEditHwnd, EM_LINEFROMCHAR, selChange->chrg.cpMin, 0);
				LRESULT charCount = hk_SendMessageA(richEditHwnd, EM_GETLINE, lineIndex, (LPARAM)&lineData);

				if (charCount > 0) {
					lineData[charCount - 1] = '\0';

					// Capture each hexadecimal form id in the format of "(XXXXXXXX)"
					for (LPSTR p = lineData; p[0] != '\0'; p++) {
						if (p[0] == '(' && strlen(p) >= 10 && p[9] == ')') {
							uint32_t id = strtoul(&p[1], NULL, 16);
							PostMessageA(MainWindow::GetWindow(), WM_COMMAND, UI_EDITOR_OPENFORMBYID, id);
						}
					}
				}

				lastClickTime = GetTickCount64() + 1000;
			}
		}
		break;
		
		case WM_TIMER: {
			if (wParam != UI_LOG_CMD_ADDTEXT)
				break;

			if (DefaultChannel->CountPend() > 0)
				WndProc(Hwnd, UI_LOG_CMD_ADDTEXT, (WPARAM)DefaultChannel, 0);

			if (ChangeLogChannel->CountPend() > 0)
				WndProc(Hwnd, UI_LOG_CMD_ADDTEXT, (WPARAM)ChangeLogChannel, 0);
		}
		return S_OK;

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case UI_LOG_CMD_DEFAULTCHANNEL:
				ChangeChannel(DefaultChannel);
				break;

			case UI_LOG_CMD_CHANGELOGCHANNEL:
				ChangeChannel(ChangeLogChannel);
				break;
			}
		}
		return S_OK;

		case UI_LOG_CMD_ADDTEXT: {
			if (CurrentChannel != (Channel*)wParam)
				((std::vector<std::string>*)((Channel*)wParam)->GetPend())->clear();

			hk_SendMessageA(richEditHwnd, WM_SETREDRAW, FALSE, 0);

			// Save old position if not scrolling
			POINT scrollRange;

			if (!autoScroll)
				hk_SendMessageA(richEditHwnd, EM_GETSCROLLPOS, 0, (WPARAM)&scrollRange);
			
			// Get a copy of all elements and clear the global list
			auto messages(std::move(*((std::vector<std::string>*)CurrentChannel->GetPend())));

			for (std::string msg : messages) {
				// Move caret to the end, then write
				CHARRANGE range;
				range.cpMax = LONG_MAX;
				range.cpMin = LONG_MAX;

				hk_SendMessageA(richEditHwnd, EM_EXSETSEL, 0, (LPARAM)&range);
				hk_SendMessageA(richEditHwnd, EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
			}

			if (!autoScroll)
				hk_SendMessageA(richEditHwnd, EM_SETSCROLLPOS, 0, (WPARAM)&scrollRange);

			hk_SendMessageA(richEditHwnd, WM_SETREDRAW, TRUE, 0);
			RedrawWindow(richEditHwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
		}
		return S_OK;

		case UI_LOG_CMD_CLEARTEXT: {
			if (CurrentChannel)
				CurrentChannel->Clear();
			
			char emptyString[1];
			emptyString[0] = '\0';

			hk_SendMessageA(richEditHwnd, WM_SETTEXT, 0, (LPARAM)&emptyString);
		}
		return S_OK;

		case UI_LOG_CMD_AUTOSCROLL:
			autoScroll = (BOOL)wParam;
			return S_OK;
		}

		return DefWindowProc(Hwnd, Message, wParam, lParam);
	}

	VOID FIXAPI LogVa(LPCSTR Format, va_list Va) {
		if (ChangeLogChannel)
			ChangeLogChannel->WriteVa(Format, Va);
	}

	VOID FIXAPI LogWcVa(LPCWSTR Format, va_list Va) {
		auto len = _vsnwprintf(NULL, 0, Format, Va);

		std::wstring message;
		message.resize(len);
		_vsnwprintf(&message[0], _TRUNCATE, Format, Va);

		std::string conv_message = XUtil::Conversion::WideToAnsi(message);

		Log("%s", conv_message.c_str());
	}

	VOID FIXAPI Log(LPCSTR Format, ...) {
		va_list va;

		va_start(va, Format);
		LogVa(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWc(LPCWSTR Format, ...) {
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

		va_list va;
		va_start(va, Message);

		auto len = _vsnprintf(NULL, 0, Message, va);
		std::string message;
		message.resize(len);
		_vsnprintf(&message[0], _TRUNCATE, Message, va);

		va_end(va);

		Log("ASSERTION: %s (%s line %d)", &message[0], File, Line);
	}

	VOID FIXAPI LogInsteadOfMsgBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
		if (lpText)
			Log("MESSAGE: %s", lpText);
	}

	VOID FIXAPI LogVaDef(LPCSTR Format, va_list Va) {
		if (DefaultChannel)
			DefaultChannel->WriteVa(Format, Va);
	}

	VOID FIXAPI LogDef(LPCSTR Format, ...) {
		va_list va;

		va_start(va, Format);
		LogVaDef(Format, va);
		va_end(va);
	}

	VOID FIXAPI LogWcVaDef(LPCWSTR Format, va_list Va) {
		auto len = _vsnwprintf(NULL, 0, Format, Va);

		std::wstring message;
		message.resize(len);
		_vsnwprintf(&message[0], _TRUNCATE, Format, Va);

		std::string conv_message = XUtil::Conversion::WideToAnsi(message);

		LogDef("%s", conv_message.c_str());
	}

	VOID FIXAPI LogWcDef(LPCWSTR Format, ...) {
		va_list va;

		va_start(va, Format);
		LogWcVaDef(Format, va);
		va_end(va);
	}
}