#pragma once

#include "../../common.h"
#include "UIBaseWindow.h"

#define UI_VISIBLE_CHECKBUTTON_MOVABLESTATICS 4033  // see resource.rc

namespace ShowHideWindow
{
	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Core::Classes::UI::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}