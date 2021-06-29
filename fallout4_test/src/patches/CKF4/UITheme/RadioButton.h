#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace RadioButton
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawPushButtonR_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd,
						COLORREF clGradientHighlighterColorStart, COLORREF clGradientHighlighterColorEnd,
						COLORREF clDividerColor, COLORREF clDividerHighlighterColor);
					VOID FIXAPI DrawPushButtonR_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButtonR_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButtonR_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButtonR_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

					VOID FIXAPI DrawRadioButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor);
					VOID FIXAPI DrawRadioButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRadioButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRadioButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawRadioButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId);
				}
			}
		}
	}
}