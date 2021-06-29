#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PushButton
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawPushButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd,
						COLORREF clGradientHighlighterColorStart, COLORREF clGradientHighlighterColorEnd,
						COLORREF clDividerColor, COLORREF clDividerHighlighterColor);
					VOID FIXAPI DrawPushButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawPushButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId);
				}
			}
		}
	}
}