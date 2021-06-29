#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"

namespace ActorWindow
{
	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Core::Classes::UI::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}