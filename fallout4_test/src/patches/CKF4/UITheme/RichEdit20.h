#pragma once

#include "..\UIGraphics.h"
#include "..\..\..\common.h"

#include <Uxtheme.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace RichEdit
			{
				HTHEME FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK RichEditSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}