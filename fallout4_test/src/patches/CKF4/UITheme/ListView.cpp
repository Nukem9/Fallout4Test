//////////////////////////////////////////
/*
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "ListView.h"

#define UI_CONTROL_CONDITION_ID 0xFA0

namespace Core {
	namespace UI {
		namespace Theme {
			namespace ListView {
				HTHEME FIXAPI Initialize(HWND hWindow) {
					SetWindowSubclass(hWindow, ListViewSubclass, 0, 0);

					ListView_SetTextColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
					ListView_SetTextBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));
					ListView_SetBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));

					return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
				}

				LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
					if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS)) {
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
					}
					else if (uMsg == WM_PAINT) {
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
						if ((abs(rc2.Width - rc.Width) > 5) && (abs(rc2.Height - rc.Height) > 5)) {
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

				LRESULT FIXAPI OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
				{
					// skip it controls
					switch (lpListView->nmcd.hdr.idFrom) {
					case 1041:
					case 1155:
					case 1156:
						return DefSubclassProc(hWindow, WM_NOTIFY, 0, (LPARAM)lpListView);
					}

					Graphics::CUICanvas Canvas(lpListView->nmcd.hdc);

					switch (lpListView->nmcd.dwDrawStage) {
					//Before the paint cycle begins
					case CDDS_PREPAINT: {
						//request notifications for individual listview items
						return CDRF_NOTIFYITEMDRAW;
					}
					//Before an item is drawn
					case CDDS_ITEMPREPAINT: {
						return CDRF_NOTIFYSUBITEMDRAW;
					}
					//Before a subitem is drawn
					case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
						switch (lpListView->nmcd.hdr.idFrom) {
						case UI_CONTROL_CONDITION_ID: {
							if (lpListView->iSubItem == 0 || lpListView->iSubItem == 5)
								lpListView->clrText = GetThemeSysColor(ThemeColor_Text_2);
							else
								lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);		

							return CDRF_NEWFONT;
						}
						default:
							lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);
							return CDRF_NEWFONT;
						}
					}
					default:
						return CDRF_DODEFAULT;
					}
				}
			}
		}
	}
}