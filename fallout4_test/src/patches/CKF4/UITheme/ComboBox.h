#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ComboBox
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawArrow_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor, COLORREF cShadowColor);
					VOID FIXAPI DrawArrow_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawArrow_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId);
				}
			}
		}
	}
}