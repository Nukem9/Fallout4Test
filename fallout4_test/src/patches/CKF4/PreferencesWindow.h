#pragma once

#include "../../common.h"
#include "UIBaseWindow.h"

namespace PreferencesWindow
{
	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Core::Classes::UI::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	VOID FIXAPI hk_SetInPreferencesToTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4);

	HWND WINAPI CreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
}