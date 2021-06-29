#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "ListBox.h"

#include <vsstyle.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ListBox
			{
				HTHEME FIXAPI Initialize(HWND hWindow)
				{
					SetWindowSubclass(hWindow, ListBoxSubclass, 0, 0);
					return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
				}

				INT_PTR FIXAPI OnCtlColorListBox(HWND hWindow, HDC hDC)
				{
					SetTextColor(hDC, GetThemeSysColor(ThemeColor_Text_3));
					SetBkColor(hDC, GetThemeSysColor(ThemeColor_ListView_Color));
		
					return reinterpret_cast<INT_PTR>(hThemeEditBrush);
				}

				LRESULT CALLBACK ListBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS))
					{
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
					}
					else if (uMsg == WM_NCPAINT)
					{
						// Paint border
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Core::Classes::UI::CUICanvas Canvas(hdc);
						Core::Classes::UI::CRECT rc, rc2;
						GetWindowRect(hWnd, (LPRECT)& rc);
						rc.Offset(-rc.Left, -rc.Top);

						if (GetFocus() == hWnd)
							Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						else
							Canvas.GradientFrame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), Core::Classes::UI::gdVert);

						rc.Inflate(-1, -1);
						Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));

						// scrollbox detected grip
						GetClientRect(hWnd, (LPRECT)& rc2);
						if ((abs(rc2.Width - rc.Width) > 5) && (abs(rc2.Height - rc.Height) > 5))
						{
							rc.Left = rc.Width - GetSystemMetrics(SM_CXVSCROLL);
							rc.Top = rc.Height - GetSystemMetrics(SM_CYHSCROLL);
							rc.Width = GetSystemMetrics(SM_CXVSCROLL);
							rc.Height = GetSystemMetrics(SM_CYHSCROLL);

							Canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						}

						ReleaseDC(hWnd, hdc);
						return result;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}