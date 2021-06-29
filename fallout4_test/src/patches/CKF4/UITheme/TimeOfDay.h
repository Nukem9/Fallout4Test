#pragma once

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace TimeOfDay
			{
				typedef struct UITimeOfDayComponentsTag
				{
					Core::Classes::UI::CUIToolWindow hWndToolBar;
					Core::Classes::UI::CUIBaseControl hWndLabel, hWndTrackBar, hWndEdit;
				} UITimeOfDayComponents, *LPUITimeOfDayComponents, *PUITimeOfDayComponents;

				HWND FIXAPI Initialization(HWND hWnd);

				extern UITimeOfDayComponents OldUITimeOfDayComponents;
				extern UITimeOfDayComponents NewUITimeOfDayComponents;

				LRESULT CALLBACK TimeOfDayClassWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			}
		}
	}
}