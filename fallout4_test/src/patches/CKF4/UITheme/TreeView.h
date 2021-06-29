#pragma once

#include <Uxtheme.h>

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TreeView
			{
				HTHEME FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK TreeViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}