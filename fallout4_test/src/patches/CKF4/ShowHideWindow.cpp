#include "ShowHideWindow.h"
#include "UICheckboxControl.h"
#include "Editor.h"

namespace ShowHideWindow
{
	namespace Classes = Core::Classes::UI;
	Classes::CUICustomWindow ShowHideWindow;
	static BOOL Initialize = FALSE;

	struct ShowHideWindowControls_t
	{
		Classes::CUICheckbox CheckBtnMovableStatics;
	} ShowHideWindowControls;

	DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(VOID)
	{
		return ShowHideWindow.Handle;
	}

	Core::Classes::UI::CUICustomWindow& FIXAPI GetWindowObj(VOID)
	{
		return ShowHideWindow;
	}

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_INITDIALOG)
		{
			if (!Initialize)
			{
				Initialize = TRUE;
				ShowHideWindow = DialogHwnd;
				ShowHideWindowControls.CheckBtnMovableStatics.CreateWnd(ShowHideWindow, ShowHideWindow.GetControl(UI_VISIBLE_CHECKBUTTON_MOVABLESTATICS), UI_VISIBLE_CHECKBUTTON_MOVABLESTATICS);
				// Default value
				ShowHideWindowControls.CheckBtnMovableStatics.Checked = TRUE;
			}			
		}
		else if (Message == WM_DESTROY)
		{
			ShowHideWindowControls.CheckBtnMovableStatics.Release();
		}
		else if (Message == WM_COMMAND)
		{
			switch (LOWORD(wParam))
			{
			case UI_VISIBLE_CHECKBUTTON_MOVABLESTATICS:
				HiddenMovableStatic(ShowHideWindowControls.CheckBtnMovableStatics.Checked);
				ShowHideWindowControls.CheckBtnMovableStatics.Checked = !ShowHideWindowControls.CheckBtnMovableStatics.Checked;
				return 0;
			}
		}

		return OldDlgProc(DialogHwnd, Message, wParam, lParam);
	}
}