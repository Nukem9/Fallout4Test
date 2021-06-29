#pragma once

#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace MoveWndBar
			{
				HWND WINAPI Initialization(HWND hParentWindow, HWND hMovableWindow);

				LRESULT CALLBACK MoveWndBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}