#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace CheckBox
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawCheck_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clShadow, COLORREF clColor);
					VOID FIXAPI DrawCheck_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawCheck_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawCheck_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawCheck_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);

					VOID FIXAPI DrawIdeterminate_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor);
					VOID FIXAPI DrawIdeterminate_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawIdeterminate_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawIdeterminate_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawIdeterminate_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId);
				}
			}
		}
	}
}