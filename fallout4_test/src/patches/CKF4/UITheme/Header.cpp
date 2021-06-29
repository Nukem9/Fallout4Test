#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "Header.h"

#include "..\MainWindow.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace Header
			{
				namespace Render
				{
					VOID FIXAPI DrawBack_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd, COLORREF cLighter)
					{
						if (clGradientColorStart == clGradientColorEnd)
							canvas.Fill(*pRect, clGradientColorStart);
						else
							canvas.GradientFill(*pRect, clGradientColorStart, clGradientColorEnd, Graphics::gdVert);

						canvas.Pen.Style = Graphics::psSolid;
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(pRect->left, pRect->bottom - 1);
						canvas.LineTo(pRect->right, pRect->bottom - 1);

						canvas.Pen.Color = cLighter;
						canvas.MoveTo(pRect->left, pRect->top);
						canvas.LineTo(pRect->right, pRect->top);
					}

					VOID FIXAPI DrawBack_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawBack_Stylesheet(canvas, pRect, 
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					VOID FIXAPI DrawBack_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Hot_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					VOID FIXAPI DrawBack_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawBack_Selected(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawBack_Stylesheet(canvas, pRect,
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Header_Normal_Gradient_End));
					}

					VOID FIXAPI DrawDiv(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						if (pRect->left == 0)
							return;

						canvas.Pen.Style = Graphics::psSolid;
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(pRect->left, pRect->top);
						canvas.LineTo(pRect->left, pRect->bottom);
					}
				}
			}
		}
	}
}