#pragma once

#include "..\UIGraphics.h"

#include <set>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace SysMenu
			{
				enum SysMenu
				{
					SM_CLOSE,
					SM_MINIMIZE,
					SM_MAXIMIZE
				};

				typedef std::set<SysMenu> CUISysMenuButtons;

				HWND WINAPI Initialization(HWND hParentWindow, HWND hTargetWindow);
				VOID WINAPI SetSysMenuButton(HWND hWnd, const CUISysMenuButtons& sets);

				LRESULT CALLBACK SysMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}