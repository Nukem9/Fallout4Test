#include "ObjectWindow.h"
#include "EditorUI.h"
#include "MainWindow.h"

#include <unordered_map>

#define UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW		(UI_CUSTOM_MESSAGE + 4)

namespace ObjectWindow
{
	OBJWNDS ObjectWindows;
	DLGPROC OldDlgProc;

	HWND FIXAPI GetWindow(UINT uId)
	{
		UINT i = 0;

		for (auto it = ObjectWindows.begin(); ObjectWindows.end() != it; it++, i++) 
		{
			if (i == uId)
				return (*it).first;
		}

		return NULL;
	}

	OBJWNDS& FIXAPI GetAllWindowObj(VOID)
	{
		return ObjectWindows;
	}

	BOOL WINAPI hk_MoveWindow(HWND hWindow, INT32 X, INT32 Y, INT32 nWidth, INT32 nHeight, BOOL bRepaint)
	{
		BOOL bResult = MoveWindow(hWindow, X, Y, nWidth, nHeight, bRepaint);

		if (auto iterator = ObjectWindows.find(GetParent(hWindow)); iterator != ObjectWindows.end())
		{
			LPOBJWND lpObjWnd = (*iterator).second;
			if (lpObjWnd) lpObjWnd->ObjectWindow.Perform(WM_COMMAND, UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW, 0);
		}

		return bResult;
	}

	VOID FIXAPI ResizeObjectWndChildControls(LPOBJWND lpObjWnd)
	{
		// The perfectionist in me is dying....

		lpObjWnd->Controls.TreeList.LockUpdate();
		lpObjWnd->Controls.ItemList.LockUpdate();
		lpObjWnd->Controls.EditFilter.LockUpdate();
		lpObjWnd->Controls.ToggleDecompose.LockUpdate();
		lpObjWnd->Controls.BtnObjLayout.LockUpdate();

		LONG w_tree = lpObjWnd->Controls.TreeList.Width;
		LONG w_left = w_tree - lpObjWnd->Controls.BtnObjLayout.Width + 1;
		lpObjWnd->Controls.BtnObjLayout.Left = w_left;
		lpObjWnd->Controls.ToggleDecompose.Left = w_left;
		lpObjWnd->Controls.ActiveOnly.Width = w_tree;

		w_left = w_left - lpObjWnd->Controls.EditFilter.Left - 3;
		lpObjWnd->Controls.EditFilter.Width = w_left;
		lpObjWnd->Controls.ComboLayout.Width = w_left;

		lpObjWnd->Controls.ItemList.Left = w_tree + 5;
		lpObjWnd->Controls.ItemList.Width = lpObjWnd->ObjectWindow.ClientWidth() - (w_tree + 5);

		RedrawWindow(lpObjWnd->ObjectWindow.Handle, NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_NOCHILDREN);
		lpObjWnd->Controls.BtnObjLayout.UnlockUpdate();
		lpObjWnd->Controls.ToggleDecompose.UnlockUpdate();
		lpObjWnd->Controls.EditFilter.UnlockUpdate();
		lpObjWnd->Controls.ItemList.UnlockUpdate();
		lpObjWnd->Controls.TreeList.UnlockUpdate();
		lpObjWnd->Controls.BtnObjLayout.Repaint();
		lpObjWnd->Controls.ToggleDecompose.Repaint();
		lpObjWnd->Controls.EditFilter.Repaint();
		lpObjWnd->Controls.ItemList.Repaint();
		lpObjWnd->Controls.TreeList.Repaint();
	}

	VOID FIXAPI SetObjectWindowFilter(LPOBJWND lpObjWnd, const std::string & name, const BOOL SkipText, const BOOL actived)
	{
		if (!SkipText)
			lpObjWnd->Controls.EditFilter.Caption = name;

		lpObjWnd->Controls.ActiveOnly.Checked = actived;
		// Force the list items to update as if it was by timer
		//lpObjWnd->ObjectWindow.Perform(WM_TIMER, 0x1B58, 0);
		// This is a bit slower but works in multi windows
		lpObjWnd->Controls.EditFilter.Caption = lpObjWnd->Controls.EditFilter.Caption;
	}

	int32_t FIXAPI hk_7FF72F57F8F0(const int64_t ObjectListInsertData, TESForm_CK* Form)
	{
		if (ObjectListInsertData)
		{
			LPOBJWND lpObjWnd = ObjectWindows.at(GetParent((*(HWND*)(ObjectListInsertData + 0x18))));
			if (lpObjWnd)
			{
				BOOL allowInsert = TRUE;

				lpObjWnd->ObjectWindow.Perform(UI_OBJECT_WINDOW_ADD_ITEM, (WPARAM)Form, (LPARAM)&allowInsert);

				if (!allowInsert)
					return 1;
			}
		}

		return ((int32_t(__fastcall*)(int64_t, TESForm_CK*))OFFSET(0x40F8F0, 0))(ObjectListInsertData, Form);
	}

