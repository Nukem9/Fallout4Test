#pragma once

#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace Panel
			{
				HWND WINAPI Initialization(HWND hParentWindow);
				VOID WINAPI SetTranslateCommandMessageWindow(HWND hWindow, HWND hTranslateWindow);

				LRESULT CALLBACK PanelWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}