#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "MDIClient.h"

#include <Uxtheme.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace MDIClient
			{
				HBRUSH generalMDIBackgroundBrush;

				VOID FIXAPI Initialize(HWND hWindow)
				{
					generalMDIBackgroundBrush = CreateSolidBrush(GetThemeSysColor(ThemeColor::ThemeColor_MDIWindow));
					SetWindowSubclass(hWindow, MDIClientSubclass, 0, 0);
					SetClassLongPtrA(hWindow, GCLP_HBRBACKGROUND, (LONG_PTR)generalMDIBackgroundBrush);
				}

				LRESULT CALLBACK MDIClientSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					switch (uMsg)
					{
					case WM_PAINT:
					{
						if (HDC hdc = GetDC(hWnd); hdc)
						{
							RECT windowArea;
							GetClientRect(hWnd, &windowArea);

							FillRect(hdc, &windowArea, generalMDIBackgroundBrush);
							ReleaseDC(hWnd, hdc);
						}
					}
					break;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}