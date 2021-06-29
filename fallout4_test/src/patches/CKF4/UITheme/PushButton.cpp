#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "PushButton.h"

#include <vsstyle.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PushButton
			{
				namespace Render
				{
					VOID FIXAPI DrawPushButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect,
						COLORREF clGradientColorStart, COLORREF clGradientColorEnd,
						COLORREF clGradientHighlighterColorStart, COLORREF clGradientHighlighterColorEnd,
						COLORREF clDividerColor, COLORREF clDividerHighlighterColor)
					{
						Core::Classes::UI::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Inflate(-1, -1);
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						if (clGradientHighlighterColorStart == clGradientHighlighterColorEnd)
							canvas.Fill(*pRect, clGradientHighlighterColorStart);
						else
							canvas.GradientFill(*pRect, clGradientHighlighterColorStart, clGradientHighlighterColorEnd, Core::Classes::UI::gdVert);
						canvas.Fill(rc_temp[0], clDividerHighlighterColor);
						canvas.GradientFill(rc_temp[1], clGradientColorStart, clGradientColorEnd, Core::Classes::UI::gdVert);
						canvas.Pen.Color = clDividerColor;
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawPushButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Hot_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed), GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Divider),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawPushButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawPushButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End), GetThemeSysColor(ThemeColor::ThemeColor_Button_Light_Disabled_Divider),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags, INT iStateId)
					{
						flags |= DT_END_ELLIPSIS;
						if (iStateId == PBS_DISABLED)
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_1);
						else
							canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}