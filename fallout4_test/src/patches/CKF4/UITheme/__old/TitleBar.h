#pragma once

#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TitleBar
			{
				HWND WINAPI Initialization(HINSTANCE hInstance, HWND hWindow);

				LRESULT CALLBACK TitleBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}