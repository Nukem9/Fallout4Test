#pragma once

#include <Uxtheme.h>

#include "..\..\..\common.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace ToolBar
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBackground(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Checked(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Pressed(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Icon(Graphics::CUICanvas& canvas, LPCRECT pRect, HIMAGELIST hImageList, INT nIndex);
				}

				VOID FIXAPI Initialize(HWND hWindow);

				LRESULT FIXAPI OnCustomDraw(HWND hWindow, LPNMTBCUSTOMDRAW lpToolBar);
			}
		}
	}
}