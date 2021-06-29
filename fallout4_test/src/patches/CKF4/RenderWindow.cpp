#include "RenderWindow.h"
#include "MainWindow.h"

namespace RenderWindow
{
	Core::Classes::UI::CUICustomWindow RenderViewWindow;

	struct RenderWindowControls_t
	{
		BOOL IsCollisionView;
	} RenderWindowControls;

	DLGPROC OldDlgProc;

	HWND GetWindow(void)
	{
		return RenderViewWindow.Handle;
	}

	Core::Classes::UI::CUICustomWindow& GetWindowObj(void)
	{
		return RenderViewWindow;
	}

	BOOL IsCollisionView(void)
	{
		return RenderWindowControls.IsCollisionView;
	}

	void SetCollisionView(const BOOL Value)
	{
		RenderWindowControls.IsCollisionView = Value;
	}

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_INITDIALOG)
		{
			RenderViewWindow = DialogHwnd;
			RenderWindowControls.IsCollisionView = FALSE;
		}
		else if (Message == WM_KEYUP)
		{
			auto ctrl = HIBYTE(GetKeyState(VK_CONTROL)) & 0x80;

			if (ctrl)
			{
				if (wParam == '5')
				{
					// Fake click fog

					MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(23).Click();
				}
			}
			else
			{
				if (wParam == 'M')
				{
					// If you click on M, the menu will still have the previous state, we will fix this. 
					// However, in fact, there should be two requests to show or hide, but the second one is ignored and this is good.

					MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(15).Click();
				}
				else if (wParam == 'S')
				{
					// Fix that only worked with the menu

					MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(17).Click();
				}
			}
		}

		return OldDlgProc(DialogHwnd, Message, wParam, lParam);
	}
}