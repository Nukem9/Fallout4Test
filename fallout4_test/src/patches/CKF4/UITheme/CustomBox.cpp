#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "CustomBox.h"

#include <Uxtheme.h>
#include <fstream>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace CustomBox
			{
				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, Graphics::CRECT& rc)
					{
						Graphics::CRECT rc_temp = rc;
						canvas.GradientFill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), Core::Classes::UI::gdVert);
						rc_temp.Inflate(-1, -1);
						canvas.Frame(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}
				}

				VOID FIXAPI Initialize(HWND hWindow, AllowBox eAllowBox)
				{
					switch (eAllowBox)
					{
					case Core::UI::Theme::CustomBox::abColor:
						SetWindowSubclass(hWindow, ColorBoxSubclass, 0, 0);
						break;
					default:
						SetWindowSubclass(hWindow, NormalBoxSubclass, 0, 0);
						break;
					}				
				}

				LRESULT CALLBACK NormalBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					if (uMsg == WM_PAINT)
					{
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Core::Classes::UI::CUICanvas Canvas(hdc);
						Core::Classes::UI::CRECT rc;
						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);

						rc.Inflate(-2, -2);
						Canvas.ExcludeRect(rc);
						rc.Inflate(2, 2);

						Render::DrawBorder(Canvas, rc);
						
						ReleaseDC(hWnd, hdc);
						return result;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}

				LRESULT CALLBACK ColorBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					// Generally strange behavior, this window does't receive WM_PAINT, however, WM_NCPAINT is current in the path

					// WM_SIZE
					// WM_MOVE
					// WM_SHOWWINDOW
					// WM_SETFONT ??? why ???
					// WM_GETDLGCODE
					// WM_NCPAINT (wParam 1)
					// ...
					// WM_NCHITTEST (very-very many)
					// ...
					// WM_GETDLGCODE
					// WM_DESTROY
					// WM_NCDESTROY

					// there is't a single message that would tell me where it gets the color from
					// PS: the application itself draws on its client area without send message the control.
					// Let's draw it once taking into account the border of 3 pixels (Windows 10)

					if (uMsg == WM_NCPAINT)
					{
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Core::Classes::UI::CUICanvas Canvas(hdc);
						Core::Classes::UI::CRECT rc;
						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);
						
						rc.Inflate(-3, -3);
						Canvas.ExcludeRect(rc);
						rc.Inflate(3, 3);

						Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						rc.Inflate(-1, -1);
						Render::DrawBorder(Canvas, rc);
						
						ReleaseDC(hWnd, hdc);
						return result;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}