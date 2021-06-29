#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "ProgressBar.h"

#include <algorithm>
#include <Uxtheme.h>
#include <vsstyle.h>

#define INTERNAL 5
#define MARQUEE_TIMER_ID (WM_USER + 0x1000)

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ProgressBar
			{
				typedef struct ProgressBarInfo
				{
					HWND hWnd;
					Graphics::CRECT rc_fill;
				} *lpProgressBarInfo;

				namespace Render
				{
					VOID FIXAPI DrawBar(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc = *pRect;

						canvas.GradientFill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start), 
							GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
							Graphics::gdVert);

						canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawFill(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc = *pRect;
						rc.Inflate(-1, -1);

						if (rc.Right > 0)
						{
							canvas.GradientFill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Gradient_End),
								Graphics::gdVert);

							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Highlighter));
							canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Highlighter_Up);
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}
					}
				}

				VOID FIXAPI Initialize(HWND hWindow)
				{
					// for marquee style
					SetWindowSubclass(hWindow, ProgressBarSubclass, 0, 0);
					// for standart progressbar (marquee does't draw)
					OpenThemeData(hWindow, VSCLASS_PROGRESS);
				}

				LRESULT CALLBACK ProgressBarSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					if (uMsg == PBM_SETMARQUEE)
					{
						if (wParam)
						{
							lpProgressBarInfo pbi = new ProgressBarInfo;
							GetClientRect(hWnd, (LPRECT)&pbi->rc_fill);
							pbi->rc_fill.Width = pbi->rc_fill.Width >> 2;
							SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)pbi);
							SetTimer(hWnd, MARQUEE_TIMER_ID, std::max((UINT)30, (UINT)lParam), NULL);
						}
						else
						{
							KillTimer(hWnd, MARQUEE_TIMER_ID);
							delete (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
							SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)NULL);
						}
							
						return S_OK;
					} 
					else if (uMsg == WM_DESTROY)
					{
						KillTimer(hWnd, MARQUEE_TIMER_ID);
						delete (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
					}
					else if (uMsg == WM_TIMER)
					{
						if (MARQUEE_TIMER_ID == wParam)
						{
							lpProgressBarInfo pbi = (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

							Graphics::CUIBaseWindow window = hWnd;
							Graphics::CRECT rc = window.ClientRect(), need_redraw;
							rc.Inflate(-1, -1);

							need_redraw = pbi->rc_fill;
							need_redraw.Inflate(INTERNAL, 0);

							pbi->rc_fill.Left += INTERNAL;
							if ((pbi->rc_fill.Left) >= (rc.Right + pbi->rc_fill.Width))
								pbi->rc_fill.Left = -pbi->rc_fill.Width;

							// to make it flicker less, redraw what you need
							InvalidateRect(hWnd, (LPRECT)&need_redraw, TRUE);
							UpdateWindow(hWnd);

							return S_OK;
						}				
					}
					else if (uMsg == WM_PAINT)
					{
						lpProgressBarInfo pbi = (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

						if (pbi)
						{
							PAINTSTRUCT ps;
							HDC hDC = BeginPaint(hWnd, &ps);
							Graphics::CUICanvas canvas = hDC;
							Graphics::CUIBaseWindow window = hWnd;
							Graphics::CRECT rc = window.ClientRect();

							Render::DrawBar(canvas, (LPRECT)&rc);
							Render::DrawFill(canvas, (LPRECT)&pbi->rc_fill);

							EndPaint(hWnd, &ps);

							return S_OK;
						}
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}