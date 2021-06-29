#include "..\UIDWMWindow.h"
#include "..\MainWindow.h"
#include "VarCommon.h"
#include "FlatMainWindow.h"
#include "Panel.h"

#include <Uxtheme.h>
#include <windowsx.h>
#include <VersionHelpers.h>

#define THEME_WINDOW 1024

#define BORDERWIDTH 1

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace FlatMainWindow
			{
				static HWND hTitleBar;
				static HWND hBandBar01;
				static Graphics::CUICustomWindow TitleBar;
				static Graphics::CUICustomWindow BandBar01;

				namespace Render
				{
					VOID WINAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF clFrame)
					{
						canvas.Frame(*pRect, clFrame);
					}

					VOID WINAPI DrawCaption(Graphics::CUICanvas& canvas, LPCSTR lpstrCaption, LPCRECT pRect)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_Border_Window));
					}

					VOID WINAPI DrawErase(Graphics::CUICanvas& canvas)
					{
						canvas.Fill(GetThemeSysColor(ThemeColor::ThemeColor_Default));
					}
				}

				namespace Event
				{
					LRESULT WINAPI OnNcHitTest(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						Graphics::CUICustomWindow Window = hWnd;

						if (Window.WindowState != wsMaximized)
						{
							POINT p;
							p.x = GET_X_LPARAM(lParam);
							p.y = GET_Y_LPARAM(lParam);

							ScreenToClient(hWnd, &p);

							Graphics::CRECT r = Window.WindowRect();
							r.Offset(-r.Left, -r.Top);

							if (p.y <= 2)
							{
								if (p.x <= 2)
									return HTTOPLEFT;
								else if (p.x >= (r.Width - 2))
									return HTTOPRIGHT;
								else
									return HTTOP;
							}
							else if (p.y >= (r.Height - 2))
							{
								if (p.x <= 2)
									return HTBOTTOMLEFT;
								else if (p.x >= (r.Width - 2))
									return HTBOTTOMRIGHT;
								else
									return HTBOTTOM;
							}
							else
							{
								if (p.x <= 2)
									return HTLEFT;
								else if (p.x >= (r.Width - 2))
									return HTRIGHT;
								else
									return HTCLIENT;
							}
						}
						else return HTCLIENT;
					}

					LRESULT WINAPI OnNcActive(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						Graphics::CUICustomWindow window = hWnd;

						HMENU hMenu = GetMenu(hWnd);
						SetMenu(hWnd, NULL);
						LRESULT lRet = DefSubclassProc(hWnd, WM_NCACTIVATE, wParam, lParam);
						SetMenu(hWnd, hMenu);

						if ((window.Style & WS_BORDER) == WS_BORDER)
						{
							HDC hDC = GetWindowDC(hWnd);
							Graphics::CUICanvas canvas = hDC;
							Graphics::CRECT rc_window = window.WindowRect();
							rc_window.Offset(-rc_window.Left, -rc_window.Top);

							Render::DrawBorder(canvas, (LPRECT)& rc_window,
								MainWindow::GetActiveApp() ? GetThemeSysColor(ThemeColor::ThemeColor_Border_Window) :
								GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));

							ReleaseDC(hWnd, hDC);
						}

						return lRet;
					}

					LRESULT WINAPI OnNcCalcSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						if (wParam)
						{
							LPNCCALCSIZE_PARAMS lpRect = (LPNCCALCSIZE_PARAMS)lParam;
							if (lpRect)
							{
								Graphics::CUICustomWindow window = hWnd;
								INT Off = (window.Style & WS_BORDER) == WS_BORDER ? BORDERWIDTH : 0;

								lpRect->rgrc[0].left += Off;
								lpRect->rgrc[0].top += Off;
								lpRect->rgrc[0].right -= Off;
								lpRect->rgrc[0].bottom -= Off;

								return S_OK;
							}
						}
						else
						{
							LPRECT lpRect = (LPRECT)lParam;
							if (lpRect)
							{
								Graphics::CUICustomWindow window = hWnd;
								INT Off = (window.Style & WS_BORDER) == WS_BORDER ? BORDERWIDTH : 0;

								lpRect->left += Off;
								lpRect->top += Off;
								lpRect->right -= Off;
								lpRect->bottom -= Off;

								return S_OK;
							}
						}

						return DefSubclassProc(hWnd, WM_NCCALCSIZE, wParam, lParam);
					}

					LRESULT WINAPI OnUIChangeWindowState(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						Graphics::CUICustomWindow Window = hWnd;
						if (wParam == wsMaximized)
							Window.Style = WS_POPUP | WS_VISIBLE;
						else
							Window.Style = WS_POPUP | WS_VISIBLE | WS_BORDER;
						SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

						return S_OK;
					}

					VOID WINAPI OnActive(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
					}

					VOID WINAPI OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						TitleBar.Width = LOWORD(lParam);
						BandBar01.Top = 32;
						BandBar01.Left = 4;
						BandBar01.Width = LOWORD(lParam) - 8;
					}

					LRESULT WINAPI OnNcPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						OnNcActive(hWnd, 0, 0);

						return S_OK;
					}
				}

				VOID WINAPI Initialize(HWND hWindow)
				{
					// DWM disable
					if (IsWindows8OrGreater())
					{
						Graphics::CUIDWMWindow DwmWindow = hWindow;
						if (DwmWindow.IsCompositionEnabled())
						{
							// enabled Windows 7 theme (disabled draw shadow)
							DwmWindow.EnabledNonClientRendering = FALSE;
							// ignore border
							MARGINS m = { 0 };
							DwmWindow.ExtendFrameIntoClientArea(m);
						}
					}

					// set new styles
					// WS_POPUP - simple window
					// WS_BORDER - border 1px
					Graphics::CUICustomWindow Window = hWindow;
					Window.Style = WS_POPUP | WS_BORDER | WS_VISIBLE;
					Window.StyleEx = WS_EX_APPWINDOW;

					// The menu draws the title.....
					if (!MainWindow::GetWindowObj().MainMenu.Handle)
						MainWindow::GetWindowObj().MainMenu = GetMenu(hWindow);
					// Handle is saved in the main window object
				

					MENUBARINFO mbi = { 0 };
					mbi.cbSize = sizeof(mbi);
					GetMenuBarInfo(hWindow, OBJID_MENU, 0, &mbi);

					//SetWindowPos(mbi.hwndMenu, NULL, 0, 0, 1, 1, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_FRAMECHANGED);
					

					//Assert((mbi.hMenu == GetMenu(hWindow)));
					//	SetMenu(hWindow, NULL);

					// create new titlebar
					hTitleBar = Core::UI::Theme::TitleBar::Initialization(GetModuleHandleA(NULL), hWindow);
					TitleBar = hTitleBar;
					//TitleBar.StyleEx |= WS_EX_TOPMOST;
					// create band for ToolBar
					hBandBar01 = Core::UI::Theme::Panel::Initialization(hTitleBar);
					BandBar01 = hBandBar01;
					BandBar01.Top = 32;
					BandBar01.Left = 4;
					BandBar01.Width = TitleBar.Width - 8;
					BandBar01.Height = 32;

				//	MainWindow::GetWindowObj().Toolbar.SetParent(BandBar01);

					SetWindowSubclass(hWindow, FlatWindowSubclass, THEME_WINDOW, reinterpret_cast<DWORD_PTR>(FlatWindowSubclass));
				}

				HWND WINAPI GetTitleBarWindow(VOID)
				{
					return hTitleBar;
				}

				HWND WINAPI GetBandWindow(VOID)
				{
					return hBandBar01;
				}

				Graphics::CUICustomWindow& WINAPI GetBandWindowObj(VOID)
				{
					return BandBar01;
				}

				Graphics::CUICustomWindow& WINAPI GetTitleBarWindowObj(VOID)
				{
					return TitleBar;
				}

				LRESULT CALLBACK FlatWindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					LRESULT lRet = S_OK;

					switch (uMsg)
					{
					case WM_SIZE:
						Event::OnSize(hWnd, wParam, lParam);
						break;

					case UI_CHANGEWINDOWSTATE:
						return Event::OnUIChangeWindowState(hWnd, wParam, lParam);

					case WM_NCCALCSIZE:
						return Event::OnNcCalcSize(hWnd, wParam, lParam);

					case WM_NCHITTEST:
						return Event::OnNcHitTest(hWnd, wParam, lParam);

					case WM_ACTIVATEAPP:
						break;

					case WM_NCACTIVATE:
						return Event::OnNcActive(hWnd, wParam, lParam);

					case WM_NCPAINT:
						return Event::OnNcPaint(hWnd, wParam, lParam);
					
					case WM_ACTIVATE:
						lRet = DefSubclassProc(hWnd, uMsg, wParam, lParam);
						Event::OnActive(hWnd, wParam, lParam);
						return lRet;

					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}