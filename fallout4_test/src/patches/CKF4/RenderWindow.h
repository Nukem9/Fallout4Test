#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"

namespace RenderWindow
{
	extern DLGPROC OldDlgProc;

	HWND GetWindow(void);
	Core::Classes::UI::CUICustomWindow& GetWindowObj(void);

	BOOL IsCollisionView(void);
	void SetCollisionView(const BOOL Value);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}