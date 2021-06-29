#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "TrackBar.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TrackBar
			{
				namespace Render
				{
					VOID FIXAPI DrawTrack(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;

						rc_temp.Offset(0, 1);
						rc_temp.Inflate(1, 0);
						canvas.GradientFrame(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), Core::Classes::UI::gdVert);
						rc_temp = *pRect;
						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						rc_temp.Inflate(-1, -1);
						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2));
					}

					VOID FIXAPI DrawSlider_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Default_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawSlider_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}

					VOID FIXAPI DrawSlider_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_Start), 
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Hot_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
						canvas.Frame(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawSlider_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
						canvas.Frame(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawSlider_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp[2];

						rc_temp[0] = *pRect;
						rc_temp[1] = rc_temp[0];
						rc_temp[1].Inflate(-1, -1);

						canvas.Fill(rc_temp[0], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled));
						canvas.GradientFill(rc_temp[1], GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_Start), 
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Gradient_End), Core::Classes::UI::gdVert);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter);
						canvas.MoveTo(rc_temp[1].Left, rc_temp[1].Top);
						canvas.LineTo(rc_temp[1].Right, rc_temp[1].Top);
					}
				}
			}
		}
	}
}