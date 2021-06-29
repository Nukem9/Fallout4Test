#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TrackBar
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawTrack(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSlider_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSlider_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSlider_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSlider_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawSlider_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}
			}
		}
	}
}