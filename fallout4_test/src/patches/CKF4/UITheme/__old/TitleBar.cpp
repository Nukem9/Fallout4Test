#include "..\..\..\common.h"
#include "..\UIBaseWindow.h"
#include "..\MainWindow.h"
#include "VarCommon.h"
#include "TitleBar.h"

#include "MoveWndBar.h"
#include "SysMenu.h"

#define CYTITLEBAR 72

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TitleBar
			{
				namespace Graphics = Core::Classes::UI;

				typedef struct tagTITLEBARDATA
				{
					HWND hMovable;
					HWND hSysMenu;
				} TITLEBARDATA, *PTITLEBARDATA, *LPTITLEBARDATA;

				#define GetTitleData(x) ((LPTITLEBARDATA)GetWindowLongPtrA(x, GWLP_USERDATA))

				namespace Event
				{
					VOID WINAPI OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						LPTITLEBARDATA lpTitleData = GetTitleData(hWnd);
						if (lpTitleData)
						{
							Graphics::CUICustomWindow WindowSysMenu = lpTitleData->hSysMenu;
							Graphics::CUICustomWindow WindowMovable = lpTitleData->hMovable;

							INT nTemp = WindowSysMenu.Width;

							WindowMovable.Left = 40;
							WindowMovable.Width = LOWORD(lParam) - (40 + nTemp);
							WindowSysMenu.Left = LOWORD(lParam) - nTemp;
						}
					}

					VOID WINAPI OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						HDC hdc;
						PAINTSTRUCT ps;
						hdc = BeginPaint(hWnd, &ps);
						EndPaint(hWnd, &ps);
					}

					VOID WINAPI OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
					{
						LPTITLEBARDATA lpTitleData = GetTitleData(hWnd);
						if (lpTitleData) delete lpTitleData;
					}
				}

				HWND WINAPI Initialization(HINSTANCE hInstance, HWND hWindow)
				{
					WNDCLASSEXA wc = { 0 };

					wc.cbSize = sizeof(wc);
					wc.hbrBackground = (HBRUSH)Comctl32GetSysColorBrushEx(COLOR_BTNFACE);
					wc.hInstance = hInstance;
					wc.style = CS_HREDRAW | CS_VREDRAW;
					wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
					wc.lpszClassName = "TitleBarClass";
					wc.lpfnWndProc = (WNDPROC)&TitleBarWndProc;

					Assert(RegisterClassExA(&wc));

					HWND hPanel = CreateWindowExA(0, wc.lpszClassName, "",
						WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, 
						0, 0, 100, CYTITLEBAR, hWindow, (HMENU)NULL, hInstance, NULL);
					Assert(hPanel);

					Graphics::CUICustomWindow Window = hPanel;
					Graphics::CUICustomWindow ParentWindow = hWindow;
					Graphics::CRECT rc_wnd = ParentWindow.ClientRect();

					LPTITLEBARDATA lpTitleData = new TITLEBARDATA;
					Assert(lpTitleData);

					lpTitleData->hSysMenu = SysMenu::Initialization(hPanel, hWindow);
					lpTitleData->hMovable = MoveWndBar::Initialization(hPanel, hWindow);

					SetWindowLongPtrA(hPanel, GWLP_USERDATA, (LONG_PTR)lpTitleData);
					Window.SetSize(rc_wnd.Width, CYTITLEBAR);

					return hPanel;
				}

				LRESULT CALLBACK TitleBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					switch (uMsg)
					{
					case WM_SIZE:
						Event::OnSize(hWnd, wParam, lParam);
						break;

					case WM_NCHITTEST:
						return HTCLIENT;

					case WM_CREATE:
						break;

					case WM_DESTROY:
						Event::OnDestroy(hWnd, wParam, lParam);
						break;

					case WM_PAINT:
						Event::OnPaint(hWnd, wParam, lParam);
						break;

					default:
						return DefWindowProcA(hWnd, uMsg, wParam, lParam);
					}

					return S_OK;
				}
			}
		}
	}
}