#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"
#include "UICheckboxControl.h"

#include <CommCtrl.h>

#define UI_LISTVIEW_PLUGINS					1056						// See: resource.rc
#define UI_EDIT_SEARCH_PLUGIN_BY_NAME		(UI_CUSTOM_MESSAGE + 4)	
#define UI_NEW_LISTVIEW_CONTROL_TO_RESULT	(UI_CUSTOM_MESSAGE + 5)	
#define UI_SETACTIVEPLUGIN_BUTTON			1121						// See: resource.rc

namespace DataWindow
{
	namespace Classes = Core::Classes::UI;

	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Classes::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}