#pragma once

#include "..\UIGraphics.h"
#include "..\UIBaseWindow.h"

#include "TitleBar.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace FlatMainWindow
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID WINAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID WINAPI DrawCaption(Graphics::CUICanvas& canvas, LPCSTR lpstrCaption, LPCRECT pRect);
					VOID WINAPI DrawErase(Graphics::CUICanvas& canvas);
				}

				VOID WINAPI Initialize(HWND hWindow);

				HWND WINAPI GetTitleBarWindow(VOID);
				HWND WINAPI GetBandWindow(VOID);
				Graphics::CUICustomWindow& WINAPI GetTitleBarWindowObj(VOID);
				Graphics::CUICustomWindow& WINAPI GetBandWindowObj(VOID);

				LRESULT CALLBACK FlatWindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}