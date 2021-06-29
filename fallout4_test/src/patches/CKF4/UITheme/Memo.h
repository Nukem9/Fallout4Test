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
			namespace Memo
			{
				HTHEME FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK MemoSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}