#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace Header
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBack_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd, COLORREF cLighter);
					VOID FIXAPI DrawBack_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawBack_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawBack_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawBack_Selected(Graphics::CUICanvas& canvas, LPCRECT pRect);

					VOID FIXAPI DrawDiv(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}
			}
		}
	}
}