#include "..\..\..\common.h"
#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "SysMenu.h"

#include <CommCtrl.h>

#define UM_COMMAND_CLOSE (WM_USER + 1)
#define UM_COMMAND_MINIMIZE (WM_USER + 2)
#define UM_COMMAND_MAXIMIZE (WM_USER + 3)
#define WM_BUTTONCHANGE (WM_USER + 4)

#define HEIGHT_BTN 32
#define WIDTH_BTN 48

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace SysMenu
			{
				constexpr LPSTR lpSysMenuClass = "SysMenuClass";

				typedef struct tagSYSMENUDATA
				{
					HWND hCloseBtn;
					HWND hMinimizeBtn;
					HWND hMaximizeBtn;
					HWND hTargetWindow;
					std::set<SysMenu> sFlags;
				} SYSMENUDATA, *PSYSMENUDATA, *LPSYSMENUDATA;

				#define GetSysMenuData(x) ((LPSYSMENUDATA)GetWindowLongPtrA(x, GWLP_USERDATA))

				HWND WINAPI Initialization(HWND hParentWindow, HWND hTargetWindow)
				{
					HINSTANCE hInst = GetModuleHandleA(NULL);

					WNDCLASSEXA wc = { 0 };
					if (!GetClassInfoExA(hInst, lpSysMenuClass, &wc))
					{
						wc.cbSize = sizeof(wc);
						wc.hbrBackground = (HBRUSH)Comctl32GetSysColorBrushEx(COLOR_BTNFACE);
						wc.hInstance = hInst;
						wc.style = CS_HREDRAW | CS_VREDRAW;
						wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
						wc.lpszClassName = lpSysMenuClass;
						wc.lpfnWndProc = (WNDPROC)&SysMenuWndProc;

						Assert(RegisterClassExA(&wc));
					}

					HWND hSysMenu = CreateWindowExA(0, lpSysMenuClass, "",
						WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						0, 0, 144, HEIGHT_BTN, hParentWindow, (HMENU)NULL, hInst, NULL);
					Assert(hSysMenu);

					LPSYSMENUDATA lpSysData = new SYSMENUDATA;
					Assert(lpSysData);

					lpSysData->sFlags = { SM_MINIMIZE, SM_MAXIMIZE, SM_CLOSE };
					lpSysData->hTargetWindow = hTargetWindow;
					lpSysData->hMinimizeBtn = CreateWindowExA(0, WC_BUTTONA, "", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
						0, 0, WIDTH_BTN, HEIGHT_BTN, hSysMenu, (HMENU)UM_COMMAND_MINIMIZE, hInst, NULL);
					lpSysData->hMaximizeBtn = CreateWindowExA(0, WC_BUTTONA, "", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
						WIDTH_BTN, 0, WIDTH_BTN, HEIGHT_BTN, hSysMenu, (HMENU)UM_COMMAND_MAXIMIZE, hInst, NULL);
					lpSysData->hCloseBtn = CreateWindowExA(0, WC_BUTTONA, "", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
						WIDTH_BTN << 1, 0, WIDTH_BTN, HEIGHT_BTN, hSysMenu, (HMENU)UM_COMMAND_CLOSE, hInst, NULL);

					SetWindowLongPtrA(hSysMenu, GWLP_USERDATA, (LONG_PTR)lpSysData);

					return hSysMenu;
				}

				VOID WINAPI SetSysMenuButton(HWND hWnd, const CUISysMenuButtons& sets)
				{
					LPSYSMENUDATA lpSysData = GetSysMenuData(hWnd);

					lpSysData->sFlags = sets;
					SendMessageA(hWnd, WM_BUTTONCHANGE, (WPARAM)lpSysData, 0);
				}

				LRESULT CALLBACK SysMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					switch (uMsg)
					{
					case WM_CREATE:
					{

					}
					return S_OK;

					case WM_COMMAND:
					{
						LPSYSMENUDATA lpSysData = GetSysMenuData(hWnd);
						if (lpSysData)
						{
							Core::Classes::UI::CUICustomWindow Window = lpSysData->hTargetWindow;

							switch (wParam)
							{
							case UM_COMMAND_MINIMIZE:
								Window.WindowState = wsMinimized;
								break;
							case UM_COMMAND_MAXIMIZE:
								Window.WindowState = Window.WindowState == wsMaximized ? wsNormal : wsMaximized;
								break;
							case UM_COMMAND_CLOSE:
								Window.Perform(WM_CLOSE, 0, 0);
								break;
							}
						}
					}
					return S_OK;

					case WM_BUTTONCHANGE:
					{
						LPSYSMENUDATA lpSysData = (LPSYSMENUDATA)wParam;

						ShowWindow(lpSysData->hMinimizeBtn, lpSysData->sFlags.count(SM_MINIMIZE) > 0 ? SW_SHOWNORMAL : SW_HIDE);
						ShowWindow(lpSysData->hMaximizeBtn, lpSysData->sFlags.count(SM_MAXIMIZE) > 0 ? SW_SHOWNORMAL : SW_HIDE);
						ShowWindow(lpSysData->hCloseBtn, lpSysData->sFlags.count(SM_CLOSE) > 0 ? SW_SHOWNORMAL : SW_HIDE);

						Core::Classes::UI::CUICustomWindow ParentWindow = hWnd;
						Core::Classes::UI::CUICustomWindow BandWindow = GetParent(hWnd);
						Core::Classes::UI::CRECT rc = ParentWindow.ClientRect();
						Core::Classes::UI::CRECT rc_need;

						rc_need.Height = HEIGHT_BTN;

						if (lpSysData->sFlags.count(SM_MINIMIZE) > 0)
						{
							Core::Classes::UI::CUICustomWindow Btn = lpSysData->hMinimizeBtn;
							rc_need.Width += Btn.Width;
							Btn.Height = rc_need.Height;
						}

						if (lpSysData->sFlags.count(SM_MAXIMIZE) > 0)
						{
							Core::Classes::UI::CUICustomWindow Btn = lpSysData->hMinimizeBtn;
							rc_need.Width += Btn.Width;
							Btn.Height = rc_need.Height;
						}

						if (lpSysData->sFlags.count(SM_CLOSE) > 0)
						{
							Core::Classes::UI::CUICustomWindow Btn = lpSysData->hMinimizeBtn;
							rc_need.Width += Btn.Width;
							Btn.Height = rc_need.Height;
						}

						SetWindowPos(hWnd, NULL, BandWindow.ClientWidth() - rc_need.Width, 0, rc_need.Width, rc_need.Height, SWP_NOZORDER | SWP_FRAMECHANGED);
					}
					return S_OK;

					case WM_SIZE:
					{
						LPSYSMENUDATA lpSysData = GetSysMenuData(hWnd);
						if (lpSysData)
						{
							Core::Classes::UI::CUICustomWindow Window = hWnd;
							Core::Classes::UI::CRECT rc = Window.ClientRect();

							if (lpSysData->sFlags.count(SM_MINIMIZE) > 0)
							{
								Core::Classes::UI::CUICustomWindow Btn = lpSysData->hMinimizeBtn;
								Btn.Left = 0;
							}

							if (lpSysData->sFlags.count(SM_MAXIMIZE) > 0)
							{
								Core::Classes::UI::CUICustomWindow Btn = lpSysData->hMaximizeBtn;
								Btn.Left = lpSysData->sFlags.count(SM_MINIMIZE) > 0 ? WIDTH_BTN : 0;
							}

							if (lpSysData->sFlags.count(SM_CLOSE) > 0)
							{
								Core::Classes::UI::CUICustomWindow Btn = lpSysData->hCloseBtn;
								Btn.Left =	(lpSysData->sFlags.count(SM_MINIMIZE) > 0 ? WIDTH_BTN : 0) +
											(lpSysData->sFlags.count(SM_MAXIMIZE) > 0 ? WIDTH_BTN : 0);
							}
						}
					}
					return S_OK;

					case WM_DESTROY:
					{
						LPSYSMENUDATA lpSysData = GetSysMenuData(hWnd);
						if (lpSysData) delete lpSysData;
					}
					return S_OK;

					case WM_PAINT:
					{
						HDC hDC;
						PAINTSTRUCT ps;
						hDC = BeginPaint(hWnd, &ps);

						EndPaint(hWnd, &ps);
					}
					return S_OK;
					}

					return DefWindowProcA(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}