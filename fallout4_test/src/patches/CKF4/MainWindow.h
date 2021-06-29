#pragma once

#include "../../common.h"
#include "TESForm_CK.h"
#include "TESFile_CK.h"

#include "UIMenus.h"
#include "UIBaseWindow.h"

#define UI_EDITOR_TOOLBAR					1
#define UI_LOG_CMD_ADDTEXT					(WM_APP + 1)
#define UI_LOG_CMD_CLEARTEXT				(WM_APP + 2)
#define UI_LOG_CMD_AUTOSCROLL				(WM_APP + 3)
#define UI_EXTMENU_ID						51001
#define UI_EXTMENU_SHOWLOG					51002
#define UI_EXTMENU_CLEARLOG					51003
#define UI_EXTMENU_AUTOSCROLL				51004
#define UI_EXTMENU_SPACER					51005
#define UI_EXTMENU_LOADEDESPINFO			51006
#define UI_EXTMENU_HARDCODEDFORMS			51007
#define UI_EXTMENU_LINKS_ID					51010
#define UI_EXTMENU_LINKS_WIKI				51011
#define UI_EXTMENU_LINKS_MATERIALEDITOR		51012
#define UI_SHOW_MATERIALEDITOR				41514
#define UI_COLLISION_GEOM_CMD				0x9CFD
#define UI_FOG_CMD							0x9FE9
#define UI_SKY_TOGGLE_CMD					0x9D1F
#define UI_EDITOR_OPENFORMBYID				(UI_CUSTOM_MESSAGE + 1)	// Sent from the LogWindow on double click
#define UI_CMD_SHOWHIDE_OBJECTWINDOW		(UI_CUSTOM_MESSAGE + 2)
#define UI_CMD_SHOWHIDE_CELLVIEWWINDOW		(UI_CUSTOM_MESSAGE + 3)

namespace MainWindow
{
	namespace Classes = Core::Classes::UI;

	extern WNDPROC OldWndProc;

	BOOL FIXAPI IsActive(VOID);
	HWND FIXAPI GetWindow(VOID);
	Classes::CUIMainWindow& FIXAPI GetWindowObj(VOID);
	Classes::CUIMenu& FIXAPI GetMainMenuObj(VOID);

	LRESULT CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}