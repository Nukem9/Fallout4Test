#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"
#include "UICheckboxControl.h"

#include <CommCtrl.h>

#define UI_CELL_VIEW_FILTER_LABEL				1165	// See: resource.rc
#define UI_CELL_VIEW_ADD_CELL_ITEM				2579
#define UI_CELL_VIEW_CHECKBOX					2580	// See: resource.rc
#define UI_CELL_VIEW_CHECK_ACTIVE_CELL_OBJECTS	2582	// See: resource.rc
#define UI_CELL_VIEW_ADD_CELL_OBJECT_ITEM		2583	
#define UI_CELL_WINDOW_ADD_ITEM					2579
#define UI_CELL_VIEW_FILTER_EDITTEXT			2585	// See: resource.rc
#define UI_CELL_VIEW_WORLDSPACE					2083	// See: resource.rc
#define UI_TIMER_FILTER_CELL					7799

namespace CellViewWindow
{
	namespace Classes = Core::Classes::UI;

	extern DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID);
	Classes::CUICustomWindow& FIXAPI GetWindowObj(VOID);

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	VOID FIXAPI hk_7FF70C322BC0(HWND ListViewHandle, TESForm_CK* Form, BOOL UseImage, int32_t ItemIndex);
	INT32 FIXAPI hk_call_5A43B5(HWND** ListViewHandle, TESForm_CK** Form, INT64 a3);
}