	INT_PTR CALLBACK DlgProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_INITDIALOG)
		{
			LPOBJWND lpObjWnd = new OBJWND;

			lpObjWnd->ObjectWindow = DialogHwnd;
				
			lpObjWnd->Controls.TreeList = lpObjWnd->ObjectWindow.GetControl(2093);
			lpObjWnd->Controls.ItemList = lpObjWnd->ObjectWindow.GetControl(1041);
			lpObjWnd->Controls.ToggleDecompose = lpObjWnd->ObjectWindow.GetControl(6027);
			lpObjWnd->Controls.BtnObjLayout = lpObjWnd->ObjectWindow.GetControl(6025);
			lpObjWnd->Controls.ComboLayout = lpObjWnd->ObjectWindow.GetControl(6024);
			lpObjWnd->Controls.EditFilter = lpObjWnd->ObjectWindow.GetControl(2581);
			lpObjWnd->Controls.Spliter = lpObjWnd->ObjectWindow.GetControl(2157);
			lpObjWnd->Controls.ActiveOnly.CreateWnd(lpObjWnd->ObjectWindow, lpObjWnd->ObjectWindow.GetControl(UI_OBJECT_WINDOW_ADD_ITEM), UI_OBJECT_WINDOW_ADD_ITEM);

			// Eliminate the flicker when resizing
			lpObjWnd->Controls.TreeList.Perform(TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
			// Eliminate the flicker when changing categories
			ListView_SetExtendedListViewStyleEx(lpObjWnd->Controls.ItemList.Handle, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

			// Erase Icon and SysMenu
			if (!ObjectWindows.size())
				lpObjWnd->ObjectWindow.Style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME;
			else
				lpObjWnd->ObjectWindow.Style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

			lpObjWnd->Controls.BtnObjLayout.Top = lpObjWnd->Controls.ComboLayout.Top - 1;
			lpObjWnd->Controls.BtnObjLayout.Height = lpObjWnd->Controls.ComboLayout.Height + 2;
			lpObjWnd->Controls.ToggleDecompose.Top = lpObjWnd->Controls.EditFilter.Top - 1;
			lpObjWnd->Controls.ToggleDecompose.Height = lpObjWnd->Controls.EditFilter.Height + 2;

			ObjectWindows.emplace(DialogHwnd, lpObjWnd);
		}
		// Don't let us reduce the window too much
		else if (Message == WM_GETMINMAXINFO)
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 350;
			lpMMI->ptMinTrackSize.y = 200;

			return S_OK;
		}
		else if (Message == WM_SIZE)
		{
			if (auto iterator = ObjectWindows.find(DialogHwnd);  iterator != ObjectWindows.end())
			{
				LPOBJWND lpObjWnd = (*iterator).second;
				if (lpObjWnd && !lpObjWnd->StartResize)
				{
					lpObjWnd->StartResize = TRUE;
					ResizeObjectWndChildControls(lpObjWnd);
				}
			}
		}
		else if (Message == UI_OBJECT_WINDOW_ADD_ITEM)
		{
			auto form = reinterpret_cast<const TESForm_CK*>(wParam);
			auto allowInsert = reinterpret_cast<BOOL*>(lParam);
			*allowInsert = TRUE;

			if (auto iterator = ObjectWindows.find(DialogHwnd); iterator != ObjectWindows.end())
			{
				LPOBJWND lpObjWnd = (*iterator).second;
				if (lpObjWnd && lpObjWnd->Controls.ActiveOnly.Checked)
				{
					if (form && !form->Active)
						*allowInsert = FALSE;
				}
			}

			return S_OK;
		}
		else if (Message == WM_COMMAND)
		{
			switch (wParam)
			{
			case UI_OBJECT_WINDOW_ADD_ITEM:
			{
				if (auto iterator = ObjectWindows.find(DialogHwnd); iterator != ObjectWindows.end())
				{
					LPOBJWND lpObjWnd = (*iterator).second;
					if (lpObjWnd) SetObjectWindowFilter(lpObjWnd, "", TRUE, !lpObjWnd->Controls.ActiveOnly.Checked);
				}
			}
			return S_OK;
				
			case UI_CMD_CHANGE_SPLITTER_OBJECTWINDOW:
			{
				if (auto iterator = ObjectWindows.find(DialogHwnd); iterator != ObjectWindows.end())
				{
					LPOBJWND lpObjWnd = (*iterator).second;
					if (lpObjWnd) ResizeObjectWndChildControls(lpObjWnd);
				}
			}
			return S_OK;

			}
		}
		else if (Message == WM_SHOWWINDOW)
		{
			if (auto iterator = ObjectWindows.find(DialogHwnd); iterator != ObjectWindows.end())
			{
				LPOBJWND lpObjWnd = (*iterator).second;
				if (lpObjWnd) ResizeObjectWndChildControls(lpObjWnd);
			}
		}
		else if (Message == WM_DESTROY)
		{
			LPOBJWND lpObjWnd = ObjectWindows.at(DialogHwnd);
			if (lpObjWnd)
			{
				ObjectWindows.erase(DialogHwnd);

				delete lpObjWnd;
				lpObjWnd = NULL;
			}
		}

		return OldDlgProc(DialogHwnd, Message, wParam, lParam);
	}
}