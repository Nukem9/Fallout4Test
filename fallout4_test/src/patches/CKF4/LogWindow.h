#pragma once

#include "../../common.h"

namespace LogWindow
{
	HWND FIXAPI GetWindow(VOID);
	HANDLE FIXAPI GetStdoutListenerPipe(VOID);

	BOOL FIXAPI Initialize(VOID);
	BOOL FIXAPI CreateStdoutListener(VOID);
	VOID FIXAPI LoadWarningBlacklist(VOID);

	LRESULT CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	VOID FIXAPI LogVa(LPCSTR Format, va_list Va);
	VOID FIXAPI Log(LPCSTR Format, ...);
	VOID FIXAPI LogWarningVa(INT32 Type, LPCSTR Format, va_list& Va);
	VOID FIXAPI LogWarning(INT32 Type, LPCSTR Format, ...);
	VOID FIXAPI LogWarningUnknown1(LPCSTR Format, ...);
	VOID FIXAPI LogWarningUnknown2(INT64 Unused, LPCSTR Format, ...);
	VOID FIXAPI LogAssert(LPCSTR File, INT32 Line, LPCSTR Message, ...);

	VOID FIXAPI LogWcVa(LPCWSTR Format, va_list Va);
	VOID FIXAPI LogWc(LPCWSTR Format, ...);
}