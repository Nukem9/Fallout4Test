#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "ScrollBar.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ScrollBar
			{
				namespace Render
				{
					VOID FIXAPI DrawBackgroundHorz(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Height /= 2;

						canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start), Core::Classes::UI::gdVert);

						rc_temp[1] = *pRect;
						rc_temp[1].Top = rc_temp[0].Bottom;

						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End), Core::Classes::UI::gdVert);

						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(rc_temp[0].Left, rc_temp[0].Top);
						canvas.LineTo(rc_temp[0].Right, rc_temp[0].Top);
					}

					VOID FIXAPI DrawBackgroundVert(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[0].Width /= 2;

						canvas.GradientFill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start), Core::Classes::UI::gdHorz);

						rc_temp[1] = *pRect;
						rc_temp[1].Left = rc_temp[0].Right;

						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Gradient_End), Core::Classes::UI::gdHorz);

						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color);
						canvas.MoveTo(rc_temp[0].Left, rc_temp[0].Top);
						canvas.LineTo(rc_temp[0].Left, rc_temp[0].Bottom);
					}

					VOID FIXAPI DrawSliderHorz_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawSliderHorz_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter_Hot);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawSliderVert_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_End), Core::Classes::UI::gdHorz);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Left, rc_temp[1].Bottom);
					}

					VOID FIXAPI DrawSliderVert_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Gradient_Hot_End), Core::Classes::UI::gdHorz);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_ScrollBar_Thumb_Highlighter_Hot);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Left, rc_temp[1].Bottom);
					}

					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.Fill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}

					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
						canvas.Fill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}
				}
			}
		}
	}
}