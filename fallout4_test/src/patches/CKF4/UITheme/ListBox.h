#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

#include <Uxtheme.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ListBox
			{
				HTHEME FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK ListBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

				INT_PTR FIXAPI OnCtlColorListBox(HWND hWindow, HDC hDC);
			}
		}
	}
}