#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ScrollBar
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBackgroundHorz(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawBackgroundVert(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSliderHorz_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSliderHorz_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSliderVert_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSliderVert_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}
			}
		}
	}
}