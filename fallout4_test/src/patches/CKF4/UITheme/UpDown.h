#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace UpDown
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawUp_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
					VOID FIXAPI DrawUp_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawUp_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor);
					VOID FIXAPI DrawDown_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawDown_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

					VOID FIXAPI DrawLeft_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
						COLORREF cHihglighter, BOOL drawHihglighter);
					VOID FIXAPI DrawLeft_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawLeft_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor, 
						COLORREF cHihglighter, BOOL drawHihglighter);
					VOID FIXAPI DrawRight_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRight_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				VOID FIXAPI Initialize(HWND hWindow);
				LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
			}
		}
	}
}