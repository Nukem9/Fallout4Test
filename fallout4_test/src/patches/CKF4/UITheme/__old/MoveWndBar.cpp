#include "..\..\..\common.h"
#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "MoveWndBar.h"

#include <windowsx.h>

// The mouse is sometimes faster and if you quickly pull the mouse, it will detach from the window area, to find out if this has happened, I hang up the timer.
#define WM_DETECTED_LEAVEMOUSE (WM_USER + 1)

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			// The window is a strip so that can move the specified window
			namespace MoveWndBar
			{
				constexpr LPSTR lpMoveWndBarClass = "MoveWndBarClass";

				typedef struct tagMOVEWNDBARDATA
				{
					BOOL bCanMoveWindow;
					INT nOldX;
					INT nOldY;
					HWND hMovableWindow;
					UINT_PTR Timer;
				} MOVEWNDBARDATA, *PMOVEWNDBARDATA, *LPMOVEWNDBARDATA;

				#define GetMoveData(x) ((LPMOVEWNDBARDATA)GetWindowLongPtrA(x, GWLP_USERDATA))

				HWND WINAPI Initialization(HWND hParentWindow, HWND hMovableWindow)
				{
					HINSTANCE hInst = GetModuleHandleA(NULL);

					WNDCLASSEXA wc = { 0 };
					if (!GetClassInfoExA(hInst, lpMoveWndBarClass, &wc))
					{
						wc.cbSize = sizeof(wc);
						wc.hbrBackground = (HBRUSH)Comctl32GetSysColorBrushEx(COLOR_BTNFACE);
						wc.hInstance = hInst;
						wc.style = CS_HREDRAW | CS_VREDRAW;
						wc.hCursor = LoadCursorA(hInst, IDC_ARROW);
						wc.lpszClassName = lpMoveWndBarClass;
						wc.lpfnWndProc = (WNDPROC)&MoveWndBarWndProc;

						Assert(RegisterClassExA(&wc));
					}

					HWND hMoveWndBar = CreateWindowExA(0, lpMoveWndBarClass, "",
						WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						0, 0, 180, 32, hParentWindow, (HMENU)NULL, hInst, NULL);
					Assert(hMoveWndBar);

					LPMOVEWNDBARDATA lpUserData = new MOVEWNDBARDATA;
					Assert(lpUserData);

					lpUserData->bCanMoveWindow = FALSE;
					lpUserData->nOldX = lpUserData->nOldY = 0;
					lpUserData->hMovableWindow = hMovableWindow;

					SetWindowLongPtrA(hMoveWndBar, GWLP_USERDATA, (LONG_PTR)lpUserData);
					return hMoveWndBar;
				}

				LRESULT CALLBACK MoveWndBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					switch (uMsg)
					{
					case WM_CREATE:
					{

					}
					return S_OK;

					case WM_DESTROY:
					{
						LPMOVEWNDBARDATA lpUserData = GetMoveData(hWnd);
						if (lpUserData) delete lpUserData;
					}
					return S_OK;

					case WM_MOUSEMOVE:
					{
						LPMOVEWNDBARDATA lpUserData = GetMoveData(hWnd);

						if (lpUserData->bCanMoveWindow)
						{
							Core::Classes::UI::CUICustomWindow MovableWindow = lpUserData->hMovableWindow;
							Core::Classes::UI::CRECT rcMovableWindow = MovableWindow.WindowRect();

							MovableWindow.Move(	rcMovableWindow.Left + (GET_X_LPARAM(lParam)) - lpUserData->nOldX,
												rcMovableWindow.Top + (GET_Y_LPARAM(lParam)) - lpUserData->nOldY,
												(MovableWindow.StyleEx & WS_EX_TOPMOST) == WS_EX_TOPMOST);
						}
					}
					return S_OK;

					case WM_PAINT:
					{
						HDC hDC;
						PAINTSTRUCT ps;
						hDC = BeginPaint(hWnd, &ps);

						// Assuming that this will be the window title in the future, I pass the text there
						LPMOVEWNDBARDATA lpUserData = GetMoveData(hWnd);

						// I use widechar to support my language
						INT nLen = GetWindowTextLengthW(lpUserData->hMovableWindow);

						if (nLen > 0)
						{
							LPWSTR lpText = new WCHAR[nLen + 1];
							Assert(lpText);
							ZeroMemory(lpText, (nLen + 1) << 1);
							GetWindowTextW(lpUserData->hMovableWindow, lpText, nLen + 1);

							Core::Classes::UI::CUICanvas canvas = hDC;
							canvas.Font.Assign(ThemeFont);
							canvas.TransparentMode = TRUE;
							canvas.ColorText = GetThemeSysColor(ThemeColor_Caption_Text);

							Core::Classes::UI::CRECT rc;
							Core::Classes::UI::CRECT rc_wnd;
							GetWindowRect(hWnd, (LPRECT)&rc_wnd);
							rc_wnd.Offset(-rc_wnd.Left, -rc_wnd.Top);
							rc = rc_wnd;

							canvas.TextRect(rc, lpText, DT_CALCRECT);

							rc.Top = (rc_wnd.Height - rc.Height) >> 1;
							rc.Left = (rc_wnd.Width - rc.Width) >> 1;

							canvas.TextRect(rc, lpText, DT_END_ELLIPSIS | DT_NOPREFIX);
							canvas.TransparentMode = FALSE;

							delete[] lpText;
						}

						EndPaint(hWnd, &ps);
					}
					return S_OK;

					case WM_TIMER:
					{
						if (wParam == WM_DETECTED_LEAVEMOUSE)
						{
							POINT p;
							GetCursorPos(&p);
							ScreenToClient(hWnd, &p);

							RECT r;
							GetClientRect(hWnd, &r);

							if (!PtInRect(&r, p))
								SendMessageA(hWnd, WM_LBUTTONUP, 0, 0);
						}
					}
					return S_OK;

					case WM_LBUTTONDOWN:
					{
						LPMOVEWNDBARDATA lpUserData = GetMoveData(hWnd);

						lpUserData->nOldX = GET_X_LPARAM(lParam);
						lpUserData->nOldY = GET_Y_LPARAM(lParam);
						lpUserData->Timer = SetTimer(hWnd, WM_DETECTED_LEAVEMOUSE, USER_TIMER_MINIMUM, NULL);

						lpUserData->bCanMoveWindow = TRUE;
					}
					return S_OK;

					case WM_LBUTTONUP:
					{
						LPMOVEWNDBARDATA lpUserData = GetMoveData(hWnd);

						if (lpUserData->bCanMoveWindow)
						{
							lpUserData->bCanMoveWindow = FALSE;
							Assert(KillTimer(hWnd, lpUserData->Timer));
						}					
					}
					return S_OK;

					}

					return DefWindowProcA(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}