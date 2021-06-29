#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PopupMenu
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBackground_NonClientArray(Graphics::CUICanvas& canvas);
					VOID FIXAPI DrawItem_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawItem_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawItem_Divider(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawItem_Checkbox(Graphics::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected, BOOL bDisabled);
					VOID FIXAPI DrawItem_Arrow(Graphics::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected);
				}

				BOOL FIXAPI IsSystemPopupMenu(HWND hWindow, HMENU hMenu);
				BOOL FIXAPI IsSystemPopupMenuBlindly(HWND hWindow);

				VOID FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK PopupMenuSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

				namespace Event
				{
					VOID FIXAPI OnInitPopupMenu(HWND hWindow, HMENU hMenu);
					VOID FIXAPI OnDrawNoClientPopupMenu(HWND hWindow, HDC hDC);
					VOID FIXAPI OnMeasureItem(HWND hWindow, LPMEASUREITEMSTRUCT lpMeasureItem);
					VOID FIXAPI OnDrawItem(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem);
				}
			}
		}
	}
}