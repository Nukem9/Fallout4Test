#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "GroupBox.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace GroupBox
			{
				namespace Render
				{
					VOID FIXAPI DrawGroupBox_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clColor, COLORREF clColorShadow)
					{
						Core::Classes::UI::CRECT rc_temp(*pRect);

						rc_temp.Bottom--;
						canvas.Frame(rc_temp, clColor);
						canvas.Pen.Color = clColorShadow;
						canvas.MoveTo(rc_temp.Left + 1, rc_temp.Top + 1);
						canvas.LineTo(rc_temp.Right - 1, rc_temp.Top + 1);
						canvas.MoveTo(rc_temp.Left, rc_temp.Bottom);
						canvas.LineTo(rc_temp.Right, rc_temp.Bottom);
					}

					VOID FIXAPI DrawGroupBox_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2), 
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Ver2));
					}

					VOID FIXAPI DrawGroupBox_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawGroupBox_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Disabled_Ver2),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Disabled_Ver2));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Text_4);
					}
				}
			}
		}
	}
}