#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PageControl
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor);
					VOID FIXAPI DrawButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_SelectedAndFocused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags);
				}

				VOID FIXAPI Initialize(HWND hWnd);
			}
		}
	}
}