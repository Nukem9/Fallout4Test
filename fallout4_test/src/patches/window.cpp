/*
Author: Perchik71 29/04/2021
This file is part of Fallout 4 Fixes source code.

Adapted for Fallout 4 and Fallout 4 CK
The original
URL: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/window.cpp
*/

#include "../common.h"
#include <future>

#define WM_APP_UPDATE_CURSOR	(WM_APP + 0x9A36)

static HWND g_Fallout4Window = NULL;
static DWORD MessageThreadId;
static WNDPROC g_OriginalWndProc;
static BOOL MouseEnableInput = TRUE;
static BOOL KeyboardEnableInput = TRUE;
static BOOL OtherEnableInput = TRUE;
static CHAR* KeyBuffer = NULL;


/*
==================
Sys_MouseToggle

Enable/Disable the mouse
==================
*/
VOID FIXAPI Sys_MouseToggle(BOOL bV)
{
	if (bV == MouseEnableInput) 
		return;

	MouseEnableInput = bV;
	if (bV)
	{
		while (ShowCursor(FALSE) >= 0) {}
	}
	else
	{
		while (ShowCursor(TRUE) < 0) {}
	}
}


/*
==================
Sys_KeyboardToggle

Enable/Disable the keyboard
==================
*/
VOID FIXAPI Sys_KeyboardToggle(BOOL bV)
{
	if (bV == MouseEnableInput)
		return;

	KeyboardEnableInput = bV;
}

/*
==================
Sys_OtherDeviceToggle

Enable/Disable the other input device
==================
*/
VOID FIXAPI Sys_OtherDeviceToggle(BOOL bV)
{
	if (bV == OtherEnableInput)
		return;

	OtherEnableInput = bV;
}


/*
==================
Sys_GetKeyboardBuffer

Returns the address of the keyboard input array
==================
*/
BOOL FIXAPI Sys_GetKeyboardBuffer(CHAR* &buffer)
{
	buffer = NULL;
	LPVOID* lpBuffer = (LPVOID*)(OFFSET(0x5A5F668, 0));
	if (!*lpBuffer)
		return FALSE;

	buffer = ((CHAR*(__fastcall*)(LPVOID))OFFSET(0x1B2BFF0, 0))(*lpBuffer) + 0x170;
	return TRUE;
}


/*
==================
WindowProc

==================
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_APP_UPDATE_CURSOR:
	{
		RECT rcClip;
		GetWindowRect(hwnd, &rcClip);

		// 1 pixel of padding
		rcClip.left += 1;
		rcClip.top += 1;

		rcClip.right -= 1;
		rcClip.bottom -= 1;

		ClipCursor(&rcClip);
	}
	return 0;

	// Handle window getting/losing focus (hide/show cursor)
	case WM_ACTIVATEAPP:
	case WM_ACTIVATE:
	case WM_SETFOCUS:
	{
		// Gained focus
		if (((uMsg == WM_ACTIVATEAPP) && (wParam == TRUE)) ||
			((uMsg == WM_ACTIVATE) && (wParam != WA_INACTIVE)) ||
			(uMsg == WM_SETFOCUS))
		{
			if (GetForegroundWindow() == g_Fallout4Window)
			{
				Sys_MouseToggle(TRUE);
				Sys_KeyboardToggle(TRUE);
				Sys_OtherDeviceToggle(TRUE);

				if (Sys_GetKeyboardBuffer(KeyBuffer))
					*(KeyBuffer + 0xA4) = 0;
			}
		}

		// Lost focus
		if ((uMsg == WM_ACTIVATEAPP && wParam == FALSE) ||
			(uMsg == WM_ACTIVATE && wParam == WA_INACTIVE))
		{
			Sys_MouseToggle(FALSE);
			Sys_KeyboardToggle(FALSE);
			Sys_OtherDeviceToggle(FALSE);

			if (Sys_GetKeyboardBuffer(KeyBuffer))
				*(KeyBuffer + 0xA4) = 0;
		}
	}
	return CallWindowProcA(g_OriginalWndProc, hwnd, uMsg, wParam, lParam);

	// Handle mouse events input
	case WM_MOUSEMOVE:
		if (g_Fallout4Window == hwnd)
			PostMessageA(hwnd, WM_APP_UPDATE_CURSOR, 0, 0);

	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
		return (MouseEnableInput) ? CallWindowProcA(g_OriginalWndProc, hwnd, uMsg, wParam, lParam) : 0;

	// Handle keyboard events input
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_CHAR:
	case WM_KEYUP:
	case WM_KEYDOWN:
		return (KeyboardEnableInput) ? CallWindowProcA(g_OriginalWndProc, hwnd, uMsg, wParam, lParam) : 0;

	// Handle other devices events input  5AA43F0 rbp+170+symbol
	case WM_INPUT:
		return (OtherEnableInput) ? CallWindowProcA(g_OriginalWndProc, hwnd, uMsg, wParam, lParam) : 0;
		
	default:
		return CallWindowProcA(g_OriginalWndProc, hwnd, uMsg, wParam, lParam);
	}
}


/*
==================
hk_CreateWindowExA

Replacement WINAPI CreateWindowExA
==================
*/
HWND WINAPI hk_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, INT x, INT y, INT nWidth, INT nHeight, 
							   HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND wnd = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (wnd && !g_Fallout4Window && !strcmp(lpClassName, "Fallout4"))
		// The original pointer must be saved BEFORE swapping it out
		g_Fallout4Window = wnd;

	return wnd;
}


/*
==================
Fix_PatchWindow

Game window patch
==================
*/
VOID FIXAPI Fix_PatchWindow(VOID)
{	
	*(uintptr_t*)&g_OriginalWndProc = Detours::X64::DetourFunctionClass(OFFSET(0xD40190, 0), &WindowProc);

	XUtil::PatchMemoryNop(OFFSET(0xD38ED6, 0), 5);		// bimbo function clear
	XUtil::PatchMemoryNop(OFFSET(0xD38EE7, 0), 41);		// skip Alt+Tab detect
	XUtil::PatchMemory(OFFSET(0x1D17EE1, 0), { 0xEB }); // No Caption
	XUtil::PatchMemory(OFFSET(0x1D17FE7, 0), { 0xEB }); // No HWND Top

	PatchIAT(hk_CreateWindowExA, "USER32.DLL", "CreateWindowExA");
}