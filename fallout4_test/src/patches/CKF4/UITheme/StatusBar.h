#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace StatusBar
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawBackground(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags);
				}
			}
		}
	}
}