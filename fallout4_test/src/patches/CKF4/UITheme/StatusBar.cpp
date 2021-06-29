#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "StatusBar.h"

#include "..\MainWindow.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace StatusBar
			{
				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_Border_Window));
					}

					VOID FIXAPI DrawBackground(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_Border_Window));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags)
					{
						flags |= DT_CENTER | DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_StatusBar_Text);
					}
				}
			}
		}
	}
}