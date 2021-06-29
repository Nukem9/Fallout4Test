#include "..\..\..\common.h"
#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "Panel.h"

#include <windowsx.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace Panel
			{
				constexpr LPSTR lpPanelClass = "PanelWndClass";

				typedef struct tagPANELDATA
				{
					HWND hTranslateCommandMessage;
				} PANELDATA, *PPANELDATA, *LPPANELDATA;

				#define GetPanelData(x) ((LPPANELDATA)GetWindowLongPtrA(x, GWLP_USERDATA))

				HWND WINAPI Initialization(HWND hParentWindow)
				{
					HINSTANCE hInst = GetModuleHandleA(NULL);

					WNDCLASSEXA wc = { 0 };
					if (!GetClassInfoExA(hInst, lpPanelClass, &wc))
					{
						wc.cbSize = sizeof(wc);
						wc.hbrBackground = (HBRUSH)Comctl32GetSysColorBrushEx(COLOR_BTNFACE);
						wc.hInstance = hInst;
						wc.style = CS_HREDRAW | CS_VREDRAW;
						wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
						wc.lpszClassName = lpPanelClass;
						wc.lpfnWndProc = (WNDPROC)&PanelWndProc;

						Assert(RegisterClassExA(&wc));
					}

					HWND hPanel = CreateWindowExA(0, lpPanelClass, "",
						WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						0, 0, 92, 54, hParentWindow, (HMENU)NULL, hInst, NULL);
					Assert(hPanel);

					return hPanel;
				}

				VOID WINAPI SetTranslateCommandMessageWindow(HWND hWindow, HWND hTranslateWindow)
				{
					LPPANELDATA lpPanelData = GetPanelData(hWindow);

					if (!lpPanelData)
					{
						lpPanelData = new PANELDATA;
						SetWindowLongPtrA(hWindow, GWLP_USERDATA, (LONG_PTR)lpPanelData);
					}

					Assert(lpPanelData);
					lpPanelData->hTranslateCommandMessage = hTranslateWindow;
				}

				LRESULT CALLBACK PanelWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					switch (uMsg)
					{
					case WM_CREATE:
					{

					}
					return S_OK;

					case WM_COMMAND:
					{
						LPPANELDATA lpPanelData = GetPanelData(hWnd);
						if (lpPanelData)
							return SendMessageA(lpPanelData->hTranslateCommandMessage, WM_COMMAND, wParam, lParam);
					}
					return S_OK;

					case WM_DESTROY:
					{

					}
					return S_OK;

					case WM_PAINT:
					{
						HDC hDC;
						PAINTSTRUCT ps;
						hDC = BeginPaint(hWnd, &ps);
						
					//	Core::Classes::UI::CUICanvas canvas = hDC;

					//	canvas.Fill(RGB(255, 0, 0));

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