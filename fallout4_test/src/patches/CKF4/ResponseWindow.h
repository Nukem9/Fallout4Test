#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"

#define UI_BUTTON_GENERATE_LIP_FILE 1016
#define UI_CHECKBUTTON_WAV_FILE		2379
#define UI_CHECKBUTTON_LTF_FILE		2380
#define UI_LISTVIEW_ITEMS			2168

namespace ResponseWindow
{
	namespace Classes = Core::Classes::UI;

	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Classes::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}