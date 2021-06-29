#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace EditText
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawEditText_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clBorderColor,
						COLORREF clColor, COLORREF clColorDividerStart, COLORREF clColorDividerEnd);

					VOID FIXAPI DrawEditText_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawEditText_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawEditText_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawEditText_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				INT_PTR FIXAPI OnCtlColorEdit(HDC hDC);
			}
		}
	}
}