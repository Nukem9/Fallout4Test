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

#pragma once

#include "..\..\..\StdAfx.h"
#include "..\UIGraphics.h"

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PageControl
			{
				namespace Graphics = Core::Classes::UI;

				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor);
					VOID FIXAPI DrawButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_SelectedAndFocused(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect);
					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect);
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags);
				}

				VOID FIXAPI Initialize(HWND hWnd);
			}
		}
	}
}