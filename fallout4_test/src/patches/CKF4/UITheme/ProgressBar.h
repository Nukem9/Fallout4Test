#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ProgressBar
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBar(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawFill(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				VOID FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK ProgressBarSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}